/******************************************************************************/
/*                                                                            */
/* J o y s t i c k                                                            */
/*                                                                            */
/******************************************************************************/

#ifndef JOYSTICK_H
#define JOYSTICK_H

typedef signed int int32_t;
//typedef signed long long int64_t;

#include <linux/joystick.h>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

//#include "def.h"

struct ActorData {
  int         servo;
  int         motorLH;
  int         motorLV;
  int         motorRH;
  int         motorRV;
  int         motorD;
  int         reserved;
};

// -----------------------------------------------------------------------------
// J o y s t i c k
// -----------------------------------------------------------------------------
class Joystick {
public:
  Joystick(string _strPortName = "");
  ~Joystick();

  bool         isConnected() { return connected; };
  bool         acquire(ActorData & ad);
private:
  bool         connected;
  int          fd;
  string strPortName;

};


#endif // JOYSTICK_H
