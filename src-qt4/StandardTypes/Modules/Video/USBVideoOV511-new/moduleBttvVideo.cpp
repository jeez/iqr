#include "moduleBttvVideo.hpp"


#ifdef IQRMODULE
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleBttvVideo,
			  "bttv video module new")

#include <cameras/ModuleIcon_CameraBttv.h>
#include <Common/Helper/iqrUtils.h>
#endif

    static V4LCaps v4lCaps; // Min/max capture width/height

static int     captureFormat;
enum {
    BigPictureTVSource        = 0,
    BigPictureCompositeSource = 1, // Camera
    BigPictureSVideoSource    = 2
};


#ifdef IQRMODULE
iqrcommon::moduleBttvVideo::moduleBttvVideo() : ClsBaseVideoItem() {
#else
    iqrcommon::moduleBttvVideo::moduleBttvVideo() {
#endif


#ifdef IQRMODULE
	clsStateVariableVideoOutput = addOutputToGroup("videoOutput", "Video Output");
#endif

	iCounter = 0;
    }

#ifdef IQRMODULE
    moduleIcon iqrcommon::moduleBttvVideo::getIcon() {
	moduleIcon mi(ModuleIcon_CameraBttv_png_data, ModuleIcon_CameraBttv_png_len, 3 ,5);
	return mi;
    }
#endif

    iqrcommon::moduleBttvVideo::~moduleBttvVideo(){ 
	cleanup();
    }

    void iqrcommon::moduleBttvVideo::init(){
	cout << "moduleBttvVideo::init()" << endl;

	deviceID = -1;
	captureBuf = NULL;

	ClsBaseVideoItem::init();
	openDevice( strDeviceName) ;


	// Select Y8 capture format

	captureFormat = V4L_FORMAT_YUV422P;
//captureFormat = V4L_FORMAT_Y8;
//	captureFormat = V4L_FORMAT_RGB24;

	// Initialize capture size based on window size
	// Initialize picture attributes to mid-range

	v4lFormat.width  = 320;//imgWidth;
	v4lFormat.height = 240;//imgHeight;
	v4lFormat.format = captureFormat;;
	V4LMSetFormat(&v4lFormat);

	V4LMGetFormat(&v4lFormat);
  
	cout << "v4lFormat.format: " << v4lFormat.format << endl;

	imgWidth = v4lFormat.width;
	imgHeight = v4lFormat.height;


	captureFrame = 0;

	if (V4LMCapture(deviceID, captureFrame) < 0) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: V4LMCapture: " +  strerror(errno));
#else
	    cerr << "Error: V4LMCapture: " <<  strerror(errno) << endl;
	    exit(1);
#endif
	}

	setPictureProperties();



	if(bShowOutput){
	    clsXWin.createWindow(imgWidth, imgHeight);
	    iScreenDepth = clsXWin.getScreenDepth();

	    if(iScreenDepth != 16 && iScreenDepth != 24){
		cerr << "Cannot screen depth of " << iScreenDepth << endl;
#ifdef IQRMODULE	
		throw ModuleError(string("Module \"") + label() + "\": Error: V4LMCapture: " +  strerror(errno));
#else
		exit(-1);
#endif
	    }
	    
	}

#ifdef IQRMODULE	
	iNrCells = clsStateVariableVideoOutput->getStateArray().getWidth();
	iGroupWidth = (int)(sqrt((double)iNrCells / fImageRatio)); 
	iGroupHeight = iNrCells/iGroupWidth; 
	iHPace = (int)ceil((double)imgWidth / (double)iGroupWidth);
	iVPace = (int)ceil((double)imgHeight / (double)iGroupHeight);

//	cout << "iHPace, iVPace: " << iHPace << ", " << iVPace << endl;
//	cout << "iGroupWidth, iGroupHeight: " << iGroupWidth << ", " << iGroupHeight << endl;
#endif


	printDeviceInfo();

    }


    void iqrcommon::moduleBttvVideo::openDevice(string strDeviceName) {
	cout << "moduleBttvVideo::openDevice(string strDeviceName)" << endl;
	cout << "device: " << strDeviceName << endl;
	errno = 0;




	if ((deviceID = open(strDeviceName.c_str(), O_RDWR)) < 0) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: Can't open: " + strDeviceName + ", Error: " +  strerror(errno));
#else
	    cerr << "Error: Can't open: " <<  strDeviceName.c_str() << ", Error: " <<  strerror(errno) << endl;
	    exit(1);
#endif
	} 

	if (V4LMGetMMInfo(deviceID, &v4lMMInfo) < 0) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: V4LMGetMMInfo: " +  strerror(errno));
#else
	    cerr << "Error: V4LMGetMMInfo: " <<  strerror(errno) << endl;
	    exit(1);
#endif
	} 
//#if (0)
	cerr << "Capture buffer size   = " << v4lMMInfo.size << endl;
	cerr << "Capture buffer frames = " << v4lMMInfo.frames << endl;
//#endif
	if (v4lMMInfo.frames < 2) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: V4LMGetMMInfo: frames < 2" +  strerror(errno));
#else
	    cerr << "Error: V4LMGetMMInfo: frames < 2" << endl;
	    exit(1);
#endif
	} 

	if ((captureBuf = (bits8 *) mmap(0, v4lMMInfo.size, PROT_READ | PROT_WRITE, MAP_SHARED, deviceID, 0)) == MAP_FAILED) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: mmap(): " +  strerror(errno));
