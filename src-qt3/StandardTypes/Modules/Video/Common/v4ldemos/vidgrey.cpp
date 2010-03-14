//---------------------------------------------------------------------------------
// vidgrey.cpp - V4L Y8 capture and display test program
//
// Author:  Ben Bridgwater
// History:
// 03/18/99 Initial version
// 03/20/99 First working version
// 03/20/99 Added XClearWindow to Expose event, for clean resize.
// 08/19/00 Converted to use Image class.
// 09/04/00 Updated for Image shared pixmap support.
// 09/10/00 Fixed DrawY8() yPlaneBytesPerLine.
//---------------------------------------------------------------------------------
// Build: g++ -O2 vidgrey.cpp v4l.c rgb2yuv.cpp image.cpp -L/usr/X11R6/lib -lX11 -lXext -o vidgrey
//---------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>     /* open */
#include <sys/mman.h>  /* mmap */
#include <unistd.h>    /* close */

#include "types.h"
#include "v4l.h"
#include "rgb2yuv.h"
#include "image.h"

//---------------------------------------------------------------------------------

#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))

//---------------------------------------------------------------------------------

static char *AtomWMDeleteWindowName = "WM_DELETE_WINDOW";

//---------------------------------------------------------------------------------

static const char *BigPictureDevice = "/dev/video";

enum {
  BigPictureTVSource        = 0,
  BigPictureCompositeSource = 1, // Camera
  BigPictureSVideoSource    = 2
};

//---------------------------------------------------------------------------------

const int MaxImageWidth    = 1280;
const int MaxImageHeight   = 1024;

// bttv bug - V4LMSync() hangs when capture size at 32x32.

const int MinCaptureWidth  = 48;
const int MinCaptureHeight = 32;

//---------------------------------------------------------------------------------

static V4LCaps v4lCaps; // Min/max capture width/height

static int     magnification;

static int     windowWidth;
static int     windowHeight;

static int     imageWidth;
static int     imageHeight;

static int     captureWidth;
static int     captureHeight;

static int     captureFormat;

//---------------------------------------------------------------------------------

// Pixel look-up tables

static ulong lutYToGrey [256];

static void BuildLUTYToGrey (Display *display, Colormap colormap, ulong lut[256]);

//---------------------------------------------------------------------------------

// Prototypes

static void ProgramExit(int exitCode, void *arg);

static int  XImageBytesPerPixel(XImage *xImage);

static void WindowResize(int v4l, int windowWidth, int windowHeight, int magnification);

static void DrawY8(Image &image, bits8 *videoFrame, int magnification);

static void Put(Window window, Image &image);

static char *ByteOrderName(int byteOrder);
static char *VisualClassName(int visualClass);

//---------------------------------------------------------------------------------

// ProgramExit variables

static Display   *display;

static int       v4l;
static V4LMMInfo v4lMMInfo;
static bits8     *captureBuf;

int main(int argc, char **argv)

