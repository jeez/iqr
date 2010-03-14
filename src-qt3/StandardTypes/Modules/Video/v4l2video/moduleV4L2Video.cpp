
//#define MODULE_COLOR
//#define MODULE_MONOCHROME
 


#include "moduleV4L2Video.hpp"

#include "ccvt-0.3/ccvt.h"

#ifdef IQRMODULE
#ifndef USE_USBVIDEO_AS_BASECLASS

#ifdef MODULE_MONOCHROME
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleV4L2Video,
			  "video4linux2 mono module")
#endif
#ifdef MODULE_COLOR
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleV4L2Video,
			  "video4linux2 color module")
#endif
#endif
#include <cameras/ModuleIcon_CameraUSB.h>
#include <Common/Helper/iqrUtils.h>
#endif


#define SHIFT  1000.




    


//iqrcommon::moduleV4L2Video::moduleV4L2Video() : ClsModule() {
    iqrcommon::moduleV4L2Video::moduleV4L2Video()  : ClsThreadModule() {


	clsStringParameterDeviceName = addStringParameter("DeviceName", /* _strName */ 
							  "Video Device", /* _strLabel */
							  "/dev/video0", /* _strValue */
							  true, /* _bEditable */
							  false, /* _bLong */
							  "Path to Video Device", /* _strDescription */
							  "Device" /* _strCategory */);

	clsIntParameterInput = addIntParameter("Input", /* _strName */
					       "Camera input", /* _strLabel */
					       0, /* _iValue */
					       0, /* _iMinimum */
					       10, /* _iMaximum */ 
					       "Camera input", /* _strDescription */
					       "Device" /* _strCategory */);


	clsIntParameterCPS = addIntParameter("CPS", /* _strName */
					       "Cycles per second", /* _strLabel */
					       25, /* _iValue */
					       -1, /* _iMinimum */
					       100, /* _iMaximum */ 
					       "Cycles per second", /* _strDescription */
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
						    "% image Brightness", /* _strLabel */
						    50, /* _iValue */
						    0, /* _iMinimum */
						    100, /* _iMaximum */ 
						    "% brightness of the Image to grab", /* _strDescription */
						    "Image" /* _strCategory */);


// 	clsIntParameterHue = addIntParameter("Hue", /* _strName */
// 					     "Image Hue", /* _strLabel */
// 					     32768, /* _iValue */
// 					     0, /* _iMinimum */
// 					     65535, /* _iMaximum */ 
// 					     "Hue of the Image to grab", /* _strDescription */
// 					     "Image" /* _strCategory */);

	clsIntParameterSaturation = addIntParameter("Saturation", /* _strName */
						    "% image saturation", /* _strLabel */
						    50, /* _iValue */
						    0, /* _iMinimum */
						    100, /* _iMaximum */ 
						    "% saturaion of the Image to grab", /* _strDescription */
						    "Image" /* _strCategory */);

	clsIntParameterContrast = addIntParameter("Contrast", /* _strName */
						  "% image Contrast ", /* _strLabel */
						  50, /* _iValue */
						  0, /* _iMinimum */
						  100, /* _iMaximum */ 
						  "% contrast of the Image to grab", /* _strDescription */
						  "Image" /* _strCategory */);

	clsIntParameterAutoGain = addIntParameter("AutoGain", /* _strName */
						  "Camera AutoGain", /* _strLabel */
						  1, /* _iValue */
						  0, /* _iMinimum */
						  1, /* _iMaximum */ 
						  "AutoGain (-1 for automatic)", /* _strDescription */
						  "Image" /* _strCategory */);

#ifdef MODULE_COLOR
	clsBoolParameterHSV = addBoolParameter("HSV", /* _strName */ 
						      "HSV color space", /* _strLabel */
						      false, /* _bValue */
						      "HSV color space", /* _strDescription */
					      "Output" /* _strCategory */);
#endif

#ifdef MODULE_MONOCHROME
	clsStateVariableVideoOutput = addOutputToGroup("videoOutput", "Video Output");
#endif
#ifdef MODULE_COLOR
	clsStateVariableVideoOutputRed = addOutputToGroup("videoOutputRed", "Video Output Red/Hue");
	clsStateVariableVideoOutputGreen = addOutputToGroup("videoOutputGreen", "Video Output Green/Saturation");
	clsStateVariableVideoOutputBlue = addOutputToGroup("videoOutputBlue", "Video Output Blue/Value");
#endif



	device_fd = -1;
	fImageRatio = .75;
	bShowOutput = false;

	io = IO_METHOD_MMAP;
	buffers = NULL;
	n_buffers = 0;

	pcRGBBuffer = NULL;

	iv4l_sizeimage = -99;
	iPixelFormat = -99;
    }


