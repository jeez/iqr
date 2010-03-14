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

#include <Common/Helper/iqrUtils.h>
#include "moduleKoalaW.hpp"

MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleKoalaW,
			  "KoalaW Module")

    iqrcommon::moduleKoalaW::moduleKoalaW() {
    clsPanTiltLeft = new ClsPanTilt<moduleKoalaW>(this, label(), 0, "Left");
    clsPanTiltRight = new ClsPanTilt<moduleKoalaW>(this, label(), 1, "Right");

    clsStateVariableBatteryStatus = addOutputToGroup("batteryStatus", "Battery Status (" + iqrUtils::int2string(NRBATTERYCELLS) + " cells)");
    clsStateVariableWhiskers = addOutputToGroup("whiskers", "Whiskers (" + iqrUtils::int2string(NRWHISKERS) + " cells)");
}


iqrcommon::moduleKoalaW::~moduleKoalaW() {
    cleanup();
}


void iqrcommon::moduleKoalaW::init() {
    ClsBaseKRobot::init();


      if((clsStateVariableBatteryStatus->getStateArray().getWidth())!=NRBATTERYCELLS){
  	throw ModuleError(string("Module \"") + label() + "\": needs " + iqrUtils::int2string(NRBATTERYCELLS) + " cells for \"Battery Status\"");
      }


     if((clsStateVariableWhiskers->getStateArray().getWidth())!=NRWHISKERS){
 	throw ModuleError(string("Module \"") + label() + "\": needs " + iqrUtils::int2string(NRWHISKERS) + "cells for \"Whiskers\"");
     }

    clsPanTiltLeft->init();
    clsPanTiltRight->init();
    clsKoalaW.OpenKoalaConnection();
}

void iqrcommon::moduleKoalaW::cleanup() {
    clsKoalaW.CloseKoalaConnection();

}

void iqrcommon::moduleKoalaW::update() {
    qmutexThread->lock(); //new
    StateArray &clsStateArrayMotorInput = stateVariableMotorIn->getTarget()->getStateArray();
    setSpeed(clsStateArrayMotorInput);
    doPanTilt();
    readBatteryLevel();
    readWhiskers();
    qmutexThread->unlock(); //new
}

