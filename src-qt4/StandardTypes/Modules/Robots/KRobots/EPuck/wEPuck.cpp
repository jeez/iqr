#include "wEPuck.hpp"

#include <qregexp.h>
#include <math.h>

#include <wMath.hpp>
#include <wSimLog.hpp>
 
WEPuck::WEPuck(WModule *parent, const QString &name)  
  : WDevice(parent,name), WRobDevice(parent,name), 
    writeLeftSpeed(0), writeRightSpeed(0),
    leftLED(false), rightLED(false),
    deviceName(), deviceSpeed(), 
    setLEDEnabled(true), setPIDEnabled(true) {
  
  addParam(&deviceName,"device","/dev/ttyS0",this,&WEPuck::initDevice)
    ->list()=QStringList()<<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyS2"<<"/dev/ttyS3";
  
  addParam(&deviceSpeed,"bitrate",2,this,&WEPuck::initDevice)
    ->list()<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
  
  WParamNode *pidNode = new WParamNode(this,"PID controller");

  WParam<int> *pidVal;
  pidVal=pidNode->addParam(&pidPropVal,"proportional",3800,
			   this,&WEPuck::setPIDMotorController);
  pidVal->setRange(0,0x7FFF); pidVal->setStep(100);
  
  pidVal=pidNode->addParam(&pidIntVal,"integral",800,
			   this,&WEPuck::setPIDMotorController);
  pidVal->setRange(0,0x7FFF); pidVal->setStep(100);
  
  pidVal=pidNode->addParam(&pidDiffVal,"differential",100,
			   this,&WEPuck::setPIDMotorController);
  pidVal->setRange(0,0x7FFF); pidVal->setStep(100);
  
  serialPort->setDfltTimeOut(50);

  addAction(&WEPuck::initDevice,"initialize robot");
  
  initDevice();
}

WEPuck::~WEPuck() {
  stop();
  serialPort->closeDevice();
}

void WEPuck::initDevice() {
  
  stop();
  
  unsigned int baudRate=B9600;
  switch (deviceSpeed) {
  case 0: baudRate=B9600; break;
  case 1: baudRate=B19200; break;
  case 2: baudRate=B38400; break;
  case 3: baudRate=B57600; break;
  case 4: baudRate=B115200; break;
  }
  
  simlog(INFO,"initializing device %s ...\n",name().latin1());
  
  serialPort->setDevice(deviceName);
  serialPort->setBaudRate(baudRate);
  
  if (serialPort->openDevice()) {
    
    serialPort->flush();
    serialPort->writeLine("restart");
    QString resp;
    
    bool timedOut;
    bool ok=false;
    do {
      resp=serialPort->readLine(500,&timedOut);
      if (!timedOut) simlog(DEBUG_1,"%s\n",resp.latin1());
      if (resp.contains("Command not found")) serialPort->writeLine("restart");
      else if (resp.contains("ROM of minirobot KHEPERA")) ok=true;
    } while (!timedOut);
    
    if (ok) {
      //      checkForTurrets();
      simlog(INFO,"initializing device %s done.\n",name().latin1());
      QThread::start();
      
    } else {
      simlog(CRIT,"Can't find robot on '%s'\n",serialPort->getDevice().latin1());
      serialPort->closeDevice();
    }
    
  } else simlog(CRIT,"Could not open '%s'\n",deviceName.latin1());
  
  if (!running())
    simlog(WARN,"initializing device %s failed\n",name().latin1());
}

void WEPuck::run() {
  
  QString str, resp;
  QRegExp regExp;
  int left, right, sensors[8];
  
//   simlog(INFO,"%s thread started\n",name().latin1());
//   stopThread=false;
  
  while (!stopThread) {
    
    if (setSpeedEnabled) {
      setSpeedEnabled=false;
      serialPort->writeLine(str.sprintf("D,%d,%d",int(rint(writeLeftSpeed*127.0f)),
				   int(rint(writeRightSpeed*127.0f))));
      resp=serialPort->readLine();
      if (!resp.contains("d")) serialPort->flush();
    }
    
    if (readSpeedEnabled) {
      regExp.setPattern("e,-?[0-9]*,-?[0-9]*");
      serialPort->writeLine("E");
      resp=serialPort->readLine();
      if (regExp.match(resp)>=0) {
	sscanf((const char*)resp,"e,%d,%d",&left,&right);
	readLeftSpeed = float(left)/127.0f;
	readRightSpeed = float(right)/127.0f;
      } else serialPort->flush();
    }
    
    if (readProxEnabled) {
      regExp.setPattern("n,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*");
      serialPort->writeLine("N");
      resp=serialPort->readLine();
      if (regExp.match(resp)>=0) {
	sscanf((const char*)resp,"n,%d,%d,%d,%d,%d,%d,%d,%d",
	       sensors,sensors+1,sensors+2,sensors+3,
	       sensors+4,sensors+5,sensors+6,sensors+7);
	
	for (int i=0; i<8; i++) proxSens[i]=float(sensors[i])/1023.0f;
      } else serialPort->flush();
    }
    
    if (readAmbEnabled) {
      regExp.setPattern("o,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*,[0-9]*");
      serialPort->writeLine("O");
      resp=serialPort->readLine();
      if (regExp.match(resp)>=0) {
	sscanf((const char*)resp,"o,%d,%d,%d,%d,%d,%d,%d,%d",
	       sensors,sensors+1,sensors+2,sensors+3,
	       sensors+4,sensors+5,sensors+6,sensors+7);
	for (int i=0; i<8; i++) ambSens[i]=float(sensors[i])/1023.0f;
      } else serialPort->flush();
    }

    if (setLEDEnabled) {
      setLEDEnabled=false;
      serialPort->writeLine("L,0,"+QString(rightLED?"1":"0"));
      if (!serialPort->readLine().contains("l")) serialPort->flush();
      serialPort->writeLine("L,1,"+QString(leftLED?"1":"0"));
      if (!serialPort->readLine().contains("l")) serialPort->flush();
    }
    
    if (setPIDEnabled) {
      setPIDEnabled=false;
      serialPort->writeLine(str.sprintf("A,%d,%d,%d",pidPropVal,pidIntVal,pidDiffVal));
      if (!serialPort->readLine().contains("a")) serialPort->flush();
    }
  }
  
  serialPort->writeLine("D,0,0");
  serialPort->writeLine("L,0,0"); 
  serialPort->writeLine("L,1,0");
  serialPort->flush();
  
//   simlog(INFO,"%s thread stopped\n",name().latin1());
}