moduleIcon iqrcommon::moduleV4L2Video::getIcon() {
//	moduleIcon mi(DSCP92_small_png_data, DSCP92_small_png_len, 3 ,5);
    moduleIcon mi(ModuleIcon_CameraUSB_png_data,  ModuleIcon_CameraUSB_png_len, 3 ,5);
    return mi;
}


iqrcommon::moduleV4L2Video::~moduleV4L2Video(){ 

}

void iqrcommon::moduleV4L2Video::init(){
//    cout << "moduleV4L2Video::init()" << endl;

	
    imgWidth = clsIntParameterImgWidth->getValue(); 
    imgHeight = clsIntParameterImgheight->getValue(); 
    string strDeviceName = clsStringParameterDeviceName->getValue(); 
    bShowOutput = clsBoolParameterShowOutput->getValue(); 
    iCPS = clsIntParameterCPS->getValue();
    
#ifdef MODULE_COLOR
    bHSV = clsBoolParameterHSV->getValue();
#endif

    openDevice(strDeviceName);
    checkDevice(strDeviceName);
    enumControls();
    enumFormats();
    selectStandard();
    selectInput();
    setPictureProperties();
    printDeviceInfo();


    setPixelformat();
    initDevice(strDeviceName);

    iRGBBufferSize = 3 * imgWidth * imgHeight; /* 3 because we use rgb24 */
    pcRGBBuffer = (unsigned char *)malloc(iRGBBufferSize);

	    

    if(bShowOutput){
	clsXWin.createWindow(imgWidth, imgHeight);
	iScreenDepth = clsXWin.getScreenDepth();
	if(iScreenDepth != 16 && iScreenDepth != 24){
	    cerr << "Cannot screen depth of " << iScreenDepth << endl;
	    throw ModuleError(string("Module \"") + getLabel() + "\": Cannot screen depth of " + iqrUtils::int2string(iScreenDepth));
	}
    }

#ifdef MODULE_MONOCHROME
    iNrCells = clsStateVariableVideoOutput->getStateArray().getWidth();
#endif
#ifdef MODULE_COLOR
    iNrCells = clsStateVariableVideoOutputRed->getStateArray().getWidth();
    	if(clsStateVariableVideoOutputRed->getStateArray().getWidth() != clsStateVariableVideoOutputBlue->getStateArray().getWidth() ||
	    clsStateVariableVideoOutputRed->getStateArray().getWidth() != clsStateVariableVideoOutputGreen->getStateArray().getWidth()){
	    throw ModuleError(string("Module \"") + getLabel() + "\": All output groups need to be the same size");
	}
#endif

    iGroupWidth = (int)(sqrt((double)iNrCells / fImageRatio)); 
    iGroupHeight = iNrCells/iGroupWidth; 
    iHPace = (int)ceil((double)imgWidth / (double)iGroupWidth);
    iVPace = (int)ceil((double)imgHeight / (double)iGroupHeight);

//	cout << "iHPace, iVPace: " << iHPace << ", " << iVPace << endl;
//	cout << "iGroupWidth, iGroupHeight: " << iGroupWidth << ", " << iGroupHeight << endl;

    iCounter = 0;
    gettimeofday(&tim0, &tz);
    startCapturing ();


    fStartTime = sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1000000.;

}

void iqrcommon::moduleV4L2Video::openDevice(string strDeviceName) {
//    cout << "moduleV4L2Video::openDevice(string strDeviceName)" << endl;
    struct stat st; 
	
    if (-1 == stat (strDeviceName.c_str(), &st)) {
	throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: Cannot identify \"" + strDeviceName + "\": " + strerror (errno));
    }
	
    if (!S_ISCHR (st.st_mode)) {
	throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: \"" + strDeviceName + "\" is no device" );
    }
	
    device_fd = open (strDeviceName.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);
	
    if (-1 == device_fd) {
	throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: Cannot open \"" + strDeviceName + "\": " + strerror (errno));
    }
}

