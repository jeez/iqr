#ifndef _VIDEODEVICEFACTORY_HPP
#define _VIDEODEVICEFACTORY_HPP

#include <string>
#include "videoDevice.hpp"
using namespace std;

/* Factory of video device classes */
class VideoDeviceFactory{
    public:
        static VideoDevice* open(const string& device_name, 
                                 string& error_message);
};

#endif	// _VIDEODEVICEFACTORY_HPP

