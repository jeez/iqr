#ifndef _V4L2DEVICE_HPP
#define _V4L2DEVICE_HPP

#include "videoDevice.hpp"
#include <linux/videodev2.h>
#include <vector>
#include <string>
using namespace std;

typedef struct buffer_t Buffer;
struct buffer_t{
    void * start;
    size_t length;
};

class V4L2Device : public VideoDevice{
    private:
        bool selectInput(int input);
        bool selectStandard(int standard);
        string listInputs();
        string listStandards();
        bool selectFPS(int fps);
        bool selectResolution(int iWidth, int iHeight);
        bool setControl(int identifier, int value);
        bool init();
        void close();

        Buffer* sBuffers;
        int iNBuffers;

    public:
        V4L2Device(const string& device_name);
        bool isValid();
        string info();
        bool setParameters(int input, 
                           int standard,
                           int fps,
                           int iWidth, int iHeight,
                           int brightness, int saturation, int contrast,
                           bool autogain);
        bool startCapturing();
        void stopCapturing();
        uchar* bgrFrame();  // Returns NULL when unable to capture
        ~V4L2Device();
};

#endif	// _V4L2DEVICE_HPP
