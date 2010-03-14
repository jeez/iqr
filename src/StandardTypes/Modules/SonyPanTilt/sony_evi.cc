/******************************************************************************
	C++ file
	
File:		sony_evi.cc
Compile with:	gcc -c
Author:		Peter Paschke	
Creation date:	07.03.1999	
Contents:	serial control of the SONY EVI-D31 camera	
Version history:
07.03.1999 parts from Michael Engel (TU Ilmenau) and Marcus Breig (Uni Dortmund)
22.06.1999 PP standalone version
******************************************************************************/

/***************************************************************************
Includes
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sony_evi.h"

/***************************************************************************
Defines
***************************************************************************/

#define WriteStringToCom(a, b) sio->write(a, b)

/***************************************************************************
Functions
***************************************************************************/

/* Hilfsroutinen für die Tools zur Ansteuerung der Sony-Kameras. Hier sind in der Hauptsache Konvertierungsroutinen definiert, die es ermöglichen, in übergeordneten SonyEvin die Sonybefehle als eine Folge von Zeichen und nicht als eine Folge von Bytewerten zu definieren.                                        */

char ToHex (char _Value)
/*************************
convert character from '0' to 'f' or 'F' to char value from 0 to 15
*/
{
  if      (('0' <= _Value) && (_Value <= '9'))
    _Value= (_Value-'0');
  else if (('A' <= _Value) && (_Value <= 'F'))
    _Value= (_Value-'A'+10);
  else if (('a' <= _Value) && (_Value <= 'f'))
    _Value= (_Value-'a'+10);
  else
    _Value= (0);
  return(_Value);
};

char ToChar (char _Value)
/*************************** 
convert char value from 0 to 15 to character 
*/
{
  if      (_Value < 10) 
    return (_Value+'0');
  else if (_Value < 16)
    return (_Value-10+'a');
  else
    return ('0');
}


char *Hex2String (char *_Dest, char *_Src)
/******************************************* 
convert a string representing a hex number to an array of numbers
e.g. "1020ff" -> #16#32#255            
*/                             
{
  unsigned int  i, DestPos=0;
  unsigned char HilfChar;

  for (i=0; i<strlen(_Src); i+=2) {
    HilfChar         = (unsigned char)(ToHex(_Src[i])*16 + ToHex(_Src[i+1]));
    _Dest[DestPos++] = HilfChar;
  }
  _Dest[DestPos]= '\0';
  return(_Dest);
}  

char *Char2Hex (char *_Dest, char _Src)
/****************************************
convert byte value to hex string
e.g. #128 -> "80"
*/
{
  int DestPos=0;

  _Dest[DestPos++]= ToChar((_Src & 0xf0)>> 4);
  _Dest[DestPos++]= ToChar(_Src & 0x0f);

  _Dest[DestPos]='\0';
  return(_Dest);
}

char *LowNibble (char *_Dest, short int _Value)
/************************************************
convert word value to hex string of the extracted 4-Bit-nibbles
e.g. 0x1234 -> "01020304"                                 
*/
{
  char HilfStr[256];

  _Dest[0]='\0';

  Char2Hex(HilfStr, (_Value & 0xf000) >> 12); strcat(_Dest, HilfStr);
  Char2Hex(HilfStr, (_Value & 0x0f00) >>  8); strcat(_Dest, HilfStr);
  Char2Hex(HilfStr, (_Value & 0x00f0) >>  4); strcat(_Dest, HilfStr);
  Char2Hex(HilfStr, (_Value & 0x000f) >>  0); strcat(_Dest, HilfStr);

  return(_Dest);
}


/***************************************************************************
Class Definition
***************************************************************************/


SonyEvi::SonyEvi(int comport)
/*****************************
*/ 
{  
char HilfStr[256];

 if(!(sio= new Com(comport, B9600, COM_DATABITS_8, COM_PARITY_NONE,COM_STOPBITS_1, 0)) ){
   fprintf(stderr,"SonyEvi::SonyEvi: new Com [FAILED] ..... edit.....\n");
   exit(1);
 }
// error handling

// initialize camera
	printf("Sende InitString\n");
  	WriteStringToCom(Hex2String(HilfStr, "88010001ff"), 5);
  	WaitForAnswer(HilfStr);
  	printf("Antwort: %s\n", HilfStr);

  	WriteStringToCom(Hex2String(HilfStr, "883001ff"),   4);
  	WaitForAnswer(HilfStr);
  	printf("Adress-Antwort: %s\n", HilfStr);

  	WriteStringToCom(Hex2String(HilfStr, "8101043903ff"), 6);
  	WaitForAnswer(HilfStr);
  	printf("Antwort: %s\n", HilfStr);
}

