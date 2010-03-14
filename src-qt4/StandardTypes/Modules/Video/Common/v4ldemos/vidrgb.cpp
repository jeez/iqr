//---------------------------------------------------------------------------------
// vidrgb.cpp - V4L RGB capture and display test program
//
// Author:  Ben Bridgwater
// History:
// 03/08/99 Initial version
// 03/12/99 Added magnification support
// 03/12/99 Added HI240 support via PseudoColor palette
// 03/14/99 Added WM delete support
// 03/14/99 Added on_exit(ProgramExit)
// 03/14/99 Added resizability
// 03/18/99 Added R/G/B mask comparisons to XImageCaptureFormat().
// 03/20/99 Added XClearWindow to Expose event, for clean resize.
// 08/19/00 Converted to use Image class.
// 09/04/00 Updated for Image shared pixmap support.
//---------------------------------------------------------------------------------
// Build: g++ -O2 vidrgb.cpp v4l.c hi240.c image.cpp -L/usr/X11R6/lib -lX11 -lXext -o vidrgb
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
#include <sys/ioctl.h>
#include <fcntl.h>     /* open */
#include <sys/mman.h>  /* mmap */
#include <unistd.h>    /* close */

#include "types.h"
#include "v4l.h"
#include "hi240.h"
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

// Prototypes

static void ProgramExit(int exitCode, void *arg);

static int  XImageBytesPerPixel(XImage *xImage);
static int  XImageCaptureFormat(XImage *xImage);

static void WindowResize(int v4l, int windowWidth, int windowHeight, int magnification);

static void Draw(Image &image, bits8 *videoFrame, int magnification);
static void DrawFast(Image &image, bits8 *videoFrame);
static void DrawMagnify(Image &image, bits8 *videoFrame, int magnification);
static void DrawMagnify2(Image &image, bits8 *videoFrame, int magnification);

static void Put(Window window, Image &image);

static char *ByteOrderName(int byteOrder);
static char *VisualClassName(int visualClass);

//---------------------------------------------------------------------------------

typedef struct {
  Display  *display;
  Colormap colormap;
} PaletteInfo;

void StoreColormapEntry(PaletteInfo *paletteInfo, int index, long red, long green, long blue)

{
  XColor color;


  color.pixel = (ulong) index;

  color.red = (ushort) red;
  color.green = (ushort) green;
  color.blue = (ushort) blue;

  color.flags = DoRed | DoGreen | DoBlue;

  XStoreColor(paletteInfo->display, paletteInfo->colormap, &color);

} // end StoreColormapEntry

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
  Colormap            colormap;
  PaletteInfo         paletteInfo;
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

  XStoreName(display, window, "V4L RGB Test");

  XGetWindowAttributes(display, window, &windowAttributes);

  if (((windowAttributes.depth == 8) && (windowAttributes.visual->c_class != PseudoColor)) ||
      ((windowAttributes.depth > 8) && (windowAttributes.visual->c_class != TrueColor))) {
    printf("Error: Visual not supported\n");
    exit(1);
  } // end if

  // Create PseudoColor HI240 colormap, if needed

  if (windowAttributes.depth == 8) {
    colormap = XCreateColormap(display, window, windowAttributes.visual, AllocAll);
    paletteInfo.display = display;
    paletteInfo.colormap = colormap;
    Hi240BuildPalette((ulong) 0x10000, (Hi240StorePaletteEntry *) StoreColormapEntry, &paletteInfo);
    XSetWindowColormap(display, window, colormap);
  } // end if

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

  // Select V4L RGB capture format to exactly match image/visual (no LUTs!)

  if ((captureFormat = XImageCaptureFormat(image.X())) < 0) {
    printf("Error: No  match for visual/image\n");
    exit(1);
  } // end if

  // Initialize capture size based on window size

  windowWidth = windowAttributes.width;
  windowHeight = windowAttributes.height;;

  WindowResize(v4l, windowWidth, windowHeight, magnification); // Does V4LMSetFormat().

  // Initialize picture attributes to mid-range

  V4LSetBrightness(v4l, 65535 / 2);
  V4LSetContrast(v4l, 65535 / 2);
  V4LSetSaturation(v4l, 65535 / 2);
  V4LSetHue(v4l, 65535 / 2);

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

      if (V4LMCapture(v4l, 1 - captureFrame) < 0) {
	printf("Error: V4LMCapture: %s\n", strerror(errno));
	exit(1);
      } // end if

      Draw(image, captureBuf + v4lMMInfo.offsets[captureFrame], magnification);

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

typedef struct {
  int               depth;
  int               bits_per_pixel;
  const V4LRGBMasks *rgbMasks;
  int               format;
} XImageMatch;

static XImageMatch xImageMatchTable[] = {
  {8,  8,  NULL,           V4L_FORMAT_HI240},
  {15, 16, &v4lRGB15Masks, V4L_FORMAT_RGB15},
  {16, 16, &v4lRGB16Masks, V4L_FORMAT_RGB16},
  {24, 24, &v4lRGB24Masks, V4L_FORMAT_RGB24},
  {24, 32, &v4lRGB32Masks, V4L_FORMAT_RGB32},
  {0}
}; // end xImageToFormat

static int XImageCaptureFormat(XImage *xImage)

