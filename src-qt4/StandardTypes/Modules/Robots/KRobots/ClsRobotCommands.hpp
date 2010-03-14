#ifndef CLSROBOTCOMMANDS_HPP
#define CLSROBOTCOMMANDS_HPP

namespace iqrcommon {

    class robotCmds {
    public:

#ifdef _WINDOWS
	static const string suffix() { return "\r"; }
#else 
	static const string suffix() { return "\n"; }
#endif
	static const char* setSpeed() { return "D"; }
	static const char* setSpeedControler() { return "A"; }
	static const char* setLEDs() { return "L"; }

	static const char* readProximity() { 
	  string str = "\N" + suffix();
	  return str.c_str(); 
	}

      static const char* readAmbient() { 
	  string str = "\O" + suffix();
	  return str.c_str(); 
      }
 


	static const int KHEPERA_NUM_IR_SENSORS(){ return 8; }
	static const double KHEPERA_AMBIENT_SENSOR_MAX(){ return 511.0; }
	static const double KHEPERA_PROXIMITY_SENSOR_MAX(){ return 1023.0; }

	static const int EPUCK_NUM_IR_SENSORS(){ return 8; }
	static const double EPUCK_AMBIENT_SENSOR_MAX(){ return 4000.0; }
	static const double EPUCK_PROXIMITY_SENSOR_MAX(){ return 3650.0; }


    };
}

#endif
