#ifndef USBUSBCHEMO_HPP
#define USBUSBCHEMO_HPP

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <bitset>
#include <vector>


//#include <Common/Item/module.hpp>
#include <Common/Item/threadModule.hpp>


#include "joystick.h"

using namespace std;

namespace iqrcommon {

    class moduleJoystick : public ClsThreadModule {
    public:
	moduleJoystick();
	~moduleJoystick();

	void init();
	void update();
	void cleanup();
	
    private:

	ClsStringParameter* clsStringParameterDeviceName;
	ClsStateVariable* clsStateVariableJSReadings;

	Joystick* joystick;
	ActorData ad;

	/* some helpers */
	double fLeftForward, fLeftBackward, fLeftLeft, fLeftRight;
	double fRightForward, fRightBackward, fRightLeft, fRightRight;
    };
}
#endif






    
    
    
    
