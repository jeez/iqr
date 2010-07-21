#include <iostream> // remove
#include <sstream>
#include "epuck.h"
using namespace std;

Epuck::Epuck(){
    // Current (initial) values
    current.speed_left = current.speed_right = 0;
    current.body_LED = current.frontal_LED = false;
    for(int i = 0; i < 8; ++i) current.crown_LEDs[i] = false;
}

string Epuck::removeSpaces(string s){
    for(int i = 0; i < (int)s.length(); ++i){
        if(s[i] == ' '){
            s.erase(i,1);
            --i;
        }
    }
    return s;
}

void Epuck::consumeReply(const char* expected_reply){
    string expected = removeSpaces(string(expected_reply));
    string reply,s;

    do{
        serialPort>>s;
        reply += s;
        cout<<reply<<endl;
    }while(expected != reply && reply.length() < expected.length()); 
    // This last condition is just a quick hack that allows the caller to use
    // "" as expected_reply. This is useful when we expect a reply of length 1
    // and we don't want to check it, but only consume it from the stream.
}

vector<float> Epuck::readReply(char code, int length, int max_value){
    string reply;
    vector<float> readings;
    char character;
    int integer;
    float floating;

    do{
        serialPort>>reply;
    }while(reply[0] != code);

    istringstream stream(reply);
    stream>>character;                 // Ignore operation code
    for(int i = 0; i < length; ++i){
        stream>>character>>integer;

        floating = (float)integer/max_value;
        if(floating > 1) floating = 1;
        readings.push_back(floating);
    }
    return readings;
}

vector<Pixel> Epuck::processImage(vector<uchar> raw_image, int mode){
    vector<Pixel> image;

    if(0 == mode){      // greyscale (1 byte/pixel)
        image.resize(raw_image.size());
        for(int i = 0; i < (int)raw_image.size(); ++i)
          image[i].rh = image[i].gs = image[i].bv = (float)raw_image[i] / 255;
    }
    else if(1 == mode){ // color (2 bytes/pixel)
        image.resize(raw_image.size()/2);
        for(int i = 0; i < (int)raw_image.size()/2; ++i){
            uchar b1 = raw_image[i*2];
            uchar b2 = raw_image[i*2+1];

            /* <-First-byte--> <-Second-byte->
             * 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
             * b b b b b g g g g g g r r r r r
             */
            image[i].rh = (float)(b1&0xF8)/255;
            image[i].gs = (float)(((b1&0x07)<<5)|((b2&0xE0)>>3))/255;
            image[i].bv = (float)((b2&0x1F)<<3)/255;
        }
    }

    return image;
}

#define min(a, b)       (((a) < (b)) ? (a) : (b))
#define MIN(a, b, c)        min(min(a, b), c)
#define max(a, b)       (((a) > (b)) ? (a) : (b))
#define MAX(a, b, c)        max(max(a, b), c)

// Adapted from http://en.wikipedia.org/wiki/HSL_and_HSV#Conversion_from_RGB_to_HSL_or_HSV
void Epuck::RGBtoHSVPixel(float r, float g, float b, 
                          float& h, float& s, float& v){
    float min, max, delta;

    min = MIN(r, g, b);
    max = MAX(r, g, b);
    delta = max-min;

    // value
    v = max;

    // saturation
    if(max != 0) s = delta / max;
    else s = 0;

    // hue
    if(!delta) h = 0;
    else if(r == max) h = (g-b) / delta;
    else if(g == max) h = (b-r) / delta + 120/60; 
    else              h = (r-g) / delta + 240/60; 

    h *= 60;               // degrees
    if(h < 0) h += 360;   // positive values
    h /= 360;              // Normalized
}

vector<Pixel> Epuck::RGBtoHSV(vector<Pixel> rgb_image){
    vector<Pixel> hsv_image(rgb_image.size());

    for(int i = 0; i < (int)rgb_image.size(); ++i)
      RGBtoHSVPixel(rgb_image[i].rh, rgb_image[i].gs, rgb_image[i].bv,
                    hsv_image[i].rh, hsv_image[i].gs, hsv_image[i].bv);

    return hsv_image;
}

vector<Pixel> Epuck::rotate90CCW(vector<Pixel> image, int width, int height){
    vector<Pixel> rotated_image(image.size());

    for(int y_src = 0; y_src < (int)height; ++y_src){
        for(int x_src = 0; x_src < (int)width; ++x_src){
            int x_dst = y_src;
            int y_dst = width-x_src-1;

            rotated_image[height*y_dst + x_dst] = image[width*y_src + x_src];
        }
    }

    return rotated_image;
}


