#include "wMath.h"
#include <string.h>
#include <stdio.h>

void randPerm(int n, int *perm)
{
  float r[n];
  for (int i=0; i < n; i++) r[i]=randu();
  sortIndex(n,r,perm);
}

float randg(int ia) {
  float x;
  if (ia<6) {
    x=1;
    for (int i=0; i<ia; i++) x*=randu();
    x=-log(x);
  } else {
    float v1,v2;
    float y, am, e, s;
    do {
      do {
	do {
	  v1=randu(-1,1);
	  v2=randu(-1,1);
	} while (v1*v1+v2*v2 > 1);
	y=v2/v1;
	am=ia-1;
	s=sqrt(2*am+1);
	x=s*y+am;
      } while (x<=0);
      e=(1+y*y)*exp(am*log(x/am)-s*y);
    } while (randu()>e);
  }
  return x;
}
