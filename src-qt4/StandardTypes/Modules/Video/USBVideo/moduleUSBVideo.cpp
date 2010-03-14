#include "moduleUSBVideo.hpp"

#include "ccvt-0.3/ccvt.h"

#ifdef IQRMODULE
#ifndef USE_USBVIDEO_AS_BASECLASS
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleUSBVideo,
			  "USB video module")
#endif
#include <cameras/ModuleIcon_CameraUSB.h>
#include <Common/Helper/iqrUtils.h>
#endif


#define SHIFT  1000.




    
    void myYUV2RGB(int imgWidth, int imgHeight, unsigned char* yuv, unsigned char* rgb){
    
    int v_start = imgHeight*imgWidth;
    int u_start = imgHeight*imgWidth + imgHeight*imgWidth/4;
//    unsigned long value;
    int ir, ig, ib;
    unsigned long int i, j;
    double yValue, uValue,vValue;
    
    j = i = 0;
    for(int iY=0; iY < imgHeight; iY++) {
	for(int iX=0; iX < imgWidth; iX++){
	    yValue = yuv[i++];
	    int iIncr = (int)(floor((double)iY /2.0) * imgWidth/2 + floor((double)iX /2.0));
	    vValue = yuv[v_start + iIncr];
	    uValue = yuv[u_start + iIncr];
	    
	    ir = lrint((1.164 * (yValue - 16)) + (2.018 * (vValue - 128)));
	    ig = lrint((1.164 * (yValue - 16)) - (0.813 * (uValue - 128)) - (0.391 * (vValue - 128)));
	    ib = lrint((1.164 * (yValue - 16)) + (1.596 * (uValue - 128)));
	    
	    ir=ir>255 ? 255:ir;
	    ig=ig>255 ? 255:ig;
	    ib=ib>255 ? 255:ib;
	    
	    ir=ir<0 ? 0 : ir;
	    ig=ig<0 ? 0 : ig;
	    ib=ib<0 ? 0 : ib;
	    
	    rgb[j++] = ib;
	    rgb[j++] = ir;
	    rgb[j++] = ig;
	}
    }
}