bool Epuck::open(const string& device){
    serialPort.Open(device);
    if(!serialPort.good()){
        // Is it necessary to perform a serialPort.close() ?
        error_message = "Could not open serial port";
        return false;
    }
    serialPort.SetBaudRate(SerialStreamBuf::BAUD_115200);
    if(!serialPort.good()){
        serialPort.Close();
        error_message = "Could not set baud rate";
        return false;
    }
    serialPort.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    if(!serialPort.good()){
        serialPort.Close();
        error_message = "Could not set char size";
        return false;
    }
    serialPort.SetParity(SerialStreamBuf::PARITY_NONE);
    if(!serialPort.good()){
        serialPort.Close();
        error_message = "Could not disable parity";
        return false;
    }
    serialPort.SetNumOfStopBits(1);
    if (!serialPort.good()){
        serialPort.Close();
        error_message = "Could not set the number of stop bits";
        return false;
    }
    serialPort.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
    if (!serialPort.good()){
        serialPort.Close();
        error_message = "Could not use hardware flow control";
        return false;
    }

    // Small hack to empty the serial port buffers
    serialPort<<"C"<<endl;
    consumeReply("");
    return true;
}

void Epuck::close(){
    serialPort.Close();
}

void Epuck::setSpeed(float left, float right){
    // Enforce speed limit
    if(left > 1) left = 1;
    else if(left < -1) left = -1;
    if(right > 1) right = 1;
    else if(right < -1) right = -1;

    if(current.speed_left == left && current.speed_right == right) return;
    serialPort<<"D,"<<(int)(left*1000)<<","<<(int)(right*1000)<<endl;
    current.speed_left = left; current.speed_right = right;
    consumeReply("");
}

void Epuck::setBodyLED(bool state){
    if(current.body_LED == state) return;
    serialPort<<"B,"<<state<<endl;
    current.body_LED = state;
    consumeReply("");
}

void Epuck::setFrontalLED(bool state){
    if(current.frontal_LED == state) return;
    serialPort<<"F,"<<state<<endl;
    current.frontal_LED = state;
    consumeReply("");
}

void Epuck::setCrownLED(int led, bool state){
    if(current.crown_LEDs[led] == state) return;
    serialPort<<"L,"<<led<<","<<state<<endl;
    current.crown_LEDs[led] = state;
    consumeReply("");
}

void Epuck::calibrateProximitySensors(){
    serialPort<<"K"<<endl;
    consumeReply("k, Starting calibration - Remove any object in sensors range");
    consumeReply("k, Calibration finished");
}

vector<float> Epuck::proximityReadout(){
    serialPort<<"N"<<endl;
    return readReply('n', 8, MAX_PROXIMITY);
}

vector<float> Epuck::lightReadout(){
    serialPort<<"O"<<endl;
    return readReply('o', 8, MAX_BRIGHTNESS);
}
        
vector<float> Epuck::audioReadout(){
    serialPort<<"U"<<endl;
    return readReply('u', 3, MAX_AUDIO);
}

vector<float> Epuck::accelerationReadout(){
    serialPort<<"A"<<endl;
    return readReply('a', 3, MAX_ACCELERATION);
}

void Epuck::setImageFormat(Channels channels, int width, int height, int zoom,
                           int x1, int y1){
    serialPort<<"J,"<<channels<<","<<width<<","<<height<<","<<zoom<<","
        <<x1<<","<<y1<<endl;
    consumeReply("");
}

vector<Pixel> Epuck::imageReadout(Colorspace cs, bool rotate){
    string reply;
    char c_tmp;
    int mode, width, height, size;
    vector<uchar> raw_image;

    serialPort<<(char)-'I'<<(char)0;

    // Header (mode,width,height)
    serialPort>>c_tmp; mode = (int)c_tmp;
    serialPort>>c_tmp; width = (int)c_tmp;
    serialPort>>c_tmp; height = (int)c_tmp;

    size = width*height;
    if(mode) size *= 2;

    // Image readout
    // In order to avoid the module hanging because of transmission errors
    // we could use the VMin and VTime parameters to set a timeout. See:
    // http://www.unixwiz.net/techtips/termios-vmin-vtime.html
    // serialPort.SetVTime(3); 
    // serialPort.SetVMin(255); + read // several times

    for(int i = 0; i < size; ++i){
        serialPort.get(c_tmp);
        raw_image.push_back((uchar) c_tmp);
    }

    vector<Pixel> image = processImage(raw_image, mode);
    if(HSV == cs) image = RGBtoHSV(image);
    if(rotate) image = rotate90CCW(image, width, height); // Rotate 90 deg. CCW

    return image;
}

void Epuck::playSound(int sound){
    serialPort<<"T,"<<sound<<endl;
    consumeReply("");
}

string Epuck::error(){
    return error_message;
}
