#include <ctime>
#include <vector>
#include "moduleEpuck.h"
#include <ModuleIcon_epuck.h>

MAKE_MODULE_DLL_INTERFACE(iqrcommon::ClsModuleEpuck, "EPuck Advanced Module")


// This operation is called when IQR is initialized, only once
iqrcommon::ClsModuleEpuck::ClsModuleEpuck() : ClsThreadModule() {
    /*_strName,_strLabel,_strValue,_bEditable,_bLong,_strDescription,_strCategory*/
    device_name = addStringParameter("device_name", "Device",
                                     "/dev/rfcomm0", true, false,
                                     "serial port device name", "General");
    /*_strName,_strLabel,_iValue,_iMinimum,_iMaximum,_strDescription,_strCategory*/
    par_cps = addIntParameter("cps", "Cycles/Second", 25, 0, 1000, 
                              "e-puck evaluations per second", "General");

    // Camera parameters
    /*_strName,_strLabel,_bValue,_strDescription,_strCategory*/
    par_grayscale = addBoolParameter("grayscale", "Grayscale", false, 
                                     "Capture grayscale images",
                                     "Image");
    par_hsv = addBoolParameter("hsv", "HSV", false, "Use HSV colorspace",
                               "Image");
    
    par_rotate = addBoolParameter("rotate", "Rotate 90 deg. CCW", false, 
                                  "Fix rotation issue in old e-pucks", "Image");

    /*_strName,_strLabel,_iValue,_iMinimum,_iMaximum,_strDescription,_strCategory*/
    par_width =  addIntParameter("width", "Width", 40, 1, 640, "Image width", 
                                 "Image");
    par_height =  addIntParameter("height", "Height", 40, 1, 480,
                                  "Image height", "Image");
    par_zoom =  addIntParameter("zoom", 
                                "Zoom (1, 4, 8) (lower value, higher zoom)", 8,
                                1, 8, "Image zoom (1,4,8)", "Image");
    par_x1 = addIntParameter("x1", "Top left corner coordinates (x1)", 1,
                             1, 640, "Image top-left corner's X", "Image");
    par_y1 = addIntParameter("y1", "Top left corner coordinates (y1)", 1,
                             1, 480, "Image top-left corner's Y", "Image");

    var_motor = addInputFromGroup("_motor", "Motor (2x2)");
    var_body_LED = addInputFromGroup("_body_LED", "Body LED (1)");
    var_frontal_LED = addInputFromGroup("_frontal_LED", "Frontal LED (1)");
    var_crown_LEDs = addInputFromGroup("_LEDs", "Crown LEDs (8)");
    var_calibration = addInputFromGroup("_calibration",
                                        "Proximity sensor calibration (1)");
    var_command_proximity = addInputFromGroup("_command_proximity",
                                              "Command proximity readout (1)");
    var_command_light = addInputFromGroup("_command_light",
                                          "Command light readout (1)");
    var_command_audio = addInputFromGroup("_command_audio",
                                          "Command audio readout (1)");
    var_command_acceleration = addInputFromGroup("_command_acceleration",
                                          "Command acceleration readout (1)");
    var_command_image = addInputFromGroup("_command_image",
                                          "Command image readout (1)");
    var_command_sound = addInputFromGroup("_command_sound", 
                                          "Command sounds (6)");

    var_proximity = addOutputToGroup("_proximity", "Proximity sensors (8)");
    var_light = addOutputToGroup("_light", "Light sensors (8)");
    var_audio = addOutputToGroup("_audio", "Microphone (3)");
    var_acceleration = addOutputToGroup("_acceleration", "Accelerometers (3)");
    var_image_RH = addOutputToGroup("_image_RH", "Red/Hue");
    var_image_GS = addOutputToGroup("_image_GS", "Green/Saturation");
    var_image_BV = addOutputToGroup("_image_BV", "Blue/Value");
}

moduleIcon iqrcommon::ClsModuleEpuck::getIcon() {
    moduleIcon mi(ModuleIcon_epuck_png_data,  ModuleIcon_epuck_png_len, 3 ,5);
    return mi;
} 

void iqrcommon::ClsModuleEpuck::checkWidth(StateArray& group, int width,
                                           const string& group_name){
    if((int)group.getWidth() != width){
        ostringstream w_int;
        w_int<<width;
        throw ModuleError(string("Module ") + label() + ": " + group_name +
                          " group must be of size " + w_int.str());
    }
}

