#ifndef MODULEKOALAW_HPP
#define MODULEKOALAW_HPP

#include <iostream>

#include <Common/Item/threadModule.hpp>
#include "ClsPanTilt.hpp"

#include "sony_evi.h" 


using namespace std;

namespace iqrcommon {

    class moduleSonyPanTilt  : public ClsThreadModule {

    public:
	moduleSonyPanTilt();
	~moduleSonyPanTilt();

	void init();
	void update();
	void cleanup();

    private:
	moduleSonyPanTilt(const moduleSonyPanTilt&);

	void doPanTilt();

	void setPanTiltAbs(int iPan, int iTilt, int iUnitID = 0){};
	void setPanTiltRel(int iPanIncr, int iTiltIncr, int iUnitID = 0){};

	const double getMinPan(){return -MAX_HORZPOS;};
	const double getMaxPan(){return MAX_HORZPOS;};
	const double getMinTilt(){return -MAX_VERTPOS;};
	const double getMaxTilt(){return MAX_VERTPOS;};

	SonyEvi *cam;

	ClsPanTilt<moduleSonyPanTilt>* clsPanTilt;

	template <class TParent> friend class ClsPanTilt;
	pair<double,double> pairPTPosition;

	ClsIntParameter* clsIntParameterCameraID;
	int iCameraID;
	ClsStringParameter* clsStringParameterDeviceName;


    };
}

#endif
