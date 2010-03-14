#include "videoDeviceFactory.hpp"
#include "v4l2Device.hpp"
#include <libv4l2.h>
#include <fcntl.h>

VideoDevice* VideoDeviceFactory::open(const string& device_name, 
                                      string& error_message){
    VideoDevice* vd;

    vd = new V4L2Device(device_name);
    if(vd->isValid()) return vd;
    else{
        error_message = vd->error();
        delete vd;
        return NULL;
    }
}

