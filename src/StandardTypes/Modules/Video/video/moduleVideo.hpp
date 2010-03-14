#ifndef _MODULEVIDEO_HPP
#define _MODULEVIDEO_HPP

#include <Common/Item/threadModule.hpp>
#include <sys/time.h>
#include "videoDevice.hpp"
#include <ClsVideoDisp.hpp>

namespace iqrcommon {
    class moduleVideo:public ClsThreadModule{
        public:
            moduleVideo();
            ~moduleVideo();
            void init();
            void update();
            void cleanup();

            const char* getLabel();
            moduleIcon getIcon();

        private:
            moduleVideo(const moduleVideo&);
            void scale(float* orig, valarray<double>& r, valarray<double>& g,
                       valarray<double>& b, int width, int height, 
                       int new_width, int new_height);

            // Groups
            ClsStateVariable* clsStateVariableRedHue;
            ClsStateVariable* clsStateVariableGreenSat;
            ClsStateVariable* clsStateVariableBlueVal;

            // Parameters
            ClsIntParameter* clsIntParameterWidth;
            ClsIntParameter* clsIntParameterHeight;
            ClsIntParameter* clsIntParameterBrightness;
            ClsIntParameter* clsIntParameterSaturation;
            ClsIntParameter* clsIntParameterContrast;
            ClsIntParameter* clsIntParameterAutogain;
            ClsIntParameter* clsIntParameterInput;
            ClsIntParameter* clsIntParameterStandard;
            ClsStringParameter* clsStringParameterDeviceName;
            ClsBoolParameter* clsBoolParameterShowOutput;
            ClsIntParameter* clsIntParameterFPS;
            ClsBoolParameter* clsBoolParameterInfoWindow;

            ClsBoolParameter* clsBoolParameterHSV;
            ClsBoolParameter* clsBoolParameterMonochrome;

            // Other
            VideoDevice* clsVideoDevice;

            ClsVideoDisp* clsVideoDisp;

            struct timeval sTimevalInit;
            int iElapsedCycles;

            int iGroupWidth, iGroupHeight;
    };
}
#endif  // _MODULEVIDEO_HPP


