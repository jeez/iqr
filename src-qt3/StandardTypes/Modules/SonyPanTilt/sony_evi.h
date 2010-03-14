/******************************************************************************
	C++ header file
	
File:		sony_evi.h
Compile with:	gcc -c
Author:		Peter Paschke	
Creation date:	07.03.1999	
Contents:	serial control of the SONY EVI-D31 camera
Version history:
07.03.1999 parts from Michael Engel (TU Ilmenau) and Marcus Breig (Uni Dortmund)
- uses NISWB class Com	
22.06.99 standalone version without NISWB
******************************************************************************/

#ifndef SONY_EVI_HEADER
#define SONY_EVI_HEADER

/***************************************************************************
Includes
***************************************************************************/
//#include <com.h>
#include "com.h"

/***************************************************************************
Defines
***************************************************************************/

/* commands */
#define CMD_CAMPOS_ABS     "010602"
#define CMD_CAM_ON         "01040002"
#define CMD_CAM_OFF        "01040003"
#define CMD_WB_AUTO        "01043500"
#define CMD_WB_INDOOR      "01043501"
#define CMD_WB_OUTDOOR     "01043502"
#define CMD_WB_ONEPUSH     "01043503"
#define CMD_WB_ONETRIGGER  "01043505"
#define CMD_GAIN_DIRECT    "01044c"
#define CMD_SHUTTER_DIRECT "01044a"
#define CMD_IRIS_DIRECT    "01044b"
#define CMD_TRACKINGON     "01070102"
#define CMD_TRACKINGOFF    "01070103"
#define CMD_TRACKINGSTART  "01070610"
#define CMD_ZOOM_DIRECT    "010447"
#define CMD_ATCHASE1       "01070700"  
#define CMD_ATCHASE2       "01070701"  
#define CMD_ATCHASE3       "01070702"  
#define CMD_CAM_AE         "010439"    /* anschliessend 00=Auto, 03=Manual */
#define CMD_DATASCREEN_ON  "01060602"
#define CMD_DATASCREEN_OFF "01060603"
#define CMD_DISP_FRAME_ON  "01070402"
#define CMD_DISP_FRAME_OFF "01070403"
#define CMD_AT_OFFSET_ON   "01070502"
#define CMD_AT_OFFSET_OFF  "01070503"
#define CMD_AT_OFFSET	   "01070510"	
#define CMD_CAM_MOVE	   "010601"
#define CMD_CAM_UP         "0301"
#define CMD_CAM_DOWN       "0302"
#define CMD_CAM_LEFT       "0103"
#define CMD_CAM_RIGHT      "0203"	

/* inquiries */
#define INQ_POS        "090612"
#define INQ_ZOOM       "090447"
#define INQ_SHUTTER    "09044a"
#define INQ_IRIS       "09044b"
#define INQ_GAIN       "09044c"
#define INQ_ATMDMODE   "090722"
#define INQ_WB         "090435"
#define INQ_CAM_AE     "090439"
#define INQ_AT_FRAME   "090720"
#define INQ_MD_FRAME   "090721"

#define MAX_HSPEED      0x18
#define MIN_HSPEED      0x01
#define MAX_VSPEED      0x14
#define MIN_VSPEED      0x01
#define MIN_HORZPOS     0xfc90
#define MAX_HORZPOS     0x0370
#define MIN_VERTPOS     0xfed4
#define MAX_VERTPOS     0x012c

#define MIN_SHUTTERVALUE 0x00
#define MAX_SHUTTERVALUE 0x1b
#define MIN_IRISVALUE    0x00
#define MAX_IRISVALUE    0x11
#define MIN_GAINVALUE    0x00
#define MAX_GAINVALUE    0x07

#define MIN_ZOOMVALUE    0x0
#define MAX_ZOOMVALUE    0x3ff

// PP 03.03.1999 Winkel in Grad - wo sind die Werte dokumentiert?
#define MAX_PAN_ANGLE	100
#define MAX_TILT_ANGLE	25

//#define MAX(a,b) ((a)<(b)?(b):(a))
//#define MIN(a,b) ((a)<(b)?(a):(b))
#define BOUND(a,b,c) ((a)<(b)?(b):(c)<(a)?(c):(a))

/***************************************************************************
Declarations
***************************************************************************/

class SonyEvi{
private:
    Com *sio;
	
    // functions by Uni Dortmund
	
    void Debug(char *_Fkt, int _KamNr);
    void ClearComBuf(void);
    char *WaitForAnswer (char *_Dest);
    int SendCommand(char *_Command, int _WaitForCompletion); 
	
public:
    SonyEvi(int comport= 1);	// 1 = Linux /dev/ttyS0  
    SonyEvi(char *comport);
    SonyEvi(const SonyEvi& c); 	// only for the compiler
    ~SonyEvi();
	
    int setAbsolutePosition(int nr, float pan, float tilt);	
    // pan and tilt angle in degree

    // functions by Uni Dortmund
					
    int SetCameraPosAbsolut (int _KamNr, 
			     int _Horz,   int _Vert, 
			     int _HSpeed, int _VSpeed, 
			     int _WaitForCompletion);
    int CameraOnOff (int _KamNr, int _CameraOn);
    int GetCameraPos(int _KamNr, int *_Horz, int *_Vert);
    int SetWhiteBalance (int _KamNr, int _Nr);
    int GetWB (int _KamNr, int *_Value);
    int SetZoom (int _KamNr, int _Value);
    int GetZoom (int _KamNr, int *_Value);
    int SetShutter (int _KamNr, int _Value);
    int GetShutter(int _KamNr, int *_Value);
    int SetGain (int _KamNr, int _Value);
    int GetGain(int _KamNr, int *_Value);
    int SetIris (int _KamNr, int _Value);
    int GetIris(int _KamNr, int *_Value);
    int SetTrackMode (int _KamNr, int _OnOff);
    int SetTrackChase (int _KamNr, int _ChaseNr);
    int StartStopTracking (int _KamNr);
    int SetCamAE (int _KamNr, int _AutoManual);
    int GetCamAE(int _KamNr, int *_Value);
		

    // functions by Michael Engel
    int GetFramePos(int _KamNr, int _Mode, int *_Horz, int *_Vert);
    int SetDataScreen(int _KamNr, int _Mode);
    int SetDispFrame(int _KamNr, int _Mode);	
    int SetCameraPosDown(int _KamNr,  int _HSpeed, int _VSpeed, 
			  int _WaitForCompletion);
    int SetCameraPosUp(int _KamNr,  int _HSpeed, int _VSpeed, 
			int _WaitForCompletion);
    int SetCameraPosLeft(int _KamNr,  int _HSpeed, int _VSpeed, 
			  int _WaitForCompletion);
    int SetCameraPosRight(int _KamNr,  int _HSpeed, int _VSpeed, 
			   int _WaitForCompletion);

};

#endif
