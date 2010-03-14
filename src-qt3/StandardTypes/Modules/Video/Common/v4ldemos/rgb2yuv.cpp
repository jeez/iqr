/*--------------------------------------------------------------------------------------*/
// File: rgb2yuv.cpp
// Desription: Rgb2yuv class implementation
// Author: Ben Bridgwater
// History:
// 09/16/00 Initial revision.
/*--------------------------------------------------------------------------------------*/

#include "rgb2yuv.h"

/*--------------------------------------------------------------------------------------*/

RGB2YUV::RGB2YUV()
{
  float f_y_, f_cb, f_cr;
  int y_, cb, cr;

  for (int r = 0; r <= 255; r++) { // yr, ur, vr
    yr[r] =   ((int) (0.2990 * r)) * 256;
    ur[r] = - ((int) (0.1678 * r)) * 256;
    vr[r] =   ((int) (0.5000 * r)) * 256;
  } // end for

  for (int g = 0; g <= 255; g++) { // yg, ug, vg
    yg[g] =   ((int) (0.5670 * g)) * 256;
    ug[g] = - ((int) (0.3313 * g)) * 256;
    vg[g] = - ((int) (0.4187 * g)) * 256;
  } // end for

  for (int b = 0; b <= 255; b++) { // yb, ub, vb
    yb[b] =   ((int) (0.1140 * b)) * 256;
    ub[b] =   ((int) (0.5000 * b)) * 256;
    vb[b] = - ((int) (0.0813 * b)) * 256;
  } // end for

  for (int y = 0; y <= 255; y++) { // y2y_
    f_y_ = ((float) 219 / 255) * (y + 16);
    y_ = (int) (f_y_ + 0.5);
    if (y_ < 16)
      y_ = 16;
    else if (y_ > 235)
      y_ = 235;
    y2y_[y] = y_;
  } // end for

  for (int u = 0; u <= 255; u++) { // u2cb
    f_cb = ((float) 112 / 127) * (u + 128);
    cb = (int) (f_cb + 0.5);
    if (cb < 16)
      cb = 16;
    else if (cb > 240)
      cb = 240;
    u2cb[u] = cb;
  } // end for

  for (int v = 0; v <= 255; v++) { // v2cr
    f_cr = ((float) 112 / 127) * (v + 128);
    cr = (int) (f_cr + 0.5);
    if (cr < 16)
      cr = 16;
    else if (cr > 240)
      cr = 240;
    v2cr[v] = cr;
  } // end for

} // end RGB2YUV::RGB2YUV

/*--------------------------------------------------------------------------------------*/

YUV2RGB::YUV2RGB()
{
  float y, u, v;

  for (int y_ = 0; y_ <= 255; y_++) { // xy
    y = ((float) 255 / 219) * (y_ - 16);
    xy[y_] = ((int) y) * 256;
  } // end for

  for (int cb = 0; cb <= 255; cb++) { // gu, bu
    u = ((float) 127 / 112) * (cb - 128);
    gu[cb] = - ((int) (0.344 * u)) * 256;
    bu[cb] =   ((int) (1.772 * u)) * 256;
  } // end for

  for (int cr = 0; cr <= 255; cr++) { // rv, gv
    v = ((float) 127 / 112) * (cr - 128);
    rv[cr] =   ((int) (1.402 * v)) * 256;
    gv[cr] = - ((int) (0.714 * v)) * 256;
  } // end for

} // end YUV2RGB::YUV2RGB

/*--------------------------------------------------------------------------------------*/