#else
	    cerr << "Error: mmap(): " <<  strerror(errno)  << endl;
	    exit(1);
#endif
	} 


//  if (V4LSetSource(v4l, BigPictureCompositeSource, VIDEO_MODE_NTSC) < 0) {
	if (V4LSetSource(deviceID, BigPictureCompositeSource, VIDEO_MODE_PAL) < 0) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: V4LSetSource: " +  strerror(errno));
#else
	    cerr << "Error: V4LSetSource: " <<  strerror(errno) << endl;
	    exit(1);
#endif
	} 


	if (V4LGetCaps(deviceID, &v4lCaps) < 0) {
#ifdef IQRMODULE	
	    throw ModuleError(string("Module \"") + label() + "\": Error: V4LGetCaps: " +  strerror(errno));
#else
	    cerr << "Error: V4LGetCaps: " <<  strerror(errno) << endl;
	    exit(1);
#endif
	} 

    }

    void iqrcommon::moduleBttvVideo::closeDevice() {
//	cout << "moduleBttvVideo::closeDevice(void)" << endl;
//	cout << "device_fd: " << device_fd << endl;
	errno = 0;
	if(errno){
	    cerr << "Error closing: " << strerror(errno) << endl;
	}

	if (deviceID > 0) {
	    if (close(deviceID) < 0) {
		cerr << "Error: Can't close: " <<  strDeviceName.c_str()  << ", Error: " <<  strerror(errno) << endl;
	    } 
	} 
	deviceID = -1;
	sleep(2);
    }


    void iqrcommon::moduleBttvVideo::setPictureProperties() {
	V4LSetBrightness(deviceID, iBrightness);
	V4LSetContrast(deviceID, iContrast );
	V4LSetSaturation(deviceID, iColour);
	V4LSetHue(deviceID, iHue);
    }


    void iqrcommon::moduleBttvVideo::printDeviceInfo() {
 	cout << "    *** Camera Info ***" << endl;;
 	cout << "Name:           " << v4lCaps.name << endl;
 	cout << "Type:           " << v4lCaps.type << endl;
 	cout << "Minimum Width:  " << v4lCaps.minwidth << endl;
 	cout << "Maximum Width:  " << v4lCaps.maxwidth << endl;
 	cout << "Minimum Height: " << v4lCaps.minheight << endl;
 	cout << "Maximum Height: " << v4lCaps.maxheight << endl;
      
// 	cout << "X:              " << v4lFormat.x << endl;
// 	cout << "Y:              " << v4lFormat.y << endl;
 	cout << "Width:          " << v4lFormat.width << endl;
 	cout << "Height:         " << v4lFormat.height << endl;
 
//orig  	cout << "brightness:     " << sVideo_picture.brightness << endl;
//orig  	cout << "hue:            " << sVideo_picture.hue << endl;       
//orig  	cout << "colour:         " << sVideo_picture.colour << endl;    
//orig  	cout << "contrast:       " << sVideo_picture.contrast << endl;  
//orig  	cout << "whiteness:      " << sVideo_picture.whiteness << endl; 
//orig  	cout << "depth:          " << sVideo_picture.depth << endl;     
	cout << "    ***************" << endl;;
    }

    
    void iqrcommon::moduleBttvVideo::update(){
//    cout << "moduleBttvVideo::update()" << endl;

	// Wait for this frame


#ifdef IQRMODULE
	StateArray &clsStateArrayOut = clsStateVariableVideoOutput->getStateArray();
    	int ii=0;
#endif

/* this is slowing every thing down !?
   if (V4LMSync(v4l, captureFrame) < 0) {
   cerr << "Error: V4LMSync: " <<  strerror(errno) << endl;
   exit(1);
   } 
*/

	// Start capture for next frame
	if (V4LMCapture(deviceID, 1 - captureFrame) < 0) {
	    cerr << "Error: V4LMCapture: " << strerror(errno) << endl;
	    exit(1);
	} 

	iPixel = 0;

//	cout << captureBuf << endl;
      
	for(int iY=0; iY < imgHeight; iY++) {
//	  cout << "iY: " << iY << endl;
	    for(int iX=0; iX < imgWidth; iX++){
		if(iGrayPixel>0 ) cout << iGrayPixel << endl;
		iGrayPixel = captureBuf[iPixel++];
		if(bShowOutput){
		    value = getPixel(iGrayPixel, iGrayPixel, iGrayPixel);
		    clsXWin.putPixel(iX, iY, value);
//		    cout  << value << " ";
		}
		
		
		
#ifdef IQRMODULE
		if(iX%iHPace == 0 && iY%iVPace == 0){
		    clsStateArrayOut[0][ii] = (double)(iGrayPixel) / ( 256.);
		    ii++;
		}
#endif
	    }

	}
	if(bShowOutput){
	    clsXWin.showImage();
	}
	captureFrame = 1 - captureFrame; // 0<->1
    }

    void iqrcommon::moduleBttvVideo::cleanup(){
	cout << "moduleBttvVideo::cleanup()" << endl;
	if(bShowOutput){
	    clsXWin.destroyWindow();
	}

	if (captureBuf != NULL) {
	    if (munmap((char *) captureBuf, v4lMMInfo.size) < 0) {
		cerr << "Error: munmap(): " <<  strerror(errno) << endl;
	    } 
	} 

	closeDevice();

    }
