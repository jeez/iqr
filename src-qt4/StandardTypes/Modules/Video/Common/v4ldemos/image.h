/*--------------------------------------------------------------------------------------*/
// File: image.h
// Desription: Image class definition
// Author: Ben Bridgwater
// History:
// 08/17/00 Initial revision.
// 09/04/00 Added shared pixmap support.
// 10/20/00 Changed wantSharedPixmap default to false, so don't need sharedPixmapDrawable.
// 10/24/00 Changed to pass window vs visual, depth to Create(), and add gc member.
/*--------------------------------------------------------------------------------------*/
#ifndef _IMAGE_H
#define _IMAGE_H
/*--------------------------------------------------------------------------------------*/

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

/*--------------------------------------------------------------------------------------*/

class Image {
 private:
  Display         *display;
  GC              gc;
  Visual          *visual;
  int             depth;
  Bool            isShared; // MITSHM
  XImage          *xImage;
  Pixmap          sharedPixmap; // None (0L) if unassigned
  XShmSegmentInfo shmInfo;
 public:
  Image() : display(NULL), gc(0), isShared(False), xImage(NULL), sharedPixmap(None) {}
  ~Image() {Destroy();}
  int VisualClass() {return (visual->c_class);}
  int Depth()  {return (depth);}
  int Width()  {return ((xImage != NULL) ? xImage->width : 0);}
  int Height() {return ((xImage != NULL) ? xImage->height : 0);}
  XImage *X() {return (xImage);};
  Bool IsShared() {return (isShared);}
  Bool HasSharedPixmap() {return ((Bool) (sharedPixmap != None));}
  Pixmap SharedPixmap() {return (sharedPixmap);}
  int Create(
    Display  *display_,
    Window   window, // Defines visual, depth (& screen for shared pixmap)
    int      width,
    int      height,
    Bool     wantShared = True,
    Bool     wantSharedPixmap = False
    );
  int Destroy();
  void Clear();
  int Put(
    Drawable d,
    int      srcX = 0,
    int      srcY = 0,
    int      dstX = 0,
    int      dstY = 0,
    int      width  = -1, // means image width
    int      height = -1  // means image height
    );
  int Get(
    Drawable d,
    int      srcX = 0,
    int      srcY = 0
    );
  int CopyArea( // Only supported if HasSharedPixmap().
    Drawable d,
    int      srcX = 0,
    int      srcY = 0,
    int      dstX = 0,
    int      dstY = 0,
    int      width  = -1,
    int      height = -1
    );
}; // end Image

/*--------------------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------------------*/
