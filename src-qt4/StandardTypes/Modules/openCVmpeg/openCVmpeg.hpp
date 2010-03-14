#ifndef MPEG_HPP
#define MPEG_HPP

//needed?? #include <sys/ioctl.h>
//needed?? #include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <string.h>
//needed?? #include <termios.h>
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


//#include <Common/Item/threadModule.hpp>
#include <Common/Item/module.hpp>

#define CONTINUOUS "Continuous play"
#define SINGLE_LOOP "Single run"
#define fImageRatio .75

extern int errno;

using namespace std;

namespace iqrcommon {

    class openCVmpeg : public ClsModule {
//    class openCVmpeg : public ClsThreadModule {

    public:

	openCVmpeg();
	~openCVmpeg();

	void init();
	void update();
	void cleanup();
	
    private:


	void reload();

	int iGroupWidth, iGroupHeight, iNrCells;
	int memorySize;
	bool *frames;

	CvCapture* capture;
	IplImage* iplImgVideoFrame;
      IplImage* iplImgScaledOutput;

      IplImage* iplImgLuminance;
      IplImage* iplImgHSV;

      IplImage* iplImgRH;
      IplImage* iplImgGS;
      IplImage* iplImgBV;



	ClsPathParameter *clsPathParameterVideoFile;
	ClsBoolParameter *clsBoolParameterShowOutput;
	ClsBoolParameter* clsBoolParameterHSV;
	ClsBoolParameter* clsBoolParameterMonochrome;
	ClsOptionsParameter *objectShape;

	StateVariablePtr *StateVariableController;

	ClsStateVariable* clsStateVariableVideoOutputR_H_G;
	ClsStateVariable* clsStateVariableVideoOutputG_S;
	ClsStateVariable* clsStateVariableVideoOutputB_V;

	ClsStateVariable *clsStateVariableEndOutput;

	bool ShowOutput;
	string strVideoFileName;
      bool bMonochrome;
      bool bHSV;

    };
}
#endif

