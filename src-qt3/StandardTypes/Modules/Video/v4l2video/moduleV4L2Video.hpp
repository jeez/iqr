#ifndef USBVIDEO_HPP
#define USBVIDEO_HPP

#include <iostream>

//--------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h> /* getopt_long() */

#include <fcntl.h> /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h> /* for videodev2.h */
#include <sys/time.h>

#include <linux/videodev2.h>
//-----------------------



#include <Common/Item/threadModule.hpp>


#include "ClsXWin.h"

extern int errno;

#define CLEAR(x) memset (&(x), 0, sizeof (x))

typedef enum {
    IO_METHOD_READ,
    IO_METHOD_MMAP,
} io_method;

struct buffer {
    void * start;
    size_t length;
};

using namespace std;

static std::string fcc2s(unsigned int val) {
    std::string s;

    s += val & 0xff;
    s += (val >> 8) & 0xff;
    s += (val >> 16) & 0xff;
    s += (val >> 24) & 0xff;
    return s;
}



namespace iqrcommon {


//    class moduleV4L2Video : public ClsModule {
    class moduleV4L2Video : public ClsThreadModule {

    public:

	moduleV4L2Video();
	~moduleV4L2Video();
	void init();
	void update();
	void cleanup();

	moduleIcon getIcon();

    private:
	moduleV4L2Video(const moduleV4L2Video&);

	void errno_exit (const char * s);

	static int xioctl (int fd, int request, void * arg) {
	    int r;
	    do r = ioctl (fd, request, arg);
	    while (-1 == r && EINTR == errno);
	    return r;
	}

	void enumerate_menu ();	    

    protected:


#ifdef MODULE_MONOCHROME
	ClsStateVariable* clsStateVariableVideoOutput;
#endif
#ifdef MODULE_COLOR
	ClsStateVariable* clsStateVariableVideoOutputRed;
	ClsStateVariable* clsStateVariableVideoOutputGreen;
	ClsStateVariable* clsStateVariableVideoOutputBlue;
#endif
	
	ClsIntParameter* clsIntParameterImgWidth;
	ClsIntParameter* clsIntParameterImgheight;
	ClsIntParameter* clsIntParameterBrightness;
//	ClsIntParameter* clsIntParameterHue;
	ClsIntParameter* clsIntParameterSaturation;
	ClsIntParameter* clsIntParameterContrast;
	ClsIntParameter* clsIntParameterAutoGain;
	ClsIntParameter* clsIntParameterInput;
	ClsStringParameter* clsStringParameterDeviceName;
	ClsBoolParameter* clsBoolParameterShowOutput;
	ClsIntParameter* clsIntParameterCPS;

#ifdef MODULE_COLOR
	ClsBoolParameter* clsBoolParameterHSV;
	bool bHSV;
#endif

	virtual const char* getLabel()  {
	    return "USB video module";
	}



	
	void myYUV422toRGB(int imgWidth, int imgHeight, unsigned char* yuv, unsigned char* rgb);
	void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );

	unsigned long int getPixel(unsigned long ir, unsigned long ig, unsigned long ib) {
	    if(iScreenDepth == 24){
		return  (0x00ff0000 & (ib << 16)) |
		    (0x0000ff00 & (ig  << 8)) |
		    (0x000000ff & (ir));
		
	    } else if(iScreenDepth == 16){
		return ((ib >>3)<<11)+((ig >>2)<<5)+((ir >>3));
	    }
	    return 0;
	}


	void openDevice(string strDeviceName);
	void initDevice(string strDeviceName);
	void checkDevice(string strDeviceName);
	void setPixelformat();
	void uninitDevice();
	void initRead (unsigned int buffer_size);
	int readFrame();
	void processFrame (const void * p);
	void init_mmap (string strDeviceName);
	void closeDevice ();
	void startCapturing();



	void printDeviceInfo();
	void setPictureProperties();
	string listInputs();
	void selectInput();
	void selectStandard();
	void enumControls();
	void enumFormats();
	void setControl(int iID, int _iValue);

	struct timeval tim0, tim1;
	struct timezone tz;

	struct timeval sTimevalCurrent;
	struct timezone sTimezone;
	double fArrivalTime;
	double fStartTime;
	double fCurrentTime;
	long int iSleep;
	int iCPS;

	io_method io;
	struct buffer * buffers;
	unsigned int n_buffers;


	int iPixelFormat;
	int iRGBBufferSize;
	unsigned char *pcRGBBuffer;
	
	ClsXWin clsXWin;

	/* parameters */
	int imgWidth,imgHeight;
	int iBrightness;
//	int iHue;
	int iSaturation;
	int iContrast;
	int iAutoGain;
	bool bShowOutput;
	/* --------------- */

	int device_fd; 
    
	int iNrCells; /* empirical */

	double fImageRatio;
	int iGroupWidth; /* hypothetical */
	int iGroupHeight; /* hypothetical */
	int iHPace, iVPace;

	unsigned long int iScreenDepth;
	int iv4l_sizeimage;

	long int iCounter;


    };
}
#endif

    
