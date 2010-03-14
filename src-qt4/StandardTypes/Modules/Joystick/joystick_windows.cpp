#include <unistd.h>
#include "joystick_windows.h"

Joystick::Joystick(string _strPortName) : strPortName(_strPortName){
  connected = false;
  wDeviceID = -1;

  UINT wNumDevs;
  wNumDevs = joyGetNumDevs();
  if(wNumDevs>0){
    JOYINFO joyinfo; 
    BOOL bDev1Attached, bDev2Attached; 
    bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
    bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != JOYERR_UNPLUGGED; 
    if(bDev1Attached || bDev2Attached) {
      wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2; 
    } else {
      cerr << "error opening joystick" << endl;
    }
  }
}


void Joystick::getJoyRanges(){
  if(wDeviceID>=0){
    JOYCAPS joycaps;
    joyGetDevCaps(wDeviceID, &joycaps, sizeof(joycaps));

/* this does not seem to work... */
    fXMin = joycaps.wXmin;
    fXRange = joycaps.wXmax - fXMin;
    
    fYMin = joycaps.wYmin;
    fYRange = joycaps.wYmax - fYMin;
    
    fRMin = joycaps.wRmin;
    fRRange = joycaps.wRmax - fRMin;
    
    fUMin = joycaps.wUmin;
    fURange = joycaps.wUmax - fUMin;

  }
}


Joystick::~Joystick() {
//TEMP   if (wDeviceID == -1){
//TEMP     close(wDeviceID);
//TEMP   }
}


bool Joystick::acquire(ActorData & ad) {

  if(wDeviceID>=0){
    JOYINFOEX  joyinfo; 
    if(joyGetPosEx(wDeviceID,&joyinfo)==JOYERR_NOERROR){
    ad.motorLH = normalize(joyinfo.dwXpos);
    ad.motorLV = normalize(joyinfo.dwYpos);
    ad.motorRH = normalize(joyinfo.dwRpos);
    ad.motorRV = normalize(joyinfo.dwUpos);

    //    cout << "joyinfo.dwXpos: " << joyinfo.dwXpos << endl;
    } else {
      return false;
    }
  }
  return true;
} 

