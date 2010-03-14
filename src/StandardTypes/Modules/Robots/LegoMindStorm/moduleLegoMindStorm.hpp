#ifndef LEGOMINDSTORM_HPP
#define LEGOMINDSTORM_HPP

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include <Common/Item/threadModule.hpp>




using namespace std;

struct rcxMotorControl {
    int motor_id;
    int motor_command;
    int iMotorPower;
    int motor_dir;
};


namespace iqrcommon {

    class moduleLegoMindStorm : public ClsThreadModule {

    public:
	moduleLegoMindStorm();
	~moduleLegoMindStorm();

	void init();
	void update();
	void cleanup();
	
    private:

	moduleLegoMindStorm(const moduleLegoMindStorm&);

	int get_sensor_mode(string str);
	int get_sensor_type(string str);

	StateVariablePtr* stateVariableMotor;
	StateVariablePtr* stateVariableFloat;
	StateVariablePtr* stateVariableFlip;

	ClsStateVariable* clsStateVariableSensors;


/* parameter */
	ClsStringParameter* clsStringParameterDeviceName;
/*
  ClsDoubleParameter* clsDoubleParameterMotorThresold;
*/

	int fd;

	static const char* SensorModeSet()  { return "set"; }
	static const char* SensorModeRaw()  { return "raw"; }
	static const char* SensorModeBoolean()  { return "boolean"; }
	
	static const char* SensorTypeRaw()  { return "raw"; }
	static const char* SensorTypeTouch()  { return "touch"; }
	static const char* SensorTypeTemp()  { return "temp"; }
	static const char* SensorTypeLight()  { return "light"; }
	static const char* SensorTypeRot()  { return "rot"; }

	vector<ClsOptionsParameter*> vSensorModeOptions;
	vector<ClsOptionsParameter*> vSensorTypeOptions;

	unsigned int iNrMotorInputs;
	vector<rcxMotorControl> vecRcxMotorControls;
	valarray<double> vaSensorReadings;

    };
}
#endif

