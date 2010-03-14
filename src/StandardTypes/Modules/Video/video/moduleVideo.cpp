#include <ModuleIcon_CameraUSB.h>
#include <Common/Helper/iqrUtils.h>
#include "moduleVideo.hpp"
#include "videoDeviceFactory.hpp"
#include "videoDevice.hpp"
#include <string>
#include <ctime>
#include <cmath>
using namespace std;

MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleVideo,
                          "video module")

iqrcommon::moduleVideo::moduleVideo():ClsThreadModule(){
    /*_strName,_strLabel,_strValue,_bEditable,_bLong,_strDescription,_strCategory*/
    clsStringParameterDeviceName = addStringParameter("DeviceName", "Video Device",
                                         "/dev/video0", true, false,
                                         "Path to Video Device", "Device");
    /*_strName,_strLabel,_iValue,_iMinimum,_iMaximum,_strDescription,_strCategory*/
    clsIntParameterInput = addIntParameter("Input", "Camera input", 0, 0, 10,
                                "Camera input", "Device");
    clsIntParameterStandard = addIntParameter("Standard", "Video Standard", 0, 0, 100,
                                   "Video Standard", "Device");
    clsIntParameterFPS = addIntParameter("FPS", "Frames per second", 25, -1, 100,
                              "Frames per second", "Device");
    /*_strName,_strLabel,_bValue,_strDescription,_strCategory*/
    clsBoolParameterInfoWindow = addBoolParameter("InfoWindow", "Info window", false,
                                       "Show info window instead of simulating",
                                       "Device");
    clsBoolParameterShowOutput = addBoolParameter("ShowOutput", "Show output", true,
                                       "Show camera output", "Output");
    /*_strName,_strLabel,_iValue,_iMinimum,_iMaximum,_strDescription,_strCategory*/
    clsIntParameterWidth = addIntParameter("imgWidth", "Image Width", 160, 160, 640,
                                "Width of the Image to grab", "Image");
    clsIntParameterHeight = addIntParameter("imgHeight", "Image Height", 120, 120, 480,
                                 "Height of the Image to grab", "Image");
    clsIntParameterBrightness = addIntParameter("Brightness", "% image Brightness", 50, 0,
                                     100, "% brightness of the Image to grab",
                                     "Image");
    clsIntParameterSaturation = addIntParameter("Saturation", "% image saturation", 50, 0,
                                     100, "% saturaion of the Image to grab",
                                     "Image");
    clsIntParameterContrast = addIntParameter("Contrast", "% image Contrast ", 50, 0, 100,
                                   "% contrast of the Image to grab", "Image");
    clsIntParameterAutogain = addIntParameter("AutoGain", "Camera AutoGain", 1, 0, 1,
                                   "AutoGain (-1 for automatic)", "Image");
    /*_strName,_strLabel,_bValue,_strDescription,_strCategory*/
    clsBoolParameterHSV = addBoolParameter("HSV", "HSV color space", false,
                               "HSV color space", "Output");
    clsBoolParameterMonochrome = addBoolParameter("Monochrome", "Monochrome", false,
                                      "Monochrome", "Output");

    clsStateVariableRedHue = addOutputToGroup("videoOutputRed", 
                                   "Video Output Red/Hue/Grayscale");
    clsStateVariableGreenSat = addOutputToGroup("videoOutputGreen", 
                                     "Video Output Green/Saturation");
    clsStateVariableBlueVal = addOutputToGroup("videoOutputBlue", 
                                    "Video Output Blue/Value");

    clsVideoDisp = NULL;
}

moduleIcon iqrcommon::moduleVideo::getIcon() {
    moduleIcon mi(ModuleIcon_CameraUSB_png_data, ModuleIcon_CameraUSB_png_len, 
                  3 ,5);
    return mi;
}

iqrcommon::moduleVideo::~moduleVideo(){ 
}

void iqrcommon::moduleVideo::init(){
    string device_name = clsStringParameterDeviceName->getValue(); 
    string error;

    iGroupWidth = clsStateVariableRedHue->getNrCellsHorizontal();
    iGroupHeight = clsStateVariableRedHue->getNrCellsVertical();
    
    if(iGroupWidth != clsStateVariableGreenSat->getNrCellsHorizontal() ||
       iGroupWidth != clsStateVariableBlueVal->getNrCellsHorizontal() ||
       iGroupHeight != clsStateVariableGreenSat->getNrCellsVertical() ||
       iGroupHeight != clsStateVariableBlueVal->getNrCellsVertical()){
        throw ModuleError(string("Module \"") + getLabel() + 
                          "\": All output groups need to be the same size");
    }

    clsVideoDevice = VideoDeviceFactory::open(device_name, error);

    if(!clsVideoDevice){
        throw ModuleError(string("Module \"") + getLabel() + 
                          "\": Could not open video device: " + error);
    }

    if(clsBoolParameterInfoWindow->getValue()){
        string info = clsVideoDevice->info();
        delete clsVideoDevice;
        throw ModuleError(string("Module \"") + getLabel() + "\" (" + 
                          device_name + " info):\n" + info);
    }

    int input = clsIntParameterInput->getValue();
    int fps = clsIntParameterFPS->getValue();
    int width = clsIntParameterWidth->getValue();
    int height = clsIntParameterHeight->getValue();
    int brightness = clsIntParameterBrightness->getValue();
    int saturation = clsIntParameterSaturation->getValue();
    int contrast = clsIntParameterContrast->getValue();
    int autogain = clsIntParameterAutogain->getValue();

    int standard = clsIntParameterStandard->getValue();

    if(!clsVideoDevice->setParameters(input, standard, fps, width, height, brightness, 
                          saturation, contrast, autogain)){
        throw ModuleError(string("Module \"") + getLabel() + 
                          "\": Error setting up video device parameters (" +
                          clsVideoDevice->error() + ").");
    }

    if(!clsVideoDevice->startCapturing()){
        throw ModuleError(string("Module \"") + getLabel() + 
                          "\": Unable to capture (" + clsVideoDevice->error() + ")");
    }

    if(clsBoolParameterShowOutput->getValue()){
        clsVideoDisp = new ClsVideoDisp("Video Display", width, height);
        clsVideoDisp->show();
    }

    gettimeofday(&sTimevalInit, NULL);
    iElapsedCycles = 0;
}