///////////////////////////////////////
void iqrcommon::moduleV4L2Video::selectStandard() {
    v4l2_std_id std_id;
    ioctl(device_fd, VIDIOC_G_STD, &std_id);
    if (-1 == ioctl(device_fd, VIDIOC_S_STD, &std_id)) {
	perror ("VIDIOC_S_STD");
    }
}

//////////////////////////////////////////
void iqrcommon::moduleV4L2Video::selectInput() {
//    cout << "moduleV4L2Video::selectInput()" << endl;
    int iIndex = clsIntParameterInput->getValue();
//    cout << "index: " << iIndex << endl;
    if (-1 == ioctl (device_fd, VIDIOC_S_INPUT, &iIndex)) {
	perror ("VIDIOC_S_INPUT");
	string strInputs = listInputs();
	closeDevice();
	throw ModuleError(string("Module \"") + getLabel() + "\": Cannot set input to " + iqrUtils::int2string(iIndex) + "\navailable inputs are:\n" + strInputs);
    }
}

void iqrcommon::moduleV4L2Video::setPictureProperties() {
    
    iBrightness = clsIntParameterBrightness->getValue(); 
//       iHue = clsIntParameterHue->getValue(); 
    iSaturation = clsIntParameterSaturation->getValue(); 
    iContrast = clsIntParameterContrast->getValue(); 
    iAutoGain = clsIntParameterAutoGain->getValue(); 
    
    setControl(V4L2_CID_BRIGHTNESS, iBrightness);
    setControl(V4L2_CID_CONTRAST, iContrast);
    setControl(V4L2_CID_SATURATION, iSaturation);
    setControl(V4L2_CID_AUTOGAIN, iAutoGain*100);

}


void iqrcommon::moduleV4L2Video::setControl(int iID, int iValuePercent){
//    cout << "moduleV4L2Video::setControl(int iID, int iValuePercent)" << endl;
    /* value are in % */

    
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
	   
	   
    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = iID;
	   

    if (-1 == ioctl (device_fd, VIDIOC_QUERYCTRL, &queryctrl)) {
	if (errno != EINVAL) {
	    perror ("VIDIOC_QUERYCTRL");
	} else {
	    printf ("iID is not supported\n");
	}
    } else {
	int iMax = queryctrl.maximum;
	       
	int iValue = (int)round((double)iValuePercent / 100. * (double)iMax);


	memset (&control, 0, sizeof (control));
	control.id = iID;
	control.value = iValue;
	if (-1 == ioctl (device_fd, VIDIOC_S_CTRL, &control)) {
	    perror ("VIDIOC_S_CTRL");
	}
    }
       	   
}



void iqrcommon::moduleV4L2Video::enumControls(){
//    cout << "moduleV4L2Video::enumControls()" << endl;

    struct v4l2_queryctrl queryctrl;

    memset (&queryctrl, 0, sizeof (queryctrl));
       
    for (queryctrl.id = V4L2_CID_BASE;
	 queryctrl.id < V4L2_CID_LASTP1;
	 queryctrl.id++) {
	if (0 == ioctl (device_fd, VIDIOC_QUERYCTRL, &queryctrl)) {
	    if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED){
		continue;
	    }
	       
	    cout << "Control: " << queryctrl.name <<
		" minimum: " << queryctrl.minimum << 
		" maximum: " << queryctrl.maximum << 
		" default_value: " << queryctrl.default_value << endl;
	       
	    if (queryctrl.type == V4L2_CTRL_TYPE_MENU){
		enumerate_menu ();
	    }
	} else {
	    if (errno == EINVAL){
		continue;
	    }
	       
	    perror ("VIDIOC_QUERYCTRL");
	    exit (EXIT_FAILURE);
	}
    }
}


void iqrcommon::moduleV4L2Video::enumFormats(){
//    cout << "moduleV4L2Video::enumFormats()" << endl;
    struct v4l2_fmtdesc fmtdesc;
       
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while (ioctl(device_fd, VIDIOC_ENUM_FMT, &fmtdesc) >= 0) {
	printf("\tPixelformat : %s", fcc2s(fmtdesc.pixelformat).c_str());
	if (fmtdesc.flags){
	    printf(" (compressed)");
	}
	printf("\n");
	printf("\tName        : %s\n", fmtdesc.description);
	printf("\n");
	fmtdesc.index++;
    }
}

