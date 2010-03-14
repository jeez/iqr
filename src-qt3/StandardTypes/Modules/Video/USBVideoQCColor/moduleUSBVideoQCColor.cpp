#include "moduleUSBVideoQCColor.hpp"

#ifdef IQRMODULE
#ifndef USE_USBVIDEO_AS_BASECLASS
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleUSBVideoQCColor,
			  "USB color quickcam module")
#endif
#include <cameras/ModuleIcon_CameraUSB.h>
#include <Common/Helper/iqrUtils.h>
#endif



#ifdef IQRMODULE
iqrcommon::moduleUSBVideoQCColor::moduleUSBVideoQCColor() : ClsModule() {
#else
    iqrcommon::moduleUSBVideoQCColor::moduleUSBVideoQCColor() {
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
					     65535, /* _iValue */
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


	clsStateVariableVideoOutputRed = addOutputToGroup("videoOutputRed", "Video Output Red");
	clsStateVariableVideoOutputGreen = addOutputToGroup("videoOutputGreen", "Video Output Green");
	clsStateVariableVideoOutputBlue = addOutputToGroup("videoOutputBlue", "Video Output Blue");
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
    moduleIcon iqrcommon::moduleUSBVideoQCColor::getIcon() {
//	moduleIcon mi(DSCP92_small_png_data, DSCP92_small_png_len, 3 ,5);
	moduleIcon mi(ModuleIcon_CameraUSB_png_data,  ModuleIcon_CameraUSB_png_len, 3 ,5);
	return mi;
    }
#endif

    iqrcommon::moduleUSBVideoQCColor::~moduleUSBVideoQCColor(){ 
	cleanup();
    }

    void iqrcommon::moduleUSBVideoQCColor::init(){
	cout << "moduleUSBVideoQCColor::init()" << endl;


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

	if((clsStateVariableVideoOutputRed->getStateArray().getWidth() != clsStateVariableVideoOutputGreen->getStateArray().getWidth()) ||
	   (clsStateVariableVideoOutputRed->getStateArray().getWidth() != clsStateVariableVideoOutputBlue->getStateArray().getWidth())){
	    throw ModuleError(string("Module \"") + label() + "\": All Video input groups must be of equal size");
	}

#ifdef IQRMODULE	
	iNrCells = clsStateVariableVideoOutputRed->getStateArray().getWidth();
	iGroupWidth = (int)(sqrt((double)iNrCells / fImageRatio)); 
	iGroupHeight = iNrCells/iGroupWidth; 
	iHPace = (int)ceil((double)sVideo_window.width / (double)iGroupWidth);
	iVPace = (int)ceil((double)sVideo_window.height / (double)iGroupHeight);

//	cout << "iHPace, iVPace: " << iHPace << ", " << iVPace << endl;
//	cout << "iGroupWidth, iGroupHeight: " << iGroupWidth << ", " << iGroupHeight << endl;
#endif



    }


    void iqrcommon::moduleUSBVideoQCColor::openDevice(string strDeviceName) {
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

    void iqrcommon::moduleUSBVideoQCColor::closeDevice() {
//	cout << "moduleUSBVideoQCColor::closeDevice(void)" << endl;
//	cout << "device_fd: " << device_fd << endl;
	errno = 0;
	close(device_fd);
	if(errno){
	    cerr << "\tERROR closing: " << strerror(errno) << endl;
	}
	device_fd = -1;
//TEMP	sleep(2);
    }


    void iqrcommon::moduleUSBVideoQCColor::setPictureProperties() {
	sVideo_picture.brightness = iBrightness;
	sVideo_picture.hue = iHue;       
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



    void iqrcommon::moduleUSBVideoQCColor::printDeviceInfo() {
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

    
    int iqrcommon::moduleUSBVideoQCColor::mapMemory(void) {
	struct video_mbuf vidbuf;
	struct video_mmap vm;
	int numframe = 0;

	vm.format = sVideo_picture.palette;
	vm.frame  = numframe;
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

	errno = 0;
	ioctl(device_fd, VIDIOCMCAPTURE, &vm);
	ioctl(device_fd, VIDIOCSYNC, &numframe);
	
	if(errno){
	    cerr << "\tmmap ERROR: " << strerror(errno) << endl;
	}



	return 0;
    }

    void iqrcommon::moduleUSBVideoQCColor::cleanup(){
//	cout << "moduleUSBVideoQCColor::cleanup()" << endl;


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

    void iqrcommon::moduleUSBVideoQCColor::update(){
//    cout << "moduleUSBVideoQCColor::update()" << endl;

	iCounter++;  

	unsigned long value;
	int ir, ig, ib;
	unsigned long int j;

	if(mapMemory()){
	    return;
	}

#ifdef IQRMODULE
	StateArray &clsStateArrayOutRed = clsStateVariableVideoOutputRed->getStateArray();
	StateArray &clsStateArrayOutGreen = clsStateVariableVideoOutputGreen->getStateArray();
	StateArray &clsStateArrayOutBlue = clsStateVariableVideoOutputBlue->getStateArray();
    	int ii=0;
#endif


    
	j = 0;
	for(int iY=0; iY < imgHeight; iY++) {
//	    cout << "iY: " << iY << endl;
	    for(int iX=0; iX < imgWidth; iX++){
//		cout << j << ":";
		ir = pcCameraBuffer[j++];
		ig = pcCameraBuffer[j++];
		ib = pcCameraBuffer[j++];
#ifdef IQRMODULE
		if(iX%iHPace == 0 && iY%iVPace == 0){
		    clsStateArrayOutRed[0][ii] = (double)ir / 256.;
		    clsStateArrayOutGreen[0][ii] = (double)ig / 256.;
		    clsStateArrayOutBlue[0][ii] = (double)ib / 256.;
		    ii++;
		}
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



	memcpy(pcRGBBuffer_last, pcRGBBuffer, iRGBBufferSize);
//	cout << "iRGBBufferSize: " << iRGBBufferSize << endl;
//	cout << "iVidbufSize: " << iVidbufSize << endl;



	if(bShowOutput){
	    clsXWin.showImage();
	}



    }


    void iqrcommon::moduleUSBVideoQCColor::writePPM(const char *filename) {
	FILE *fp = fopen(filename, "w");

	fprintf(fp, "P3 %d %d 255\n", sVideo_window.width, sVideo_window.height);
	for(int i = 0; i < iRGBBufferSize; i++)
	{
	    fprintf(fp, "%d\n", pcCameraBuffer[i]);
	}
	fclose(fp);
    }                                                                               



    
    
    
    
    
