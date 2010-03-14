#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <iterator>
#include <list>
#include <unistd.h>

#include <Common/Helper/iqrUtils.h>
#include "moduleSonyPanTilt.hpp"

MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleSonyPanTilt,
			  "Sony pan-tilt")


    iqrcommon::moduleSonyPanTilt::moduleSonyPanTilt() {
    clsPanTilt = new ClsPanTilt<moduleSonyPanTilt>(this, label(), 0, "Pan-Tilt");
    cam = NULL;
    iCameraID = 1;
    clsIntParameterCameraID = addIntParameter("cameraID", /* _strName */
					      "Camera ID",  /* _strLabel */
					      1, /* _iValue */
					      1, /* _iMinimum */
					      3, /* _iMaximum */ 
					      "Camera ID" /* _strDescription */,
					      "Pan/Tilt" /* _strCategory */);

    clsStringParameterDeviceName = addStringParameter("DeviceName", /* _strName */ 
						      "Pan/Tilt Device", /* _strLabel */
						      "/dev/ttyS0", /* _strValue */
						      true, /* _bEditable */
						      false, /* _bLong */
						      "Path to Pan/Tilt Device", /* _strDescription */
						      "Pan/Tilt" /* _strCategory */);
}


iqrcommon::moduleSonyPanTilt::~moduleSonyPanTilt() {
    cleanup();
}


void iqrcommon::moduleSonyPanTilt::init() {
    pairPTPosition.first =  0;
    pairPTPosition.second = 0;

    clsPanTilt->init();

    iCameraID = clsIntParameterCameraID->getValue(); 

    string strDeviceName = clsStringParameterDeviceName->getValue(); 
    cam= new SonyEvi((char*)strDeviceName.c_str());

    cam->SetCameraPosAbsolut (iCameraID,
			      (int)pairPTPosition.first, (int)pairPTPosition.second,
			      MAX_HSPEED, MAX_VSPEED, 
			      0);
}

void iqrcommon::moduleSonyPanTilt::cleanup() {
    if(cam !=NULL){
	delete cam;
    }
    cam = NULL;
}

void iqrcommon::moduleSonyPanTilt::update() {
    qmutexThread->lock(); //new
    doPanTilt();
    qmutexThread->unlock(); //new
    usleep((int)10e5);
}


void iqrcommon::moduleSonyPanTilt::doPanTilt() {
    
    
    pairPTPosition = clsPanTilt->calculatePanTilt((int)pairPTPosition.first, (int)pairPTPosition.second);
//    cout << "cx,cy: " << pairPTPosition.first << ", " << pairPTPosition.second << endl;
    
    cam->SetCameraPosAbsolut (iCameraID, (int)pairPTPosition.first, (int)pairPTPosition.second,
			      MAX_HSPEED, MAX_VSPEED, 
			      0);
    
}

