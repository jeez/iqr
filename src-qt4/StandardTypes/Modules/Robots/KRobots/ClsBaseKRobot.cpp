#include "ClsBaseKRobot.hpp"

#ifdef IQRMODULE
#include <Common/Helper/iqrUtils.h>
#endif



iqrcommon::ClsBaseKRobot::ClsBaseKRobot() {
    
#ifdef IQRMODULE
//-     clsIntParameterMovementType = addIntParameter("MovementType", /* _strName */
//- 						  "Type of Movement",  /* _strLabel */
//- 						  0, /* _iValue */
//- 						  0, /* _iMinimum */
//- 						  1, /* _iMaximum */ 
//- 						  "Type of Movement (...)" /* _strDescription */,
//- 						  "Options" /* _strCategory */);

    clsDoubleParameterSpeedMax = addDoubleParameter("SpeedMax", /* _strName */
						    "Max Speed",  /* _strLabel */
						    255, /* _dValue */
						    0, /* _dMinimum */
						    1024, /* _dMaximum */ 
						    2, /* _iPrecision */
						    "Maximum Speed for the Robot", /* _strDescription */
						    "Robot" /* _strCategory */);

#ifdef USE_MOTORCONTROL_TYPES
    clsOptionsParameterMotorControlType = addOptionsParameter("MotorControl",
							  "Type of Motor Control",
							  false,
							  "Type of Motor Control",
							  "Robot");

    clsOptionsParameterMotorControlType->addOption(MCDiscreteSpeed());
    clsOptionsParameterMotorControlType->addOption(MCContinuousSpeed());
    setParameter("MotorControl", MCDiscreteSpeed());
#endif

    clsOptionsParameterMotorMapType = addOptionsParameter("MotorMap",
							  "Type of Motor Map",
							  false,
							  "Type of Motor Map",
							  "Robot");

    clsOptionsParameterMotorMapType->addOption(MMVector());
    clsOptionsParameterMotorMapType->addOption(MMTable());
    setParameter("MotorMap", MMTable());



    clsIntParameterSCProportional = addIntParameter("SCProportional", /* _strName */
					      "Speed Controler Proportional",  /* _strLabel */
					      3800, /* _iValue */
					      0, /* _iMinimum */
					      3800, /* _iMaximum */ 
					      "Set Speed Controler Proportional Value" /* _strDescription */,
					      "Robot" /* _strCategory */);
    
    clsIntParameterSCIntegral = addIntParameter("SCIntegral", /* _strName */
					      "Speed Controler Integral",  /* _strLabel */
					      800, /* _iValue */
					      0, /* _iMinimum */
					      3800, /* _iMaximum */ 
					      "Set Speed Controler Integral Value" /* _strDescription */,
					      "Robot" /* _strCategory */);
    
    clsIntParameterSCDifferential = addIntParameter("SCDifferential", /* _strName */
					      "Speed Controler Differential",  /* _strLabel */
					      100, /* _iValue */
					      0, /* _iMinimum */
					      3800, /* _iMaximum */ 
					      "Set Speed Controler Differential Value" /* _strDescription */,
					      "Robot" /* _strCategory */);

    clsStateVariableProxialOutput = addOutputToGroup("proximalOutput", "Proxmial Sensors Output (8 cells)");
    clsStateVariableAmbientOutput = addOutputToGroup("ambientOutput", "Ambient Sensors Output (8 cells)" );
    clsStateVariableMonitorOutput = addOutputToGroup("monitorOutput", "Position Monitor Output (2 cells)");


    stateVariableMotorIn = addInputFromGroup("motorInput", "Motor Input");
#endif

}


void iqrcommon::ClsBaseKRobot::init(){
//    cout << "ClsBaseKRobot::init()" << endl;

#ifdef IQRMODULE	
//-    iMovementType = clsIntParameterMovementType->getValue(); 
    fSpeedMax = clsDoubleParameterSpeedMax->getValue();

#ifdef USE_MOTORCONTROL_TYPES
    string str = clsOptionsParameterMotorControlType->getValue();
    if(!str.compare(MCDiscreteSpeed())){
	bUseDiscreteMotoControl = true;
    }else {
	bUseDiscreteMotoControl = false;
    }
#endif

    string str = clsOptionsParameterMotorMapType->getValue();
    if(!str.compare(MMVector())){
	bUseVectorMotorMap = true;
    }else {
	bUseVectorMotorMap = false;
    }



    iSCProportional = clsIntParameterSCProportional->getValue();
    iSCIntegral = clsIntParameterSCIntegral->getValue();
    iSCDifferential = clsIntParameterSCDifferential->getValue();

#else
//-    iMovementType = 0;
    fSpeedMax = 255;
#ifdef USE_MOTORCONTROL_TYPES
    bUseDiscreteMotoControl = false;
#endif
    iSCProportional = 3800;
    iSCIntegral = 800;
    iSCDifferential = 100;
#endif

	    
#ifdef IQRMODULE	
    /* required sizes: */
    if(clsStateVariableProxialOutput->getStateArray().getWidth()!=(unsigned int)PROXIMAL_OUT_COUNT){
	throw ModuleError(string("Module \"") + "" /*label()*/ + "\": needs" + iqrUtils::int2string(PROXIMAL_OUT_COUNT) + "cells for proximal sensor data");
    }

    if(clsStateVariableAmbientOutput->getStateArray().getWidth()!=(unsigned int)AMBIENT_OUT_COUNT){
	throw ModuleError(string("Module \"") + "" /*label()*/ + "\": needs" + iqrUtils::int2string(AMBIENT_OUT_COUNT) + "cells for ambient sensor data");
    }

    if(clsStateVariableMonitorOutput->getStateArray().getWidth()!=(unsigned int)MONITOR_OUT_COUNT){
	throw ModuleError(string("Module \"") + "" /*label()*/ + "\": needs" + iqrUtils::int2string(MONITOR_OUT_COUNT) + "cells for monitor data");
    }
#endif
}

    

