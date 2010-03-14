#ifndef MODULEKOALAW_HPP
#define MODULEKOALAW_HPP

#include <iostream>

#include <Common/Item/module.hpp>
#include <ClsBaseKRobot.hpp>
#include <Common/Item/stateArray.hpp>
#include <Common/Item/namedPtr.hpp>
#include <Common/Item/stateVariable.hpp>
#include "ClsPanTilt.hpp"
#include "ClsKoalaW.hpp"
#include "KoalaSharedDef.h"

#define NRWHISKERS 8
#define NRBATTERYCELLS 1

#define MAX_KOALA_COMMAND_LENGTH	128
#define MAX_KOALA_REPLY_LENGTH		128
#define MAX_KOALA_COMM_RETRIES  0x10


using namespace std;

namespace iqrcommon {

    class moduleKoalaW  : public ClsBaseKRobot {

    public:
	moduleKoalaW();
	~moduleKoalaW();

	void init();
	void update();
	void cleanup();

    private:
	moduleKoalaW(const moduleKoalaW&);

	void setSpeed(StateArray& stateArray);
	void readWhiskers();
	void readBatteryLevel();
	void doPanTilt();


	void setPanTiltAbs(int iPan, int iTilt, int iUnitID = 0){};
	void setPanTiltRel(int iPanIncr, int iTiltIncr, int iUnitID = 0){};

/*
  const double getCurrentPan();
  const double getCurrentTilt();
*/

	/*unsigned*/ char KoalaCommandString[MAX_KOALA_COMMAND_LENGTH];
	unsigned char KoalaReplyString[MAX_KOALA_REPLY_LENGTH];
	unsigned int KoalaCommRetry;
	int KoalaWhiskerStatus;


	const double getMinPan(){return 50.;};
	const double getMaxPan(){return 900.;};
	const double getMinTilt(){return 50.;};
	const double getMaxTilt(){return 900.;};

	ClsKoalaW clsKoalaW;

	ClsPanTilt<moduleKoalaW>* clsPanTiltLeft;
	ClsPanTilt<moduleKoalaW>* clsPanTiltRight;

	template <class TParent> friend class ClsPanTilt;
	pair<double,double> pairPTLeftPosition;
	pair<double,double> pairPTRightPosition;


	ClsStateVariable* clsStateVariableBatteryStatus;
	ClsStateVariable* clsStateVariableWhiskers;
	
    };
}

#endif
