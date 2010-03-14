#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include <SSCL.h>
#include "mex.h"

#include "KoalaSharedDef.h"				// the interface definitions

// *****************************************************************************************
// ** Constants and Definitions                                                           **
// *****************************************************************************************


#define MAX_MATLAB_PARAMETERS		4


#define MAX_KOALA_COMM_RETRIES  0x10

// *****************************************************************************************
// ** Global Variables                                                                    **
// *****************************************************************************************


double Parameter[MAX_MATLAB_PARAMETERS];	// parameters to command from matlab
unsigned char KoalaCommandString[MAX_KOALA_COMMAND_LENGTH];
unsigned char KoalaCommRetry;			// Counting missed communications with Koala
unsigned char KoalaAnswerChar;

long KoalaStepsLeft;				// How many steps to go...?
long KoalaStepsRight;

long KoalaSpeedLeft;				// How fast to move...?
long KoalaSpeedRight;







// *****************************************************************************************
// ** Parse parameters                                                                    **
// *****************************************************************************************

void ParseParameters(unsigned char NumberRequired, int nrhs, const mxArray *prhs[])
{
  mxArray *In_ptr;		// Get data from MatLab

  if (NumberRequired==1)
  {
    if (nrhs==2)
    {
      In_ptr = (mxArray *) prhs[1];
      Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
    } else {
      mexErrMsgTxt("Koala.c: wrong number of parameters.");
    }
  }

  if (NumberRequired==2)
  {
    if ((nrhs==2) || (nrhs==3))
    {
      if (nrhs==2)
      {
        In_ptr = (mxArray *) prhs[1];
        Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
	Parameter[1] = Parameter[0];
      } else {
        In_ptr = (mxArray *) prhs[1];
        Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
        In_ptr = (mxArray *) prhs[2];
        Parameter[1] = ((double) (*((double *) mxGetPr(In_ptr))));
      }
    } else {
      mexErrMsgTxt("Koala.c: wrong number of parameters.");
    }
  }

  if (NumberRequired==4)
  {
    if ((nrhs==2) || (nrhs==3) || (nrhs==5))
    {
      if (nrhs==2)
      {
        In_ptr = (mxArray *) prhs[1];
        Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
	Parameter[1] = Parameter[0];
	Parameter[2] = Parameter[0];
	Parameter[3] = Parameter[0];
      } else {
        if (nrhs==3)
        {
          In_ptr = (mxArray *) prhs[1];
          Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
	  Parameter[2] = Parameter[0];
          In_ptr = (mxArray *) prhs[2];
          Parameter[1] = ((double) (*((double *) mxGetPr(In_ptr))));
	  Parameter[3] = Parameter[1];
        } else {
          In_ptr = (mxArray *) prhs[1];
          Parameter[0] = ((double) (*((double *) mxGetPr(In_ptr))));
          In_ptr = (mxArray *) prhs[2];
          Parameter[1] = ((double) (*((double *) mxGetPr(In_ptr))));
          In_ptr = (mxArray *) prhs[3];
          Parameter[2] = ((double) (*((double *) mxGetPr(In_ptr))));
          In_ptr = (mxArray *) prhs[4];
          Parameter[3] = ((double) (*((double *) mxGetPr(In_ptr))));
	}
      }
    } else {
      mexErrMsgTxt("Koala.c: wrong number of parameters.");
    }
  }
}



// *****************************************************************************************
// ** MatLab Interface (this function will be called by MatLab                            **
// *****************************************************************************************


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
int i;
unsigned char CharDummy;

  char KoalaCommand[32];		// store the command string from MatLab

  unsigned char ShowKoalaUsage=0;

  unsigned char ConvStringPos;		// temp. counter to convert command string to UPPERCASE

  double *MatLabReturnDouble;

  int KoalaWhiskerStatus;

  float s;

  if (nrhs>5) {
    mexErrMsgTxt("Koala.c allows at most five input arguments.");
  }
  if (nlhs > 1) {
    mexErrMsgTxt("Koala.c requires at most one output argument.");
  }

  if (nrhs==0)
  {
    ShowKoalaUsage=1;
    KoalaCommand[0]=0;		// clear command string
  } else {
    (void) mxGetString(prhs[0], KoalaCommand, sizeof(KoalaCommand)-1);
  }

  for (ConvStringPos=0; ConvStringPos<strlen(KoalaCommand); ConvStringPos++)	// translate string to upper case
    KoalaCommand[ConvStringPos] = toupper(KoalaCommand[ConvStringPos]);


  // ------------------------------------------------------------------------------------- Free serial port (close)
  if (strncmp(KoalaCommand, "FR", 2)==0)
  {
    CloseKoalaConnection();
  }







  // ------------------------------------------------------------------------------------- Usage

  if ((strncmp(KoalaCommand, "?", 1) == 0) | (ShowKoalaUsage==1))
  {
    printf ("\nUsage:\n");
    printf ("  Koala('CMD', Argument [, Argument, Argument, ...])\n");
    printf ("  Assumes Koala attached to serial port 1 (to leave 0 for other programs!)\n\n");
    printf ("  CMD:\n");
    printf ("    KF  - move Koala forward, distance in meter\n");
    printf ("    KS  - set Koala motion speed, in m/sec\n");
    printf ("    KT  - turn Koala counterclockwise, in degree\n");
    printf ("         \n");
    printf ("    SDO - Status Display Led off, parameter codes 8 leds 0x00-0xFF\n");
    printf ("    SDR - Status Display Led red\n");
    printf ("    SDG - Status Display Led green\n");
    printf ("    SDB - Status Display Beeper 0=off, 1=on, ++=timer in x times 60ms\n");
    printf ("         \n");
    printf ("    WHR - Reset Whisker Coils\n");
    printf ("    WHG - Get Data from Whiskers, L+R\n");
    printf ("    WHW - Show off and wipe whiskers once...\n");
    printf ("         \n");
    printf ("    PT  - set PanTilt system, angles in degree, LP, LT, RP, RT\n");
    printf ("    PR+ - set PanTilt system, angles relative to current position\n");
    printf ("         \n");
    printf ("    BT  - return the voltage of the battery\n");
    printf ("         \n");
    printf ("    FR  - free Koala serial port (close)\n");
    printf ("    ?   - Show this help\n");
    printf (" (c) 2002 Jorg Conradt <conradt@ini.phys.ethz.ch>\n\n");
    return;
  }
  
}