SonyEvi::SonyEvi(char *comport)
/*****************************
*/ 
{  
char HilfStr[256];

sio= new Com(comport, B9600, COM_DATABITS_8, COM_PARITY_NONE,COM_STOPBITS_1, 0);
// error handling

// initialize camera
	printf("Sende InitString\n");
  	WriteStringToCom(Hex2String(HilfStr, "88010001ff"), 5);
  	WaitForAnswer(HilfStr);
  	printf("Antwort: %s\n", HilfStr);

  	WriteStringToCom(Hex2String(HilfStr, "883001ff"),   4);
  	WaitForAnswer(HilfStr);
  	printf("Adress-Antwort: %s\n", HilfStr);

  	WriteStringToCom(Hex2String(HilfStr, "8101043903ff"), 6);
  	WaitForAnswer(HilfStr);
  	printf("Antwort: %s\n", HilfStr);
}
SonyEvi::~SonyEvi()
/*******************
*/
{
if (sio) delete sio;
}

void SonyEvi::Debug(char *_Fkt, int _KamNr)
/********************************************
*/
{
#ifdef DEBUG
  fprintf(stderr, "Befehl(%i): %s\n", _KamNr, _Fkt); 
#endif
}

void SonyEvi::ClearComBuf(void)
/*******************************
*/
{
//sio->clearComBuffer();
// read all characters from serial buffer
}

char *SonyEvi::WaitForAnswer(char *_Dest)
/****************************************
*/
{
  char  HilfChar;

  HilfChar='\0';
  _Dest[0]='\0';
  while ((unsigned char)HilfChar != 0xff) {
    // PP if (ReadFromCom(&HilfChar)) {
    if (sio->read(&HilfChar ,1)){
      Char2Hex(&_Dest[strlen(_Dest)], HilfChar); 
    }
  }
#ifdef DEBUG
  fprintf(stderr, "%s\n", _Dest);
#endif
  return(_Dest);
}    /* WaitForAnswer */

/* --- */

int SonyEvi::SendCommand(char *_Command, int _WaitForCompletion) 
/*************************************************************** 
Sendet das "Klartext-Kommando an die ser. Schnittstelle und wartet auf die ACK Message der Kamera. Zusaetzlich kann noch auf die Beendigung des Kommandos gewartet werden.                                            
*/
{
  char  HilfStr[256];

  WriteStringToCom(Hex2String(HilfStr, _Command), strlen(_Command) / 2);
  WaitForAnswer(HilfStr);
  if (_WaitForCompletion)
    WaitForAnswer(HilfStr);
  return(1==1);
}    /* SendCommand */

int SonyEvi::SetCameraPosAbsolut (int _KamNr, int _Horz, int _Vert, int _HSpeed, int _VSpeed, int _WaitForCompletion)
/******************************************************************************
*/
{  
  char          Command[256]= "\0"; 
  char          HilfStr[256]= "\0";
  int           Res;

  Debug("KameraPos", _KamNr);

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  strcat(Command, CMD_CAMPOS_ABS);

  _HSpeed= BOUND(_HSpeed, 1,           MAX_HSPEED);
  _VSpeed= BOUND(_VSpeed, 1,           MAX_VSPEED);
  _Horz  = BOUND((short)_Horz,   (short)MIN_HORZPOS, (short)MAX_HORZPOS);
  _Vert  = BOUND((short)_Vert,   (short)MIN_VERTPOS, (short)MAX_VERTPOS);

  strcat(Command, Char2Hex(HilfStr, (unsigned char)_HSpeed));
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_VSpeed));
  strcat(Command, LowNibble(HilfStr, _Horz));
  strcat(Command, LowNibble(HilfStr, _Vert));
  strcat(Command, "ff");

  Res= SendCommand(Command, _WaitForCompletion);
  return(Res);

}     /* SetCameraPosAbsolut */

