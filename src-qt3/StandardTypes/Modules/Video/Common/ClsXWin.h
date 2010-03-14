#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <iostream>
#include <inttypes.h>

using namespace std;







//XNextEvent(display, &event);
//if(event.type == DestroyNotify){
//}




class ClsXWin {


public:
    ClsXWin() {
	dpy = NULL;
	gc = 0;
	win = 0;
	vis = NULL;
	ximg = NULL;
	rt = 0;
	sId = 0;
	
	iScreenDepth = 0;
    };


    ~ClsXWin(){
	destroyWindow();
    };

    int createWindow(int _iWindowWidth, int _iWindowHeight) {
	iWindowWidth = _iWindowWidth;
	iWindowHeight = _iWindowHeight;
	
	if(ximg == NULL && dpy == NULL){
	    dpy = XOpenDisplay("");
	    if (dpy==0){
		cerr << __FILE__ << " could not open X display" << endl;
		return -1;
	    } else {
		rt     = DefaultRootWindow(dpy);
		sId    = DefaultScreen(dpy);
		vis    = DefaultVisual(dpy,sId);
		win   = XCreateSimpleWindow(dpy, rt, 100, 100, iWindowWidth, iWindowHeight, 5, 0, BlackPixel(dpy,sId));
		gc     = XCreateGC (dpy,rt,0,0);
		ximg = XGetImage(dpy, rt, 0, 0, iWindowWidth, iWindowHeight, AllPlanes, ZPixmap);

		wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
		(void) XSetWMProtocols(dpy, win, &wm_delete_window, 1);

/*
  XVisualInfo *visinfo = (XVisualInfo *) malloc( sizeof(XVisualInfo) );
  if (!XMatchVisualInfo( dpy, sId, 24, TrueColor, visinfo )) {
  cerr << "Couldn't get 24-bit TrueColor visual!" << endl;
  exit(1);
  } else {
  iScreenDepth =  visinfo->depth; 
  
  }*/
	    
		iScreenDepth = DefaultDepth(dpy, sId);
		cout << "screen depth:\t" << iScreenDepth << endl;

      
     
		XSelectInput (dpy,win,PointerMotionMask|ButtonPressMask|ButtonReleaseMask);
		XMapRaised (dpy,win);
		XFlush(dpy);
	    } 
	}
	return 0;
    }

    void destroyWindow() {
	if(dpy != NULL){
	    XDestroyWindow(dpy,win);
	    XCloseDisplay(dpy);
	}
	dpy = NULL;
	gc = 0;
	win = 0;
	vis = NULL;
	ximg = NULL;
	rt = 0;
	sId = 0;
	
	iScreenDepth = 0;
    }

    int getScreenDepth(){return iScreenDepth;};
    void putPixel(int iX, int iY, unsigned long value){
//	cerr << "XCheckTypedWindowEvent: " << XCheckTypedWindowEvent(dpy, win, DestroyNotify, &event) << endl;
	XPutPixel(ximg, iX, iY, value);

    };

    void showImage(){
	XPutImage(dpy, win, gc, ximg, 0, 0, 0, 0, iWindowWidth, iWindowHeight);
    }



private:

    Display *dpy;
    GC gc;
    Window win;
    Visual *vis;
    XImage *ximg;
    XEvent event;
    int rt, sId;
    int iWindowWidth, iWindowHeight;
    unsigned long int iScreenDepth;

Atom wm_delete_window;


};