{
  Atom                atomWMDeleteWindow;
  int	              screenNumber;
  Screen              *screen;
  Window              window;
  XWindowAttributes   windowAttributes;
  Image               image;
  XImage              *xImage;
  int                 x, y;
  int                 captureFrame;
  XEvent              event;
  bool                sizeChanged;


  // ProgramExit initialization

  display = NULL;
  v4l = -1;
  captureBuf = NULL;

  on_exit(ProgramExit, NULL);

  // Get command line options

  magnification = 1;

  if (argc > 1) {
    magnification = atoi(argv[1]);
  } // end if

  magnification = max(1, magnification);

  printf("Magnification is %i\n", magnification);

  // Open display

  if ((display = XOpenDisplay(NULL)) == NULL) { // NULL for DISPLAY
    printf("Error: XOpenDisplay() failed\n");
    exit(1);
  } // end if

  screenNumber = DefaultScreen(display);

  screen = XScreenOfDisplay(display, screenNumber);

  // Obtain WM protocols atom for ClientMessage exit event

  if ((atomWMDeleteWindow = XInternAtom(display, AtomWMDeleteWindowName, True)) == None) {
    printf("Error: %s atom does not exist\n", AtomWMDeleteWindowName);
    exit(1);
  } // end if

  // Create window, inheriting depth and visual from root window

  window = XCreateSimpleWindow(
    display,
    RootWindowOfScreen(screen),
    0, // x
    0, // y
    640, // width
    480, // height
    0,                          // border width
    BlackPixelOfScreen(screen), // border
    BlackPixelOfScreen(screen)  // background
    );

  XStoreName(display, window, "V4L YUV Test");

  XGetWindowAttributes(display, window, &windowAttributes);

  if (windowAttributes.visual->c_class != TrueColor) {
    printf("Error: Visual not supported\n");
    exit(1);
  } // end if

  // Build LUT

  BuildLUTYToGrey (display, windowAttributes.colormap, lutYToGrey);

  // Create image

  if (image.Create(
    display,
    window, // Defines visual, depth
    MaxImageWidth,
    MaxImageHeight,
    True // MITSHM
    ) < 0) {
    printf("Error: image.Create() failed\n");
    exit(1);
  } // end if

  image.Clear();

#if (1)
  printf("\nDisplay:\n");
  printf("Image byte order = %s\n", ByteOrderName(ImageByteOrder(display)));
  printf("Bitmap unit      = %i\n", BitmapUnit(display));
  printf("Bitmap bit order = %s\n", ByteOrderName(BitmapBitOrder(display)));
  printf("Bitmap pad       = %i\n", BitmapPad(display));

  printf("\nWindow:\n");
  printf("Depth            = %i\n", windowAttributes.depth);
  printf("Visual ID        = 0x%02x\n", windowAttributes.visual->visualid);
  printf("Visual class     = %s\n", VisualClassName(windowAttributes.visual->c_class));
  printf("Red mask         = 0x%08lx\n", windowAttributes.visual->red_mask);
  printf("Green mask       = 0x%08lx\n", windowAttributes.visual->green_mask);
  printf("Blue mask        = 0x%08lx\n", windowAttributes.visual->blue_mask);
  printf("Bits per R/G/B   = %i\n", windowAttributes.visual->bits_per_rgb); // log2 # colors

  xImage = image.X();
  printf("\nImage:\n");
  printf("Image byte order = %s\n", ByteOrderName(xImage->byte_order));
  printf("Bitmap unit      = %i\n", xImage->bitmap_unit);
  printf("Bitmap bit order = %s\n", ByteOrderName(xImage->bitmap_bit_order));
  printf("Bitmap pad       = %i\n", xImage->bitmap_pad);
  printf("Depth            = %i\n", xImage->depth);
  printf("Red mask         = 0x%08lx\n", xImage->red_mask);
  printf("Green mask       = 0x%08lx\n", xImage->green_mask);
  printf("Blue mask        = 0x%08lx\n", xImage->blue_mask);
  printf("Bits per pixel   = %i\n", xImage->bits_per_pixel); // ZPixmap
  printf("Bytes per line   = %i\n", xImage->bytes_per_line);
  printf("IsShared         = %s\n", image.IsShared() ? "True" : "False");
  printf("HasSharedPixmap  = %s\n", image.HasSharedPixmap() ? "True" : "False");
#endif

  // V4L stuff

  if ((v4l = open(BigPictureDevice, O_RDWR)) < 0) {
    printf("Error: Can't open %s: %s\n", BigPictureDevice, strerror(errno));
    exit(1);
  } // end if

  if (V4LMGetMMInfo(v4l, &v4lMMInfo) < 0) {
    printf("Error: V4LMGetMMInfo: %s\n", strerror(errno));
    exit(1);
  } // end if
#if (0)
  printf("Capture buffer size   = %i\n", v4lMMInfo.size);
  printf("Capture buffer frames = %i\n", v4lMMInfo.frames);
#endif
  if (v4lMMInfo.frames < 2) {
    printf("Error: V4LMGetMMInfo: frames < 2\n");
    exit(1);
  } // end if

  if ((captureBuf = (bits8 *) mmap(0, v4lMMInfo.size, PROT_READ | PROT_WRITE, MAP_SHARED, v4l, 0)) == MAP_FAILED) {
    printf("Error: mmap(): %s\n", strerror(errno));
    exit(1);
  } // end if

  if (V4LSetSource(v4l, BigPictureCompositeSource, VIDEO_MODE_NTSC) < 0) {
    printf("Error: V4LSetSource: %s\n", strerror(errno));
    exit(1);
  } // end if

  if (V4LGetCaps(v4l, &v4lCaps) < 0) {
    printf("Error: V4LGetCaps: %s\n", strerror(errno));
    exit(1);
  } // end if

  // Select Y8 capture format

  captureFormat = V4L_FORMAT_Y8;

  // Initialize capture size based on window size

  windowWidth = windowAttributes.width;
  windowHeight = windowAttributes.height;;

  WindowResize(v4l, windowWidth, windowHeight, magnification); // Does V4LMSetFormat().

  // Initialize picture attributes to mid-range

  V4LSetBrightness(v4l, 65535 / 2);
  V4LSetContrast(v4l, 65535 / 2);
  V4LSetSaturation(v4l, 65535 / 2);
  V4LSetHue(v4l, 65535 / 2);

#if (0)
  V4LSetSaturation(v4l, 65535); // Max saturation (chroma gain)
#endif

  // Ready to start: Display window, select events, and initiate capture sequence

  XMapRaised(display, window);

  XSetWMProtocols(display, window, &atomWMDeleteWindow, 1);

  XSelectInput(display, window, StructureNotifyMask | ExposureMask);

  captureFrame = 0;

  if (V4LMCapture(v4l, captureFrame) < 0) {
    printf("Error: V4LMCapture: %s\n", strerror(errno));
    exit(1);
  } // end if

  while (1) {

    if (XPending(display) > 0) {

      XNextEvent(display, &event);

      switch (event.type) {
      case ClientMessage: // From WM
	if (event.xclient.data.l[0] == atomWMDeleteWindow) {
	  exit(0);
	} // end if
	break;
      case ConfigureNotify:
	sizeChanged = false;
	if (event.xconfigure.width != windowWidth) {
	  sizeChanged = true;
	  windowWidth = event.xconfigure.width;
	} // end if
	if (event.xconfigure.height != windowHeight) {
	  sizeChanged = true;
	  windowHeight = event.xconfigure.height;
	} // end if
	if (sizeChanged) {
	  image.Clear();
	  XClearWindow(display, window);
	  WindowResize(v4l, windowWidth, windowHeight, magnification);
	} // end if
	break;
      case Expose:
	if (event.xexpose.count == 0) {
	  Put(window, image);
	} // end if
	break;
      } // end switch

    } else {

      // Wait for this frame

      if (V4LMSync(v4l, captureFrame) < 0) {
	printf("Error: V4LMSync: %s\n", strerror(errno));
	exit(1);
      } // end if

      // Start capture for next frame
#if (0)
      memset(captureBuf + v4lMMInfo.offsets[1 - captureFrame], 0xff, v4lMMInfo.offsets[1]);
#endif
      if (V4LMCapture(v4l, 1 - captureFrame) < 0) {
	printf("Error: V4LMCapture: %s\n", strerror(errno));
	exit(1);
      } // end if

      DrawY8(image, captureBuf + v4lMMInfo.offsets[captureFrame], magnification);

      Put(window, image);

      captureFrame = 1 - captureFrame; // 0<->1

    } // endif 

  } // end while

  printf("Error: Fell out of event loop!\n");

  exit(1);

} // end main

