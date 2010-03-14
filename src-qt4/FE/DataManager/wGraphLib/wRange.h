#ifndef WRANGE_HPP
#define WRANGE_HPP

#include <float.h>
#include <QDataStream>
#include <math.h>

/*! The WRange class */
class WRange {
public:
  WRange () : rmin(0), rmax(1), scale(1) {}
  WRange(float min, float max) {
    if (max > min) { rmax=max; rmin=min; } 
    else { rmax=min; rmin=max; }
    scale=1/(rmax-rmin); 
  }
  
  void resetForRangeCheck() { rmax=-FLT_MAX; rmin=FLT_MAX; };
  
  bool valid() const { return finite(rmax) && finite(rmin) && rmax>=rmin; }
  
  float scaleValue(float f) const { return (f-rmin)*scale; };
  float unscaleValue(float f) const { return f*(rmax-rmin)+rmin; };
  
  WRange unscaleRange(WRange r) {
    return r*(rmax-rmin)+rmin;
  }

  bool contains(float v) const { return (v>=rmin) && (v<=rmax); }
  bool contains(WRange r) const { return (r.rmin>=rmin) && (r.rmax<=rmax); }

  int adjustToFit(WRange r) {
    if (interval()>r.interval()) { *this=r; return 2; }
    else
      if (rmin<r.rmin) { *this+=r.rmin-rmin; return 1; }
      else if (rmax>r.rmax) { *this+=r.rmax-rmax; return 1; }
      else return 0;
  }

  void adjust(float val) { 
    if (val<rmax && val>rmin) return;
    rmax = (val > rmax) ? val : rmax; rmin = (val < rmin) ? val : rmin; 
    scale=1/(rmax-rmin); 
  };

  void expand(float perc) { 
    rmax = rmax+(rmax-rmin)*perc; rmin = rmin-(rmax-rmin)*perc; 
    scale=1/(rmax-rmin); 
  }
  
  float min() const { return rmin; };
  float max() const { return rmax; };
  
  float interval() const { return rmax-rmin; };
  
  bool isEmpty() const { return rmax==rmin; }
  
  bool comp(WRange r, float tol) {
    return (r.rmax < (rmax+(rmax-rmin)*tol) &&
	    r.rmax > (rmax-(rmax-rmin)*tol) &&
	    r.rmin < (rmin+(rmax-rmin)*tol) &&
	    r.rmin > (rmin-(rmax-rmin)*tol));
  }
  
  bool operator==(WRange r) { return (rmax==r.rmax && rmin==r.rmin); };
  bool operator!=(WRange r) { return (rmax!=r.rmax || rmin!=r.rmin); };
  
  WRange operator|(WRange r) {
    return WRange(rmin<r.rmin?rmin:r.rmin,
		  rmax>r.rmax?rmax:r.rmax); 
  }

  WRange &operator|=(WRange r) {
    rmin=rmin<r.rmin?rmin:r.rmin;
    rmax=rmax>r.rmax?rmax:r.rmax;
    return *this;
  }

  WRange operator&(WRange r) {
    return WRange(rmin>r.rmin?rmin:r.rmin,
		  rmax<r.rmax?rmax:r.rmax); 
  }

  WRange &operator&=(WRange r) {
    rmin=rmin>r.rmin?rmin:r.rmin;
    rmax=rmax<r.rmax?rmax:r.rmax; 
    return *this;
  }

  WRange operator+ (float c) { return WRange(rmin+c,rmax+c); };
  WRange operator-(float c) { return WRange(rmax-c,rmin-c); };
  
  WRange operator*(float c) { return WRange(rmin*c,rmax*c); };
  WRange operator/(float c) { return WRange(rmin/c,rmax/c); };

  WRange operator*=(float c) {
    rmax*=c; rmin*=c; scale*=c; 
    return *this;
  };
  WRange operator+=(float c) {
    rmax+=c; rmin+=c;
    return *this;
  };
  
  bool operator>(float c) { return rmin>c; }
  bool operator<(float c) { return rmax<c; }

protected:
  
  float rmin, rmax, scale; 
};

inline QDataStream & operator<<(QDataStream &s, const WRange &r) {
  s<<r.min()<<r.max();
  return s;
};

inline QDataStream & operator>>(QDataStream &s, WRange &r){
  float min,max;
  s>>min>>max;
  r=WRange(min,max);
  return s;
};


//ZZZinline Q3TextStream & operator<<(Q3TextStream &s, const WRange &r) {
//ZZZ  s<<r.min()<<" "<<r.max()<<" ";
//ZZZ  return s;
//ZZZ};
//ZZZ
//ZZZinline Q3TextStream & operator>>(Q3TextStream &s, WRange &r){
//ZZZ  float min,max;
//ZZZ  s>>min>>max;
//ZZZ  r=WRange(min,max);
//ZZZ  return s;
//ZZZ};

//ZZZinline QDataStream & operator<<(QDataStream &s, const bool &b){
//ZZZ    return s.writeBytes((const char*)&b,sizeof(bool)); //ZZZ
//ZZZ};
//ZZZ
//ZZZinline QDataStream & operator>>(QDataStream &s, bool &b) {
//ZZZ    return s.readBytes((char *)&b,sizeof(bool)); //ZZZ
//ZZZ}

inline float clampr(float v, const WRange &r) {
  return v<r.min() ? r.min() : v>r.max() ? r.max() : v;
}

#endif// WRANGE_HPP