void iqrcommon::ClsModuleEpuck::init(){
    StateArray& motor = var_motor->getTarget()->getStateArray();
    StateArray& body_LED = var_body_LED->getTarget()->getStateArray();
    StateArray& frontal_LED = var_frontal_LED->getTarget()->getStateArray();
    StateArray& crown_LEDs = var_crown_LEDs->getTarget()->getStateArray();
    StateArray& calibration = var_calibration->getTarget()->getStateArray();
    StateArray& command_proximity = 
        var_command_proximity->getTarget()->getStateArray();
    StateArray& proximity = var_proximity->getStateArray(); 
    StateArray& command_light = var_command_light->getTarget()->getStateArray();
    StateArray& command_acceleration = 
        var_command_acceleration->getTarget()->getStateArray();
    StateArray& command_image = var_command_image->getTarget()->getStateArray();
    StateArray& light = var_light->getStateArray(); 
    StateArray& command_audio = var_command_audio->getTarget()->getStateArray();
    StateArray& audio = var_audio->getStateArray(); 
    StateArray& acceleration = var_acceleration->getStateArray(); 
    StateArray& image_RH = var_image_RH->getStateArray(); 
    StateArray& image_GS = var_image_GS->getStateArray(); 
    StateArray& image_BV = var_image_BV->getStateArray();
    StateArray& command_sound = var_command_sound->getTarget()->getStateArray();

    checkWidth(motor, 4, "Motor");
    checkWidth(body_LED, 1, "Body LED");
    checkWidth(frontal_LED, 1, "Frontal LED");
    checkWidth(crown_LEDs, 8, "Crown LEDs");
    checkWidth(calibration, 1, "Proximity calibration");
    checkWidth(command_proximity, 1, "Command proximity readout");
    checkWidth(command_light, 1, "Command light readout");
    checkWidth(command_audio, 1, "Command audio readout");
    checkWidth(command_acceleration, 1, "Command acceleration readout");
    checkWidth(command_image, 1, "Command camera readout");
    checkWidth(command_sound, 6, "Sound player");
    checkWidth(proximity, 8, "Proximity sensors");
    checkWidth(light, 8, "Light sensors");
    checkWidth(audio, 3, "Microphone");
    checkWidth(acceleration, 3, "Accelerometers");
    
    int width = par_width->getValue();
    int height = par_height->getValue();
    if(width*height > 1600)
      throw ModuleError(string("Module ")+label()
                        +string(": The WidthxHeight product must be inferior\
                                to 1600"));
    int zoom = par_zoom->getValue();
    int x1 = par_x1->getValue();
    int y1 = par_y1->getValue();
    bool grayscale = par_grayscale->getValue();

    checkWidth(image_RH, width*height, "Red/Hue");
    checkWidth(image_GS, width*height, "Green/Saturation");
    checkWidth(image_BV, width*height, "Blue/Value");

    if(!epuck.open(device_name->getValue()))
      throw ModuleError(string("Module ")+label()+string(":")+epuck.error());

    epuck.setImageFormat(grayscale ? GRAYSCALE:COLOR, width, height, zoom, x1,
                         y1);

    gettimeofday(&time_init, NULL);
    elapsed_cicles = 0;
}

void iqrcommon::ClsModuleEpuck::relax(){
    struct timeval time_now;
    double elapsed_time, next_computation_time, delta;
    int cps = par_cps->getValue();
    struct timespec sleep_time;

    if(cps){            // User specified a cycles per second value
        gettimeofday(&time_now, NULL);

        elapsed_time = (time_now.tv_sec - time_init.tv_sec) +
            (time_now.tv_usec - time_init.tv_usec)/1e6;

        next_computation_time = (double)elapsed_cicles/cps;
        delta = next_computation_time - elapsed_time;

        if(delta > 0){
            sleep_time.tv_sec = (time_t)floor(delta);
            delta -= floor(delta);
            sleep_time.tv_nsec = (long)(delta*1e9);
            nanosleep(&sleep_time, NULL);
        }
        ++elapsed_cicles;
    }
}

void iqrcommon::ClsModuleEpuck::vectorToGroup(vector<float> origin, 
                                              StateArray& destination){
    for(int i = 0; i < (int)origin.size(); ++i)
      destination[0][i] = origin[i];
}

void iqrcommon::ClsModuleEpuck::imageToGroups(vector<Pixel> image, 
                                              StateArray& red_hue,
                                              StateArray& green_saturation,
                                              StateArray& blue_value){
    for(int i = 0; i < (int)image.size(); ++i){
        red_hue[0][i] = image[i].rh;
        green_saturation[0][i] = image[i].gs;
        blue_value[0][i] = image[i].bv;
    }
}