//---------------------------------------------------------------------------------

static void ProgramExit(int exitCode, void *arg)

{
  if (captureBuf != NULL) {
    if (munmap((char *) captureBuf, v4lMMInfo.size) < 0) {
      printf("Error: munmap(): %s\n", strerror(errno));
    } // end if
  } // end if

  if (v4l > 0) {
    if (close(v4l) < 0) {
      printf("Error: Can't close %s: %s\n", BigPictureDevice, strerror(errno));
    } // end if
  } // end if

  if (display != NULL) {
    XCloseDisplay(display);
  } // end if

  fflush(stdout);

} // end ProgramExit

//---------------------------------------------------------------------------------

static int XImageBytesPerPixel(XImage *xImage)

{
  return ((xImage->bits_per_pixel + 7) / 8);

} // end XImageBytesPerPixel

//---------------------------------------------------------------------------------

static void WindowResize(int v4l, int windowWidth, int windowHeight, int magnification)

{
  int        minCaptureWidth;
  int        minCaptureHeight;
  V4LFormat  v4lFormat;


  // Recalculate image and capture size based on window size

  imageWidth = min(windowWidth, MaxImageWidth);
  imageHeight = min(windowHeight, MaxImageHeight);

  captureWidth = min(imageWidth / magnification, v4lCaps.maxwidth);
  captureHeight = min(imageHeight / magnification, v4lCaps.maxheight);

  minCaptureWidth = max(v4lCaps.minwidth, MinCaptureWidth);
  minCaptureHeight = max(v4lCaps.minheight, MinCaptureHeight);

  captureWidth = max(captureWidth, minCaptureWidth);
  captureHeight = max(captureHeight, minCaptureHeight);

  assert(captureWidth >= v4lCaps.minwidth);
  assert(captureHeight >= v4lCaps.minheight);

  assert(captureWidth <= v4lCaps.maxwidth);
  assert(captureHeight <= v4lCaps.maxheight);

  assert(captureWidth >= MinCaptureWidth);
  assert(captureHeight >= MinCaptureHeight);

  // Request capture size, then update image size based on actual capture size

  v4lFormat.width  = captureWidth;
  v4lFormat.height = captureHeight;
  v4lFormat.format = captureFormat;;
  V4LMSetFormat(&v4lFormat);

  V4LMGetFormat(&v4lFormat);
  
  captureWidth = v4lFormat.width;
  captureHeight = v4lFormat.height;

  printf("Capture size is %3i x %3i\n", captureWidth, captureHeight);

  imageWidth = captureWidth * magnification;
  imageHeight = captureHeight * magnification;

  assert(imageWidth <= MaxImageWidth);
  assert(imageHeight <= MaxImageHeight);

#if (0)
  printf("Window size  is %3i x %3i\n", windowWidth, windowHeight);
  printf("Image size   is %3i x %3i\n", imageWidth, imageHeight);
#endif

} // end WindowResize

