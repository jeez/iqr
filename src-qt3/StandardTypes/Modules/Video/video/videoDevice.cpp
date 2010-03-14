#include "videoDevice.hpp"
#include <cstring>

string VideoDevice::error(){
    return strErrorMessage;
}

void VideoDevice::bgr_to_rgb(){
    for(int i = 0; i < iWidth*iHeight; ++i){
        fpFrame[3*i+0] = cpRawFrame[3*i+2]/256.;  /* red   */
        fpFrame[3*i+1] = cpRawFrame[3*i+1]/256.;  /* green */
        fpFrame[3*i+2] = cpRawFrame[3*i+0]/256.;  /* blue  */
    }
}

#define min(a, b)       (((a) < (b)) ? (a) : (b))
#define MIN(a, b, c)        min(min(a, b), c)
#define max(a, b)       (((a) > (b)) ? (a) : (b))
#define MAX(a, b, c)        max(max(a, b), c)

// Adapted from http://en.wikipedia.org/wiki/HSL_and_HSV#Conversion_from_RGB_to_HSL_or_HSV
void VideoDevice::rgb_to_hsv_pixel(float r, float g, float b, 
                                   float *h, float *s, float *v){
    float min, max, delta;

    min = MIN(r, g, b);
    max = MAX(r, g, b);
    delta = max-min;

    // value
    *v = max;

    // saturation
    if(max != 0) *s = delta / max;
    else *s = 0;

    // hue
    if(!delta) *h = 0;
    else if(r == max) *h = (g-b) / delta;
    else if(g == max) *h = (b-r) / delta + 120/60; 
    else              *h = (r-g) / delta + 240/60; 

    *h *= 60;               // degrees
    if(*h < 0) *h += 360;   // positive values
    *h /= 360;              // Normalized
}

void VideoDevice::bgr_to_hsv(){
    float r, g, b, *h, *s, *v;

    for(int i = 0; i < iWidth*iHeight; ++i){
        r = cpRawFrame[3*i+2]/256.;
        g = cpRawFrame[3*i+1]/256.;
        b = cpRawFrame[3*i+0]/256.;
        h = &(fpFrame[3*i+0]);
        s = &(fpFrame[3*i+1]);
        v = &(fpFrame[3*i+2]);
        rgb_to_hsv_pixel(r, g, b, h, s, v);
    }
}

void VideoDevice::bgr_to_monochrome(){
    for(int i = 0; i < iWidth*iHeight; ++i){
        float value = cpRawFrame[3*i] + cpRawFrame[3*i+1] + cpRawFrame[3*i+2];
        value /= (256*3);

        fpFrame[3*i+0] = value;  /* red   */
        fpFrame[3*i+1] = value;  /* green */
        fpFrame[3*i+2] = value;  /* blue  */
    }
}

float* VideoDevice::capture(FrameFormat ff, uchar** cpRawFrame){
    if(!(*cpRawFrame = bgrFrame())) return NULL;

    switch(ff){
      case MONOCHROME:
        bgr_to_monochrome();
        break;
      case HSV:
        bgr_to_hsv();
        break;
      case RGB:
        bgr_to_rgb();
        break;
    }

    return fpFrame;
}

VideoDevice::~VideoDevice(){}