void iqrcommon::ClsModuleEpuck::update(){
    relax();    // Don't stress the e-puck

    StateArray &motor = var_motor->getTarget()->getStateArray();
    StateArray &body_LED = var_body_LED->getTarget()->getStateArray();
    StateArray &frontal_LED = var_frontal_LED->getTarget()->getStateArray();
    StateArray &crown_LEDs = var_crown_LEDs->getTarget()->getStateArray();
    StateArray &calibration = var_calibration->getTarget()->getStateArray();
    StateArray &command_proximity 
        = var_command_proximity->getTarget()->getStateArray();
    StateArray& proximity = var_proximity->getStateArray(); 
    StateArray &command_light 
        = var_command_light->getTarget()->getStateArray();
    StateArray& light = var_light->getStateArray(); 
    StateArray& command_audio = var_command_audio->getTarget()->getStateArray();
    StateArray& audio = var_audio->getStateArray(); 
    StateArray& command_acceleration = 
        var_command_acceleration->getTarget()->getStateArray();
    StateArray& acceleration = var_acceleration->getStateArray(); 
    StateArray& command_image = var_command_image->getTarget()->getStateArray();
    StateArray& image_RH = var_image_RH->getStateArray(); 
    StateArray& image_GS = var_image_GS->getStateArray(); 
    StateArray& image_BV = var_image_BV->getStateArray(); 
    StateArray& command_sound = var_command_sound->getTarget()->getStateArray();

    float l_wheel, r_wheel;
    qmutexThread->lock();
    l_wheel = motor[0][0]-motor[0][2];
    r_wheel = motor[0][1]-motor[0][3];
    qmutexThread->unlock();
    epuck.setSpeed(l_wheel, r_wheel);

    epuck.setBodyLED((0 != body_LED[0][0]));
    epuck.setFrontalLED((0 != frontal_LED[0][0]));
    for(int i = 0; i < 8; ++i) epuck.setCrownLED(i, (0 != crown_LEDs[0][i]));

    if(command_proximity[0][0])
      vectorToGroup(epuck.proximityReadout(), proximity);
    else vectorToGroup(vector<float>(8,0), proximity);

    if(command_light[0][0])
      vectorToGroup(epuck.lightReadout(), light);
    else vectorToGroup(vector<float>(8,0), light);
    
    if(command_audio[0][0])
      vectorToGroup(epuck.audioReadout(), audio);
    else vectorToGroup(vector<float>(3,0), audio);

    if(command_acceleration[0][0])
      vectorToGroup(epuck.accelerationReadout(), acceleration);
    else vectorToGroup(vector<float>(3,0), acceleration);

    for(int i = 0; i < 6; ++i) if(command_sound[0][i] > 0){
        epuck.playSound(i);
        break;          // Only first active sound is played 
    }

    if(command_image[0][0]){
        bool hsv = par_hsv->getValue();
        bool rotate = par_rotate->getValue();
        bool grayscale = par_grayscale->getValue();

        vector<Pixel> image;

        if(!grayscale && hsv) image = epuck.imageReadout(HSV, rotate); // HSV
        else image = epuck.imageReadout(RGB, rotate); // RGB and grayscale

        qmutexThread->lock();
        imageToGroups(image, image_RH, image_GS, image_BV);
        qmutexThread->unlock();

        // We reset the clock, since this operation takes a lot of time and
        // sets it off track
        gettimeofday(&time_init, NULL); 
        elapsed_cicles = 0;
    }
    else{
        int width = par_width->getValue();
        int height = par_height->getValue();
        qmutexThread->lock();
        vectorToGroup(vector<float>(width*height,0), image_RH);
        vectorToGroup(vector<float>(width*height,0), image_GS);
        vectorToGroup(vector<float>(width*height,0), image_BV);
        qmutexThread->unlock();
    }

    if(0 != calibration[0][0]){
        epuck.calibrateProximitySensors();
        // We reset the clock, since this operation takes a lot of time and
        // sets it off track
        gettimeofday(&time_init, NULL); 
        elapsed_cicles = 0;
    }
}

void iqrcommon::ClsModuleEpuck::cleanup(){
    epuck.setSpeed(0, 0);

    epuck.setBodyLED(0);
    epuck.setFrontalLED(0);
    for(int i = 0; i < 8; ++i) epuck.setCrownLED(i, 0);

    epuck.playSound(0);

    epuck.close();
}
