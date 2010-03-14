/*--------------------------------------------------------------------------------------*/
// File: rgb2yuv.h
// Desription: Rgb2yuv class definition
// Author: Ben Bridgwater
// History:
// 09/16/00 Initial revision.
/*--------------------------------------------------------------------------------------*/
#ifndef _RGB2YUV_H
#define _RGB2YUV_H
/*--------------------------------------------------------------------------------------*/

#include "types.h"

/*--------------------------------------------------------------------------------------*/

struct RGB {
  uint1 r;  // 0-255
  uint1 g;  // 0-255
  uint1 b;  // 0-255
};

struct YCbCr {
  uint1 y_; // 16-235 (luma)
  uint1 cb; // 16-240, 0 at 128
  uint1 cr; // 16-240, 0 at 128
};

/*--------------------------------------------------------------------------------------*/

class RGB2YUV {
 private:
  static const int MinYUV256 = -127 * 256;
  static const int MaxYUV256 =  127 * 256;

private:
  // All multiplied by 256
  int yr[256];
  int yg[256];
  int yb[256];
  int ur[256];
  int ug[256];
  int ub[256];
  int vr[256];
  int vg[256];
  int vb[256];

  uint y2y_[256];
  uint u2cb[256];
  uint v2cr[256];

public:
  RGB2YUV();
  ~RGB2YUV() {}

  void RGBToYCbCr(uint1 r, uint1 g, uint1 b, YCbCr *yCbCr);

}; // end RGB2YUV

/*--------------------------------------------------------------------------------------*/

class YUV2RGB {
private:
  static const int MaxRGB256 = 255 * 256;

private:
  // All multiplied by 256
  int xy[256];
  int rv[256];
  int gu[256];
  int gv[256];
  int bu[256];

public:
  YUV2RGB();
  ~YUV2RGB() {}

  void  YCbCrToRGB(uint1 y_, uint1 cb, uint1 cr, RGB *rgb);

  uint1 YCbCrToBGR233(uint1 y_, uint1 cb, uint1 cr);
  uint2 YCbCrToRGB555(uint1 y_, uint1 cb, uint1 cr);
  uint2 YCbCrToRGB565(uint1 y_, uint1 cb, uint1 cr);
  uint4 YCbCrToARGB32(uint1 y_, uint1 cb, uint1 cr);
#if (0)
  uint1 YCbCrToHi240 (uint y_, uint cb, uint cr);
#endif
}; // end YUV2RGB

/*--------------------------------------------------------------------------------------*/

inline void RGB2YUV::RGBToYCbCr(uint1 r, uint1 g, uint1 b, YCbCr *yCbCr)
{
  // y,u,v all (-127..+127) * 256, since components multiplied by 256

  int4 y = yr[r] + yg[g] + yb[b];
  int4 u = ur[r] + ug[g] + ub[b];
  int4 v = vr[r] + vg[g] + vb[b];

  if (y < MinYUV256)
    y = MinYUV256;
  else if (y > MaxYUV256)
    y = MaxYUV256;
  
  if (u < MinYUV256)
    u = MinYUV256;
  else if (u > MaxYUV256)
    u = MaxYUV256;
  
  if (v < MinYUV256)
    v = MinYUV256;
  else if (v > MaxYUV256)
    v = MaxYUV256;
  
  yCbCr->y_ = y2y_[y >> 8];
  yCbCr->cb = u2cb[u >> 8];
  yCbCr->cr = v2cr[v >> 8];

} // end RGB2YUV::RGBToYCbCr

/*--------------------------------------------------------------------------------------*/

inline void YUV2RGB::YCbCrToRGB(uint1 y_, uint1 cb, uint1 cr, RGB *rgb)
{
  // r,g,b all (0..255) * 256, since components multiplied by 256
  
  int4 r = xy[y_] + rv[cr];
  int4 g = xy[y_] + gu[cb] + gv[cr];
  int4 b = xy[y_] + bu[cb];
  
  if (r < 0)
    r = 0;
  else if (r > MaxRGB256)
    r = MaxRGB256;
  
  if (g < 0)
    g = 0;
  else if (g > MaxRGB256)
    g = MaxRGB256;
  
  if (b < 0)
    b = 0;
  else if (b > MaxRGB256)
    b = MaxRGB256;
  
  rgb->r = r >> 8;
  rgb->g = g >> 8;
  rgb->b = b >> 8;

} // end YUV2RGB::YCbCrToRGB