string iqrcommon::moduleV4L2Video::listInputs(){
    stringstream ssI;
    struct v4l2_input input;
       
    input.index = 0;
    while (ioctl(device_fd, VIDIOC_ENUMINPUT, &input) >= 0) {
	ssI << "input: " << input.index << ", name: " << input.name << endl;
	input.index++;
    }
    return ssI.str();
}

void iqrcommon::moduleV4L2Video::printDeviceInfo() {
//    cout << "moduleV4L2Video::printDeviceInfo()" << endl;

    struct v4l2_input input;
    int index;
	
    if (-1 == ioctl (device_fd, VIDIOC_G_INPUT, &index)) {
	perror ("VIDIOC_G_INPUT");
	exit (EXIT_FAILURE);
    }
	
    memset (&input, 0, sizeof (input));
    input.index = index;
	
    if (-1 == ioctl (device_fd, VIDIOC_ENUMINPUT, &input)) {
	perror ("VIDIOC_ENUMINPUT");
	exit (EXIT_FAILURE);
    }
	
    printf ("Current input: %s\n", input.name);
}


    
void iqrcommon::moduleV4L2Video::cleanup(){
//    cout << "moduleV4L2Video::cleanup()" << endl;

    uninitDevice ();
    closeDevice ();

    if(bShowOutput){
	clsXWin.destroyWindow();
    }
}

void iqrcommon::moduleV4L2Video::update(){
//    cout << "moduleV4L2Video::update()" << endl;

    readFrame();
    
/* FOR TESTING THE CPS
    int iInterval = 100;
    if(iCounter%iInterval == 0){
	gettimeofday(&tim1, &tz);
	int iDeltaTime = (tim1.tv_sec-tim0.tv_sec);//+tim1.tv_usec-tim0.tv_usec;
	cout << "fps: " << (double)iInterval / (double)iDeltaTime  << endl;
	gettimeofday(&tim0, &tz);
    }
*/
  
    
    if(iCPS>0){
	/* jitter correction: */
	fArrivalTime = fStartTime + (double)iCounter / (double)iCPS;
	/* no jitter correction: */
	
	gettimeofday(&sTimevalCurrent, &sTimezone);
	fCurrentTime = sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1e6;
	iSleep = (int)((fArrivalTime-fCurrentTime)*1e6);
//	    cout << "fArrivalTime: " << fStartTime + (double)iRoundCounter / fCPS << endl;
//	    cout << "fCurrentTime: " << sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1e6 << endl;
//	    cout << "iSleep: " << iSleep << endl;
	if(iSleep>0){
	    usleep(iSleep);
	}
    }
    iCounter++;
}


