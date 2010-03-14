#ifndef USBVIDEOPT_HPP
#define USBVIDEOPT_HPP

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

#define __le16 __u16 //this is a pretty ugly hack...

#include <pwc-ioctl.h>

#include <Common/Item/module.hpp>


//#include "ClsXWin.h"
#include <ClsPanTilt.hpp>
#include <moduleUSBVideo.hpp>



//extern int errno;


using namespace std;

namespace iqrcommon {


//    class moduleUSBVideoPT : public ClsModule {
    class moduleUSBVideoPT : public moduleUSBVideo {

    public:

	moduleUSBVideoPT();
	~moduleUSBVideoPT();
	void init();
	void update();
	void cleanup();

	moduleIcon getIcon();

    private:
	moduleUSBVideoPT(const moduleUSBVideoPT&);

	ClsPanTilt<moduleUSBVideoPT>* clsPanTilt;

	/* pan/tilt stuff */
	struct pwc_mpt_angles sPwc_mpt_angles;
	struct pwc_mpt_range sPwc_mpt_range;


	double fPanRange;
	double fTiltRange;

	void printDeviceInfo();
	const char* getLabel()  {
	    return "USB video module with Pan/Tilt";
	}


	const double getCurrentPan();
	const double getCurrentTilt();
	const double getMinPan();
	const double getMaxPan();
	const double getMinTilt();
	const double getMaxTilt();

	void setPanTiltAbs(int iPan, int iTilt, int iUnitID = 0);
	void setPanTiltRel(int iPanIncr, int iTiltIncr, int iUnitID = 0);
	void resetPanTilt();
	void printPanTiltPosition();

	template <class TParent> friend class ClsPanTilt;

    };
}
#endif

    
