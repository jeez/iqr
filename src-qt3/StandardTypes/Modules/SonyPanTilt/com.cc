/*----------------------------------------------------------------------------
Neuroinformatik Software Basis NISWB -- Implementation File
File: 			Com.cc
Verfasser:		Rolf Nestler
Beschreibung:		Ansteuerung einer seriellen Schnittstelle - class Com
Verwendete Header:	Com.h

$Id: com.cc,v 1.3 2002/04/03 14:42:04 expodev Exp $

PP 22.06.99 standalone version
-----------------------------------------------------------------------------*/ 
	
#include "com.h"
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <iostream>

#define TIMEOUT 1 //seconds *3 for timeout in case Visca cable disconnected

#ifdef LINUX

#define	EASYIO	"/dev/ttyE"	// Treiber fuer EASYIO 
#define	COM	"/dev/ttyS"	// Treiber fuer normale serielle Schnittstellen

#else

#define COM1	"/dev/ttya"
#define COM2	"/dev/ttyb"

#endif

Com::Com(int comport, int hw_protocol)
//-----------------------------------
{
  timeout=TIMEOUT;
	state = init(comport, B9600, COM_DATABITS_8, 
		COM_PARITY_NONE, COM_STOPBITS_1, hw_protocol);
}

Com::Com(int comport, int baud, int bits, int parity, int stop,
					int hw_protocol)
//-------------------------------------------------------------
{ 
timeout=TIMEOUT;
	state = init(comport, baud, bits, parity, stop, hw_protocol);
}

Com::Com(char *comport, int baud, int bits, int parity, int stop,
					int hw_protocol)
//-------------------------------------------------------------
{
  timeout=TIMEOUT;
  state = init(comport, baud, bits, parity, stop, hw_protocol);
}


Com::~Com()
//---------
{	if (state)
		close(com_handle);
}

int Com::init(int comport, int baud, int bits, int parity, int stop,
					int hw_protocol)
//------------------------------------------------------------------
{
/*	
	String comport_string;
#ifdef LINUX
	if ((comport == 1) || (comport == 2))
		comport_string = String(COM) + String("%d", comport-1); 
	else if ((comport > 2) && (comport < 11))
		comport_string = String(EASYIO) + String("%d", comport-3); 
	else
		return 0;
#else
	if (comport == 1) comport_string = String(COM1);
	else if (comport == 2) comport_string = String(COM2);
	else return 0;
#endif
//	com_handle = open(comport_string(), O_RDWR | O_NOCTTY | O_NDELAY);
//	com_handle = open(comport_string(), O_RDWR | O_NOCTTY);
*/
        
	char comport_string[100];
	sprintf(comport_string, "/dev/ttyS%i", comport-1);
	com_handle = open(comport_string, O_RDWR | O_NOCTTY | O_NONBLOCK);
	// com_handle = open(comport_string, O_RDWR | O_NOCTTY);
	if (com_handle == -1)
	{	fprintf(stderr, "int Com::init(): open failed\n");
		return 0;
	}
	termios options;
	// fcntl(com_handle, F_SETFL, FNDELAY);
	tcgetattr(com_handle, &options);
	cfsetispeed(&options, baud);
	cfsetospeed(&options, baud);
	options.c_cflag |= (CLOCAL | CREAD);
	switch (stop)			// Stop-Bits
	{	case COM_STOPBITS_1:
			options.c_cflag &= ~CSTOPB; 
			break;
		case COM_STOPBITS_2:
			options.c_cflag |= CSTOPB; 
			break;
	}			
	switch (parity)			// Paritaet
	{	case COM_PARITY_NONE:
			options.c_cflag &= ~PARENB; 
			break;
		case COM_PARITY_EVEN:
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			break;
		case COM_PARITY_ODD:
			options.c_cflag |= PARENB;
			options.c_cflag |= PARODD;
			break;
	}
	options.c_cflag &= ~CSIZE;
	switch (bits)
	{	case	COM_DATABITS_7:
			options.c_cflag |= CS7;
			break;
		case	COM_DATABITS_8:
			options.c_cflag |= CS8;
			break;
	}
	if (!hw_protocol)
		options.c_cflag &= ~CRTSCTS;
	else
		options.c_cflag |= CRTSCTS;
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	tcsetattr(com_handle, TCSANOW, &options); 
	
	//m_setparms(com_handle, "38400", "E", "8", 0, 0);
	
	return 1;
}

