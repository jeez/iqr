#ifndef SSCL_H
#define SSCL_H

#ifdef __cplusplus
extern "C" {
#endif


							// open a new serial device
							// parameters: 0-3 specify the port, BaudRate in real numbers
							//  returns the port handler, needed for all other calls!
/* extern */ int SSCL_OpenDevice (int SSCL_ComPortNumber, unsigned long SSCL_BaudRate);

							// Closes the Device
/* extern */ void SSCL_CloseDevice (int SSCL_DevNumber);

							// Read a single character
/* extern */ unsigned char SSCL_ReadChar (int SSCL_DevNumber);

							// Write a single character
/* extern */ void SSCL_SendChar (int SSCL_DevNumber, unsigned char SSCL_Char);

							// Wait for input to arrive, terminate after the specified time
							// if time==0, return immediately, just check if characters present
							// returns 0 if no char present, 1 if char is present
/* extern */ char SSCL_WaitForInput (int SSCL_DevNumber, unsigned int SSCL_WaitSec, unsigned int SSCL_WaitUSec);

							// Wait for space in the transmission queue
							// returns 0 if no space available, 1 if a char can be stored
/* extern */ char SSCL_WaitForOutputBufferSpace (int SSCL_DevNumber, unsigned int SSCL_WaitSec, unsigned int SSCL_WaitUSec);

							// Set the logic level of the DTR pin
/* extern */ void SSCL_SetDTR (int SSCL_DevNumber, unsigned char SSCL_DTRLevel);		// old code - maybe rather like SetRTS??
/* extern */ void SSCL_SetRTS (int SSCL_DevNumber, unsigned char SSCL_RTSLevel);		// not yet tested!!

							// Read the logic level of the input pins
/* extern */ int SSCL_GetDCD(int SSCL_DevNumber);
/* extern */ int SSCL_GetDSR(int SSCL_DevNumber);
/* extern */ int SSCL_GetCTS(int SSCL_DevNumber);

#ifdef __cplusplus
}
#endif


#endif
