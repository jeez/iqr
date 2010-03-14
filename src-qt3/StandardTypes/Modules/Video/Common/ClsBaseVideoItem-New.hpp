#ifndef CLSBASEVIDEOITEM_HPP
#define CLSBASEVIDEOITEM_HPP

#include <iostream>
#include <sys/time.h>
#include <linux/videodev.h>

#ifdef IQRMODULE
#include <Common/Item/item.hpp>
#endif


#ifdef IQRMODULE
//#include <Common/Item/module.hpp>
#include <Common/Item/threadModule.hpp>
#endif


#include "ClsXWin.h"
using namespace std;

namespace iqrcommon {

#ifdef IQRMODULE
//    class ClsBaseVideoItem : public ClsModule {
    class ClsBaseVideoItem : public ClsThreadModule {
#else
	class ClsBaseVideoItem  {
#endif

	public:
	    ClsBaseVideoItem(){
#ifdef IQRMODULE
		clsStringParameterDeviceName = addStringParameter("DeviceName", /* _strName */ 
								  "Video Device", /* _strLabel */
								  "/dev/video0", /* _strValue */
								  true, /* _bEditable */
								  false, /* _bLong */
								  "Path to Video Device", /* _strDescription */
								  "Device" /* _strCategory */);


		clsBoolParameterShowOutput = addBoolParameter("ShowOutput", /* _strName */ 
							      "Show output", /* _strLabel */
							      true, /* _bValue */
							      "Show camera output", /* _strDescription */
							      "Output" /* _strCategory */);
	    

		clsIntParameterImgWidth = addIntParameter("imgWidth", /* _strName */
							  "Image Width",  /* _strLabel */
							  160, /* _iValue */
							  160, /* _iMinimum */
							  640, /* _iMaximum */ 
							  "Width of the Image to grab" /* _strDescription */,
							  "Image" /* _strCategory */);

		clsIntParameterImgheight = addIntParameter("imgHeight", /* _strName */
							   "Image Height", /* _strLabel */
							   120, /* _iValue */
							   120, /* _iMinimum */
							   480, /* _iMaximum */ 
							   "Height of the Image to grab", /* _strDescription */
							   "Image" /* _strCategory */);

		clsIntParameterBrightness = addIntParameter("Brightness", /* _strName */
							    "Image Brightness", /* _strLabel */
							    32768, /* _iValue */
							    0, /* _iMinimum */
							    65535, /* _iMaximum */ 
							    "Brightness of the Image to grab", /* _strDescription */
							    "Image" /* _strCategory */);

		clsIntParameterHue = addIntParameter("Hue", /* _strName */
						     "Image Hue", /* _strLabel */
						     32768, /* _iValue */
						     0, /* _iMinimum */
						     65535, /* _iMaximum */ 
						     "Hue of the Image to grab", /* _strDescription */
						     "Image" /* _strCategory */);

		clsIntParameterColour = addIntParameter("Colour", /* _strName */
							"Image Colour", /* _strLabel */
							32768, /* _iValue */
							0, /* _iMinimum */
							65535, /* _iMaximum */ 
							"Colour of the Image to grab", /* _strDescription */
							"Image" /* _strCategory */);

		clsIntParameterContrast = addIntParameter("Contrast", /* _strName */
							  "Image Contrast", /* _strLabel */
							  32768, /* _iValue */
							  0, /* _iMinimum */
							  65535, /* _iMaximum */ 
							  "Contrast of the Image to grab", /* _strDescription */
							  "Image" /* _strCategory */);



#endif

		deviceID = -1;
		fImageRatio = .75;
		bShowOutput = false;
	    }

	protected:

	    void init(){
		fImageRatio = .75;
		iRoundCounter = 0;
		bFirstTime = true;

#ifdef IQRMODULE	
		imgWidth = clsIntParameterImgWidth->getValue(); 
		imgHeight = clsIntParameterImgheight->getValue(); 
		iBrightness = clsIntParameterBrightness->getValue(); 
		iHue = clsIntParameterHue->getValue(); 
		iColour = clsIntParameterColour->getValue(); 
		iContrast = clsIntParameterContrast->getValue(); 
		strDeviceName = clsStringParameterDeviceName->getValue(); 
		bShowOutput = clsBoolParameterShowOutput->getValue(); 
#else
		iBrightness = 32768;
		iHue = 32768;
		iColour = 32768;
		iContrast = 32768;
//	imgWidth = 320; imgHeight = 240;
		imgWidth = 160; imgHeight = 120;
//	imgWidth = 80; imgHeight = 60;
//    imgWidth = 176; imgHeight = 144;
//    imgWidth = 352; imgHeight = 288;
//    imgWidth = 640; imgHeight = 480;
//	imgWidth = 48; imgHeight = 32;

/*
  sqcif: 128x96
  qsif: 160x120
  qcif: 176x144
  sif: 320x240
  cif: 352x288
  vga: 640x480
*/
		bShowOutput = true;
		strDeviceName = "/dev/video0";
#endif

	    }

	    inline unsigned long int getPixel(unsigned long ir, unsigned long ig, unsigned long ib) {
		if(iScreenDepth == 24){
		    return  (0x00ff0000 & (ib << 16)) |
			(0x0000ff00 & (ig  << 8)) |
			(0x000000ff & (ir));
		
		} else if(iScreenDepth == 16){
		    return ((ib >>3)<<11)+((ig >>2)<<5)+((ir >>3));
		}
		return 0;
	    }

#ifdef IQRMODULE
	    ClsIntParameter* clsIntParameterImgWidth;
	    ClsIntParameter* clsIntParameterImgheight;
	    ClsIntParameter* clsIntParameterBrightness;
	    ClsIntParameter* clsIntParameterHue;
	    ClsIntParameter* clsIntParameterColour;
	    ClsIntParameter* clsIntParameterContrast;
	    ClsStringParameter* clsStringParameterDeviceName;
	    ClsBoolParameter* clsBoolParameterShowOutput;
#endif

	    ClsXWin clsXWin;

	    /* parameters */
	    int imgWidth,imgHeight;
	    int iBrightness;
	    int iHue;
	    int iColour;
	    int iContrast;
	    bool bShowOutput;
	    int iPalette;
	    string strDeviceName;
	    /* --------------- */

	    unsigned long int iScreenDepth;
	    int deviceID; 
	    double fImageRatio;

	    long int iRoundCounter;
	
	    int iNrCells; /* empirical */

	    int iGroupWidth; /* hypothetical */
	    int iGroupHeight; /* hypothetical */
	    int iHPace, iVPace;


//	    struct video_capability sVideo_capability;
//	    struct video_window sVideo_window;
//	    struct video_picture sVideo_picture;
//	    struct video_mmap sVideo_mmap_0, sVideo_mmap_1;
//	    struct video_mbuf sVideo_mbuf;

	    bool bFirstTime;
//	    char *pcCameraBuffer;


	    double fCPS;
	    
	    double fStartTime, fArrivalTime, fCurrentTime;

	    struct timeval sTimeval;
	    struct timezone sTimezone;
	    int iSleep;


	};
    }

#endif