int Com::init(char *comport, int baud, int bits, int parity, int stop,
					int hw_protocol)
//------------------------------------------------------------------
{
  	com_handle = open(comport, O_RDWR | O_NOCTTY | O_NONBLOCK);
  //	com_handle = open(comport, O_RDWR | O_NOCTTY);
	if (com_handle == -1)
	{	fprintf(stderr, "int Com::init(): open failed\n");
		return 0;
	}
	termios options;
	// fcntl(com_handle, F_SETFL, FNDELAY);
	tcgetattr(com_handle, &options);
	cfsetispeed(&options, baud);
	cfsetospeed(&options, baud);
	options.c_cflag |= (CLOCAL | CREAD);
	switch (stop)			// Stop-Bits
	{	case COM_STOPBITS_1:
			options.c_cflag &= ~CSTOPB; 
			break;
		case COM_STOPBITS_2:
			options.c_cflag |= CSTOPB; 
			break;
	}			
	switch (parity)			// Paritaet
	{	case COM_PARITY_NONE:
			options.c_cflag &= ~PARENB; 
			break;
		case COM_PARITY_EVEN:
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			break;
		case COM_PARITY_ODD:
			options.c_cflag |= PARENB;
			options.c_cflag |= PARODD;
			break;
	}
	options.c_cflag &= ~CSIZE;
	switch (bits)
	{	case	COM_DATABITS_7:
			options.c_cflag |= CS7;
			break;
		case	COM_DATABITS_8:
			options.c_cflag |= CS8;
			break;
	}
	if (!hw_protocol)
		options.c_cflag &= ~CRTSCTS;
	else
		options.c_cflag |= CRTSCTS;
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	tcsetattr(com_handle, TCSANOW, &options); 
	
	//m_setparms(com_handle, "38400", "E", "8", 0, 0);
	
	return 1;
}


void Com::setBaudrate(int newRate)
//---------------------------------
{	termios options;
	tcgetattr(com_handle, &options);
	cfsetispeed(&options, newRate);
	cfsetospeed(&options, newRate);
	tcsetattr(com_handle, TCSANOW, &options);
}


int Com::write(char *data, int size)
  //----------------------------------
{	 int retval = 1;
 char *run = data;
 time_t startingTime=::time(NULL);
 if (state)
   {	while (size)
     {
//       cout<<"write: "<<::time(NULL)-startingTime<<endl;
       if( timeout <= ::time(NULL)-startingTime) {

	 perror("int Com::read(char *data, int size)");
	 retval = -1;
	 break;
       }
       int res = ::write(com_handle, run, size);
       if (res == -1)
	 {
	   continue;
	 }
       size -= res;
       run += res;
     }
   return retval;
   }
 else
   return 0;
}

int Com::read(char *data, int size)
//---------------------------------
{
  if (!state) return 0;
  int retval = 1;
  char *run = data;
  int sumred = 0;
  time_t startingTime=::time(NULL);
  while (sumred < size)
    {	
      //cout<<"read: "<<::time(NULL)-startingTime<<endl;
      if( timeout <= ::time(NULL)-startingTime) {
	    perror("int Com::read(char *data, int size)");
	    retval = -1;
	    break;
      }
      int red = ::read(com_handle, run, size-sumred);
      if (red != -1)
	{
	  sumred += red;
	  run += red;
	}
//       else
// 	{	perror("int Com::read(char *data, int size)");
// 	return 0;
// 	}
    }
  
  return retval;
}	

int Com::setTimeout(int _timeout)
//-------------------------------
{
  timeout=_timeout;
	return 0;
}

/*
int	Com::write(const String &s)
//-------------------------
{	return write((char*)s(), s.length());
}

int	Com::read(String &s)
//-------------------------
{	char buffer;
   	s = "";
	do
   	{	if (!read((char *)&buffer, 1)) return 0;
		s += buffer;
   	}
   	while (buffer != '\n');
  	return 1;
}
*/

void Com::clearComBuffer()
//-------------------------------
{
  //char	buf[100];
  
  //  while (::read(com_handle, buf, sizeof(buf))) ;
  //char ch;
  //while (::read(com_handle, &ch, 1) == 1); // ???
}

/*************** END OF FILE **********************************************/


