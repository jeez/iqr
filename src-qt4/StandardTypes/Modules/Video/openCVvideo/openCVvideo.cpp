#include "openCVvideo.hpp"
#include <iostream>


MAKE_MODULE_DLL_INTERFACE(iqrcommon::openCVvideo,
                          "video module")

 
iqrcommon::openCVvideo::openCVvideo() : ClsThreadModule() {

    
    
  clsBoolParameterShowOutput = addBoolParameter("ShowOutput", /* _strName */ 
						"Show output", /* _strLabel */
						true, /* _bValue */
						"Show camera output", /* _strDescription */
						"Output" /* _strCategory */);

  clsBoolParameterMonochrome = addBoolParameter("Monochrome", /* _strName */ 
						"Monochrome", /* _strLabel */
						false, /* _bValue */
						"Monochrome", /* _strDescription */
						"Output" /* _strCategory */);


       
  clsBoolParameterHSV = addBoolParameter("HSV", /* _strName */ 
					 "HSV color space", /* _strLabel */
					 false, /* _bValue */
					 "HSV color space", /* _strDescription */
					 "Output" /* _strCategory */);


  clsStateVariableVideoOutputR_H_G = addOutputToGroup("videoOutputRed", "Video Output Red/Hue/Grey");
  clsStateVariableVideoOutputG_S = addOutputToGroup("videoOutputGreen", "Video Output Green/Saturation");
  clsStateVariableVideoOutputB_V = addOutputToGroup("videoOutputBlue", "Video Output Blue/Value");



  capture = NULL;
  iplImgVideoFrame = NULL;
  iplImgScaledOutput = NULL;
  iplImgLuminance = NULL;
  iplImgRH = NULL;
  iplImgGS = NULL;
  iplImgBV = NULL;

}


iqrcommon::openCVvideo::~openCVvideo(){ 
  //  cout << "iqrcommon::openCVvideo::~openCVvideo()" << endl;
  cleanup();
}


void iqrcommon::openCVvideo::init(){
  //    cout << "openCVvideo::init()" << endl;
    
  bMonochrome = clsBoolParameterMonochrome->getValue();
  bHSV = clsBoolParameterHSV->getValue();


  capture = cvCaptureFromCAM( 0 ); //FIX: device

  /* none of this seems to do anything...
     cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST, .2);
     cvSetCaptureProperty(capture, CV_CAP_PROP_BRIGHTNESS, .2);
     cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST, .2);
     cvSetCaptureProperty(capture, CV_CAP_PROP_SATURATION, .2);
     cvSetCaptureProperty(capture, CV_CAP_PROP_HUE, .2);
  */


  if(!capture){
    throw ModuleError(string("Module \"") + label() + "\": error opening video device");
  }

  iplImgVideoFrame = cvQueryFrame( capture );


  int iImageWidth =  (int)(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
  int iImageHeight = (int)(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));


  ShowOutput = clsBoolParameterShowOutput->getValue(); 

  if(ShowOutput){
    cvNamedWindow( "image", 0 );
  }


  iNrCells = clsStateVariableVideoOutputR_H_G->getStateArray().getWidth();
  if(clsStateVariableVideoOutputR_H_G->getStateArray().getWidth() != clsStateVariableVideoOutputB_V->getStateArray().getWidth() ||
     clsStateVariableVideoOutputR_H_G->getStateArray().getWidth() != clsStateVariableVideoOutputG_S->getStateArray().getWidth()){
    throw ModuleError(string("Module \"") + label() + "\": All output groups need to be the same size");
  }

  iGroupWidth = clsStateVariableVideoOutputR_H_G->getNrCellsHorizontal();
  iGroupHeight = clsStateVariableVideoOutputR_H_G->getNrCellsVertical();

  iplImgScaledOutput = cvCreateImage (cvSize (iGroupWidth,iGroupHeight), iplImgVideoFrame->depth,iplImgVideoFrame->nChannels);
  //  iplImgScaledOutput = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);



  if(bMonochrome){
    iplImgLuminance = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);
  } else {
    iplImgRH = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);
    iplImgGS = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);
    iplImgBV = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);
    if(bHSV){
      iplImgHSV = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 3);
    }
  }


}

void iqrcommon::openCVvideo::update(){
  //    cerr << "openCVvideo::update()" << endl;

  iplImgVideoFrame = cvQueryFrame( capture );


  if( !iplImgVideoFrame ) {
    cout << "end of file" << endl;
    cerr << "ERROR" << endl; //fix
  }


  //    cvCvtColor(iplImgVideoFrame, iplImgLuminance, CV_BGR2GRAY);
  //    cvResize (iplImgLuminance,iplImgScaledOutput,1);


  cvResize (iplImgVideoFrame,iplImgScaledOutput,1);

  if(bMonochrome){
    cvCvtColor(iplImgScaledOutput, iplImgLuminance, CV_BGR2GRAY);
  } else {
    if(bHSV){
      cvCvtColor(iplImgScaledOutput, iplImgHSV, CV_BGR2HSV);
      cvSplit(iplImgHSV, iplImgBV, iplImgGS, iplImgRH, NULL);
    } else {
      cvSplit(iplImgScaledOutput, iplImgBV, iplImgGS, iplImgRH, NULL);
    }
  }

	

  if(ShowOutput){
    cvShowImage("image", iplImgScaledOutput );
    cvWaitKey(10);
  }
	
	
  qmutexThread->lock(); 

  if(bMonochrome){
    StateArray &clsStateArrayOutR_H_G = clsStateVariableVideoOutputR_H_G->getStateArray();
    for (int i=0 ; i<iNrCells ; i++) {
      clsStateArrayOutR_H_G[0][i] = ((unsigned char) (iplImgLuminance->imageData[i])/255.0); 
    }
  } else {
    StateArray &clsStateArrayOutR_H_G = clsStateVariableVideoOutputR_H_G->getStateArray();
    StateArray &clsStateArrayOutG_S = clsStateVariableVideoOutputG_S->getStateArray();
    StateArray &clsStateArrayOutB_V = clsStateVariableVideoOutputB_V->getStateArray();

    for (int i=0 ; i<iNrCells ; i++) {
      clsStateArrayOutR_H_G[0][i] = ((unsigned char) (iplImgRH->imageData[i])/255.0); 
      clsStateArrayOutG_S[0][i] = ((unsigned char) (iplImgBV->imageData[i])/255.0); 
      clsStateArrayOutB_V[0][i] = ((unsigned char) (iplImgGS->imageData[i])/255.0); 
    }


  }
  qmutexThread->unlock(); 



    
}

void iqrcommon::openCVvideo::cleanup(){
  //  cout << "iqrcommon::openCVvideo::cleanup()" << endl;
  cvDestroyAllWindows();

  if(capture != NULL){
    cvReleaseCapture( &capture );
  }
}