/*--------------------------------------------------------------------------------------*/

inline uint1 YUV2RGB::YCbCrToBGR233(uint1 y_, uint1 cb, uint1 cr)
{
  // r,g,b all (0..255) * 256, since components multiplied by 256
  
  int4 r = xy[y_] + rv[cr];
  int4 g = xy[y_] + gu[cb] + gv[cr];
  int4 b = xy[y_] + bu[cb];
  
  if (r < 0)
    r = 0;
  else if (r > MaxRGB256)
    r = MaxRGB256;
  
  if (g < 0)
    g = 0;
  else if (g > MaxRGB256)
    g = MaxRGB256;
  
  if (b < 0)
    b = 0;
  else if (b > MaxRGB256)
    b = MaxRGB256;

  // 16 -> 8 = 8. b >> 8, g >> 8 + b2, r >> 8 + b2 + g3
  
  return (((r >> 13) & 0x07) | ((g >> 10) & 0x038) | ((b >> 8) & 0x0c0)); // RGB masks

} // end YUV2RGB::YCbCrToBGR233

/*--------------------------------------------------------------------------------------*/

inline uint2 YUV2RGB::YCbCrToRGB555(uint1 y_, uint1 cb, uint1 cr)
{
  // r,g,b all (0..255) * 256, since components multiplied by 256
  
  int4 r = xy[y_] + rv[cr];
  int4 g = xy[y_] + gu[cb] + gv[cr];
  int4 b = xy[y_] + bu[cb];
  
  if (r < 0)
    r = 0;
  else if (r > MaxRGB256)
    r = MaxRGB256;
  
  if (g < 0)
    g = 0;
  else if (g > MaxRGB256)
    g = MaxRGB256;
  
  if (b < 0)
    b = 0;
  else if (b > MaxRGB256)
    b = MaxRGB256;
  
  // 16 -> 15 = 1. r >> 1, g >> 1 + r5, b >> 1 + r5 + g5

  return (((r >> 1) & 0x07c00) | ((g >> 6) & 0x03e0) | ((b >> 11) & 0x01f)); // RGB masks

} // end YUV2RGB::YCbCrToRGB555

/*--------------------------------------------------------------------------------------*/

inline uint2 YUV2RGB::YCbCrToRGB565(uint1 y_, uint1 cb, uint1 cr)
{
  // r,g,b all (0..255) * 256, since components multiplied by 256
  
  int4 r = xy[y_] + rv[cr];
  int4 g = xy[y_] + gu[cb] + gv[cr];
  int4 b = xy[y_] + bu[cb];
  
  if (r < 0)
    r = 0;
  else if (r > MaxRGB256)
    r = MaxRGB256;
  
  if (g < 0)
    g = 0;
  else if (g > MaxRGB256)
    g = MaxRGB256;
  
  if (b < 0)
    b = 0;
  else if (b > MaxRGB256)
    b = MaxRGB256;
  
  // 16 -> 16 = 0. r >> 0, g >> 0 + r5, b >> 0 + r5 + g6

  return (((r >> 0) & 0x0f800) | ((g >> 5) & 0x07e0) | ((b >> 11) & 0x01f)); // RGB masks

} // end YUV2RGB::YCbCrToRGB565

/*--------------------------------------------------------------------------------------*/

inline uint4 YUV2RGB::YCbCrToARGB32(uint1 y_, uint1 cb, uint1 cr)
{
  // r,g,b all (0..255) * 256, since components multiplied by 256
  
  int4 r = xy[y_] + rv[cr];
  int4 g = xy[y_] + gu[cb] + gv[cr];
  int4 b = xy[y_] + bu[cb];
  
  if (r < 0)
    r = 0;
  else if (r > MaxRGB256)
    r = MaxRGB256;
  
  if (g < 0)
    g = 0;
  else if (g > MaxRGB256)
    g = MaxRGB256;
  
  if (b < 0)
    b = 0;
  else if (b > MaxRGB256)
    b = MaxRGB256;

  return ((uint4) (((r << 8) & 0x0ff0000) | (g & 0x0ff00) | ((b >> 8) & 0x0ff)));
  
} // end YUV2RGB::YCbCrToARGB32

/*--------------------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------------------*/