pair<double,double> iqrcommon::ClsBaseKRobot::calculateVectorMM(StateArray& stateArray) {
//    cout << "ClsBaseKRobot::calculateVectorMM(StateArray& stateArray)" << endl;
    double fTLSum = 0, fTRSum = 0, fBLSum = 0, fBRSum = 0;

    int iNrCells = stateArray.getWidth();
    double fEdgeLength = sqrt((double)iNrCells);

    for(int ix=0; ix<(fEdgeLength/2); ix++){
	for(int iy=0; iy<(fEdgeLength/2); iy++){
	    int iIndex = (int)((iy*fEdgeLength)+ix);
	    fTLSum += stateArray[0][iIndex];
//	    cout << "\tix, iy, ix*iy: " << ix << ", " << iy << ", " << (iy*fEdgeLength)+ix << endl;
	}
    }

    for(int ix=(int)(fEdgeLength/2); ix<fEdgeLength; ix++){
	for(int iy=0; iy<(fEdgeLength/2); iy++){
	    int iIndex = (int)((iy*fEdgeLength)+ix);
	    fTRSum += stateArray[0][iIndex];
//	    cout << "\tix, iy, ix*iy: " << ix << ", " << iy << ", " << (iy*fEdgeLength)+ix << endl;
	}
    }

    for(int ix=0; ix<(fEdgeLength/2); ix++){
	for(int iy=(int)(fEdgeLength/2); iy<fEdgeLength; iy++){
	    int iIndex = (int)((iy*fEdgeLength)+ix);
	    fBLSum += stateArray[0][iIndex];
//	    cout << "\tix, iy, ix*iy: " << ix << ", " << iy << ", " << (iy*fEdgeLength)+ix << endl;
	}
    }

    for(int ix=(int)(fEdgeLength/2); ix<fEdgeLength; ix++){
	for(int iy=(int)(fEdgeLength/2); iy<fEdgeLength; iy++){
	    int iIndex = (int)((iy*fEdgeLength)+ix);
	    fBRSum += stateArray[0][iIndex];
//	    cout << "\tix, iy, ix*iy: " << ix << ", " << iy << ", " << (iy*fEdgeLength)+ix << endl;
	}
    }

    
    fTLSum /= ((double)iNrCells/4.);
    fTRSum /= ((double)iNrCells/4.);
    fBLSum /= ((double)iNrCells/4.);
    fBRSum /= ((double)iNrCells/4.);


/*	    
    cout << "fTLSum: " << fTLSum << endl;
    cout << "fTRSum: " << fTRSum << endl;
    cout << "fBLSum: " << fBLSum << endl;
    cout << "fBRSum: " << fBRSum << endl;
*/

/*
    double fLeft = (fTLSum - fTRSum) * fSpeedMax;
    double fRight = (fBLSum - fBRSum) * fSpeedMax;
*/

    double fLeft = (fTLSum - fBLSum) * fSpeedMax/2;
    double fRight = (fTRSum - fBRSum) * fSpeedMax/2;


    pair<double,double> pairSpeed(fLeft, fRight);
    return pairSpeed;
}


pair<double,double> iqrcommon::ClsBaseKRobot::calculateTabularMM(StateArray& stateArray) {
//    cout << "ClsBaseKRobot::calculateTabularMM(StateArray& stateArray)" << endl;
    double fMax = stateArray[0].max();
    
    int iNrCells = stateArray.getWidth();
    double fEdgeLength = sqrt((double)iNrCells);

/* we try to find a vector that point to the center between all max value field */
    double fXMaxCoord = 0, fYMaxCoord = 0;
    int ie = 0;
    for(int ii=0; ii<iNrCells; ii++){
	if(stateArray[0][ii] >= fMax){
	    int iY = (int)(ii/fEdgeLength) + 1;
	    int iX = (int)(ii - (iY -1) * fEdgeLength) + 1;
//	    cout << "iX, iY: " << iX << ", " << iY << endl;
	    fXMaxCoord += iX;
	    fYMaxCoord += iY;
	    ie++;
	}
    }
//    cout << "ie: " << ie << endl;
    fXMaxCoord = fXMaxCoord/(double)ie -1;
    fYMaxCoord = fYMaxCoord/(double)ie -1;


//    cout << "fXMaxCoord, fYMaxCoord: " << fXMaxCoord << ", " << fYMaxCoord << endl;

    double fLeft = fXMaxCoord/(fEdgeLength-1) - fYMaxCoord/(fEdgeLength-1);
    double fRight = (fEdgeLength-1-fYMaxCoord)/(fEdgeLength-1) - fXMaxCoord/(fEdgeLength-1);

    fLeft *= fSpeedMax;
    fRight *= fSpeedMax;

/*
    cout << "fRight: " << fRight << endl;
    cout << "fLeft: " << fLeft << endl;
*/

    pair<double,double> pairSpeed(fLeft, fRight);
    return pairSpeed;
}
