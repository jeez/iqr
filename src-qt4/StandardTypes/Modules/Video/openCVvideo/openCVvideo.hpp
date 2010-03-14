#ifndef MPEG_HPP
#define MPEG_HPP

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#endif

//MPEG LIBRARY
//#define GLOBAL


#include <Common/Item/threadModule.hpp>

#define fImageRatio .75

extern int errno;

using namespace std;

namespace iqrcommon {

    class openCVvideo : public ClsThreadModule {

    public:

	openCVvideo();
	~openCVvideo();

	void init();

	void update();

	void cleanup();
	
    private:



	/* parameter */
	
	int iGroupWidth, iGroupHeight, iNrCells;
      //--	int memorySize;
      //--	bool *frames;


	CvCapture* capture;
	IplImage* iplImgVideoFrame;
      IplImage* iplImgScaledOutput;

      IplImage* iplImgLuminance;
      IplImage* iplImgHSV;

      IplImage* iplImgRH;
      IplImage* iplImgGS;
      IplImage* iplImgBV;


	ClsBoolParameter *clsBoolParameterShowOutput;
	ClsBoolParameter* clsBoolParameterHSV;
	ClsBoolParameter* clsBoolParameterMonochrome;


	ClsStateVariable* clsStateVariableVideoOutputR_H_G;
	ClsStateVariable* clsStateVariableVideoOutputG_S;
	ClsStateVariable* clsStateVariableVideoOutputB_V;



      bool ShowOutput;
      bool bMonochrome;
      bool bHSV;

    };
}
#endif