{
  XImageMatch *xImageMatch;

  for (xImageMatch = xImageMatchTable; xImageMatch->depth > 0; xImageMatch++) {
    if ((xImage->depth == xImageMatch->depth) &&
	(xImage->bits_per_pixel == xImageMatch->bits_per_pixel) &&
	((xImageMatch->rgbMasks == NULL) || (
	 ((u_int32_t) xImage->red_mask   == xImageMatch->rgbMasks->redMask) &&
	 ((u_int32_t) xImage->green_mask == xImageMatch->rgbMasks->greenMask) &&
	 ((u_int32_t) xImage->blue_mask  == xImageMatch->rgbMasks->blueMask)
	 ))) {
      return (xImageMatch->format);
    } // end if
  } // end for

  return (-1);

} // end XImageCaptureFormat

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

} // end WindowResize

//---------------------------------------------------------------------------------

static void Draw(Image &image, bits8 *videoFrame, int magnification)

{
  if (magnification == 1) {
    DrawFast(image, videoFrame);
  } else {
    switch (XImageBytesPerPixel(image.X())) {
    case 2: // depth 15, 16
      DrawMagnify2(image, videoFrame, magnification); // Optimized
      break;
    default:
      DrawMagnify(image, videoFrame, magnification);
      break;
    } // end switch
  } // end if

} // end Draw

//---------------------------------------------------------------------------------

static void DrawFast(Image &image, bits8 *videoFrame)

{
  XImage *xImage = image.X();
  int    imageBytesPerPixel = XImageBytesPerPixel(xImage);
  int    videoBytesPerLine  = captureWidth * imageBytesPerPixel; // Video sample matches image
  bits8  *imageLine;
  bits8  *videoLine;
  int    y;


  imageLine = (bits8 *) xImage->data;
  videoLine = videoFrame;
  for (y = 0; y < captureHeight; y++) {
    memcpy(imageLine, videoLine, videoBytesPerLine);
    imageLine += xImage->bytes_per_line;
    videoLine += videoBytesPerLine;
  } // end for

} // end DrawFast

//---------------------------------------------------------------------------------

static void DrawMagnify(Image &image, bits8 *videoFrame, int magnification)

{
  XImage *xImage = image.X();
  int    imageBytesPerPixel = XImageBytesPerPixel(xImage);
  int    videoBytesPerLine  = captureWidth * imageBytesPerPixel; // Video sample matches image
  bits8  *imageLine;
  bits8  *imagePixel;
  bits8  *videoLine;
  bits8  *videoSample;
  int    y, x;
  int    ymag, xmag;
  int    byte;


  imageLine = (bits8 *) xImage->data;
  videoLine = videoFrame;
  for (y = 0; y < captureHeight; y++) {
    for (ymag = 0; ymag < magnification; ymag++) {
      // Copy videoLine to imageLine, advance imageLine
      imagePixel = (bits8 *) imageLine;
      videoSample = (bits8 *) videoLine;
      for (x = 0; x < captureWidth; x++) {
	for (xmag = 0; xmag < magnification; xmag++) {
	  // Copy videoSample to imagePixel, advance imagePixel
	  for (byte = 0; byte < imageBytesPerPixel; byte++) {
	    imagePixel[byte] = videoSample[byte];
	  } // end for
	  imagePixel += imageBytesPerPixel;
	} // end for
	videoSample += imageBytesPerPixel;
      } // end for
      imageLine += xImage->bytes_per_line;
    } // end for
    videoLine += videoBytesPerLine;
  } // end for

} // end DrawMagnify

//---------------------------------------------------------------------------------

static void DrawMagnify2(Image &image, bits8 *videoFrame, int magnification)

{
  XImage *xImage = image.X();
  int    imageBytesPerPixel = XImageBytesPerPixel(xImage);
  int    videoBytesPerLine  = captureWidth * imageBytesPerPixel; // video sample matches image
  bits8  *imageLine;
  bits16 *imagePixel;
  bits8  *videoLine;
  bits16 *videoSample;
  int    y, x;
  int    ymag, xmag;
  int    byte;


  imageLine = (bits8 *) xImage->data;
  videoLine = videoFrame;
  for (y = 0; y < captureHeight; y++) {
    for (ymag = 0; ymag < magnification; ymag++) {
      // Copy videoLine to imageLine, advance imageLine
      imagePixel = (bits16 *) imageLine;
      videoSample = (bits16 *) videoLine;
      switch (magnification) { // Optimize common cases
      case 1:
        for (x = 0; x < captureWidth; x++) {
	  *imagePixel++ = *videoSample;
	  videoSample++;
	} // end for
	break;
      case 2:
        for (x = 0; x < captureWidth; x++) {
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  videoSample++;
	} // end for
	break;
      case 3:
        for (x = 0; x < captureWidth; x++) {
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  videoSample++;
	} // end for
	break;
      case 4:
        for (x = 0; x < captureWidth; x++) {
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  *imagePixel++ = *videoSample;
	  videoSample++;
	} // end for
	break;
      default:
        for (x = 0; x < captureWidth; x++) {
	  for (xmag = 0; xmag < magnification; xmag++) {
	    // Copy videoSample to imagePixel, advance imagePixel
	    *imagePixel++ = *videoSample;
	  } // end for
	  videoSample++;
	} // end for
	break;
      } // end switch
      imageLine += xImage->bytes_per_line;
    } // end for
    videoLine += videoBytesPerLine;
  } // end for

} // end DrawMagnify2

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