/* --- */
int SonyEvi::SetCameraPosUp (int _KamNr, 
			 int _HSpeed, int _VSpeed, 
			 int _WaitForCompletion){  
  char          Command[256]= "\0"; 
  char          HilfStr[256]= "\0";
  int           Res;

  Debug("KameraPos", _KamNr);

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  strcat(Command, CMD_CAM_MOVE);

  _HSpeed= BOUND(_HSpeed, 1,           MAX_HSPEED);
  _VSpeed= BOUND(_VSpeed, 1,           MAX_VSPEED);
 
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_HSpeed));
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_VSpeed));
  strcat(Command, CMD_CAM_UP);
  strcat(Command, "ff");

  Res= SendCommand(Command, _WaitForCompletion);
  return(Res);

}     /* SetCameraPosUp */

int SonyEvi::SetCameraPosDown (int _KamNr, 
			 int _HSpeed, int _VSpeed, 
			 int _WaitForCompletion){  
  char          Command[256]= "\0"; 
  char          HilfStr[256]= "\0";
  int           Res;

  Debug("KameraPos", _KamNr);

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  strcat(Command, CMD_CAM_MOVE);

  _HSpeed= BOUND(_HSpeed, 1,           MAX_HSPEED);
  _VSpeed= BOUND(_VSpeed, 1,           MAX_VSPEED);
 
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_HSpeed));
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_VSpeed));
  strcat(Command, CMD_CAM_DOWN);
  strcat(Command, "ff");

  Res= SendCommand(Command, _WaitForCompletion);
  return(Res);

}     /* SetCameraPosDown */

int SonyEvi::SetCameraPosLeft (int _KamNr, 
			 int _HSpeed, int _VSpeed, 
			 int _WaitForCompletion){  
  char          Command[256]= "\0"; 
  char          HilfStr[256]= "\0";
  int           Res;

  Debug("KameraPos", _KamNr);

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  strcat(Command, CMD_CAM_MOVE);

  _HSpeed= BOUND(_HSpeed, 1,           MAX_HSPEED);
  _VSpeed= BOUND(_VSpeed, 1,           MAX_VSPEED);
 
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_HSpeed));
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_VSpeed));
  strcat(Command, CMD_CAM_LEFT);
  strcat(Command, "ff");

  Res= SendCommand(Command, _WaitForCompletion);
  return(Res);

}     /* SetCameraPosLeft */

int SonyEvi::SetCameraPosRight (int _KamNr, 
			 int _HSpeed, int _VSpeed, 
			 int _WaitForCompletion){  
  char          Command[256]= "\0"; 
  char          HilfStr[256]= "\0";
  int           Res;

  Debug("KameraPos", _KamNr);

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  strcat(Command, CMD_CAM_MOVE);

  _HSpeed= BOUND(_HSpeed, 1,           MAX_HSPEED);
  _VSpeed= BOUND(_VSpeed, 1,           MAX_VSPEED);
 
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_HSpeed));
  strcat(Command, Char2Hex(HilfStr, (unsigned char)_VSpeed));
  strcat(Command, CMD_CAM_RIGHT);
  strcat(Command, "ff");

  Res= SendCommand(Command, _WaitForCompletion);
  return(Res);

}     /* SetCameraPosRight */


int SonyEvi::CameraOnOff (int _KamNr, int _CameraOn)
{
  char Command[256];

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';
  if (_CameraOn) 
    strcat(Command, CMD_CAM_ON);
  else
    strcat(Command, CMD_CAM_OFF);
  strcat(Command, "ff");
  return(SendCommand(Command, 0==1));
}     /* CameraOnOff */

/* --- */


int  SonyEvi::GetCameraPos(int _KamNr, int *_Horz, int *_Vert)
{
  char Command[256];
  char RetString[256];

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_POS);
  strcat(Command, "ff");

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Horz= (short)((ToHex(RetString[ 5]) << 12) +
		    (ToHex(RetString[ 7]) <<  8) +
		    (ToHex(RetString[ 9]) <<  4) +
		    (ToHex(RetString[11]) <<  0));
    *_Vert= (short)((ToHex(RetString[13]) << 12) +
		    (ToHex(RetString[15]) <<  8) +
		    (ToHex(RetString[17]) <<  4) +
		    (ToHex(RetString[19]) <<  0));
    return(1==1);
  }
  else
    return(1==0);
}     /* GetCameraPos */

/* --- */

