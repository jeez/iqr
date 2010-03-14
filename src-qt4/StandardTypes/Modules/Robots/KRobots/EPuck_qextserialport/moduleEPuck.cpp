//--#include <sys/ioctl.h>
//--#include <sys/types.h>
//--#include <sys/stat.h>

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

#include "moduleEPuck.hpp"


// Interface for dynamic loading is built using a macro.
MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleEPuck, "EPuck Module")

iqrcommon::moduleEPuck::moduleEPuck() {
    clsStringParameterSerialPort = addStringParameter("SerialPort", /* _strName */ 
						      "Serial Port", /* _strLabel */
						      "/dev/rfcomm0", /* _strValue */
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
    


    clsEPuckDisp = NULL;
    serialPort = NULL;
}


iqrcommon::moduleEPuck::~moduleEPuck(){
    cleanup();
}


void iqrcommon::moduleEPuck::init(){
//    cout << "moduleEPuck::init()" << endl;


    ClsBaseKRobot::init();

    string strSerialPort = clsStringParameterSerialPort->getValue(); 
    bShowOutput = clsBoolParameterShowOutput->getValue(); 
    bLEDFrontal = clsBoolParameterLEDFrontal->getValue();
    bLEDLateral = clsBoolParameterLEDLateral->getValue();

    string strReply;    

    int iSerialSpeed;


    serialPort = new QextSerialPort(strSerialPort.c_str());


//--    iSerialSpeed = B38400;
//--//    iSerialSpeed = B115200;

    if(!serialPort->open(QIODevice::ReadWrite)) {
	throw ModuleError(string("Module \"") + label() + "\": Error opening serial port " + strSerialPort);
    }

    serialPort->setBaudRate(BAUD19200);
    serialPort->setBaudRate(BAUD115200);
    serialPort->setFlowControl(/*FLOW_HARDWARE*/ FLOW_OFF);
    serialPort->setParity(PAR_NONE);    
    serialPort->setDataBits(DATA_8);   
    serialPort->setStopBits(STOP_1);    
    serialPort->setTimeout( 0, 3000);


    cerr << "port open: " << serialPort->isOpen() << endl;	

    std::stringstream ssSpeed;
    ssSpeed << robotCmds::setSpeed() << ",0,0" << robotCmds::suffix();
    serialPort->write(ssSpeed.str().c_str(), ssSpeed.str().length()); // cout << "+command: " << ssSpeed.str() << endl;


    strReply=readLine();
    if (!strReply.find("d")!=std::string::npos) {
	serialPort->flush();
    }
    
    std::stringstream ssControler;
    ssControler << robotCmds::setSpeedControler() 
		<< "," << iSCProportional
		<< "," << iSCIntegral
		<< "," << iSCDifferential
		<< robotCmds::suffix();
    serialPort->write(ssControler.str().c_str(), ssControler.str().length()); // cout << "+command: " << ssControler.str() << endl;

    if (!readLine().find("a")!=std::string::npos){
	serialPort->flush();
    }

    std::stringstream ssLEDFrontal;
    ssLEDFrontal << robotCmds::setLEDs() << ",1," <<  (bLEDFrontal ? 1 : 0) << robotCmds::suffix();
    serialPort->write(ssLEDFrontal.str().c_str(), ssLEDFrontal.str().length()); // cout << "+command: " << ssLEDFrontal.str() << endl;
    if (!readLine().find("l")!=std::string::npos) {
	serialPort->flush();
    }

    std::stringstream ssLEDLateral;
    ssLEDLateral << robotCmds::setLEDs() << ",0," << (bLEDLateral ? 1 : 0) << robotCmds::suffix();
    serialPort->write(ssLEDLateral.str().c_str(), ssLEDLateral.str().length()); // cout << "+command: " << ssLEDLateral.str() << endl;
    if (!readLine().find("l")!=std::string::npos) {
	serialPort->flush();
    }


    if(bShowOutput){
	clsEPuckDisp = new ClsEPuckDisp("EPuck Display");
	clsEPuckDisp->setGeometry(0, 0, 250, 250);
	clsEPuckDisp->DrawStatus( );
	
	clsEPuckDisp->show();
    }
};


void iqrcommon::moduleEPuck::update(){
//    cout << "moduleEPuck::update()" << endl;


    qmutexThread->lock(); //new
    StateArray &clsStateArrayProxialOutput = clsStateVariableProxialOutput->getStateArray();
    StateArray &clsStateArrayAmbientOutput = clsStateVariableAmbientOutput->getStateArray();
    StateArray &clsStateArrayMotorInput = stateVariableMotorIn->getTarget()->getStateArray();


    
    readProximity(clsStateArrayProxialOutput);
    usleep(10e4);
    readAmbient(clsStateArrayAmbientOutput);
    usleep(10e4);
    setSpeed(clsStateArrayMotorInput);


    if(bShowOutput){
	clsEPuckDisp->setAmbient(clsStateArrayAmbientOutput[0]);
	clsEPuckDisp->setProximity(clsStateArrayProxialOutput[0]);
    }

//    cout << "Proximity: ";
//    printStateArray(clsStateArrayProxialOutput);
//    cout << "Ambient: ";
//    printStateArray(clsStateArrayAmbientOutput);
  
  
//    cout << "Motor: ";
//    printStateArray(clsStateArrayMotorInasput);

    qmutexThread->unlock(); //new

    //    usleep(10e4);


};





void iqrcommon::moduleEPuck::setSpeed(StateArray& stateArray){

    pair<double, double> pairSpeed;
//    cout << "bUseVectorMotorMap: " << bUseVectorMotorMap << endl;
    if(bUseVectorMotorMap){
	pairSpeed = calculateVectorMM(stateArray);
    } else {
	pairSpeed = calculateTabularMM(stateArray);
    }


    double tmpLeft = pairSpeed.first;
    double tmpRight = pairSpeed.second;

    if ((lastFLeft != tmpLeft) || (lastFRight != tmpRight)) {
//	cout << "fRight: " << tmpLeft << ", lastFRight "<< lastFRight << endl;
//	cout << "fLeft: " << tmpRight << ", lastFLeft "<< lastFLeft << endl;


	lastFLeft = tmpLeft;
	lastFRight = tmpRight;

	int iLeftSpeed = (int)(rint(pairSpeed.first));
	int iRightSpeed = (int)(rint(pairSpeed.second));
//	cout << "L,R" << iLeftSpeed << "|" << iRightSpeed << endl;

	std::stringstream ssSpeed;
	ssSpeed << robotCmds::setSpeed() 
		<< "," << iLeftSpeed
		<< "," << iRightSpeed
		<< robotCmds::suffix();

	serialPort->write(ssSpeed.str().c_str(), ssSpeed.str().length()); //	cout << "+command: " << ssSpeed.str() << endl;

	string strReply=readLine();
	if (!strReply.find("d")!=std::string::npos){
//	    cout << "FLUSH" << endl;
//	    serialPort->flush();
	}

    }
    if(bShowOutput){
	QCEventData* ce = new QCEventData(pairSpeed);
	QApplication::postEvent( clsEPuckDisp, ce );  
    }


}



void iqrcommon::moduleEPuck::readProximity(StateArray& stateArray){
//    cout << "moduleEPuck::readProximity(StateArray& stateArray): " << robotCmds::readProximity() << endl;
    serialPort->write(robotCmds::readProximity(), strlen(robotCmds::readProximity()));
    string strReply=readLine();
//    cout << "strReply: " << strReply << endl;
    replyToData(strReply, stateArray, robotCmds::EPUCK_PROXIMITY_SENSOR_MAX());
}

void iqrcommon::moduleEPuck::readAmbient(StateArray& stateArray){
//    cout << "moduleEPuck::readAmbient(StateArray& stateArray): " << robotCmds::readAmbient() << endl;
    serialPort->write(robotCmds::readAmbient(), strlen(robotCmds::readAmbient()));

    string strReply=readLine();
//    cout << "strReply: " << strReply << endl;
    replyToData(strReply, stateArray, robotCmds::EPUCK_AMBIENT_SENSOR_MAX());
}





void iqrcommon::moduleEPuck::replyToData(string strReply, StateArray& stateArray, double fMaxValue){
//    cout << "moduleEPuck::replyToData(string strReply, StateArray& stateArray, double fMaxValue)" << endl;
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



void iqrcommon::moduleEPuck::cleanup(){
//    cout << "moduleEPuck::cleanup()" << endl;

  
    if(serialPort!=NULL){
	if(serialPort->isOpen()){
	    std::stringstream ssSpeed;
	    ssSpeed << robotCmds::setSpeed() << ",0,0" << robotCmds::suffix();
      
	    serialPort->write(ssSpeed.str().c_str(), ssSpeed.str().length()); // cout << "+command: " << ssSpeed.str() << endl;

	    string strReply=readLine();
	    if (!strReply.find("d")!=std::string::npos) {
		serialPort->flush();
	    }
	    serialPort->close();
	}
    }
  
    if(clsEPuckDisp!=NULL){
	clsEPuckDisp->hide();
	delete clsEPuckDisp;
	clsEPuckDisp = NULL;
    }
  
    delete serialPort;
    serialPort = NULL;
}


void iqrcommon::moduleEPuck::cleanupReply(string &str){
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

string iqrcommon::moduleEPuck::readLine(){
//    cout << "iqrcommon::moduleEPuck::readLine()" << endl;

    char buf[255];
    int i = serialPort->readLine(buf, 255);
    string strBuf = buf;
    //    cout << "buf: " << i << ":" << strBuf << endl << "--------------" << endl;
    return strBuf;





/*
  char buff[1024];
  int numBytes;
  
  numBytes = serialPort->bytesAvailable();
  if(numBytes > 0) {
    if(numBytes > 1024) {
      numBytes = 1024;
    }

    int i = serialPort->read(buff, numBytes);
    buff[i] = '\0';
    QString msg = buff;
    cout << "buf: " << msg.toStdString() << endl << "--------------" << endl;
      
  }
  return "";
*/
}






