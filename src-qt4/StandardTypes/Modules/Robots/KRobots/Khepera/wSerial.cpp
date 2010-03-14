#include <strings.h> //MARK
#include "wSerial.hpp"

WSerial::WSerial() : dfltTimeOut(-1), baudRate(B38400), opened(false) {}

void WSerial::setDevice(string _device) {
    WSerial::device = _device;
    if (opened) openDevice();
}

void WSerial::setBaudRate(unsigned int baudRate) {
    WSerial::baudRate=baudRate;
    if (opened) openDevice(); 
}

bool WSerial::openDevice() {
  
    closeDevice();
  
    if (device.size()<=0) return false;
    fd = open(device.c_str(), 
	      O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC); 
    if (fd <0) {
	opened=false;
	perror(device.c_str()); 
	return false;
    } else opened=true;

    pollInfo.fd=fd;
    pollInfo.events=POLLIN;

    tcgetattr(fd,&oldtio); 
    bzero(&newtio, sizeof(newtio)); 
 
    newtio.c_cflag = baudRate | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;

    newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
    newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtio.c_cc[VERASE]   = 0;     /* del */
    newtio.c_cc[VKILL]    = 0;     /* @ */
    newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
    newtio.c_cc[VSWTC]    = 0;     /* '\0' */
    newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
    newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtio.c_cc[VEOL]     = 0;     /* '\0' */
    newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtio.c_cc[VEOL2]    = 0;     /* '\0' */
  
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
  
    return true;
}


void WSerial::closeDevice() {
    if (opened) {
	flush();
	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);
	opened=false;
    }
}

string WSerial::readLine(int timeOut, bool *timedOut) {
    if (!opened) {
//	cerr << "WSerial::readLine: not opened" << endl;
	return "";
    }
    if (timeOut==-1) {
	timeOut=dfltTimeOut;
    }
    if (poll(&pollInfo,1,timeOut)==0) {
	if (timedOut) {
	    *timedOut=true;
	}
	cerr << "timeout" << endl;
	return "";
    } else if (timedOut) {
	*timedOut=false;
    }

    int res;
    char buf[255];
    res=read(fd,buf,255);
    buf[res-1]=0;
    return buf;
}

void WSerial::writeLine(const string& str) {
    if (!opened) {
//	cerr << "WSerial::writeLine: not opened" << endl;
    }
    else {
	write(fd,(const char*)(str + string("\n")).c_str(),str.length()+1);
    }
}