int  SonyEvi::GetFramePos(int _KamNr, int _Mode, int *_Horz, int *_Vert)
{
  char Command[256];
  char RetString[256];

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  //_Mode 0 = AT_ObjectPosInq
  //_Mode 1 = MD_ObjectPosInq
  
  if (_Mode)
  	strcat(Command, INQ_AT_FRAME);
  else
  	strcat(Command, INQ_MD_FRAME);
  	
  strcat(Command, "ff");	 

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Horz= (short)((ToHex(RetString[ 5]) << 12) +
		    (ToHex(RetString[ 7]) <<  8) +
		    (ToHex(RetString[ 9]) <<  4) +
		    (ToHex(RetString[11]) <<  0));
    *_Vert= (short)((ToHex(RetString[13]) << 12) +
		    (ToHex(RetString[15]) <<  8) +
		    (ToHex(RetString[17]) <<  4) +
		    (ToHex(RetString[19]) <<  0));
    return(1==1);
  }
  else
    return(1==0);


}

int  SonyEvi::SetDataScreen(int _KamNr, int _Mode)
{
  char Command[256];

  Debug("SetDataScreen", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  switch (_Mode) {
    case 0: strcat(Command, CMD_DATASCREEN_OFF);
            break;
    case 1: strcat(Command, CMD_DATASCREEN_ON);
            break; 
  }
  strcat(Command, "ff");
  return(SendCommand(Command, 1==1));
}
int SonyEvi::SetDispFrame(int _KamNr, int _Mode)	 
{
  char Command[256];

  Debug("SetDispFrame", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  switch (_Mode) {
    case 0: strcat(Command, CMD_DISP_FRAME_OFF);
            break;
    case 1: strcat(Command, CMD_DISP_FRAME_ON);
            break; 
  }
  strcat(Command, "ff");
  return(SendCommand(Command, 1==1));

}



int SonyEvi::SetWhiteBalance (int _KamNr, int _Nr)
{
  char Command[256];

  Debug("WhiteBalance", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  switch (_Nr) {
    case 1: strcat(Command, CMD_WB_AUTO);
            break;
    case 2: strcat(Command, CMD_WB_INDOOR);
            break;
    case 3: strcat(Command, CMD_WB_OUTDOOR);
            break;
    case 4: strcat(Command, CMD_WB_ONEPUSH);
            break;
    case 5: strcat(Command, CMD_WB_ONETRIGGER);
            break;
  }
  strcat(Command, "ff");
  return(SendCommand(Command, 1==1));
}    /* SetWhiteBalance */

/* --- */

int SonyEvi::GetWB (int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256]= "\0";

  ClearComBuf();

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_WB);
  strcat(Command, "ff");
  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= ToHex(RetString[ 5])+1;
    return(1==1);
  }
  else
    return(1==0);
}    /* GetZoom */

/* --- */

int SonyEvi::SetZoom (int _KamNr, int _Value)
{
  char Command[256];
  char HilfStr[256]= "\0";

  Debug("Zoom", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_ZOOM_DIRECT);
  
  _Value= BOUND(_Value, MIN_ZOOMVALUE, MAX_ZOOMVALUE);
  strcat(Command, LowNibble(HilfStr, _Value));
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetZoom */

/* --- */

int SonyEvi::GetZoom (int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256]= "\0";

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_ZOOM);
  strcat(Command, "ff");
  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= (ToHex(RetString[ 5]) << 12) +
             (ToHex(RetString[ 7]) <<  8) +
             (ToHex(RetString[ 9]) <<  4) +
             (ToHex(RetString[11]) <<  0);
    return(1==1);
  }
  else
    return(1==0);

}    /* GetZoom */

/* --- */

int SonyEvi::SetShutter (int _KamNr, int _Value)
{
  char Command[256];
  char HilfStr[256]= "\0";

  Debug("Shutter", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_SHUTTER_DIRECT);
  
  _Value= BOUND(_Value, MIN_SHUTTERVALUE, MAX_SHUTTERVALUE);
  strcat(Command, LowNibble(HilfStr, _Value));
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetShutter */

int  SonyEvi::GetShutter(int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256];


  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_SHUTTER);
  strcat(Command, "ff");

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= (ToHex(RetString[ 5]) << 12) +
             (ToHex(RetString[ 7]) <<  8) +
             (ToHex(RetString[ 9]) <<  4) +
             (ToHex(RetString[11]) <<  0);
    return(1==1);
  }
  else
    return(1==0);
}     /* GetShutter */

/* --- */

