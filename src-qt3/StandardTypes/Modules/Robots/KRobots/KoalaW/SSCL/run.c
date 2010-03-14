#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SSCL.h"

int main()
{

  int ser_port, i,j,k;
  char dummy_char;
  char dummy_char_kbd=0;

  ser_port = SSCL_OpenDevice (1, 19200);

// *****************************************************************************************
// ** Start the communication                                                             **
// *****************************************************************************************

  printf ("Press ESC to quit! \n");

  system("stty -echo raw");

  while (dummy_char_kbd!=27)
  {
    if (SSCL_WaitForInput(ser_port, 0, 0))
    {
      dummy_char = SSCL_ReadChar (ser_port);
      printf ("%c", dummy_char);
    }

    if (SSCL_WaitForInput(0, 0, 0))
    {
      dummy_char_kbd=getchar();
      if (dummy_char_kbd != 27)
      {
        printf ("%c", dummy_char_kbd); fflush(stdout);
        SSCL_SendChar(ser_port, dummy_char_kbd);
      }
    }
  }

  system("stty echo -raw");

exit(0);

  for (i=0; i<100; i++)
  {

    for (j=0; j<10; j++)
    {
printf ("this one %d is next...\n", j); fflush(stdout);

      (void) SSCL_WaitForInput (ser_port, 0, 100);
//      dummy_char = SSCL_ReadChar (ser_port);

printf ("this one %d is next...\n", j); fflush(stdout);
      (void) SSCL_WaitForOutputBufferSpace (ser_port, 0, 100);
      (void) SSCL_SendChar (ser_port, (char) ('A'+j) );

printf ("this one %d is next...\n", j); fflush(stdout);
      for (k=1; k<j; k++)
        usleep(1000);

printf ("this one %d is next...\n", j); fflush(stdout);

      if ((j%2)==0)
      {
        SSCL_SetDTR (ser_port, 0);
      } else {
        SSCL_SetDTR (ser_port, 1);
      }

printf ("this one %d is next...\n", j); fflush(stdout);
    }

  }

  SSCL_CloseDevice (ser_port);

  return(0);
}