void iqrcommon::moduleV4L2Video::processFrame(const void * pBuffer){
#ifdef MODULE_MONOCHROME
    StateArray &clsStateArrayOut = clsStateVariableVideoOutput->getStateArray();
#endif
#ifdef MODULE_COLOR
    StateArray &clsStateArrayOutRed = clsStateVariableVideoOutputRed->getStateArray();
    StateArray &clsStateArrayOutGreen = clsStateVariableVideoOutputGreen->getStateArray();
    StateArray &clsStateArrayOutBlue = clsStateVariableVideoOutputBlue->getStateArray();
#endif

    int ii=0;

    if(iPixelFormat == V4L2_PIX_FMT_RGB24){
	memcpy(pcRGBBuffer, pBuffer, iRGBBufferSize);
    }
    else if(iPixelFormat == V4L2_PIX_FMT_YUV420){
	ccvt_420p_bgr24( imgWidth, imgHeight, pBuffer, pcRGBBuffer);
    } 
    else if (iPixelFormat == V4L2_PIX_FMT_YUYV){
//	ccvt_yuyv_rgb32( imgWidth, imgHeight, pBuffer, pcRGBBuffer);
	myYUV422toRGB( imgWidth, imgHeight, (unsigned char*)pBuffer, pcRGBBuffer);
//	memcpy(pcRGBBuffer, pBuffer, iRGBBufferSize);
    }



    qmutexThread->lock(); //new

#ifdef MODULE_MONOCHROME
    clsStateArrayOut[0] = 0.;
#endif
#ifdef MODULE_COLOR
    clsStateArrayOutRed[0] = 0.;
    clsStateArrayOutGreen[0] = 0.;
    clsStateArrayOutBlue[0] = 0.;
#endif
    
    unsigned long value;
    int ir, ig, ib;
    float fH, fS, fV;
    unsigned long int j;

    j = 0;
    for(int iY=0; iY < imgHeight; iY++) {
//	    cout << "iY: " << iY << endl;
	for(int iX=0; iX < imgWidth; iX++){
//		cout << j << ":";
	    ir = pcRGBBuffer[j++];
	    ig = pcRGBBuffer[j++];
	    ib = pcRGBBuffer[j++];
//		j++;



	    int iCoord = int(iY/iVPace) * iGroupWidth + int(iX/iHPace);
#ifdef MODULE_MONOCHROME
	    clsStateArrayOut[0][iCoord ] += (double)(ir+ig+ib) / ( 3. * 256.) + SHIFT; 
#endif
#ifdef MODULE_COLOR
	    if(bHSV){
		RGBtoHSV( (float)ir/256., (float)ig/256., (float)ib/265., &fH, &fS, &fV );
		clsStateArrayOutRed[0][iCoord ] += fH / ( 360.) + SHIFT; 
		clsStateArrayOutGreen[0][iCoord ] += fS + SHIFT; 
		clsStateArrayOutBlue[0][iCoord ] += fV + SHIFT; 
	    } else{
		clsStateArrayOutRed[0][iCoord ] += (double)(ir) / ( 256.) + SHIFT; 
		clsStateArrayOutGreen[0][iCoord ] += (double)(ig) / ( 256.) + SHIFT; 
		clsStateArrayOutBlue[0][iCoord ] += (double)(ib) / ( 256.) + SHIFT; 
	    }
#endif
	    if(bShowOutput){
		value = getPixel(ir, ig, ib);
//		value = getPixel(ir, ib, ig);
		clsXWin.putPixel(iX, iY, value);
	    }
	}
    }



    for(unsigned int ii=0; ii<iNrCells; ii++){
#ifdef MODULE_MONOCHROME
	double fFactor = (int)(clsStateArrayOut[0][ii] / SHIFT);
	clsStateArrayOut[0][ii] = (clsStateArrayOut[0][ii] - fFactor * SHIFT) / fFactor;
#endif
#ifdef MODULE_COLOR
	double fFactor = (int)(clsStateArrayOutRed[0][ii] / SHIFT);
	clsStateArrayOutRed[0][ii] = (clsStateArrayOutRed[0][ii] - fFactor * SHIFT) / fFactor;
	fFactor = (int)(clsStateArrayOutGreen[0][ii] / SHIFT);
	clsStateArrayOutGreen[0][ii] = (clsStateArrayOutGreen[0][ii] - fFactor * SHIFT) / fFactor;
	fFactor = (int)(clsStateArrayOutBlue[0][ii] / SHIFT);
	clsStateArrayOutBlue[0][ii] = (clsStateArrayOutBlue[0][ii] - fFactor * SHIFT) / fFactor;
#endif
    }

    qmutexThread->unlock(); 


    if(bShowOutput){
	clsXWin.showImage();
    }


}

