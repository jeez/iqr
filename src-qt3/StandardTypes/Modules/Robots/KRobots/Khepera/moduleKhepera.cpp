#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <iterator>
#include <list>

#include <Common/Helper/iqrUtils.h>
#include "ClsRobotCommands.hpp"

extern int errno;

#include "moduleKhepera.hpp"

/* flushing:
   ss.seekp(0, ios::beg);
*/

#ifdef IQRMODULE
// Interface for dynamic loading is built using a macro.
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleKhepera,
			  "Khepera Module")
#endif

    iqrcommon::moduleKhepera::moduleKhepera() {
#ifdef IQRMODULE
    clsStringParameterSerialPort = addStringParameter("SerialPort", /* _strName */ 
						      "Serial Port", /* _strLabel */
						      "/dev/ttyS0", /* _strValue */
						      true, /* _bEditable */
						      false, /* _bLong */
						      "Path to Serial Port", /* _strDescription */
						      "Device" /* _strCategory */);

    clsBoolParameterShowOutput = addBoolParameter("ShowOutput", /* _strName */ 
						  "Show output", /* _strLabel */
						  false, /* _bValue */
						  "Show Output", /* _strDescription */
						  "Output" /* _strCategory */);
    

    clsBoolParameterLEDFrontal = addBoolParameter("LEDFrontal", /* _strName */ 
						  "Frontal LED on", /* _strLabel */
						  false, /* _bValue */
						  "Turn Frontal LED on", /* _strDescription */
						  "Robot" /* _strCategory */);

    clsBoolParameterLEDLateral = addBoolParameter("LEDLateral", /* _strName */ 
						  "Lateral LED on", /* _strLabel */
						  false, /* _bValue */
						  "Turn Lateral LED on", /* _strDescription */
						  "Robot" /* _strCategory */);
    

#endif

    clsKhepDisp = NULL;

}


iqrcommon::moduleKhepera::~moduleKhepera(){
    cleanup();
}


