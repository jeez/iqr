#ifndef WMATH_H
#define WMATH_H

#include <cmath>
#include <complex>
#include <stdlib.h>

/* from N. Schraudolph, A fast, compact approximation of the
   exponential function, Neural Computation 11:853-62, 1999 */
inline float fexp(float y) {
  static union {
    double d;
    struct {
#ifdef _BIG_ENDIAN
      int i, j;
#else
      int j, i;
#endif
    } n;
  } eco;
  eco.n.i=int((1<<20)/M_LN2*y+((1<<20)*0x3ff-60801));
  return eco.d;
}

inline int clampi(int v, int min, int max) {
  return v<min ? min : v>=max ? max-1 : v; 
}

inline float clampf(float v, float min=0, float max=1) {
  return v<min ? min : v>max ? max : v; 
}

// inline unsigned int clamp(unsigned int v, unsigned int min, unsigned int max) {
//   return v<min ? min : v>=max ? max-1 : v; 
// }

// inline float clamp(float v, float min=0.0, float max=1.0) {
//   return v<min ? min : v>=max ? max : v; 
// }

template<class num>
inline num sign(const num c) { return c>0?1:c<0?-1:0; }

inline int randi(int min, int max) {
  return min+int((max-min)*(rand()/(RAND_MAX+1.0)));
}

inline int randi(int max) {
  return int(max*(rand()/(RAND_MAX+1.0)));
}

inline float randu(float min, float max) { 
  return min+(max-min)*rand()/(RAND_MAX+1.0);
}

inline float randu() { return rand()/(RAND_MAX+1.0); }

inline float randn() {
  static int set=0;
  static float gset;
  float fac, r, v1, v2;
  
  if (set) {
    set=0;
    return gset;
  } else {
    do {
      v1=2*randu()-1;
      v2=2*randu()-1;
      r=v1*v1+v2*v2;
    } while (r>=1);
    fac = sqrt(-2*log(r)/r);
    gset= v1*fac;
    set = 1;
    return v2*fac;
  }
}

float randg(int );

template<class D> 
D erfc(D x) {
  D z, t, ans;
  z=fabs(x);
  t=1.0/(1.0+0.5*z);
  
  ans=t*exp(-z*z-1.26551223+
	    t*(1.00002368+
	       t*(0.37409196+
		  t*(0.09678418+ 
		     t*(-0.18628806+
			t*(0.27886807+
			   t*(-1.13520398+
			      t*(1.48851587+
				 t*(-0.82215223+
				    t*0.17087277)))))))));
  return (x>=0.0)?ans:2.0-ans;
}

template<class D>
D normcdf(D x) {
  return 0.5*erfc(-x/sqrt(2));
}

template<class D>
void sort(int n, D *ra) { 
  ra--;

  D rra;
  int i,j;
  int l=(n >> 1)+1;
  int ir = n;

  for (;;) {
    if (l>1) rra=ra[--l];
    else {
      rra=ra[ir];
      ra[ir]=ra[1];
      if (--ir ==1) {
	ra[1]=rra;
	return;
      }
    }
    i=l;
    j=l << 1;
    while (j <= ir) {
      if (j < ir && ra[j] < ra[j+1]) ++j;
      if (rra < ra[j]) {
	ra[i]=ra[j];
	j+= (i=j);
      }
      else j=ir+1;
    }
    ra[i]=rra;
  }
}

template<class D>
void sortIndex(int n, D *ra, int *indx) {

  ra--; indx--;
  
  D q;
  int indxt, i, j;
  
  for (j=1; j <= n; j++) indx[j]=j;

  int l = (n >> 1)+1;
  int ir = n;

  for (;;) {
    if (l > 1) 
      q=ra[(indxt=indx[--l])];
    else {
      q=ra[(indxt=indx[ir])];
      indx[ir]=indx[1];
      if (--ir == 1) {
	indx[1]=indxt;
	for (j=1; j<=n; j++) indx[j]--;
	return;
      }
    }
    i=l;
    j=l << 1;
    while (j <= ir) {
      if (j < ir && ra[indx[j]] < ra[indx[j+1]]) ++j;
      if (q < ra[indx[j]]) {
	indx[i]=indx[j];
	j += (i=j);
      }
      else j=ir+1;
    }
    indx[i]=indxt;
  }
}
 
void randPerm(int , int *);

#endif // WMATH_H
