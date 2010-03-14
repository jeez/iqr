#ifndef MODULEEPUCK_HPP
#define MODULEEPUCK_HPP

#include <iostream>

#ifdef IQRMODULE
#include <Common/Item/module.hpp>
#endif

#include <ClsBaseKRobot.hpp>
#include <Common/Item/stateArray.hpp>
#include <Common/Item/namedPtr.hpp>

#include <ClsEPuckDisp.hpp>

#include <qextserialport.h>


using namespace std;

namespace iqrcommon {

    class moduleEPuck : public ClsBaseKRobot {

    public:
	moduleEPuck();
	~moduleEPuck();

	void init();
	void update();
	void cleanup();

    private:

#ifdef IQRMODULE
	ClsStringParameter* clsStringParameterSerialPort;
	ClsBoolParameter* clsBoolParameterShowOutput;
	ClsBoolParameter* clsBoolParameterLEDFrontal;
	ClsBoolParameter* clsBoolParameterLEDLateral;
#endif

	moduleEPuck(const moduleEPuck&);

	void setSpeed(StateArray& stateArray);
	void readProximity(StateArray& stateArray);
	void readAmbient(StateArray& stateArray);

	void replyToData(string strReply, StateArray& stateArray, double fMaxValue);
	void cleanupReply(string &str);
      string readLine();


	QextSerialPort *serialPort;


/* parameter */
	bool bLEDFrontal;
	bool bLEDLateral;

	bool bShowOutput;

// ivan debug
	double lastFLeft;
	double lastFRight;

//	std::stringstream ss;
	int iCounter;

/* display */
	ClsEPuckDisp* clsEPuckDisp;

    };
}

#endif
