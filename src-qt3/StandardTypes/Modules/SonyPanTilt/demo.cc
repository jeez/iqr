/******************************************************************************
SONY EVI control test
	
File:		demo.cc
Compile with:	gcc -c
Author:		Peter Paschke	
Creation date:	22.06.1999	
Contents:	*	
Version history:	
22.06.1999 initial version

to do:
error handling
switch to AT mode?
******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include "sony_evi.h" 

using namespace std;
SonyEvi *cam;

int main(int argc, char *argv[])
/**********************************
*/
{
printf("SONY EVI control test\n");
printf("*********************\n");

int i;
struct timeval tim0, tim1;
struct timezone tz;

float x= 0.0;
float y= 0.0;

//printf("%d parameters\n", argc);
for (i=0; i<argc; i++)
{
//  printf("%d. %s\n", i, argv[i]);

  if (argv[i][1]=='x'){
     if (i+1<argc){ 
       x= atof(argv[i+1]);
     }
  }
  if (argv[i][1]=='y'){
     if (i+1<argc){ 
       y= atof(argv[i+1]);
     }
  }
/*
  if (argv[i][1]=='v') ;
*/
  if (argv[i][1]=='h') { 
    printf("demo -h	help\n");
    printf("demo -x pan\n");
    printf("demo -y tilt\n");    
    exit(1);
  }
}
printf("x=%f y=%f\n", x, y);

// AT mode bleibt eingeschaltet
//cam= new SonyEvi(1);
cam= new SonyEvi("/dev/ttyS0");

gettimeofday(&tim0, &tz);

y= 0;
cam->SetCameraPosAbsolut (1,
			  0, 0,
			  MAX_HSPEED, MAX_VSPEED, 
			  0);



int iHPos = 0;
int iSleep;
double fSpeed;

#define HORIZONTAL
#ifdef HORIZONTAL
double fa = -26 / ((double)MAX_HSPEED * .9);
double fb = 27;


for(;;){
    for(int iDivisor = 1; iDivisor <= 10; iDivisor++) {
//    printf("x=%d y=%d\n", ix, iy);
//    cam->setAbsolutePosition(1, x, y);
	
	iHPos = (iDivisor % 2 == 0 ? MAX_HORZPOS : -MAX_HORZPOS);
	
	
	fSpeed = (double)MAX_HSPEED / (double)iDivisor;
	cam->SetCameraPosAbsolut (1,
				  iHPos, 0,
				  (int)fSpeed, MAX_HSPEED, 
				  0);
	
	iSleep = fabs(fb + fa * fSpeed);
	
	
	cout << "iHPos: " << iHPos << endl;
	cout << "fSpeed: " << fSpeed << endl;
	cout << "iSleep: " << iSleep << endl;
	
//    usleep(10e5);
	sleep(iSleep);
    }
}
#endif

#ifdef VERTICAL
double fa = -26 / ((double)MAX_VSPEED * .9);
double fb = 27;


for(;;){
    for(int iDivisor = 1; iDivisor <= 10; iDivisor++) {
//    printf("x=%d y=%d\n", ix, iy);
//    cam->setAbsolutePosition(1, x, y);
	
	iHPos = (iDivisor % 2 == 0 ? MAX_VERTPOS : -MAX_VERTPOS);
	
	
	fSpeed = (double)MAX_VSPEED / (double)iDivisor;
	cam->SetCameraPosAbsolut (1,
				  0, iHPos,
				  MAX_VSPEED/10, (int)fSpeed, 
				  0);
	
	iSleep = abs(fb + fa * fSpeed);
	
	
	cout << "iHPos: " << iHPos << endl;
	cout << "fSpeed: " << fSpeed << endl;
	cout << "iSleep: " << iSleep << endl;
	
//    usleep(10e5);
	sleep(iSleep);
    }
}
#endif


/*
// How can i switch auto tracking on??? 
cam->SetTrackMode(1, 1);
cam->SetTrackChase(1, 0);
cam->StartStopTracking(1);
*/

gettimeofday(&tim1, &tz);
int mus= (tim1.tv_sec-tim0.tv_sec)*1000000+tim1.tv_usec-tim0.tv_usec;
printf("%i mus\n", mus);  
}

