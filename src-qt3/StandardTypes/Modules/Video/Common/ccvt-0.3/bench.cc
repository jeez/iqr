#include <stdio.h>
#include <sys/param.h>
#include <sys/times.h>

#include "ccvt.h"

#define LOOPS 500

static struct tms OldTime, NewTime;

static void StartProfile(const char *txt)
{
   printf("Performing %-15s ", txt);
   times(&OldTime);
}

static void EndProfile()
{
   int elap;

   times(&NewTime);
   elap = (int)(NewTime.tms_utime - OldTime.tms_utime);
   printf("Elapsed: User=%5d (%5.2f msec/loop) System=%5d\n", elap , (float)(1000 * elap) / (LOOPS * HZ), (int)(NewTime.tms_stime - OldTime.tms_stime));
}

int main(int argc, char *argv[])
{
   unsigned char Input[256 * 256 + 2 * 128 * 128];
   unsigned char Output[4 * 256 * 256];
   unsigned char *p;
   int x, y, t;

   /* Fill input buffer with pattern for testing. Use a gray gradient
      from left to right, and two diagonal color gradients.
    */
   p = &Input[0];
   for (x = 0; x < 256; x++)
      for (y = 0; y < 256; y++)
         *p++ = (x + y) / 2;
   for (x = 0; x < 128; x++)
      for (y = 0; y < 128; y++)
         *p++ = 2 * x;
   for (x = 0; x < 128; x++)
      for (y = 0; y < 128; y++)
         *p++ = 2 * y;
   
   StartProfile("420p_bgr32");
   for (t = 0; t < LOOPS; t++) 
      ccvt_420p_bgr32(256, 256, Input, Output);
   EndProfile();
   StartProfile("420p_bgr24");
   for (t = 0; t < LOOPS; t++) 
      ccvt_420p_bgr24(256, 256, Input, Output);
   EndProfile();
   StartProfile("420p_rgb32");
   for (t = 0; t < LOOPS; t++) 
      ccvt_420p_rgb32(256, 256, Input, Output);
   EndProfile();
   StartProfile("420p_rgb24");
   for (t = 0; t < LOOPS; t++) 
      ccvt_420p_rgb24(256, 256, Input, Output);
   EndProfile();
}
