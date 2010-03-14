#ifndef _VIDEODEVICE_HPP
#define _VIDEODEVICE_HPP

#include <string>
using namespace std;

typedef unsigned char uchar;

class VideoDevice{
    private:
        void bgr_to_rgb();
        void rgb_to_hsv_pixel(float r, float g, float b, 
                              float *h, float *s, float *v);
        void bgr_to_hsv();
        void bgr_to_monochrome();

    protected:
        int iFD;
        string strErrorMessage;
        int iWidth, iHeight;
        float* fpFrame;
        uchar* cpRawFrame;

    public:
        typedef enum{RGB=0, HSV=1, MONOCHROME=2} FrameFormat;

        virtual bool isValid() = 0;
        string error();
        virtual string info() = 0;  // Returns info on open supported device
        virtual bool setParameters(int input, 
                                   int standard, 
                                   int fps,
                                   int iWidth, int iHeight,
                                   int brightness, int saturation, int contrast,
                                   bool autogain) = 0;
        virtual bool startCapturing() = 0;
        virtual void stopCapturing() = 0;
        float* capture(FrameFormat ff, uchar** cpRawFrame); // Returns NULL if
        // unable to capture
        virtual uchar* bgrFrame() = 0;  // Returns NULL if unable to capture

        virtual ~VideoDevice();
};

#endif	// _VIDEODEVICE_HPP
