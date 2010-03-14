#ifndef MODULEKHEPERA_HPP
#define MODULEKHEPERA_HPP

#include <iostream>

#ifdef IQRMODULE
#include <Common/Item/module.hpp>
#endif

#include <ClsBaseKRobot.hpp>
#include <Common/Item/stateArray.hpp>
#include <Common/Item/namedPtr.hpp>

#include <ClsKhepDisp.hpp>

#include "wSerial.hpp"

using namespace std;

namespace iqrcommon {

    class moduleKhepera : public ClsBaseKRobot {

    public:
	moduleKhepera();
	~moduleKhepera();

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

	moduleKhepera(const moduleKhepera&);

	void setSpeed(StateArray& stateArray);
	void readProximity(StateArray& stateArray);
	void readAmbient(StateArray& stateArray);

	void replyToData(string strReply, StateArray& stateArray, double fMaxValue);
	void cleanupReply(string &str);


	WSerial serialPort;


/* parameter */
	bool bLEDFrontal;
	bool bLEDLateral;

	bool bShowOutput;

//	std::stringstream ss;
	int iCounter;

/* display */
	ClsKhepDisp* clsKhepDisp;

    };
}

#endif
