#ifndef USBVIDEO_HPP
#define USBVIDEO_HPP

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/videodev.h>
#include <iostream>
#include <inttypes.h>
#include <errno.h>


//#include "/usr/src/linux/drivers/usb/pwc-ioctl.h"

#ifdef IQRMODULE
//#include <Common/Item/module.hpp>
#include <Common/Item/threadModule.hpp>
#endif

#include "ClsXWin.h"

extern int errno;


using namespace std;

namespace iqrcommon {

#ifdef IQRMODULE
//    class moduleUSBVideo : public ClsModule {
    class moduleUSBVideo : public ClsThreadModule {
#else
	class moduleUSBVideo  {
#endif

	public:

	    moduleUSBVideo();
	    ~moduleUSBVideo();
	    virtual void init();
	    virtual void update();
	    virtual void cleanup();

#ifdef IQRMODULE
	    moduleIcon getIcon();
#endif

	private:
	    moduleUSBVideo(const moduleUSBVideo&);

	protected:


#ifdef IQRMODULE  
	    ClsStateVariable* clsStateVariableVideoOutput;

	    ClsIntParameter* clsIntParameterImgWidth;
	    ClsIntParameter* clsIntParameterImgheight;
	    ClsIntParameter* clsIntParameterBrightness;
	    ClsIntParameter* clsIntParameterHue;
	    ClsIntParameter* clsIntParameterColour;
	    ClsIntParameter* clsIntParameterContrast;
	    ClsIntParameter* clsIntParameterAutoGain;
	    ClsStringParameter* clsStringParameterDeviceName;
	    ClsBoolParameter* clsBoolParameterShowOutput;

	    virtual const char* getLabel()  {
		return "USB video module";
	    }

#endif

	    ClsXWin clsXWin;

	    /* parameters */
	    int imgWidth,imgHeight;
	    int iBrightness;
	    int iHue;
	    int iColour;
	    int iContrast;
	    int iAutoGain;
	    bool bShowOutput;
	    /* --------------- */

	    bool bFirstTime;
	    int device_fd; 
	    int iPalette;
	    int iRGBBufferSize;
	    int iRGBBufferDepth;
    
	    struct video_capability sVideo_capability;
	    struct video_window sVideo_window;
	    struct video_picture sVideo_picture;


/* for testing */
	    double f1; 
	    long int iCounter;
/* ------------------- */

	
	    int iNrCells; /* empirical */

	    double fImageRatio;
	    int iGroupWidth; /* hypothetical */
	    int iGroupHeight; /* hypothetical */
	    int iHPace, iVPace;

	    unsigned long int iScreenDepth;
	    int iVidbufSize;


	    unsigned char *pcCameraBuffer;

	    unsigned char *pcRGBBuffer;
	    unsigned char *pcRGBBuffer_last;
    

	    unsigned long int getPixel(unsigned long ir, unsigned long ig, unsigned long ib) {
		if(iScreenDepth == 24){
		    return  (0x00ff0000 & (ib << 16)) |
			(0x0000ff00 & (ig  << 8)) |
			(0x000000ff & (ir));

		} else if(iScreenDepth == 16){
		    return ((ib >>3)<<11)+((ig >>2)<<5)+((ir >>3));
//		    return ((ib >>3)<<10)+((ig >>3)<<5)+((ir >>3));
		}
		return 0;
	    }

	    void openDevice(string strDeviceName);
	    void closeDevice();

	    void printDeviceInfo();


	    void setPictureProperties();
	    void writePPM(const char *filename);    
	    int mapMemory();

	};
    }
#endif

    