int SonyEvi::SetGain (int _KamNr, int _Value)
{
  char Command[256];
  char HilfStr[256]= "\0";

  Debug("Gain", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_GAIN_DIRECT);
  
  _Value= BOUND(_Value, MIN_GAINVALUE, MAX_GAINVALUE);
  strcat(Command, LowNibble(HilfStr, _Value));
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetGAIN */

/* --- */

int  SonyEvi::GetGain(int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256];


  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_GAIN);
  strcat(Command, "ff");

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= (ToHex(RetString[ 5]) << 12) +
             (ToHex(RetString[ 7]) <<  8) +
             (ToHex(RetString[ 9]) <<  4) +
             (ToHex(RetString[11]) <<  0);
    return(1==1);
  }
  else
    return(1==0);
}     /* GetGain */

/* --- */

int SonyEvi::SetIris (int _KamNr, int _Value)
{
  char Command[256];
  char HilfStr[256]= "\0";

  Debug("Iris", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_IRIS_DIRECT);
  
  _Value= BOUND(_Value, MIN_IRISVALUE, MAX_IRISVALUE);
  strcat(Command, LowNibble(HilfStr, _Value));
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetIris */

/* --- */

int  SonyEvi::GetIris(int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256];


  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_IRIS);
  strcat(Command, "ff");

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= (ToHex(RetString[ 5]) << 12) +
             (ToHex(RetString[ 7]) <<  8) +
             (ToHex(RetString[ 9]) <<  4) +
             (ToHex(RetString[11]) <<  0);
    return(1==1);
  }
  else
    return(1==0);
}     /* GetGain */

/* --- */

int SonyEvi::SetTrackMode (int _KamNr, int _OnOff)
{
  char Command[256];

  Debug("TrackMode", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  if (_OnOff)
    strcat(Command, CMD_TRACKINGON);
  else
    strcat(Command, CMD_TRACKINGOFF);
  
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetTrackMode */

/* --- */

int SonyEvi::SetTrackChase (int _KamNr, int _ChaseNr)
{
  char Command[256];

  Debug("TrackChase", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  switch (_ChaseNr) {
    case 1: strcat(Command, CMD_ATCHASE1);
            break;
    case 2: strcat(Command, CMD_ATCHASE2);
            break;
    case 3: strcat(Command, CMD_ATCHASE3);
            break;
    default:
      strcat(Command, CMD_ATCHASE3);
  }
  
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetTrackChase */

/* --- */

int SonyEvi::StartStopTracking (int _KamNr)
{
  char Command[256];

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_TRACKINGSTART);
    
  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* StartStopTracking */

/* --- */

int SonyEvi::SetCamAE (int _KamNr, int _AutoManual)
{
  char Command[256];

  Debug("SetCamAE", _KamNr);

  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, CMD_CAM_AE);
  
  if (_AutoManual) 
    strcat(Command, "00");
  else
    strcat(Command, "03");

  strcat(Command, "ff");

  return(SendCommand(Command, 1==1));
}    /* SetCamAE */

/* --- */

int  SonyEvi::GetCamAE(int _KamNr, int *_Value)
{
  char Command[256];
  char RetString[256];


  Command[0]= '8';
  Command[1]= ToChar(_KamNr);
  Command[2]= '\0';

  strcat(Command, INQ_CAM_AE);
  strcat(Command, "ff");

  WriteStringToCom(Hex2String(RetString, Command), strlen(Command) / 2);
  
  WaitForAnswer(RetString);
  if ((RetString[0] == (ToChar(8+_KamNr))) &&
      (RetString[2] == ('5'))           ) {
    *_Value= ToHex(RetString[ 5]);
    if (*_Value == 0)
      *_Value= 1;
    else
      *_Value= 0;
    return(1==1);
  }
  else
    return(1==0);
}     /* GetCamAE */

int SonyEvi::setAbsolutePosition(int nr, float pan, float tilt)
/***************************************************************
*/
{
//printf("x %i y %i\n", (int)(pan*MAX_HORZPOS/MAX_PAN_ANGLE), (int)(tilt*MAX_VERTPOS/MAX_TILT_ANGLE));
SetCameraPosAbsolut(nr, (int)(pan*MAX_HORZPOS/MAX_PAN_ANGLE), (int)(tilt*MAX_VERTPOS/MAX_TILT_ANGLE), MAX_HSPEED, MAX_VSPEED, 1);
return 1;
}