#ifdef IQRMODULE
//iqrcommon::moduleUSBVideo::moduleUSBVideo() : ClsModule() {
iqrcommon::moduleUSBVideo::moduleUSBVideo()  : ClsThreadModule() {
#else
    iqrcommon::moduleUSBVideo::moduleUSBVideo() {
#endif

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
						    31744, /* _iValue */
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

	clsIntParameterAutoGain = addIntParameter("AutoGain", /* _strName */
						  "Camera AutoGain", /* _strLabel */
						  -1, /* _iValue */
						  -1, /* _iMinimum */
						  65535, /* _iMaximum */ 
						  "AutoGain (-1 for automatic)", /* _strDescription */
						  "Image" /* _strCategory */);


	clsStateVariableVideoOutput = addOutputToGroup("videoOutput", "Video Output");
#endif


/* defaults 
   iBrightness = 31744;
   iHue = 65535;
   iColour = 32768;
   iContrast = 32768;
*/

	device_fd = -1;
	f1 = 0.;
	fImageRatio = .75;
	iCounter = 0;
	pcCameraBuffer = NULL;
	pcRGBBuffer = NULL;
	pcRGBBuffer_last = NULL;
	bShowOutput = false;
    }

#ifdef IQRMODULE
    moduleIcon iqrcommon::moduleUSBVideo::getIcon() {
//	moduleIcon mi(DSCP92_small_png_data, DSCP92_small_png_len, 3 ,5);
	moduleIcon mi(ModuleIcon_CameraUSB_png_data,  ModuleIcon_CameraUSB_png_len, 3 ,5);
	return mi;
    }
#endif

    iqrcommon::moduleUSBVideo::~moduleUSBVideo(){ 
	cleanup();
    }

    void iqrcommon::moduleUSBVideo::init(){
	cout << "moduleUSBVideo::init()" << endl;


	bFirstTime = true;
#ifdef IQRMODULE	
	imgWidth = clsIntParameterImgWidth->getValue(); 
	imgHeight = clsIntParameterImgheight->getValue(); 
	iBrightness = clsIntParameterBrightness->getValue(); 
	iHue = clsIntParameterHue->getValue(); 
	iColour = clsIntParameterColour->getValue(); 
	iContrast = clsIntParameterContrast->getValue(); 
	iAutoGain = clsIntParameterAutoGain->getValue(); 
	string strDeviceName = clsStringParameterDeviceName->getValue(); 
	bShowOutput = clsBoolParameterShowOutput->getValue(); 
#else
	iBrightness = 31744;
	iHue = 65535;
	iColour = 32768;
	iContrast = 32768;
	iAutoGain = -1; /* 0..65535 < 0: automatic mode */
	imgWidth = 320; imgHeight = 240;
//	imgWidth = 160; imgHeight = 120;
//     imgWidth = 176; imgHeight = 144;
//    imgWidth = 352; imgHeight = 288;
//    imgWidth = 640; imgHeight = 480;

/*
  sqcif: 128x96
  qsif: 160x120
  qcif: 176x144
  sif: 320x240
  cif: 352x288
  vga: 640x480
*/

	bShowOutput = true;
	string strDeviceName = "/dev/video0";
#endif

	openDevice(strDeviceName);
	    
/*
	iRGBBufferSize = sVideo_picture.depth / 8 * sVideo_window.width * sVideo_window.height;

	this will crash if sVideo_picture.dept==12 !
	iRGBBufferSize = (int)((double)sVideo_picture.depth / 8. * 
	    (double)sVideo_window.width * (double)sVideo_window.height);
*/

	iRGBBufferSize = 3 * sVideo_window.width * sVideo_window.height;

//	cout << "iRGBBufferSize: " << iRGBBufferSize << endl;

	pcRGBBuffer = (unsigned char *)malloc(iRGBBufferSize);
	pcRGBBuffer_last = (unsigned char *)malloc(iRGBBufferSize);
	
/*
	cout << "sVideo_picture.depth: " << sVideo_picture.depth << endl;
	cout <<  "sVideo_window.width: " << sVideo_window.width  << endl;
	cout << "sVideo_window.height: " << sVideo_window.height << endl;
	cout << "iRGBBufferSize: " << iRGBBufferSize << endl;
*/

	printDeviceInfo();
	setPictureProperties();

	if(bShowOutput){
	    clsXWin.createWindow(imgWidth, imgHeight);
	    iScreenDepth = clsXWin.getScreenDepth();
	    if(iScreenDepth != 16 && iScreenDepth != 24){
		cerr << "Cannot screen depth of " << iScreenDepth << endl;
#ifdef IQRMODULE	
		throw ModuleError(string("Module \"") + getLabel() + "\": Cannot screen depth of " + iqrUtils::int2string(iScreenDepth));
#else
		exit(-1);
#endif
	    }
	}

#ifdef IQRMODULE	
	iNrCells = clsStateVariableVideoOutput->getStateArray().getWidth();
	iGroupWidth = (int)(sqrt((double)iNrCells / fImageRatio)); 
	iGroupHeight = iNrCells/iGroupWidth; 
	iHPace = (int)ceil((double)sVideo_window.width / (double)iGroupWidth);
	iVPace = (int)ceil((double)sVideo_window.height / (double)iGroupHeight);

//	cout << "iHPace, iVPace: " << iHPace << ", " << iVPace << endl;
//	cout << "iGroupWidth, iGroupHeight: " << iGroupWidth << ", " << iGroupHeight << endl;
#endif



    }


    void iqrcommon::moduleUSBVideo::openDevice(string strDeviceName) {
	errno = 0;

	device_fd = open(strDeviceName.c_str(), O_RDWR);
	if(device_fd < 0) {
	    cerr << "Device " << strDeviceName << " couldn't be opened" << endl;
	    cerr << "\tERROR: " << strerror(errno) << endl;
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: " + strerror(errno));
#endif
	}

	ioctl(device_fd, VIDIOCGWIN, &sVideo_window);
	sVideo_window.x = 0;
	sVideo_window.y = 0;
	sVideo_window.width = imgWidth;
	sVideo_window.height = imgHeight;

	errno = 0;    
	ioctl(device_fd, VIDIOCSWIN, &sVideo_window);
	if(errno>0){
	    cerr << "ERROR setting size: " << strerror(errno) << endl;
	}


	/* set to what can be set */
	ioctl(device_fd, VIDIOCGWIN, &sVideo_window);
	imgWidth = sVideo_window.width;
	imgHeight = sVideo_window.height;

	cout << "Using: "<< endl;
	cout << "\tWidth: " << imgWidth << endl;
	cout << "\tHeight: " << imgHeight << endl;

   
	ioctl(device_fd, VIDIOCGCAP, &sVideo_capability);
	ioctl(device_fd, VIDIOCGPICT, &sVideo_picture);
	iRGBBufferDepth = sVideo_picture.depth;


// 	if(sVideo_picture.depth != 24 && sVideo_picture.depth != 32){
// 	    cerr << "cannot handle depth of " << sVideo_picture.depth << endl;
// #ifdef IQRMODULE	
// 	    throw ModuleError(string("Module \"") + getLabel() + "\": cannot handle depth of" + iqrUtils::int2string(sVideo_picture.depth));
// #endif
// 	}

// 	iPalette = sVideo_picture.palette;

// 	if(sVideo_picture.palette == VIDEO_PALETTE_RGB24){
// 	    cerr << "not handling VIDEO_PALETTE_RGB24 at the moment; quit" << endl;
// #ifdef IQRMODULE	
// 	    throw ModuleError(string("Module \"") + getLabel() + "\": not handling VIDEO_PALETTE_RGB24 at the moment");
// #endif
// 	} 
// 	else if(sVideo_picture.palette == VIDEO_PALETTE_YUV420P){
// 	    /* we're fine */
// 	} 
// 	else {
// 	    cerr << "Unknown palette nr: " << sVideo_picture.palette << "; quit" << endl;
// #ifdef IQRMODULE	
// 	    throw ModuleError(string("Module \"") + getLabel() + "\": Unknown palette");
// #endif
// 	}

    }

    void iqrcommon::moduleUSBVideo::closeDevice() {
//	cout << "moduleUSBVideo::closeDevice(void)" << endl;
//	cout << "device_fd: " << device_fd << endl;
	errno = 0;
	close(device_fd);
	if(errno){
	    cerr << "\tERROR closing: " << strerror(errno) << endl;
	}
	device_fd = -1;
//TEMP	sleep(2);
    }


    void iqrcommon::moduleUSBVideo::setPictureProperties() {
	sVideo_picture.brightness = iBrightness;
	sVideo_picture.hue = 332767 * .5;//iHue;       
	sVideo_picture.colour = iColour;    
	sVideo_picture.contrast = iContrast;
	ioctl(device_fd, VIDIOCSPICT, &sVideo_picture);


/* let's forget about gain control form the moment
   videodev2.h would support it natively 
	int agc;
	agc = iAutoGain;
	ioctl(device_fd, VIDIOCPWCSAGC, &agc);
*/
    }



    void iqrcommon::moduleUSBVideo::printDeviceInfo() {
	cout << "    *** Camera Info ***" << endl;;
	cout << "Name:           " << sVideo_capability.name << endl;
	cout << "Type:           " << sVideo_capability.type << endl;
	cout << "Minimum Width:  " << sVideo_capability.minwidth << endl;
	cout << "Maximum Width:  " << sVideo_capability.maxwidth << endl;
	cout << "Minimum Height: " << sVideo_capability.minheight << endl;
	cout << "Maximum Height: " << sVideo_capability.maxheight << endl;
     
	cout << "X:              " << sVideo_window.x << endl;
	cout << "Y:              " << sVideo_window.y << endl;
	cout << "Width:          " << sVideo_window.width << endl;
	cout << "Height:         " << sVideo_window.height << endl;

	cout << "brightness:     " << sVideo_picture.brightness << endl;
	cout << "hue:            " << sVideo_picture.hue << endl;       
	cout << "colour:         " << sVideo_picture.colour << endl;    
	cout << "contrast:       " << sVideo_picture.contrast << endl;  
	cout << "whiteness:      " << sVideo_picture.whiteness << endl; 
	cout << "depth:          " << sVideo_picture.depth << endl;     
	cout << "palette:        " << sVideo_picture.palette << endl;   

	if(sVideo_capability.type & VID_TYPE_MONOCHROME){
	    cout << "Color:         no" << endl;
	}
	else {
	    cout << "Color:         yes" << endl;	
	}

/* let's forget about framereate form the moment
   videodev2.h would support it natively (?)
	if (sVideo_window.flags & PWC_FPS_FRMASK){
	    cout << "framerate:       " << ((sVideo_window.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT) << endl;
	} else {
	    cout << "can't get framerate" << endl;
	}
*/	    
	cout << "    ***************" << endl;;
    }

    
    int iqrcommon::moduleUSBVideo::mapMemory(void) {
	struct video_mbuf vidbuf;
	struct video_mmap vm;
	int numframe = 0;

	vm.format = sVideo_picture.palette;
	vm.frame  = 0;
	vm.width  = sVideo_window.width;
	vm.height = sVideo_window.height;

	if(bFirstTime){
	    errno = 0;
	    ioctl(device_fd, VIDIOCGMBUF, &vidbuf);
	    pcCameraBuffer = (unsigned char *) mmap(NULL, vidbuf.size, PROT_READ, MAP_SHARED, device_fd, 0);	    
	    iVidbufSize = vidbuf.size;
	    if(errno){
		cerr << "\tmmap ERROR: " << strerror(errno) << endl;
	    }
	    
	    ioctl(device_fd, VIDIOCMCAPTURE, &vm);
	    bFirstTime = false;
	}

	ioctl(device_fd, VIDIOCMCAPTURE, &vm);
	ioctl(device_fd, VIDIOCSYNC, &numframe);
	return 0;
    }

    void iqrcommon::moduleUSBVideo::cleanup(){
//	cout << "moduleUSBVideo::cleanup()" << endl;


	if(device_fd>=0){
	    errno = 0;
	    munmap(pcCameraBuffer, iVidbufSize);
	    if(errno){
		cerr << "\tERROR: " << strerror(errno) << endl;
	    }
	    
	    bFirstTime = true;
	    
	    closeDevice();
	}

	if(bShowOutput){
	    clsXWin.destroyWindow();
	}

/* crash...*/
	if(pcRGBBuffer!=NULL){
	    free(pcRGBBuffer);
	}

	
/* crash... */
	if(pcRGBBuffer_last!=NULL){
	    free(pcRGBBuffer_last);
	}



	pcRGBBuffer = NULL;
	pcRGBBuffer_last = NULL;
	pcCameraBuffer = NULL;

	f1 = 0.;
	iCounter = 0;
    }

    void iqrcommon::moduleUSBVideo::update(){
//    cout << "moduleUSBVideo::update()" << endl;

	iCounter++;  

	unsigned long value;
	int ir, ig, ib;
	unsigned long int j;

	if(mapMemory()){
	    return;
	}

#ifdef IQRMODULE
	StateArray &clsStateArrayOut = clsStateVariableVideoOutput->getStateArray();
    	int ii=0;
#endif

	if(sVideo_picture.depth == 12){
//	    myYUV2RGB( imgWidth, imgHeight, pcCameraBuffer, pcRGBBuffer);
	    ccvt_420p_bgr24( imgWidth, imgHeight, pcCameraBuffer, pcRGBBuffer);
	} else if(sVideo_picture.depth == 24){
	    ccvt_420p_bgr24( imgWidth, imgHeight, pcCameraBuffer, pcRGBBuffer);
	} else if(sVideo_picture.depth == 32 ){
	    ccvt_420p_bgr32( imgWidth, imgHeight, pcCameraBuffer, pcRGBBuffer);
	}

	qmutexThread->lock(); //new

	clsStateArrayOut[0] = 0.;
    
	j = 0;
	for(int iY=0; iY < imgHeight; iY++) {
//	    cout << "iY: " << iY << endl;
	    for(int iX=0; iX < imgWidth; iX++){
//		cout << j << ":";
		ir = pcRGBBuffer[j++];
		ig = pcRGBBuffer[j++];
		ib = pcRGBBuffer[j++];


#define NEW_SUBSAMPLING
#ifdef IQRMODULE
#ifdef OLD_SUBSAMPLING
  		if(iX%iHPace == 0 && iY%iVPace == 0){
  //		    clsStateArrayOut[0][ii] = (double)(ir + ig + ib) / ( 3. * 256.) ; /* gray */
  		    clsStateArrayOut[0][ii] = (double)(ir) / ( 256.) ; /* red */
  		    ii++;
  		}
#endif

#ifdef NEW_SUBSAMPLING
		int iCoord = int(iY/iVPace) * iGroupWidth + int(iX/iHPace);
		clsStateArrayOut[0][iCoord ] += (double)(ir) / ( 256.) + SHIFT; 
#endif
#endif
		if(bShowOutput){
//temp		    value = getPixel(ir, ig, ib);
		    value = getPixel(ir, ig, ib);
//		    cout << value << endl;
		    clsXWin.putPixel(iX, iY, value);
		}
	    }
	}
//	    cout << endl;



#ifdef NEW_SUBSAMPLING
 	for(unsigned int ii=0; ii<clsStateArrayOut.getWidth(); ii++){
 	    double fFactor = (int)(clsStateArrayOut[0][ii] / SHIFT);
 	    clsStateArrayOut[0][ii] = (clsStateArrayOut[0][ii] - fFactor * SHIFT) / fFactor;
 	}
#endif

	qmutexThread->unlock(); 


	memcpy(pcRGBBuffer_last, pcRGBBuffer, iRGBBufferSize);


	if(bShowOutput){
	    clsXWin.showImage();
	}



    }


    void iqrcommon::moduleUSBVideo::writePPM(const char *filename) {
	FILE *fp = fopen(filename, "w");

	fprintf(fp, "P3 %d %d 255\n", sVideo_window.width, sVideo_window.height);
	for(int i = 0; i < iRGBBufferSize; i++)
	{
	    fprintf(fp, "%d\n", pcCameraBuffer[i]);
	}
	fclose(fp);
    }                                                                               



    
    
    
    
    
