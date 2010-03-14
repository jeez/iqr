#ifndef WSERIAL_H
#define WSERIAL_H

#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/poll.h>
#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <iostream>
#include <string>

using namespace std;

class WSerial {
private:
    struct pollfd pollInfo;
    int dfltTimeOut;
  
    string device;
    unsigned int baudRate;
    bool opened;
  
    struct termios oldtio,newtio;
    int fd;
   
public:
    WSerial();
    virtual ~WSerial() { closeDevice(); }
  
    void setDevice(string );
    const string &getDevice() { return device; }
  
    void setBaudRate(unsigned int );
    unsigned int getBaudRate() { return baudRate; }
  
    bool openDevice();
    bool isOpen() { return opened; }
    void closeDevice();
    void flush() { tcflush(fd,TCIOFLUSH); }
  
    string readLine(int =-1, bool * =0);
    void writeLine(const string& );
  
    void setDfltTimeOut(int t) { dfltTimeOut=t; }
};

#endif // WSERIAL_H
