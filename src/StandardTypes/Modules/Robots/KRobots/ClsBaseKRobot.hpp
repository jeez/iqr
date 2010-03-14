#ifndef ROBOTBASEMODULE_HPP
#define ROBOTBASEMODULE_HPP  

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/videodev.h>
#include <iostream>
#include <inttypes.h>
#include <errno.h>
#include <string>

#ifdef IQRMODULE
//#include <Common/Item/module.hpp>
#include <Common/Item/threadModule.hpp>
#endif

#include <Common/Item/stateArray.hpp>

using namespace std;

namespace iqrcommon {

#ifdef IQRMODULE
    class ClsBaseKRobot : public ClsThreadModule {
//    class ClsBaseKRobot : public ClsModule {
#else
    class ClsBaseKRobot  {
#endif

    public:

	ClsBaseKRobot();
	virtual void init();

#ifdef IQRMODULE
//	moduleIcon getIcon();
#endif

    protected:

	pair<double,double>  calculateVectorMM(StateArray& stateArray);
	pair<double,double> calculateTabularMM(StateArray& stateArray);


	virtual void setSpeed(StateArray& stateArray)=0;

	inline void printStateArray(StateArray& stateArray){
	    for(unsigned int ii=0; ii<stateArray.getWidth(); ii++){
		cout << stateArray[0][ii] << ";";
	    }
	    cout << endl;
	}

#ifdef IQRMODULE  
	ClsBaseKRobot(const ClsBaseKRobot&);
	ClsStateVariable* clsStateVariableProxialOutput;
	ClsStateVariable* clsStateVariableAmbientOutput;
	ClsStateVariable* clsStateVariableMonitorOutput;

	StateVariablePtr* stateVariableMotorIn;

//-	ClsIntParameter* clsIntParameterMovementType;
	ClsOptionsParameter* clsOptionsParameterMotorControlType;
	ClsOptionsParameter* clsOptionsParameterMotorMapType;
	ClsDoubleParameter* clsDoubleParameterSpeedMax;
	ClsIntParameter* clsIntParameterSCProportional;
	ClsIntParameter* clsIntParameterSCIntegral;
	ClsIntParameter* clsIntParameterSCDifferential;

#endif
	/* parameters */
//-	int iMovementType;
	double fSpeedMax;
#ifdef USE_MOTORCONTROL_TYPES
	bool bUseDiscreteMotoControl;
#endif
	bool bUseVectorMotorMap;

	int iSCProportional;
	int iSCIntegral;
	int iSCDifferential;


	static const int PROXIMAL_OUT_COUNT = 8;
	static const int AMBIENT_OUT_COUNT = 8;
	static const int MONITOR_OUT_COUNT = 2;

    private:
#ifdef USE_MOTORCONTROL_TYPES
	static const char* MCContinuousSpeed()  { return "ContinuousSpeed"; }
	static const char* MCDiscreteSpeed()  { return "DiscreteSpeed"; }
#endif

	static const char* MMVector()  { return "VectorMotorMap"; }
	static const char* MMTable()  { return "TabularMotorMap"; }


    };
}
#endif

