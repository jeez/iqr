#include <stdio.h>
#include <termio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "SSCL.h"

// *****************************************************************************************
// ** Initialize (open) the Device                                                        **
// *****************************************************************************************
int SSCL_OpenDevice (int SSCL_ComPortNumber, unsigned long SSCL_BaudRate)
{
  int     fd;
  struct termios tty;

  switch (SSCL_ComPortNumber) {
    case 0:
      if ((fd = open("/dev/ttyS0", O_RDWR)) < 0){
	printf("can't open /dev/ttyS0\n");
	return -1;
      }
      break;
    case 1:
      if ((fd = open("/dev/ttyS1", O_RDWR)) < 0){
	printf("can't open /dev/ttyS1\n");
	return -1;
      }
      break;
    case 2:
      if ((fd = open("/dev/ttyS2", O_RDWR)) < 0){
	printf("can't open /dev/ttyS2\n");
	return -1;
      }
      break;
    case 3:
      if ((fd = open("/dev/ttyS3", O_RDWR)) < 0){
	printf("can't open /dev/ttyS3\n");
	return -1;
      }
      break;
    default:
      printf ("Only 0 to 3 are allowed\n");
      return -1;
  }

							// set parameters
  tcgetattr(fd, &tty);

  switch (SSCL_BaudRate)
  {
    case 0:
		cfsetospeed(&tty, (speed_t) B0);
		cfsetispeed(&tty, (speed_t) B0);
		break;;
    case 1200:
		cfsetospeed(&tty, (speed_t) B1200);
		cfsetispeed(&tty, (speed_t) B1200);
		break;;
    case 2400:
		cfsetospeed(&tty, (speed_t) B2400);
		cfsetispeed(&tty, (speed_t) B2400);
		break;;
    case 4800:
		cfsetospeed(&tty, (speed_t) B4800);
		cfsetispeed(&tty, (speed_t) B4800);
		break;;
    case 9600:
		cfsetospeed(&tty, (speed_t) B9600);
		cfsetispeed(&tty, (speed_t) B9600);
		break;;
    case 19200:
		cfsetospeed(&tty, (speed_t) B19200);
		cfsetispeed(&tty, (speed_t) B19200);
		break;;
    case 38400:
		cfsetospeed(&tty, (speed_t) B38400);
		cfsetispeed(&tty, (speed_t) B38400);
		break;;
    case 57600:
		cfsetospeed(&tty, (speed_t) B57600);
		cfsetispeed(&tty, (speed_t) B57600);
		break;;
    case 115200:
		cfsetospeed(&tty, (speed_t) B115200);
		cfsetispeed(&tty, (speed_t) B115200);
		break;;
    default:
		printf ("Baud rate %ld not supported! Port closed!\n", SSCL_BaudRate);
		close(fd);
		return(-1);
  }

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;           /* 8 bits */

							/* Set into raw, no echo mode */
  tty.c_iflag = IGNBRK;
  tty.c_lflag = 0; 
  tty.c_oflag = 0;
  tty.c_cflag |= CLOCAL | CREAD;

							/* to emulate Berkely Raw mode */
  tty.c_lflag &= ~ICANON;
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 5;

  tcsetattr(fd, TCSANOW, &tty);

  return(fd);
}


// *****************************************************************************************
// ** Close the Device                                                                    **
// *****************************************************************************************
void SSCL_CloseDevice (int SSCL_DevNumber)
{
  close(SSCL_DevNumber);
}


// *****************************************************************************************
// ** Read a character from the specified Device                                          **
// *****************************************************************************************
unsigned char SSCL_ReadChar (int SSCL_DevNumber)
{
  int bytes_read;
  unsigned char buffer;
  
  bytes_read = read(SSCL_DevNumber, &buffer, 1);
  return (unsigned char) buffer;
}


// *****************************************************************************************
// ** Send a character to the specified Device                                            **
// *****************************************************************************************
void SSCL_SendChar (int SSCL_DevNumber, unsigned char SSCL_Char)
{
  write( SSCL_DevNumber, &SSCL_Char, 1);
}


