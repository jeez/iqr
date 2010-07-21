#ifndef MODULE_EPUCK_HPP
#define MODULE_EPUCK_HPP

#include <Common/Item/threadModule.hpp>
#include <sys/time.h>
#include "epuck.h"

namespace iqrcommon{
	class ClsModuleEpuck : public ClsThreadModule{
		public:
			ClsModuleEpuck();
			void init();
			void update();
			void cleanup();

            moduleIcon getIcon();

		private:
			ClsModuleEpuck(const ClsModuleEpuck&);
            void relax();
            void vectorToGroup(vector<float> origin, StateArray& destination);
            void imageToGroups(vector<Pixel> image, StateArray& red_hue,
                               StateArray& green_saturation,
                               StateArray& blue_value);
            void checkWidth(StateArray& group, int width, 
                            const string& group_name);

            Epuck epuck;

            struct timeval time_init;
            int elapsed_cicles;

			StateVariablePtr* var_motor;
			StateVariablePtr* var_body_LED;
			StateVariablePtr* var_frontal_LED;
			StateVariablePtr* var_crown_LEDs;
            StateVariablePtr* var_calibration;
            StateVariablePtr* var_command_proximity;
            StateVariablePtr* var_command_light;
            StateVariablePtr* var_command_audio;
            StateVariablePtr* var_command_acceleration;
            StateVariablePtr* var_command_image;
            StateVariablePtr* var_command_sound;

			ClsStateVariable* var_proximity;
			ClsStateVariable* var_light;
			ClsStateVariable* var_audio;
			ClsStateVariable* var_acceleration;
			ClsStateVariable* var_image_RH;
			ClsStateVariable* var_image_GS;
			ClsStateVariable* var_image_BV;

            ClsStringParameter *device_name;
			ClsIntParameter *par_cps;

            // Camera parameters
			ClsBoolParameter *par_grayscale;
			ClsBoolParameter *par_hsv;
			ClsBoolParameter *par_rotate;
            ClsIntParameter *par_width;
            ClsIntParameter *par_height;
            ClsIntParameter *par_zoom; // 1,4,8
            ClsIntParameter *par_x1;
            ClsIntParameter *par_y1;
            //ClsIntParameter *par_fps;
	};
}

#endif