void iqrcommon::moduleVideo::cleanup(){
    clsVideoDevice->stopCapturing();
    delete clsVideoDevice;

    if(clsVideoDisp!=NULL){
        clsVideoDisp->hide();
        delete clsVideoDisp;
        clsVideoDisp = NULL;
    }
}

const char* iqrcommon::moduleVideo::getLabel(){
    return "Video module";
}

void iqrcommon::moduleVideo::update(){
    StateArray &rh = clsStateVariableRedHue->getStateArray();
    StateArray &gs = clsStateVariableGreenSat->getStateArray();
    StateArray &bv = clsStateVariableBlueVal->getStateArray();
    int width = clsIntParameterWidth->getValue();
    int height = clsIntParameterHeight->getValue();
    bool hsv = clsBoolParameterHSV->getValue();
    bool monochrome = clsBoolParameterMonochrome->getValue();

    float* frame;
    uchar* rawframe;        // BGR format

    struct timeval time_now;
    double elapsed_time, next_frame_time, delta;
    int fps = clsIntParameterFPS->getValue();
    struct timespec sleep_time;

    if(fps){            // User specified a frames per second value
        gettimeofday(&time_now, NULL);

        elapsed_time = (time_now.tv_sec - sTimevalInit.tv_sec) + 
            (time_now.tv_usec - sTimevalInit.tv_usec)/1e6; 

        next_frame_time = (double)iElapsedCycles/fps;
        delta = next_frame_time - elapsed_time;

        if(delta > 0){
            sleep_time.tv_sec = (time_t)floor(delta);
            delta -= floor(delta);
            sleep_time.tv_nsec = (long)(delta*1e9);
            nanosleep(&sleep_time, NULL);
        }
        ++iElapsedCycles;
    }

    if(monochrome) {
        frame = clsVideoDevice->capture(VideoDevice::MONOCHROME, &rawframe);
    } else if(hsv) {
        frame = clsVideoDevice->capture(VideoDevice::HSV, &rawframe);
    } else {
        frame = clsVideoDevice->capture(VideoDevice::RGB, &rawframe);
    }

    qmutexThread->lock();

    if(iGroupWidth != width || iGroupHeight != height){
        scale(frame, rh[0], gs[0], bv[0], width, height, 
              iGroupWidth, iGroupHeight);
    }
    else{
        for(int i = 0; i < width*height; ++i){
            rh[0][i] = frame[3*i];
            gs[0][i] = frame[3*i+1];
            bv[0][i] = frame[3*i+2];
        }
    }

    qmutexThread->unlock(); 

    QCEventData* ce = new QCEventData(rawframe);
    if(clsBoolParameterShowOutput->getValue()){
        QApplication::postEvent(clsVideoDisp, ce);
    }
}

double nrv(double x){       // next round value
    if(0 < ceil(x)-x) return ceil(x);
    else return x+1;
}

void iqrcommon:: moduleVideo::scale(float* orig, valarray<double>& r, 
                                    valarray<double>& g, valarray<double>& b, 
                                    int width, int height, int new_width, 
                                    int new_height){
    double sfx = (double)new_width/width;    // scaling factors
    double sfy = (double)new_height/height; 
    // Each pixel on the new image represents an area of n pixels on the old one
    double pixel_area = 1/(sfx*sfy);

    r = g = b = 0;

    for(int j = 0; j < height; ++j){
        double beg_y = j*sfy;
        double end_y = beg_y+sfy;
        for(int i = 0; i < width; ++i){
            double beg_x = i*sfx;
            double end_x = beg_x+sfx;

            for(double start_y = beg_y; start_y < end_y;
                start_y = nrv(start_y)){
                for(double start_x = beg_x; start_x < end_x; 
                    start_x = nrv(start_x)){
                    // Original pixel maps to square defined by:
                    // ((beg_x,beg_y),(end_x,end_y))
                    double beg_x2 = floor(start_x);
                    double beg_y2 = floor(start_y);
                    double end_x2 = beg_x2 + 1;
                    double end_y2 = beg_y2 + 1;
                    // Destination pixel maps to square defined by:
                    // ((beg_x2,beg_y2),(end_x2,end_y2))

                    // Common area between the two squares:
                    double area_beg_x = max(beg_x, beg_x2);
                    double area_beg_y = max(beg_y, beg_y2);
                    double area_end_x = min(end_x, end_x2);
                    double area_end_y = min(end_y, end_y2);
                    double area = (area_end_x - area_beg_x) *
                        (area_end_y - area_beg_y);

                    r[beg_y2*new_width+beg_x2] += area * orig[3*(j*width+i)];
                    g[beg_y2*new_width+beg_x2] += area * orig[3*(j*width+i)+1];
                    b[beg_y2*new_width+beg_x2] += area * orig[3*(j*width+i)+2];
                }
            }
        }
    }
}

