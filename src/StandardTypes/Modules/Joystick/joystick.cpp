/******************************************************************************/
/*                                                                            */
/* J o y s t i c k                                                            */
/*                                                                            */
/******************************************************************************/


#include "joystick.h"
#include <math.h>

Joystick::Joystick(string _strPortName) : strPortName(_strPortName){
  connected = false;
  fd = -1;
}


Joystick::~Joystick() {
  if (fd == -1) close(fd);
}


bool Joystick::acquire(ActorData & ad)
{
  bool action = false;
  struct js_event JoyStickEvent;
  static int JoyStickAxis[8];
  static int JoyStickButton[8];
  int diff;
  bool emergencyTrigger;

  if (fd == -1)
  {
    unsigned char axes;
    unsigned char buttons;

    if ((fd = open(strPortName.c_str(), (O_RDONLY | O_NONBLOCK))) < 0)
    {
      connected = false;
    }
    else {
      connected = true;
    } // else

    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);
    //qDebug("axes = %d buttons = %d\n", axes, buttons);

  }

  while (read(fd, &JoyStickEvent, sizeof(struct js_event)) != -1)
  {
    switch(JoyStickEvent.type & ~JS_EVENT_INIT)
    {
      case JS_EVENT_BUTTON:
        JoyStickButton[JoyStickEvent.number] = JoyStickEvent.value;
        break;;

      case JS_EVENT_AXIS:
        JoyStickAxis[JoyStickEvent.number] = JoyStickEvent.value;
        break;;
    }
    //qDebug("Axes x: %5d y: %5d t: %5d", JoyStickAxis[0], JoyStickAxis[1], JoyStickAxis[3]);

 // joystick tranformation
    action = true;
  }

  // emergency is assigned to trigger of right stick
  emergencyTrigger = (JoyStickButton[0] == 1);

  bzero(&ad, sizeof(ad));
  
  // Four motors
  // -----------
//???  ad.motorLH = (JoyStickAxis[1] / 128) + (JoyStickAxis[0] / (128*1));
//???  ad.motorRH = (JoyStickAxis[1] / 128) - (JoyStickAxis[0] / (128*1));
  

//????  diff = ad.motorRH -ad.motorLH;
//????  if (diff > 0) {
//????      if (ad.motorRH > 255) {
//????          ad.motorRH = 255;
//????          ad.motorLH = ad.motorRH -diff;
//????      } // if
//????      else if (ad.motorLH < -255) {
//????          ad.motorLH = -255;
//????          ad.motorRH = ad.motorLH +diff;
//????      } // else if
//????  } // if
//????  else {
//????      if (ad.motorLH > 255) {
//????          ad.motorLH = 255;
//????          ad.motorRH = ad.motorLH +diff;
//????      } // if
//????      else if (ad.motorRH < -255) {
//????          ad.motorRH = -255;
//????          ad.motorLH = ad.motorRH -diff;
//????      } // else if
//????  } // else
  

//???  ad.motorLV = JoyStickAxis[3]/128;
//???  ad.motorRV = JoyStickAxis[3]/128;
//???



  ad.motorLH = JoyStickAxis[0]/128;
  ad.motorLV = JoyStickAxis[1]/128;
  ad.motorRH = JoyStickAxis[2]/128;
  ad.motorRV = JoyStickAxis[3]/128;

  
  if (emergencyTrigger) {
      bzero(&ad, sizeof(ad));
      ad.motorLV = -255;
      ad.motorRV = -255;
  } // if
  
  return action;
} // acquire

