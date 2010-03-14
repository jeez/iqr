#ifndef JOYSTICK_H
#define JOYSTICK_H

typedef signed int int32_t;

#include <windows.h>

#include <iostream>
#include <string>

using namespace std;


struct ActorData {
  int         servo;
  int         motorLH;
  int         motorLV;
  int         motorRH;
  int         motorRV;
  int         motorD;
  int         reserved;
};

class Joystick {
public:
  Joystick(string _strPortName = "");
  ~Joystick();

  bool isConnected() { return connected; };
  bool acquire(ActorData & ad);

private:
  void getJoyRanges();
  inline double normalize(double fValue){
    //    return (fValue *2.)- 255.;
    return (fValue /128.)- 256.;

  }

  bool connected;
  int wDeviceID; 

  string strPortName;
  
  double fXMin, fXRange;
  double fYMin, fYRange;
  double fRMin, fRRange;
  double fUMin, fURange;
};


#endif // JOYSTICK_H