int iqrcommon::moduleV4L2Video::readFrame () {
//	cout << "moduleV4L2Video::readFrame ()" << endl;
    struct v4l2_buffer buf;
    unsigned int i;
	
	
    switch (io) {
    case IO_METHOD_READ:
	if (-1 == read (device_fd, buffers[0].start, buffers[0].length)) {
	    switch (errno) {
	    case EAGAIN:
		return 0;
		    
	    case EIO:
		/* Could ignore EIO, see spec. */
		    
		/* fall through */
		    
	    default:
		errno_exit ("read");
	    }
	}
	processFrame (buffers[0].start);
	break;
    case IO_METHOD_MMAP:
	CLEAR (buf);
	    
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	    
	if (-1 == xioctl (device_fd, VIDIOC_DQBUF, &buf)) {
	    switch (errno) {
	    case EAGAIN:
		return 0;
		    
	    case EIO:
		/* Could ignore EIO, see spec. */
		    
		/* fall through */
		    
	    default:
		errno_exit ("VIDIOC_DQBUF");
	    }
	}
	    
//FIX	    assert (buf.index < n_buffers);
	processFrame (buffers[buf.index].start);
	    
	if (-1 == xioctl (device_fd, VIDIOC_QBUF, &buf)){
	    errno_exit ("VIDIOC_QBUF");
	}
	    
	break;
	    
	    
    }
    return 1;
}
    
    
void iqrcommon::moduleV4L2Video::setPixelformat(){
//    cout << "moduleV4L2Video::setPixelformat()" << endl;
    struct v4l2_format fmt_set;
	
    unsigned int min;
	
    CLEAR (fmt_set);

    list<int> lstFormats;
    lstFormats.push_back(V4L2_PIX_FMT_RGB24);
    lstFormats.push_back(V4L2_PIX_FMT_YUV420);
    lstFormats.push_back(V4L2_PIX_FMT_YUYV);
	
    fmt_set.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt_set.fmt.pix.field = V4L2_FIELD_INTERLACED;
	
    bool bFormatFound = false;
    list<int>::iterator it;
    it=lstFormats.begin();

    while (it!=lstFormats.end() && !bFormatFound){
	fmt_set.fmt.pix.width = imgWidth; 
	fmt_set.fmt.pix.height = imgHeight;
	fmt_set.fmt.pix.pixelformat = *it;
	    
	cout << "trying pixelformat: " << fcc2s(*it) << endl;
	
	/* in principle, this should case an error is it doesn't work;
	   but sometimes, there is no return error... */
	if (-1 == xioctl (device_fd, VIDIOC_S_FMT , &fmt_set)){
	    cerr << "cannot set VIDIOC_S_FMT" << endl;
	} else {
	    if(fmt_set.fmt.pix.pixelformat == *it){
		iPixelFormat = *it;
		bFormatFound = true;
	    } 
	}
	it++;
    }

    if(iPixelFormat<0){
	throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: was not able to set pixel format to a supported type");
	closeDevice();
    }

    cout << "set pixelformat to: " << fcc2s(iPixelFormat) << endl;
	
    /* Buggy driver paranoia. */
    min = fmt_set.fmt.pix.width * 2;
    if (fmt_set.fmt.pix.bytesperline < min){
	fmt_set.fmt.pix.bytesperline = min;
    }
    min = fmt_set.fmt.pix.bytesperline * fmt_set.fmt.pix.height;
    if (fmt_set.fmt.pix.sizeimage < min){
	fmt_set.fmt.pix.sizeimage = min;
    }
	
    iv4l_sizeimage = fmt_set.fmt.pix.sizeimage;
	
    cout << "fmt_set.fmt.pix.sizeimage: " << fmt_set.fmt.pix.sizeimage << endl;
    cout << "fmt_set.fmt.pix.bytesperline: " << fmt_set.fmt.pix.bytesperline << endl;
    cout << "fmt_set.fmt.pix.width: " << fmt_set.fmt.pix.width << endl;
    cout << "fmt_set.fmt.pix.height: " << fmt_set.fmt.pix.height << endl;
}
    
void iqrcommon::moduleV4L2Video::checkDevice(string strDeviceName){
//    cout << "moduleV4L2Video::checkDevice()" << endl;
	
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
	
	
    if (-1 == xioctl (device_fd, VIDIOC_QUERYCAP, &cap)) {
	throw ModuleError(string("Module \"") + getLabel() + "\": \"" + strDeviceName + "\"  no V4L2 device"  );
    }
	
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
	throw ModuleError(string("Module \"") + getLabel() + "\": \"" + strDeviceName + "\" is no video capture device" );
    }
	
    if(io == IO_METHOD_READ){
	if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
	    fprintf (stderr, "%s does not support read i/o\n", strDeviceName.c_str());
	    throw ModuleError(string("Module \"") + getLabel() + "\": \"" + strDeviceName + "\" does not support read i/o" );
	}
    }
	

    bool bPrintDeviceInfo = true;
    if(bPrintDeviceInfo){
	cout << "    *** Camera Info ***" << endl;;
	cout << "driver:      " << cap.driver << endl;
	cout << "card:        " << cap.card << endl;
	cout << "bus info:    " << cap.bus_info << endl;

/*
  cout << "Maximum Width:  " << cap.maxwidth << endl;
  cout << "Minimum Height: " << cap.minheight << endl;
  cout << "Maximum Height: " << cap.maxheight << endl;
*/

    }


    /* Select video input, video standard and tune here. */
    //FIX: what to do with this???
    CLEAR (cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == xioctl (device_fd, VIDIOC_CROPCAP, &cropcap)) {
	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c = cropcap.defrect; /* reset to default */

	if (-1 == xioctl (device_fd, VIDIOC_S_CROP, &crop)) {
	    switch (errno) {
	    case EINVAL:
		/* Cropping not supported. */
		break;
	    default:
		/* Errors ignored. */
		break;
	    }
	}
    } else { 
	/* Errors ignored. */
    }

}

    
void iqrcommon::moduleV4L2Video::initDevice (string strDeviceName) {
//    cout << "moduleV4L2Video::initDevice (string strDeviceName)" << endl;
    struct v4l2_format fmt;

    ASSERT(iv4l_sizeimage>0);

    switch (io) {
    case IO_METHOD_READ:
	initRead (iv4l_sizeimage);
	break;

    case IO_METHOD_MMAP:
	init_mmap (strDeviceName);
	break;

    }
}
    