// *****************************************************************************************
// ** Wait for a character on the specified device                                        **
// *****************************************************************************************
char SSCL_WaitForInput (int SSCL_DevNumber, unsigned int SSCL_WaitSec, unsigned int SSCL_WaitUSec)
{
  fd_set		fd_set;		/* set of file descriptors to wait for */
  struct timeval	tv;		/* specifies the time to wait */

  					/* wait time = 0sec 0msec */
  tv.tv_sec = SSCL_WaitSec;
  tv.tv_usec = SSCL_WaitUSec;

  /* Set parameters for the select-function (checks for new serial data) */
  FD_ZERO(&fd_set);
  FD_SET(SSCL_DevNumber, &fd_set);

  return(select(FD_SETSIZE, &fd_set, NULL, NULL, &tv));

		/* returns:	-1 if there was an error */
		/* 		 0 is no character before timeout */
		/* 		+1 if there is a char waiting */

}


// *****************************************************************************************
// ** Wait for space in the output buffer                                                 **
// *****************************************************************************************
char SSCL_WaitForOutputBufferSpace (int SSCL_DevNumber, unsigned int SSCL_WaitSec, unsigned int SSCL_WaitUSec)
{
  fd_set		fd_set;		/* set of file descriptors to wait for */
  struct timeval	tv;		/* specifies the time to wait */

  					/* wait time = 0sec 0msec */
  tv.tv_sec = SSCL_WaitSec;
  tv.tv_usec = SSCL_WaitUSec;

  /* Set parameters for the select-function (checks for new serial data) */
  FD_ZERO(&fd_set);
  FD_SET(SSCL_DevNumber, &fd_set);

  return(select(FD_SETSIZE, NULL, &fd_set, NULL, &tv));

		/* returns:	-1 if there was an error */
		/* 		 0 is no character before timeout */
		/* 		+1 if there is a char waiting */
}



// *****************************************************************************************
// ** Set the logic level of the output pins                                              **
// *****************************************************************************************
void SSCL_SetDTR (int SSCL_DevNumber, unsigned char SSCL_DTRLevel)
{
  int mctl = TIOCM_DTR;
  unsigned int ctrl_cmd;

  if (SSCL_DTRLevel)
  {
    ctrl_cmd = TIOCMBIC;
  } else {
    ctrl_cmd = TIOCMBIS;
  }

#if !defined( TIOCM_VALUE )
    if ( ioctl(SSCL_DevNumber, ctrl_cmd, &mctl ) < 0 )
#else
    if ( ioctl(SSCL_DevNumber, ctrl_cmd, (char *) mctl ) < 0 )
#endif
    {
        printf("error");
    }
}

void SSCL_SetRTS (int SSCL_DevNumber, unsigned char SSCL_RTSLevel)
{
  int state;
  ioctl(SSCL_DevNumber, TIOCMGET, &state);    /* read interface */
  if (SSCL_RTSLevel)
  {
    state |= TIOCM_RTS;
  } else {
    state &= ~(TIOCM_RTS);
  }
  ioctl(SSCL_DevNumber, TIOCMSET, &state);    /* set interface */
}

// *****************************************************************************************
// ** Read the logic level of the input pins                                              **
// *****************************************************************************************
int SSCL_GetDCD(int SSCL_DevNumber)
{
  int state;
  ioctl(SSCL_DevNumber, TIOCMGET, &state);    /* read interface */
  if (state & TIOCM_CD) {
    return (1);
  }
  return (0);
}

int SSCL_GetDSR(int SSCL_DevNumber)
{
  int state;
  ioctl(SSCL_DevNumber, TIOCMGET, &state);    /* read interface */
  if (state & TIOCM_DSR) {
    return (1);
  }
  return (0);
}

int SSCL_GetCTS(int SSCL_DevNumber)
{
  int state;
  ioctl(SSCL_DevNumber, TIOCMGET, &state);    /* read interface */
  if (state & TIOCM_CTS) {
    return (1);
  }
  return (0);
}

