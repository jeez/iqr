#include "moduleJoystick.hpp"

#include <unistd.h>
#include <sstream>
#include <iomanip>

#include <Common/Helper/iqrUtils.h>

#define JSCELLS 8

//#define DEBUG_MODULEUSBCHEMO


MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleJoystick,
			  "Joystick Module")

   
    iqrcommon::moduleJoystick::moduleJoystick() : ClsThreadModule() {
    tid = 0;
    clsStringParameterDeviceName = addStringParameter("DeviceName", /* _strName */ 
						      "Serial Port", /* _strLabel */
						      "/dev/input/js0", /* _strValue */
						      true, /* _bEditable */
						      false, /* _bLong */
						      "Path to Joystick Port", /* _strDescription */
						      "" /* _strCategory */);

    clsStateVariableJSReadings = addOutputToGroup("JSOutput", "Joystick Readings");

    joystick = NULL;

}


iqrcommon::moduleJoystick::~moduleJoystick(){ 
#ifdef DEBUG_MODULEUSBCHEMO
    cout << "moduleJoystick::~moduleJoystick()" << endl;
#endif
    cleanup();
}


void iqrcommon::moduleJoystick::init(){
#ifdef DEBUG_MODULEUSBCHEMO
    cout << "moduleJoystick::init()" << endl;
#endif
    if(clsStateVariableJSReadings->getStateArray().getWidth()!=(unsigned int)JSCELLS){
	throw ModuleError(string("Module \"") + label() + "\": needs " + iqrUtils::int2string(JSCELLS) + " cells for joystick data");
    }

    string strSerialPort = clsStringParameterDeviceName->getValue(); 
    joystick = new Joystick(strSerialPort);
}



void iqrcommon::moduleJoystick::update(){
//    cerr << "moduleJoystick::update()" << endl;
/*
       ------------------------------
       | LeftForward | RightForward |
       ------------------------------
       | LeftBackwd  | RightBackwd  |
       ------------------------------
       |   LeftUp    |   RightUp    |
       ------------------------------
       |  LeftDown   |  RightDown   |
       ------------------------------
*/

    qmutexThread->lock(); 
    StateArray &stateArray = clsStateVariableJSReadings->getStateArray();

    if(joystick->acquire(ad)){
	fLeftForward = (ad.motorLV < 0 ?  -ad.motorLV/255. : 0);
	fLeftBackward = (ad.motorLV > 0 ?  ad.motorLV/255. : 0);
	fLeftLeft = (ad.motorLH < 0 ? -ad.motorLH/255. : 0);
	fLeftRight = (ad.motorLH > 0 ? ad.motorLH/255. : 0);
	
	fRightForward = (ad.motorRV < 0 ?  -ad.motorRV/255. : 0);
	fRightBackward = (ad.motorRV > 0 ?  ad.motorRV/255. : 0);
	fRightLeft = (ad.motorRH < 0 ? -ad.motorRH/255. : 0); 
	fRightRight = (ad.motorRH > 0 ? ad.motorRH/255. : 0);


	stateArray[0][0] = fLeftForward + fLeftLeft - fLeftRight;
	stateArray[0][1] = fLeftForward + fLeftRight - fLeftLeft;

	stateArray[0][2] = fLeftBackward - fLeftLeft + fLeftRight;
	stateArray[0][3] = fLeftBackward - fLeftRight + fLeftLeft;

	stateArray[0][4] = fRightForward + fRightLeft - fRightRight;
	stateArray[0][5] = fRightForward + fRightRight - fRightLeft;

	stateArray[0][6] = fRightBackward - fRightLeft + fRightRight;
	stateArray[0][7] = fRightBackward - fRightRight + fRightLeft;

	const double fMax = 1;
	stateArray[0][stateArray[0] > fMax] = fMax;
    }
    qmutexThread->unlock(); 
    usleep(10e4);
}

void iqrcommon::moduleJoystick::cleanup(){
    delete joystick;
}