void iqrcommon::moduleKhepera::init(){
//    cout << "moduleKhepera::init()" << endl;


    ClsBaseKRobot::init();

#ifdef IQRMODULE
    string strSerialPort = clsStringParameterSerialPort->getValue(); 
    bShowOutput = clsBoolParameterShowOutput->getValue(); 
    bLEDFrontal = clsBoolParameterLEDFrontal->getValue();
    bLEDLateral = clsBoolParameterLEDLateral->getValue();
#else
    string strSerialPort = "/dev/ttyS0";
    bLEDFrontal = true;
    bLEDLateral = true;
    bShowOutput = true;
#endif

    string strReply;    

    int iSerialSpeed;


    iSerialSpeed = B38400;
//    iSerialSpeed = B115200;
    serialPort.setDfltTimeOut(50);
    serialPort.setDevice(strSerialPort);
    serialPort.setBaudRate(iSerialSpeed);


    if (serialPort.openDevice()) {
	serialPort.flush();
	serialPort.writeLine("restart");
//	serialPort.writeLine("B");
	bool timedOut;
	bool ok=false;
	do {

	    cout << "strReply: " << strReply << endl;

	    strReply=serialPort.readLine(1000 ,&timedOut);
	    if (strReply.find("Command not found")!=std::string::npos){
		serialPort.writeLine("restart");
	    }
	    else if (strReply.find("ROM of minirobot")!=std::string::npos ||
		strReply.find("hemisson")!=std::string::npos){
		ok=true;
	    }
	} while (!timedOut);

	
	if (!ok) {
	    throw ModuleError(string("Module \"") + label() + "\": Cannot find robot on " + strSerialPort );
	    serialPort.closeDevice();
	}


    } else {
	throw ModuleError(string("Module \"") + label() + "\": opening serial port " + strSerialPort);
    }

    iCounter = 0;


//    serialPort.writeLine(str.sprintf("%s,0,0", robotCmds::setSpeed()));
//    ss.clear();
    std::stringstream ssSpeed;
    ssSpeed << robotCmds::setSpeed() << ",0,0";
    serialPort.writeLine(ssSpeed.str()); // cout << "+command: " << ssSpeed.str() << endl;
    strReply=serialPort.readLine();
    if (!strReply.find("d")!=std::string::npos) {
	serialPort.flush();
    }
    
//    serialPort.writeLine(str.sprintf("%s,%d,%d,%d",  robotCmds::setSpeedControler(), 
//				      iSCProportional, 
//				      iSCIntegral, 
//				      iSCDifferential));
//    ss.clear();
    std::stringstream ssControler;
    ssControler << robotCmds::setSpeedControler() 
		<< "," << iSCProportional
		<< "," << iSCIntegral
		<< "," << iSCDifferential;
    serialPort.writeLine(ssControler.str()); // cout << "+command: " << ssControler.str() << endl;
    if (!serialPort.readLine().find("a")!=std::string::npos){
	serialPort.flush();
    }

//    serialPort.writeLine(str.sprintf("%s,1,%d",  robotCmds::setLEDs(), 
//				      (bLEDFrontal ? 1 : 0)));
//    ss.clear();
    std::stringstream ssLEDFrontal;
    ssLEDFrontal << robotCmds::setLEDs() << ",1," <<  (bLEDFrontal ? 1 : 0);
    serialPort.writeLine(ssLEDFrontal.str()); // cout << "+command: " << ssLEDFrontal.str() << endl;
    if (!serialPort.readLine().find("l")!=std::string::npos) {
	serialPort.flush();
    }


//    serialPort.writeLine(str.sprintf("%s,0,%d",  robotCmds::setLEDs(), 
//	    (bLEDLateral ? 1 : 0)));
//    ss.clear();
    std::stringstream ssLEDLateral;
    ssLEDLateral << robotCmds::setLEDs() << ",0," << (bLEDLateral ? 1 : 0);
    serialPort.writeLine(ssLEDLateral.str()); // cout << "+command: " << ssLEDLateral.str() << endl;
    if (!serialPort.readLine().find("l")!=std::string::npos) {
	serialPort.flush();
    }


    if(bShowOutput){
	clsKhepDisp = new ClsKhepDisp("Khepera Display");
	clsKhepDisp->setGeometry(0, 0, 250, 250);
	clsKhepDisp->DrawStatus( );
	
	clsKhepDisp->show();
    }
};


void iqrcommon::moduleKhepera::update(){
//    cout << "moduleKhepera::update()" << endl;

    iCounter++;

#ifdef IQRMODULE  
    qmutexThread->lock(); //new
    StateArray &clsStateArrayProxialOutput = clsStateVariableProxialOutput->getStateArray();
    StateArray &clsStateArrayAmbientOutput = clsStateVariableAmbientOutput->getStateArray();
    StateArray &clsStateArrayMotorInput = stateVariableMotorIn->getTarget()->getStateArray();
#else
    StateArray clsStateArrayMotorInput(16);

    clsStateArrayMotorInput[0][2] = rand()/RAND_MAX;
    clsStateArrayMotorInput[0][3] = rand()/RAND_MAX;;
    clsStateArrayMotorInput[0][14] = rand()/RAND_MAX;
    clsStateArrayMotorInput[0][15] = rand()/RAND_MAX;
    
    StateArray clsStateArrayProxialOutput(8);
    StateArray clsStateArrayAmbientOutput(8);
#endif


    if(bShowOutput){
	clsKhepDisp->setAmbient(clsStateArrayAmbientOutput[0]);
	clsKhepDisp->setProximity(clsStateArrayProxialOutput[0]);
    }
    
    
    readProximity(clsStateArrayProxialOutput);
    readAmbient(clsStateArrayAmbientOutput);
    setSpeed(clsStateArrayMotorInput);


//    cout << "Proximity: ";
//    printStateArray(clsStateArrayProxialOutput);
//    cout << "Ambient: ";
//    printStateArray(clsStateArrayAmbientOutput);
  
  
//    cout << "Motor: ";
//    printStateArray(clsStateArrayMotorInasput);

    qmutexThread->unlock(); //new


#ifndef IQRMODULE      
    //     usleep(300000);
    sleep(1);
#endif

    usleep(100000);


};





