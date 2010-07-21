#ifndef _EPUCK_H
#define _EPUCK_H

#include <string>
#include <vector>
#include "serialstream/SerialStream.h"
using namespace std;
using namespace LibSerial;

typedef struct current_t Current;
struct current_t{
    float speed_left;
    float speed_right;
    bool body_LED;
    bool frontal_LED;
    bool crown_LEDs[8];
};

const int MAX_PROXIMITY = 3700;
const int MAX_BRIGHTNESS = 4000;
const int MAX_AUDIO = 1000;
const int MAX_ACCELERATION = 4100;

typedef struct pixel_t Pixel;
struct pixel_t{
    float rh, gs, bv; // RGB or HSV
};

typedef enum{ RGB=0, HSV=1} Colorspace;
typedef enum{ GRAYSCALE=0, COLOR=1} Channels;
typedef unsigned char uchar;

class Epuck{
    private:
        string error_message;
        SerialStream serialPort;
        Current current;        // Current values

        string removeSpaces(string s);
        void consumeReply(const char* expected_reply);
        vector<float> readReply(char code, int length, int max_value);

        vector<Pixel> processImage(vector<uchar> raw_image, int mode);
        void RGBtoHSVPixel(float r, float g, float b, 
                           float& h, float& s, float& v);
        vector<Pixel> RGBtoHSV(vector<Pixel> rgb_image);
        vector<Pixel> rotate90CCW(vector<Pixel> image, int width, int height);
         
    public:
        Epuck();
        bool open(const string& device);
        void close();
        void setSpeed(float left, float right);
        void setBodyLED(bool state);
        void setFrontalLED(bool state);
        void setCrownLED(int led, bool state);
        void calibrateProximitySensors();
        vector<float> proximityReadout();
        vector<float> lightReadout();
        vector<float> audioReadout();
        vector<float> accelerationReadout();
        void setImageFormat(Channels channels, int width, int height, int zoom,
                            int x1, int y1);
        vector<Pixel> imageReadout(Colorspace cs, bool rotate);
        void playSound(int sound);

        string error();
};

#endif // _EPUCK_H