void iqrcommon::moduleV4L2Video::initRead (unsigned int buffer_size) {
//    cout << "moduleV4L2Video::initRead (unsigned int buffer_size)" << endl;
    buffers = (buffer*)calloc (1, sizeof (*buffers));

    if (!buffers) {
	fprintf (stderr, "Out of memory\n");
	exit (EXIT_FAILURE);
    }

    buffers[0].length = buffer_size;
    buffers[0].start = malloc (buffer_size);

    if (!buffers[0].start) {
	fprintf (stderr, "Out of memory\n");
	exit (EXIT_FAILURE);
    }
}

void iqrcommon::moduleV4L2Video::uninitDevice () {
//    cout << "moduleV4L2Video::uninitDevice ()" << endl;
    unsigned int i;

    switch (io) {
    case IO_METHOD_READ:
	free (buffers[0].start);
	break;

    case IO_METHOD_MMAP:
	for (i = 0; i < n_buffers; ++i)
	    if (-1 == munmap (buffers[i].start, buffers[i].length))
		errno_exit ("munmap");
	break;
    }

    free (buffers);
}


void iqrcommon::moduleV4L2Video::closeDevice () {
//    cout << "moduleV4L2Video::closeDevice ()" << endl;
    if (-1 == close (device_fd)){
	errno_exit ("close");
    }
    device_fd = -1;
}

void iqrcommon::moduleV4L2Video::startCapturing () {
//    cout << "moduleV4L2Video::startCapturing ()" << endl;
    unsigned int i;
    enum v4l2_buf_type type;

    switch (io) {
    case IO_METHOD_READ:
	/* Nothing to do. */
	break;

    case IO_METHOD_MMAP:
	for (i = 0; i < n_buffers; ++i) {
	    struct v4l2_buffer buf;

	    CLEAR (buf);

	    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    buf.memory = V4L2_MEMORY_MMAP;
	    buf.index = i;

	    if (-1 == xioctl (device_fd, VIDIOC_QBUF, &buf))
		errno_exit ("VIDIOC_QBUF");
	}
 
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl (device_fd, VIDIOC_STREAMON, &type)){
	    errno_exit ("VIDIOC_STREAMON");
	}

	break;

    }
}