//---------------------------------------------------------------------------------

static void DrawY8(Image &image, bits8 *videoFrame, int magnification)

{
  XImage *xImage = image.X();
  int    yPlaneBytesPerLine  = captureWidth; // Y8
  bits8  *imageLine;
  bits16 *imagePixel;
  bits8  *planeLine;
  bits8  *planeSample;
  int    y, x;
  int    ymag, xmag;


  if (XImageBytesPerPixel(xImage) != 2) return;

  if (captureFormat != V4L_FORMAT_Y8) return;

  imageLine = (bits8 *) xImage->data;
  planeLine = videoFrame;
  for (y = 0; y < captureHeight; y++) {
    for (ymag = 0; ymag < magnification; ymag++) {
      // Copy planeLine to imageLine, advance imageLine
      imagePixel = (bits16 *) imageLine;
      planeSample = (bits8 *) planeLine;
      for (x = 0; x < captureWidth; x++) {
	for (xmag = 0; xmag < magnification; xmag++) {
	  // Copy planeSample to imagePixel, advance imagePixel
	  *imagePixel++ = lutYToGrey[*planeSample];
	} // end for
	planeSample++;
      } // end for
      imageLine += xImage->bytes_per_line;
    } // end for
    planeLine += yPlaneBytesPerLine;
  } // end for

} // end DrawY8

//---------------------------------------------------------------------------------

static void Put(Window window, Image &image)

{
  image.Put(
    window,
    0, // srcX
    0, // srcY
    0, // dstX
    0, // dstY
    imageWidth,
    imageHeight
    );

} // end Put

//---------------------------------------------------------------------------------

static char *ByteOrderName(int byteOrder)

{
  switch (byteOrder) {
  case LSBFirst: return ("LSBFirst");
  case MSBFirst: return ("MSBFirst");
  default:       return ("?");
  } // end switch

} // end ByteOrderName

//---------------------------------------------------------------------------------

static char *VisualClassName(int visualClass)

{
  switch (visualClass) {
  case StaticGray:  return ("StaticGray");
  case GrayScale:   return ("GrayScale");
  case StaticColor: return ("StaticColor");
  case PseudoColor: return ("PseudoColor");
  case TrueColor:   return ("TrueColor");
  case DirectColor: return ("DirectColor");
  default:	    return ("?");
  } // end switch

} // end VisualClassName

//---------------------------------------------------------------------------------

#if (0)
RGB2YUV rgb2yuv;
YUV2RGB yuv2rgb;
#endif

static void BuildLUTYToGrey(Display *display, Colormap colormap, ulong lut[256])

{
  YCbCr  white;
  uint   y_; // 16-253 (or 0-255)
  RGB    rgb;
  XColor color;

#if (0)
  rgb2yuv.RGBToYCbCr(255, 255, 255, &white);
#endif

  for (y_ = 0; y_ <= 255; y_++) {
#if (0)
    yuv2rgb.YCbCrToRGB(y_, white.cb, white.cr, &rgb);
    color.red   = rgb.r * 256;
    color.green = rgb.g * 256;
    color.blue  = rgb.b * 256;
#else
    color.red   = y_ * 256;
    color.green = y_ * 256;
    color.blue  = y_ * 256;
#endif
    if (XAllocColor(display, colormap, &color) == 0) {
      printf("Error: BuildLUTYToGrey: XAllocColor failed\n");
      exit(1);
    } // end if
    lut[y_] = color.pixel;
  } // end for

} // end BuildLUTYToGrey

//---------------------------------------------------------------------------------
