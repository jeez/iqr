#include "KoalaSharedDef.h"
#include "helper.h"
#include "ClsKoalaW.hpp"


iqrcommon::ClsKoalaW::ClsKoalaW(){
    KoalaConnection=-1;

}

void iqrcommon::ClsKoalaW::ClearKoalaReceiveBuffer(void) {
    // Clear eventually waiting input
    while (SSCL_WaitForInput(KoalaConnection, 0, 0))   {
	(void) SSCL_ReadChar(KoalaConnection);
    }
}


void iqrcommon::ClsKoalaW::OpenKoalaConnection(void) {
    KoalaConnection = SSCL_OpenDevice(KOALA_SER_PORT, KOALA_BAUD_RATE);

//     KoalaStatusDisplay[0] = 0;         // initialize Koala Status Display (all off)
//     KoalaStatusDisplay[1] = 0;

//     KoalaPanTiltAngle[0] = 0;         // in degree
//     KoalaPanTiltAngle[1] = 0;
//     KoalaPanTiltAngle[2] = 0;
//     KoalaPanTiltAngle[3] = 0;

//--    mexAtExit(CloseKoalaConnection);
}


void iqrcommon::ClsKoalaW::CloseKoalaConnection(void) {
    SSCL_CloseDevice(KoalaConnection);
    KoalaConnection = -1;
}

//void iqrcommon::ClsKoalaW::SendKoalaCommand(unsigned char *KoalaCommandString, unsigned char KoalaCommandStringLength) {
void iqrcommon::ClsKoalaW::SendKoalaCommand(/*unsigned*/ char *KoalaCommandString, size_t KoalaCommandStringLength){
    unsigned char CommandCounter;
//--    unsigned char KoalaCheckSum;

    if (KoalaConnection == -1)   {
	(void) OpenKoalaConnection();
    }

    (void) ClearKoalaReceiveBuffer();

    SSCL_SendChar(KoalaConnection, BASE_STATION_ID);   // Wireless Sender ID
    SSCL_SendChar(KoalaConnection, NEW_KOALA_ID);      // Wireless Recipient ID

    SSCL_SendChar(KoalaConnection, 0);         // Sender Modul ID

    // Now send real command starting with Recipient ID
    for (CommandCounter=0; CommandCounter<KoalaCommandStringLength; CommandCounter++) {
	SSCL_SendChar(KoalaConnection, ((unsigned char) KoalaCommandString[CommandCounter]));
	//printf ("Sending Cmd: %02X  -- %c!\n", KoalaCommandString[i], KoalaCommandString[i]);
    }

    SSCL_SendChar(KoalaConnection, 0x0A);         // NewLine to finish command
}


int iqrcommon::ClsKoalaW::WaitForKoalaAnswer() {
    unsigned char KoalaAnswerCounter;
//--    char i;

    KoalaAnswerChar=0;
    KoalaAnswerCounter=0;

    while ((KoalaAnswerChar != 0x0A) && ((KoalaAnswerCounter+1)<MAX_KOALA_REPLY_LENGTH)) { 
	if (SSCL_WaitForInput(KoalaConnection, 0, 100000) == 0) {   // wait at most 0.1 s
	    return(-1);
	} else {
	    KoalaAnswerChar=SSCL_ReadChar(KoalaConnection);
	    KoalaReplyString[KoalaAnswerCounter]=KoalaAnswerChar;
	    KoalaAnswerCounter++;
	}
    }

    KoalaReplyString[KoalaAnswerCounter]=0;      // terminate string

    if ((KoalaReplyString[0] == NEW_KOALA_ID) &&      // answer good?
	(KoalaReplyString[1] == BASE_STATION_ID) &&
	(KoalaReplyString[2] == RECEIVED_DATA_NOTIFY)) {
	return(0);
    }

    return(1);
}

int iqrcommon::ClsKoalaW::WaitForKoalaBatteryStatus() {
    // the Koala is answering, e.g. to a battery status request. Parse that number!
    unsigned char KoalaAnswerCounter;
    int KoalaReplyValue;
//--    char i;

    KoalaAnswerChar=0;
    KoalaAnswerCounter=0;

    while ((KoalaAnswerChar != 0x0A) && ((KoalaAnswerCounter+1)<MAX_KOALA_REPLY_LENGTH)) { 
	if (SSCL_WaitForInput(KoalaConnection, 0, 100000) == 0) { // wait at most 0.1 s
	    return(-1);
	} else {
	    KoalaAnswerChar=SSCL_ReadChar(KoalaConnection);
	    KoalaReplyString[KoalaAnswerCounter]=KoalaAnswerChar;
	    KoalaAnswerCounter++;
	}
    }

    KoalaReplyString[KoalaAnswerCounter]=0;      // terminate string

    if ((strlen((const char*)KoalaReplyString) > 12) || (strlen((const char*)KoalaReplyString)<9)) {
	return(-2);
    }

    unsigned char* cp = &(KoalaReplyString[7]);

//--    KoalaReplyValue = atoi((&(KoalaReplyString[7])));   // ingnore trailing cmd reply!
    KoalaReplyValue = atoi((const char*)cp);   // ingnore trailing cmd reply!

    return(KoalaReplyValue);
}


int iqrcommon::ClsKoalaW::WaitForKoalaWhiskerStatus(double *ReturnValues)
{            // the Koala is answering. Parse the string!
    unsigned char KoalaAnswerCounter;
//--    double KoalaReplyValue;
    char i;
//--    unsigned short WH, WV;

    KoalaAnswerChar=0;
    KoalaAnswerCounter=0;

    while ((KoalaAnswerChar != 0x0A) && ((KoalaAnswerCounter+1)<MAX_KOALA_REPLY_LENGTH)) { 
	if (SSCL_WaitForInput(KoalaConnection, 0, 100000) == 0) {   // wait at most 0.1 s
	    return(-1);
	} else {
	    KoalaAnswerChar=SSCL_ReadChar(KoalaConnection);
	    KoalaReplyString[KoalaAnswerCounter]=KoalaAnswerChar;
	    KoalaAnswerCounter++;
	}
    }

    KoalaReplyString[KoalaAnswerCounter]=0;      // terminate string

    if (strlen((const char*)KoalaReplyString) != 99) {
	return(-2);
    }

    for (i=0; i<8; i++) {
	ReturnValues[4*i +0] = (double) (256*Hex2Char('0', KoalaReplyString[ 2+6*i+0])) + Hex2Char(KoalaReplyString[ 2+6*i+2], KoalaReplyString[ 2+6*i+3]);
	ReturnValues[4*i +1] = (double) (256*Hex2Char('0', KoalaReplyString[ 2+6*i+1])) + Hex2Char(KoalaReplyString[ 2+6*i+4], KoalaReplyString[ 2+6*i+5]);

	ReturnValues[4*i +2] = (double) (256*Hex2Char('0', KoalaReplyString[50+6*i+0])) + Hex2Char(KoalaReplyString[50+6*i+2], KoalaReplyString[50+6*i+3]);
	ReturnValues[4*i +3] = (double) (256*Hex2Char('0', KoalaReplyString[50+6*i+1])) + Hex2Char(KoalaReplyString[50+6*i+4], KoalaReplyString[50+6*i+5]);
    }

    return(0);
}
