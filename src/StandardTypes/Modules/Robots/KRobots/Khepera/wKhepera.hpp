#ifndef WKHEPERA_HPP
#define WKHEPERA_HPP

#include "wSerial.hpp"
#include "wRobDevice.hpp"

class WKhepera : public WRobDevice {
public:
  WKhepera(WModule *, const QString & ="Khepera");
  ~WKhepera();
  
  void initDevice();
  
  void setSpeed(float l, float r) { 
    setSpeedEnabled=(writeLeftSpeed!=l || writeRightSpeed!=r);
    writeLeftSpeed=l; writeRightSpeed=r; 
  }
  
  void readSpeed(float &l, float &r) const { l=readLeftSpeed; r=readRightSpeed; }
  
  void setLED(bool l, bool r) {
    setLEDEnabled=(leftLED!=l || rightLED!=r);
    leftLED=l; rightLED=r;
  }
  
  void writeSetting (const QString &) {};
  void readSetting (const QString &) {};

  void setPIDMotorController() { setPIDEnabled=true; }

protected:
  
  void run();

  float writeLeftSpeed, writeRightSpeed;
  float readLeftSpeed, readRightSpeed;
  
  bool leftLED, rightLED;

  QString deviceName;
  unsigned int deviceSpeed;
  
  bool setLEDEnabled, setPIDEnabled;
  
  WSerial serial;
};

#endif // WKHEPERA_HPP
