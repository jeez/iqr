/*--------------------------------------------------------------------------------------*/
// File: image.cpp
// Desription: Image class implementation
// Author: Ben Bridgwater
// History:
// 08/17/00 Initial revision.
// 09/04/00 Added shared pixmap support.
// 10/24/00 Changed to pass window vs visual, depth to Create(), and add gc member.
/*--------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h> // XDestroyImage
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include "image.h"

/*--------------------------------------------------------------------------------------*/

int Image::Create( // format always ZPixmap
    Display *display_,
    Window  window,
    int     width,
    int     height,
    Bool    wantShared,
    Bool    wantSharedPixmap
    )
{
  int                majorVersion;
  int                minorVersion;
  Bool               sharedPixmapsSupported;
  XGCValues          gcValues;
  ulong	             gcValuesMask;
  XWindowAttributes  windowAttributes;

  if (xImage != NULL) {
    Destroy();
  } // end if

  display = display_;

  gcValues.function = GXcopy;
  gcValuesMask = GCFunction;
  gc = XCreateGC(display, window, gcValuesMask, &gcValues);

  XGetWindowAttributes(display, window, &windowAttributes);

  visual = windowAttributes.visual;
  depth = windowAttributes.depth;

#if (1)
  if (wantShared && XShmQueryExtension(display)) {
#else
  if (wantShared && XShmQueryVersion(display, &majorVersion, &minorVersion, &sharedPixmapsSupported)) {
#endif
    isShared = True;
  } else {
    isShared = False;
  } // end if

  try {
    errno = 0;
    xImage = NULL;
    sharedPixmap = None;
    if (isShared) {
      shmInfo.shmid = -1;
      shmInfo.shmaddr = NULL;
      if ((xImage = XShmCreateImage(display, visual, depth, ZPixmap, NULL, &shmInfo, width, height)) == NULL) {
	throw ("XShmCreateImage");
      } // end if
      if ((shmInfo.shmid = shmget(IPC_PRIVATE, xImage->bytes_per_line * xImage->height, IPC_CREAT | 0777)) < 0) { // Create segment
	throw ("shmget");
      } // end if
      if ((shmInfo.shmaddr = (char *) shmat(shmInfo.shmid, 0, 0)) < 0) {  // We attach
	shmInfo.shmaddr = NULL;
	throw ("shmat");
      } // end if
      xImage->data = shmInfo.shmaddr;
      shmInfo.readOnly = False;
      if (!XShmAttach(display, &shmInfo)) { // X attaches
	throw ("XShmAttach");
      } // end if
      if (wantSharedPixmap && (XShmPixmapFormat(display) == ZPixmap)) {
	if ((sharedPixmap = XShmCreatePixmap(display, window, shmInfo.shmaddr, &shmInfo, width, height, depth)) == None) {
	  ; // HasSharedPixmap() will return false.
	} // end if
      } // end if
    } else {
      if ((xImage = XCreateImage(display, visual, depth, ZPixmap, 0, NULL, width, height, 16, 0)) == NULL) {
	throw ("XCreateImage");
      } // end if
      if ((xImage->data = (char *) malloc(xImage->bytes_per_line * xImage->height)) == NULL) {
	throw ("malloc");
      } // end if
    } // end if
    return (0);
  } catch (char *function) {
#if (1)
    cout << "Error: Image::Create failed in " << function << ": " << ((errno == 0) ? "No further info" : strerror(errno)) << endl;
#endif
    Destroy();
    return (-1);
  } // end if

} // end Image::Create

/*--------------------------------------------------------------------------------------*/

int Image::Destroy()
{
  if (xImage == NULL) return (0); // Nothing to do

  if (isShared) {
    if (shmInfo.shmid >= 0) {
      XShmDetach(display, &shmInfo); // X detaches
      shmdt(shmInfo.shmaddr); // We detach
      shmInfo.shmaddr = NULL;
      shmctl(shmInfo.shmid, IPC_RMID, 0); // Destroy segment
      shmInfo.shmid = -1;
    } // end if
  } else {
    if (xImage->data != NULL) {
      free(xImage->data);
    } // end if
  } // end if

  xImage->data = NULL;

  XDestroyImage(xImage);

  xImage = NULL;

  if (sharedPixmap != None) {
    XFreePixmap(display, sharedPixmap);
    sharedPixmap = None;
  } // end if

  if (display != NULL) {
    XFreeGC(display, gc);
    display = NULL;
  } // end if

  return (0);

} // end Image::Destroy

/*--------------------------------------------------------------------------------------*/

void Image::Clear()
{
  if (xImage == NULL) return;

  memset(xImage->data, 0, xImage->height * xImage->bytes_per_line);

} // end Image::Clear

/*--------------------------------------------------------------------------------------*/

int Image::Put(
    Drawable d,
    int      srcX,
    int      srcY,
    int      dstX,
    int      dstY,
    int      width,
    int      height
    )
{
  if (xImage == NULL) return (-1);

  if (width < 0) width = Width();
  if (height < 0) height = Height();

  if (isShared) {
    XShmPutImage(display, d, gc, xImage, srcX, srcY, dstX, dstY, width, height, False);
  } else {
    XPutImage(display, d, gc, xImage, srcX, srcY, dstX, dstY, width, height);
  } // end if

  return (0);

} // end Image::Put

/*--------------------------------------------------------------------------------------*/

int Image::Get(
    Drawable d,
    int      srcX,
    int      srcY
    )
{
  if (xImage == NULL) return (-1);

  // ZFormat, so plane_mask N/A (pass 0L)

  if (isShared) {
    XShmGetImage(display, d, xImage, srcX, srcY, 0L);
  } else {
    XGetSubImage(display, d, srcX, srcY, xImage->width, xImage->height, 0L, xImage->format, xImage, 0, 0);
  } // end if

  return (0);

} // end Image::Get

/*--------------------------------------------------------------------------------------*/

int Image::CopyArea(
    Drawable d,
    int      srcX,
    int      srcY,
    int      dstX,
    int      dstY,
    int      width,
    int      height
    )
{
  if (xImage == NULL) return (-1);

  if (sharedPixmap == None) return (-1);

  if (width < 0) width = Width();
  if (height < 0) height = Height();

  XCopyArea(display, sharedPixmap, d, gc, srcX, srcY, width, height, dstX, dstY);

  return (0);

} // end Image::CopyArea

/*--------------------------------------------------------------------------------------*/
