#include "moduleUSBVideoPT.hpp"

#include "ccvt-0.3/ccvt.h"

MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleUSBVideoPT,
			  "USB video module with Pan/Tilt")

#include <cameras/ModuleIcon_CameraUSB.h>
#include <Common/Helper/iqrUtils.h>


    iqrcommon::moduleUSBVideoPT::moduleUSBVideoPT() : moduleUSBVideo() {
    clsPanTilt = new ClsPanTilt<moduleUSBVideoPT>(this, label());
}


iqrcommon::moduleUSBVideoPT::~moduleUSBVideoPT(){ 
    cleanup();
}

void iqrcommon::moduleUSBVideoPT::init(){
//	cout << "moduleUSBVideoPT::init()" << endl;


    moduleUSBVideo::init();
    ioctl(device_fd, VIDIOCPWCMPTGRANGE, &sPwc_mpt_range);
    fPanRange = abs(sPwc_mpt_range.pan_min - sPwc_mpt_range.pan_max);
    fTiltRange = abs(sPwc_mpt_range.tilt_min - sPwc_mpt_range.tilt_max);

    clsPanTilt->init();
    printDeviceInfo();
    resetPanTilt();
}



void iqrcommon::moduleUSBVideoPT::resetPanTilt(){
//	cout << "moduleUSBVideoPT::resetPanTilt()" << endl;
    int ii;
    ii = 0;
    ioctl(device_fd,VIDIOCPWCMPTRESET, &ii);
    ii = 1;
    ioctl(device_fd,VIDIOCPWCMPTRESET, &ii);
//TEMP	sleep(5);
//    setPanTiltAbs(0, 0);
}



const double iqrcommon::moduleUSBVideoPT::getCurrentPan(){
    ioctl(device_fd, VIDIOCPWCMPTGANGLE, &sPwc_mpt_angles);
//    cout << "sPwc_mpt_angles.pan: " << sPwc_mpt_angles.pan << endl;
    return sPwc_mpt_angles.pan;
};

const double iqrcommon::moduleUSBVideoPT::getCurrentTilt(){
    ioctl(device_fd, VIDIOCPWCMPTGANGLE, &sPwc_mpt_angles);
//    cout << "sPwc_mpt_angles.tilt: " << sPwc_mpt_angles.tilt << endl;
    return sPwc_mpt_angles.tilt;
};

const double iqrcommon::moduleUSBVideoPT::getMinPan(){ return sPwc_mpt_range.pan_min; };
const double iqrcommon::moduleUSBVideoPT::getMaxPan(){ return sPwc_mpt_range.pan_max; };

const double iqrcommon::moduleUSBVideoPT::getMinTilt(){ return sPwc_mpt_range.tilt_min; };
const double iqrcommon::moduleUSBVideoPT::getMaxTilt(){ return sPwc_mpt_range.tilt_max; };

void iqrcommon::moduleUSBVideoPT::setPanTiltAbs(int iPan, int iTilt, int iUnitID){
//	cout << "moduleUSBVideoPT::setPanTiltAbs(int iPan, int iTilt, int iUnitID)" << endl;
/* Set angles; when absolute = 1, the angle is absolute and the
   driver calculates the relative offset for you.*/

/*
  iPan = (iPan < getMinPan() ? getMinPan() : iPan);
  iPan = (iPan > getMaxPan() ? getMaxPan() : iPan);

  iTilt = (iTilt < getMinTilt() ? getMinTilt() : iTilt);
  iTilt = (iTilt > getMaxTilt() ? getMaxTilt() : iTilt);
*/

    sPwc_mpt_angles.absolute = 1;  
    sPwc_mpt_angles.pan = iPan;           /* degrees * 100 */
    sPwc_mpt_angles.tilt = iTilt;         /* degress * 100 */
//	sPwc_mpt_angles.zoom = -1;            /* N/A, set to -1 */
    ioctl(device_fd, VIDIOCPWCMPTSANGLE, &sPwc_mpt_angles);
}


//     void iqrcommon::moduleUSBVideoPT::setPanTiltAbs(double fPan, double fTilt){
// 	int iPan = (int)((fPan -.5) * fPanRange);
// 	int iTilt = (int)((fTilt - .5) * fTiltRange);
// //	cout << "iPan, iTilt: " << iPan << ", " << iTilt << endl;
// 	setPanTiltAbs(iPan, iTilt);
//     }


void iqrcommon::moduleUSBVideoPT::setPanTiltRel(int iPanIncr, int iTiltIncr,  int iUnitID){
//	cout << "iPanIncr: " << iPanIncr <<  endl;
    /* performe boundary check before setting the increment */
    //-- ioctl(device_fd, VIDIOCPWCMPTGANGLE, &sPwc_mpt_angles);
    sPwc_mpt_angles.absolute = 0;  
    sPwc_mpt_angles.pan = iPanIncr; 
    sPwc_mpt_angles.tilt = iTiltIncr;
//	sPwc_mpt_angles.zoom = -1;   
    ioctl(device_fd, VIDIOCPWCMPTSANGLE, &sPwc_mpt_angles);
}

void iqrcommon::moduleUSBVideoPT::printPanTiltPosition(){
    ioctl(device_fd, VIDIOCPWCMPTGANGLE, &sPwc_mpt_angles);
    cout << "sPwc_mpt_angles.pan: " << sPwc_mpt_angles.pan << endl;
    cout << "sPwc_mpt_angles.tilt: " << sPwc_mpt_angles.tilt << endl;
}

void iqrcommon::moduleUSBVideoPT::printDeviceInfo() {
    moduleUSBVideo::printDeviceInfo();
    cout << "pan min :       " << sPwc_mpt_range.pan_min  << endl;
    cout << "pan max :       " << sPwc_mpt_range.pan_max  << endl;
    cout << "tilt min:       " << sPwc_mpt_range.tilt_min << endl;
    cout << "tilt max:       " << sPwc_mpt_range.tilt_max << endl;
//	cout << "zoom min:       " << sPwc_mpt_range.zoom_min << endl;
//	cout << "zoom max:       " << sPwc_mpt_range.zoom_max << endl;
}

    
void iqrcommon::moduleUSBVideoPT::cleanup(){
//	cout << "moduleUSBVideoPT::cleanup()" << endl;

    if(device_fd>=0){
	resetPanTilt(); //doesn't seem to work... 

    }
    moduleUSBVideo::cleanup();
}

void iqrcommon::moduleUSBVideoPT::update(){
//    cout << "moduleUSBVideoPT::update()" << endl;
    moduleUSBVideo::update();

    clsPanTilt->doPanTilt((int)getCurrentPan(), (int)getCurrentTilt());
    
}



moduleIcon iqrcommon::moduleUSBVideoPT::getIcon() {
    moduleIcon mi(ModuleIcon_CameraUSB_png_data,  ModuleIcon_CameraUSB_png_len, 3 ,5);
    return mi;
}



    
    
    
    
    
