#include "openCVmpeg.hpp"
#include <iostream>


MAKE_MODULE_DLL_INTERFACE(iqrcommon::openCVmpeg,"OpenCV mpeg Video Player")

 
    iqrcommon::openCVmpeg::openCVmpeg() : ClsModule() {
    //iqrcommon::openCVmpeg::openCVmpeg() : ClsThreadModule() {

    
    objectShape = addOptionsParameter ("Video mode",
				       "Video mode",
				       "Choose the video mode",
				       "File");

    objectShape->addOption (CONTINUOUS);
    objectShape->addOption (SINGLE_LOOP);
    
    clsBoolParameterShowOutput = addBoolParameter("ShowOutput", /* _strName */ 
						  "Show output", /* _strLabel */
						  true, /* _bValue */
						  "Show camera output", /* _strDescription */
						  "Output" /* _strCategory */);
    
    clsPathParameterVideoFile = addPathParameter("File Name", /* _strName */ 
						      "File name", /* _strLabel */
						      "", /* _strValue */
						      true, /* _bEditable */
						      "Path to video file", /* _strDescription */
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


    clsStateVariableEndOutput = addOutputToGroup("End of video", "End of video");

    StateVariableController = addInputFromGroup("Controler","Controler");
    capture = NULL;
  iplImgVideoFrame = NULL;
  iplImgScaledOutput = NULL;
  iplImgLuminance = NULL;
  iplImgRH = NULL;
  iplImgGS = NULL;
  iplImgBV = NULL;

}


iqrcommon::openCVmpeg::~openCVmpeg(){ 
//    cout << "openCVmpeg::~openCVmpeg()" << endl;
  cleanup();
}

void iqrcommon::openCVmpeg::init(){
//    cout << "openCVmpeg::init()" << endl;
    
    strVideoFileName = clsPathParameterVideoFile->getValue();
    
    if(strVideoFileName.size()<=0){
 	throw ModuleError(string("Module \"") + label() + "\": missing file name for video");
    } else {
	ifstream ifstreamVideoTest;
	ifstreamVideoTest.open(strVideoFileName.c_str(), ifstream::in);
	ifstreamVideoTest.close();
	if(ifstreamVideoTest.fail()) {
	    ifstreamVideoTest.clear(ios::failbit);
	    throw ModuleError(string("Module \"") + label() + "\": unable to open file " + "\"" + strVideoFileName + "\"");
	}

    }

  bMonochrome = clsBoolParameterMonochrome->getValue();
  bHSV = clsBoolParameterHSV->getValue();



    capture = cvCaptureFromFile(strVideoFileName.c_str() );
    if(!capture){
	throw ModuleError(string("Module \"") + label() + "\": error opening file");
    }

    iplImgVideoFrame = cvQueryFrame( capture );

/*
    int iImageWidth = clsIntParameterImageWidth->getValue();
    int iImageHeight = clsIntParameterImageHeight->getValue();
*/

    int iImageWidth =  (int)(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
    int iImageHeight = (int)(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));


    ShowOutput = clsBoolParameterShowOutput->getValue(); 

    if(ShowOutput){
	cvNamedWindow( "video", 0 );
    }


    if(StateVariableController->getTarget()->getStateArray().getWidth() < 2 ){
	throw ModuleError(string("Module \"") + label() + "\": needs 2 cells for \"Controller\"");
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



    reload();
}

void iqrcommon::openCVmpeg::update(){
//    cerr << "openCVmpeg::update()" << endl;

    
    StateArray &controlPlayer = StateVariableController->getTarget()->getStateArray();
    StateArray &endFlag = clsStateVariableEndOutput->getStateArray();
    endFlag [0][0] = 0;    

	
    // controlPlayer [0] = play movie, controlPlayer [1] = reset
    if (controlPlayer[0][1]) {
	reload();
    } else {
	iplImgVideoFrame = cvQueryFrame( capture );
	
	if( !iplImgVideoFrame ) {
	    cout << "end of file" << endl;
	    endFlag [0][0] = 1;
	    reload();
	}


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
	    cvShowImage("video", iplImgVideoFrame );
	    cvWaitKey(10);
	}
	
	//  qmutexThread->lock(); 

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
  //  qmutexThread->unlock(); 




    }
}

void iqrcommon::openCVmpeg::cleanup(){
    cvWaitKey(10);
    cvDestroyAllWindows();
    cvWaitKey(10);
    if(capture!=NULL){
      cvReleaseCapture( &capture );
    }

}


void iqrcommon::openCVmpeg::reload(){
    cvReleaseCapture( &capture );
    capture = cvCaptureFromFile(strVideoFileName.c_str() );
    iplImgVideoFrame = cvQueryFrame( capture );
}