void iqrcommon::moduleV4L2Video::init_mmap (string strDeviceName) {
//    cout << "moduleV4L2Video::init_mmap (string strDeviceName)" << endl;
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (device_fd, VIDIOC_REQBUFS, &req)) {
	if (EINVAL == errno) {
	    fprintf (stderr, "%s does not support memory mapping\n", strDeviceName.c_str());
	    exit (EXIT_FAILURE);
	} else {
	    errno_exit ("VIDIOC_REQBUFS");
	}
    }

    if (req.count < 2) {
	fprintf (stderr, "Insufficient buffer memory on %s\n", strDeviceName.c_str());
	exit (EXIT_FAILURE);
    }

    buffers = (buffer*)calloc (req.count, sizeof (*buffers));

    if (!buffers) {
	fprintf (stderr, "Out of memory\n");
	exit (EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
	struct v4l2_buffer buf;

	CLEAR (buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = n_buffers;

	if (-1 == xioctl (device_fd, VIDIOC_QUERYBUF, &buf)){
	    errno_exit ("VIDIOC_QUERYBUF");
	}

	buffers[n_buffers].length = buf.length;
	buffers[n_buffers].start = mmap (NULL /* start anywhere */,
					 buf.length,
					 PROT_READ | PROT_WRITE /* required */,
					 MAP_SHARED /* recommended */,
					 device_fd, buf.m.offset);
	
	if (MAP_FAILED == buffers[n_buffers].start){
	    errno_exit ("mmap");
	}
    }
}
    

	void iqrcommon::moduleV4L2Video::enumerate_menu () {
	    cout << "Menu items:" << endl;
		
	    struct v4l2_querymenu querymenu;
	    struct v4l2_queryctrl queryctrl;
		
	    memset (&querymenu, 0, sizeof (querymenu));
	    querymenu.id = queryctrl.id;
		
	    for (querymenu.index = queryctrl.minimum; querymenu.index <= queryctrl.maximum; querymenu.index++) {
		if (0 == ioctl (device_fd, VIDIOC_QUERYMENU, &querymenu)) {
		    cout << "querymenu.name: " << querymenu.name << endl;
		} else {
		    perror ("VIDIOC_QUERYMENU");
//		    exit (EXIT_FAILURE);
		}
	    }
	}


	void iqrcommon::moduleV4L2Video::errno_exit (const char * s) {
	    fprintf (stderr, "%s error %d, %s\n", s, errno, strerror (errno));
	    throw ModuleError(string("Module \"") + getLabel() + "\": ERROR: " + s + " error " +  strerror (errno));
//		exit (EXIT_FAILURE);
	}


	void iqrcommon::moduleV4L2Video::myYUV422toRGB(int imgWidth, int imgHeight, unsigned char* yuv, unsigned char* rgb){
	    
	    int v_start = imgHeight*imgWidth;
	    int u_start = imgHeight*imgWidth + imgHeight*imgWidth/4;
	    int ir, ig, ib;
	    unsigned long int i, j, k;
	    double yValue, uValue,vValue = 0.;
	    int y1,y2,u,v;    
	    
	    j = i = k = 0;
	    for(int iY=0; iY < imgHeight; iY++) {
		for(int iX=0; iX < imgWidth; iX++){
		    yValue = yuv[i];
		    if(k%2){
			vValue = yuv[i+1];
		    } else {
			uValue = yuv[i+1];
		    }
		    
		    ir = lrint((1.164 * (yValue - 16)) + (2.018 * (vValue - 128)));
		    ig = lrint((1.164 * (yValue - 16)) - (0.813 * (uValue - 128)) - (0.391 * (vValue - 128)));
		    ib = lrint((1.164 * (yValue - 16)) + (1.596 * (uValue - 128)));
		    
		    ir=ir>255 ? 255:ir;
		    ig=ig>255 ? 255:ig;
		    ib=ib>255 ? 255:ib;
		    
		    ir=ir<0 ? 0 : ir;
		    ig=ig<0 ? 0 : ig;
		    ib=ib<0 ? 0 : ib;
		    
//ORIG 		    rgb[j++] = ib;
//ORIG 		    rgb[j++] = ir;
//ORIG 		    rgb[j++] = ig;

		    rgb[j++] = ib;
		    rgb[j++] = ig;
		    rgb[j++] = ir;

		    i+=2;
		    k++;
		}
	    }
	}

#define	min(a, b)		(((a) < (b)) ? (a) : (b))
#define	MIN(a, b, c)		min(min(a, b), c)
#define	max(a, b)		(((a) > (b)) ? (a) : (b))
#define	MAX(a, b, c)		max(max(a, b), c)

void iqrcommon::moduleV4L2Video::RGBtoHSV( float r, float g, float b, float *h, float *s, float *v ) {
    float min, max, delta;
    
    min = MIN( r, g, b );
    max = MAX( r, g, b );
    *v = max;				// v
    
    delta = max - min;
    
    if( max != 0 ) {
	*s = delta / max;		// s
    } else {
	// r = g = b = 0		// s = 0, v is undefined
	*s = 0;
	*h = 0;
	return;
    }
    
    if( r == max ){
    	*h = ( g - b ) / delta;		// between yellow & magenta
    } else if( g == max ){
	*h = 2 + ( b - r ) / delta;	// between cyan & yellow
    } else{
	*h = 4 + ( r - g ) / delta;	// between magenta & cyan
    }
    
    *h *= 60;				// degrees
    if( *h < 0 ){
	*h += 360;
    }
    
}
