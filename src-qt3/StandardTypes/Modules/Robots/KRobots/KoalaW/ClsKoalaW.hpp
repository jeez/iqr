#ifndef KOALAW_HPP
#define KOALAW_HPPs 

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/videodev.h>
#include <iostream>
#include <inttypes.h>
#include <errno.h>
#include <string>

#include "SSCL/SSCL.h"


#define KOALA_SER_PORT			1		// leave Port 0 for other stuff!
#define KOALA_BAUD_RATE			115200

#define MAX_KOALA_COMMAND_LENGTH	128
#define MAX_KOALA_REPLY_LENGTH		128

using namespace std;

namespace iqrcommon {

    class ClsKoalaW {

    public:
	ClsKoalaW();
	void ClearKoalaReceiveBuffer(void);
	void OpenKoalaConnection(void);
	void CloseKoalaConnection(void);
	void SendKoalaCommand(/*unsigned*/ char *KoalaCommandString, size_t KoalaCommandStringLength);
	int WaitForKoalaAnswer();
	int WaitForKoalaBatteryStatus();
	int WaitForKoalaWhiskerStatus(double *ReturnValues);

    private:
	int KoalaConnection;				// file pointer to serial port


	unsigned char KoalaAnswerChar;
	unsigned char KoalaReplyString[MAX_KOALA_REPLY_LENGTH];

    };
}
#endif