void iqrcommon::moduleKoalaW::setSpeed(StateArray& stateArray){
    pair<double, double> pairSpeed;
    if(bUseVectorMotorMap){
	pairSpeed = calculateTabularMM(stateArray);
    }else {
	pairSpeed = calculateVectorMM(stateArray);
    }

    int iLeftSpeed = (int)(rint(pairSpeed.first));
    int iRightSpeed = (int)(rint(pairSpeed.second));



    sprintf (KoalaCommandString, "%c",
//	     TWI_KOALA_ID,
	     CMD_KOALA_SEND
	     );


    sprintf (KoalaCommandString, "%c%cD,%d,%d", TWI_KOALA_ID, CMD_KOALA_SEND, iLeftSpeed, iRightSpeed);


    KoalaCommRetry=0;
    while ((clsKoalaW.WaitForKoalaAnswer() != 0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {
	clsKoalaW.SendKoalaCommand(KoalaCommandString, strlen((const char*)KoalaCommandString));
    }

    if (KoalaCommRetry == MAX_KOALA_COMM_RETRIES) {
	printf ("Unrecoverable error sending command to Koala\n");
    }

}

/* skipped the entire
   Koala Status Display
   section... */


void iqrcommon::moduleKoalaW::readWhiskers() {
    StateArray &clsStateArrayOut = clsStateVariableWhiskers->getStateArray();
    double faReturnValues[NRWHISKERS];

    sprintf (KoalaCommandString, "%c%c", TWI_WHISKERBOARDS_ID, TWI_SCAN_ALL_WHISKERS);	// Generate and send new command
    KoalaCommRetry=0;
    while ((KoalaWhiskerStatus<0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {	// Koala reply is in mAh
	while ((clsKoalaW.WaitForKoalaAnswer() != 0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {
	    clsKoalaW.SendKoalaCommand(KoalaCommandString, strlen((const char*)KoalaCommandString));
	}
	KoalaWhiskerStatus=clsKoalaW.WaitForKoalaWhiskerStatus((faReturnValues));
    }

    if (KoalaWhiskerStatus<0) 	    {
	printf ("Unrecoverable error sending command to Koala\n");
    }

    for(int ii = 0; ii< NRWHISKERS; ii++){
	clsStateArrayOut[0][ii] = faReturnValues[ii];
    }
}



void iqrcommon::moduleKoalaW::readBatteryLevel() {
    StateArray &clsStateArrayOut = clsStateVariableBatteryStatus->getStateArray();

    int BatteryLevel = -1;
    sprintf (KoalaCommandString, "%c%cS", TWI_KOALA_ID, CMD_KOALA_SEND);	// Generate and send new command
    KoalaCommRetry=0;

    while ((BatteryLevel<0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {	// Koala reply is in mAh
	while ((clsKoalaW.WaitForKoalaAnswer() != 0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {
	    clsKoalaW.SendKoalaCommand(KoalaCommandString, strlen((const char*)KoalaCommandString));
	}
	BatteryLevel = clsKoalaW.WaitForKoalaBatteryStatus();
    }

    if (BatteryLevel>=0) {
	clsStateArrayOut[0][0] = BatteryLevel;
    } else {
	printf ("Koala: unrecoverable error  receiving battery voltage!\n");
	clsStateArrayOut[0][0] = 0;
    }
}

void iqrcommon::moduleKoalaW::doPanTilt() {

/*
    pair<double,double> pairPTLeft =
	clsPanTiltLeft->calculatePanTilt(pairPTLeftPosition.first, pairPTLeftPosition.second,
					 getMinPan(), getMaxPan(),
					 getMinTilt(), getMaxTilt());

    pair<double,double> pairPTRight =
	clsPanTiltRight->calculatePanTilt(pairPTRightPosition.first, pairPTRightPosition.second,
					  getMinPan(), getMaxPan(),
					  getMinTilt(), getMaxTilt());


    if(!clsPanTiltLeft->absolutePosition()){
	pairPTLeft.first += pairPTLeftPosition.first;
	pairPTLeft.second += pairPTLeftPosition.second;

    }

    if(!clsPanTiltRight->absolutePosition()){
	pairPTRight.first += pairPTRightPosition.first;
	pairPTRight.second += pairPTRightPosition.second;
    }
*/

    pairPTRightPosition = clsPanTiltRight->calculatePanTilt((int)pairPTRightPosition.first, (int)pairPTRightPosition.second);
    pairPTLeftPosition = clsPanTiltLeft->calculatePanTilt((int)pairPTLeftPosition.first, (int)pairPTLeftPosition.second);











    sprintf (KoalaCommandString, "%c%c%03X%03X%03X%03X%03X%03X%03X%03X",
	     TWI_PANTILT_ID, CMD_PANTILT_SET,
	     ((short)pairPTRightPosition.first /* pan */),
	     ((short)pairPTLeftPosition.second /* tilt */),
	     512, 512,
	     ((short)pairPTRightPosition.first /* pan */),
	     ((short)pairPTLeftPosition.second /* tilt */),
	     512, 512);	// Generate and send new command
    KoalaCommRetry=0;
    while ((clsKoalaW.WaitForKoalaAnswer() != 0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES)) {
	clsKoalaW.SendKoalaCommand(KoalaCommandString, strlen((const char*)KoalaCommandString));
    }

    if (KoalaCommRetry == MAX_KOALA_COMM_RETRIES)
    {
	printf ("Unrecoverable error sending PanTilt command to Koala\n");
    }

}


// void iqrcommon::moduleKoalaW::PanTiltMotion() {
//     // ------------------------------------------------------------------------------------- Koala PanTilt Motion
//     /* OUTPUT SIZE = 4 */
// //                           	offset       range
// #define PanAngleLeft(x)		((440.0)+(270.0*x/45.0))
// #define TiltAngleLeft(x)	((579.0)+(220.0*x/45.0))
// #define PanAngleRight(x)	((434.0)+(270.0*x/45.0))
// #define TiltAngleRight(x)	((560.0)+(220.0*x/45.0))

// // size valarray: 4

//     if(bAddAngles){
// 	KoalaPanTiltAngle[0] += PanAngleLeft(Parameter[0]);
// 	KoalaPanTiltAngle[1] += TiltAngleRight(-Parameter[1]);
// 	KoalaPanTiltAngle[2] += PanAngleLeft(Parameter[2]);
// 	KoalaPanTiltAngle[3] += TiltAngleRight(Parameter[3]);
//     } else {
// 	KoalaPanTiltAngle[0] = PanAngleLeft(Parameter[0]);
// 	KoalaPanTiltAngle[1] = TiltAngleLeft(-Parameter[1]);
// 	KoalaPanTiltAngle[2] = PanAngleRight(Parameter[2]);
// 	KoalaPanTiltAngle[3] = TiltAngleRight(Parameter[3]);
//     }

//     if (KoalaPanTiltAngle[0] > 900.0) KoalaPanTiltAngle[0]= 900.0;
//     if (KoalaPanTiltAngle[0] <  50.0) KoalaPanTiltAngle[0]=  50.0;
//     if (KoalaPanTiltAngle[1] > 900.0) KoalaPanTiltAngle[1]= 900.0;
//     if (KoalaPanTiltAngle[1] <  50.0) KoalaPanTiltAngle[1]=  50.0;
//     if (KoalaPanTiltAngle[2] > 900.0) KoalaPanTiltAngle[2]= 900.0;
//     if (KoalaPanTiltAngle[2] <  50.0) KoalaPanTiltAngle[2]=  50.0;
//     if (KoalaPanTiltAngle[3] > 900.0) KoalaPanTiltAngle[3]= 900.0;
//     if (KoalaPanTiltAngle[3] <  50.0) KoalaPanTiltAngle[3]=  50.0;

//     sprintf (KoalaCommandString, "%c%c%03X%03X%03X%03X%03X%03X%03X%03X",
// 	     TWI_PANTILT_ID, CMD_PANTILT_SET,
// 	     ((short) KoalaPanTiltAngle[2]), ((short) KoalaPanTiltAngle[3]),
// 	     512, 512,
// 	     ((short) KoalaPanTiltAngle[0]), ((short) KoalaPanTiltAngle[1]),
// 	     512, 512);	// Generate and send new command
//     KoalaCommRetry=0;
//     do {
// 	clsKoalaW.SendKoalaCommand(KoalaCommandString, strlen((const char*)KoalaCommandString));
//     }
//     while ((clsKoalaW.WaitForKoalaAnswer() != 0) & (++KoalaCommRetry<MAX_KOALA_COMM_RETRIES));
//     if (KoalaCommRetry == MAX_KOALA_COMM_RETRIES)
//     {
// 	printf ("Unrecoverable error sending PanTilt command to Koala\n");
//     }

// }