void iqrcommon::moduleKhepera::setSpeed(StateArray& stateArray){

    pair<double, double> pairSpeed;
//    cout << "bUseVectorMotorMap: " << bUseVectorMotorMap << endl;
    if(bUseVectorMotorMap){
	pairSpeed = calculateVectorMM(stateArray);
    }else {
	pairSpeed = calculateTabularMM(stateArray);
    }


    int iLeftSpeed = (int)(rint(pairSpeed.first));
    int iRightSpeed = (int)(rint(pairSpeed.second));
//    cout << "L,R" << iLeftSpeed << "|" << iRightSpeed << endl;
//    serialPort.writeLine(str.sprintf("D,%d,%d",iLeftSpeed,iRightSpeed));
//    ss.clear();
    std::stringstream ssSpeed;
    ssSpeed << robotCmds::setSpeed() 
	    << "," << iLeftSpeed
	    << "," << iRightSpeed;

    serialPort.writeLine(ssSpeed.str()); // cout << "+command: " << ssSpeed.str() << endl;
    string strReply=serialPort.readLine();
    if (!strReply.find("d")!=std::string::npos){
	serialPort.flush();
    }

    if(bShowOutput){
	QCEventData* ce = new QCEventData(pairSpeed);
	QApplication::postEvent( clsKhepDisp, ce );  
    }
}



void iqrcommon::moduleKhepera::readProximity(StateArray& stateArray){
//    cout << "moduleKhepera::readProximity(StateArray& stateArray)" << endl;
    serialPort.writeLine(robotCmds::readProximity());
    string strReply=serialPort.readLine();
    replyToData(strReply, stateArray, robotCmds::KHEPERA_PROXIMITY_SENSOR_MAX());
}

void iqrcommon::moduleKhepera::readAmbient(StateArray& stateArray){
//    cout << "moduleKhepera::readAmbient(StateArray& stateArray)" << endl;
    serialPort.writeLine(robotCmds::readAmbient());
    string strReply=serialPort.readLine();
    replyToData(strReply, stateArray, robotCmds::KHEPERA_AMBIENT_SENSOR_MAX());
}





void iqrcommon::moduleKhepera::replyToData(string strReply, StateArray& stateArray, double fMaxValue){
//    cout << "moduleKhepera::replyToData(string strReply, StateArray& stateArray, double fMaxValue)" << endl;
    int ii = 0;    
    cleanupReply(strReply);

    istringstream is(strReply);
    istream_iterator<int> itStart(is);
    istream_iterator<int> itEnd;
    
    int iContainerSize = stateArray.getWidth(); 
    while (itStart != itEnd && ii<iContainerSize){
//	cout << "itStart: " << *itStart << endl;
	stateArray[0][ii]  = (double)*itStart / fMaxValue; 
//	cout << (double)*itStart << endl;
	++itStart;
	ii++;
    }
}



void iqrcommon::moduleKhepera::cleanup(){
//    cout << "moduleKhepera::cleanup()" << endl;

//    serialPort.writeLine(str.sprintf("%s,0,0", robotCmds::setSpeed()));
//    ss.clear();
    std::stringstream ssSpeed;
    ssSpeed << robotCmds::setSpeed() << ",0,0";

    if(serialPort.isOpen()){
	serialPort.writeLine(ssSpeed.str()); // cout << "+command: " << ssSpeed.str() << endl;
	string strReply=serialPort.readLine();
	if (!strReply.find("d")!=std::string::npos) {
	    serialPort.flush();
	}
	serialPort.closeDevice();
    }
    
    if(clsKhepDisp!=NULL){
	clsKhepDisp->hide();
	delete clsKhepDisp;
	clsKhepDisp = NULL;
    }

}


void iqrcommon::moduleKhepera::cleanupReply(string &str){
    str.erase(0,2);
    
    string strSearch = ",";
    while (true) {
	string::size_type idx;
	idx = str.find(strSearch);
	if (idx == string::npos){
	    break;
	}
	str.replace(idx, strSearch.size(), " ");
    }   
}




