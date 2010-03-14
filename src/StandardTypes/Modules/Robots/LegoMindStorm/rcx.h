/******************************************************************************

rcx module header file
kynan September 2000

******************************************************************************/

#ifndef RCX_H
#define RCX_H

#include <sys/time.h>
#include <module.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include "rcxlib.h"

// Minimum cell group sizes
#define MIN_STATUS_SIZE    9
#define MIN_CONTROL_SIZE   9
#define MIN_POWER_SIZE     9
#define MIN_SENSOR_SIZE    1024

// Control cell functions
#define CONTROL_ON         0
#define CONTROL_OFF        1
#define CONTROL_FLOAT      2
#define CONTROL_FWD        3
#define CONTROL_REV        4
#define CONTROL_FLIP       5


class rcx: Module
{
private:

#define string char*

int     cycles;
char    grpName[100];

gpt    *tgPtr;               /* Status Group pointers */
cpt    *tcPtr;               /* Status Cell pointers */
double *tsPtr;               /* Status Activity pointers */

gpt    *cgPtr[MAX_MOTOR];    /* Control Group pointers */
cpt    *ccPtr[MAX_MOTOR];    /* Control Cell pointers */
double *csPtr[MAX_MOTOR];    /* Control Activity pointers */

gpt    *pgPtr[MAX_MOTOR];    /* Power Group pointers */
cpt    *pcPtr[MAX_MOTOR];    /* Power Cell pointers */
double *psPtr[MAX_MOTOR];    /* Power Activity pointers */

gpt    *sgPtr[MAX_SENSOR];   /* Sensor Group pointers */
cpt    *scPtr[MAX_SENSOR];   /* Sensor Cell pointers */
double *ssPtr[MAX_SENSOR];   /* Sensor Activity pointers */


public:

  rcx();
  virtual ~rcx();

  virtual void setParameters(char *parms);
  virtual void run();
  virtual void process();
};


#endif RCX_H

