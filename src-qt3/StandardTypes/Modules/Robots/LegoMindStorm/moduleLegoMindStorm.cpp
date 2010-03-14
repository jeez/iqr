#include "moduleLegoMindStorm.hpp"
#include "rcxlib.h"
#include <Common/Helper/iqrUtils.h>


MAKE_MODULE_DLL_INTERFACE(iqrcommon::moduleLegoMindStorm,
			  "Lego MindStorm")

   

    iqrcommon::moduleLegoMindStorm::moduleLegoMindStorm() : ClsThreadModule() {
    fd = -1;

    clsStringParameterDeviceName = addStringParameter("DeviceName", /* _strName */ 
						      "Serial Port", /* _strLabel */
						      "/dev/ttyS0", /* _strValue */
						      true, /* _bEditable */
						      false, /* _bLong */
						      "Path to Serial Port", /* _strDescription */
						      "Device" /* _strCategory */);



    string strParamName;
    string strParamLabel;

    for (int ii=0; ii<MAX_SENSOR; ii++){
	strParamName = "SensorMode" + iqrUtils::int2string(ii+1);
	strParamLabel = "Sensor " + iqrUtils::int2string(ii+1) + " Mode";
	
	ClsOptionsParameter* clsOptionsParameterSensorMode =
	    addOptionsParameter(strParamName,  /* _strName */
				strParamLabel,  /* _strLabel */
				false /* _bReadOnly */,
				strParamLabel,  /* _strDescription */
				"Sensors" /* _strCategory */);
	clsOptionsParameterSensorMode->addOption(SensorModeSet());
	clsOptionsParameterSensorMode->addOption(SensorModeRaw());
	clsOptionsParameterSensorMode->addOption(SensorModeBoolean());
	setParameter(strParamName, SensorModeRaw());
	vSensorModeOptions.push_back(clsOptionsParameterSensorMode);
	
	
	strParamName = "SensorType" + iqrUtils::int2string(ii+1);
	strParamLabel = "Sensor " + iqrUtils::int2string(ii+1) + " Type";
	
	ClsOptionsParameter* clsOptionsParameterSensorType =
	    addOptionsParameter(strParamName,  /* _strName */
				strParamLabel,  /* _strLabel */
				false /* _bReadOnly */,
				strParamLabel,  /* _strDescription */
				"Sensors" /* _strCategory */);
	clsOptionsParameterSensorType->addOption(SensorTypeRaw());
	clsOptionsParameterSensorType->addOption(SensorTypeTouch());
	clsOptionsParameterSensorType->addOption(SensorTypeTemp());
	clsOptionsParameterSensorType->addOption(SensorTypeLight());
	clsOptionsParameterSensorType->addOption(SensorTypeRot());
	setParameter(strParamName, SensorModeRaw());
	vSensorTypeOptions.push_back(clsOptionsParameterSensorType);
    }



//    clsOptionsParameterSensorMode = addOptionsParameter("SensorMode",  /* _strName */
//							"Sensor Mode",  /* _strLabel */
//							false /* _bReadOnly */,
//							"Sensor Mode",  /* _strDescription */
//							"Sensors" /* _strCategory */);
//    clsOptionsParameterSensorMode->addOption(SensorModeSet());
//    clsOptionsParameterSensorMode->addOption(SensorModeRaw());
//    clsOptionsParameterSensorMode->addOption(SensorModeBoolean());
//    setParameter("SensorMode", SensorModeRaw());
	
	
//    clsOptionsParameterSensorType = addOptionsParameter("SensorType",  /* _strName */
//							"Sensor Type",  /* _strLabel */
//							false /* _bReadOnly */,
//							"Sensor Type",  /* _strDescription */
//							"Sensors" /* _strCategory */);
//    clsOptionsParameterSensorType->addOption(SensorTypeRaw());
//    clsOptionsParameterSensorType->addOption(SensorTypeTouch());
//    clsOptionsParameterSensorType->addOption(SensorTypeTemp());
//    clsOptionsParameterSensorType->addOption(SensorTypeLight());
//    clsOptionsParameterSensorType->addOption(SensorTypeRot());
//    setParameter("SensorType", SensorTypeRaw());
	

    

    stateVariableMotor = addInputFromGroup("motorInput", "Motor Input");
    stateVariableFloat = addInputFromGroup("floatInput", "Float Input");
    stateVariableFlip = addInputFromGroup("flipInput", "Flip Input");

    clsStateVariableSensors = addOutputToGroup("SensorOutput", "Sensors");

}


iqrcommon::moduleLegoMindStorm::~moduleLegoMindStorm(){ 
//    cout << "moduleLegoMindStorm::~moduleLegoMindStorm()" << endl;
    cleanup();
}


void iqrcommon::moduleLegoMindStorm::init(){
//    cout << "moduleLegoMindStorm::init()" << endl;

    fd = -1;
    iNrMotorInputs = stateVariableMotor->getTarget()->getStateArray().getWidth()/2;

    if(iNrMotorInputs > MAX_MOTOR){
	throw ModuleError(string("Module \"") + label() + "\": \"Motor Input\": Maximum Motors =" + iqrUtils::int2string(MAX_MOTOR));
    }

    if(((stateVariableMotor->getTarget()->getStateArray().getWidth())%2)!=0 ){
	throw ModuleError(string("Module \"") + label() + "\": needs even number cells in \"Motor Input\"");
    }
    
    if(stateVariableFloat->getTarget()->getStateArray().getWidth()!=iNrMotorInputs){
	throw ModuleError(string("Module \"") + label() + "\": \"Float Input\": has to be =" + iqrUtils::int2string(iNrMotorInputs));
    }

    if(stateVariableFlip->getTarget()->getStateArray().getWidth()!=iNrMotorInputs){
	throw ModuleError(string("Module \"") + label() + "\": \"Flip Input\": has to be =" + iqrUtils::int2string(iNrMotorInputs));
    }

    if(clsStateVariableSensors->getStateArray().getWidth()> MAX_SENSOR){
	throw ModuleError(string("Module \"") + label() + "\": Maximum Sensors =" + iqrUtils::int2string(MAX_SENSOR));
    }


    
    string strDeviceName = clsStringParameterDeviceName->getValue(); 
  
    fd = rcx_init(strDeviceName.c_str());
    if(fd < 0) {
	cerr << "Device " << strDeviceName << " couldn't be opened" << endl;
	cerr << "\tERROR: " << strerror(errno) << endl;
	throw ModuleError(string("Module \"") + label() + "\": ERROR: " + strerror(errno));
    }
    
    for (int ii=0; ii<MAX_SENSOR; ii++){
	int iSensorType = get_sensor_type(vSensorTypeOptions[ii]->getValue());
	rcx_set_sensor_type(fd, ii, iSensorType);
    }

    vecRcxMotorControls.resize(iNrMotorInputs);
    vaSensorReadings.resize(clsStateVariableSensors->getStateArray().getWidth());

}



void iqrcommon::moduleLegoMindStorm::update(){
//    cerr << "moduleLegoMindStorm::update()" << endl;
/*
  double fMotorThresold = clsDoubleParameterMotorThresold ->getValue();    
*/


/* Update sensors:
   this is a potentially very time consuming activity, we there for don't want it do be inside the mutex lock */
    for (unsigned int ii = 0; ii < clsStateVariableSensors->getStateArray().getWidth(); ii++) {
	int iSensorMode = get_sensor_mode(vSensorModeOptions[ii]->getValue());
	int raw_value = (int) rcx_get_value(fd, iSensorMode, ii);
//	cout << "raw_value(ii): " << raw_value << "(" << ii << ")" << endl;
	vaSensorReadings[ii] = (iSensorMode == MODE_RAW_ID ? (double)(1024-raw_value) / 1024. : raw_value);
//	cout << "clsStateArraySensors[0][ii] : " << clsStateArraySensors[0][ii]  << endl;
    }


    qmutexThread->lock(); //new
    StateArray &clsStateArrayMotorInput = stateVariableMotor->getTarget()->getStateArray();
    StateArray &clsStateArrayFloatInput = stateVariableFloat->getTarget()->getStateArray();
    StateArray &clsStateArrayFlipInput = stateVariableFlip->getTarget()->getStateArray();
    StateArray &clsStateArraySensors = clsStateVariableSensors->getStateArray();


    for (int iCell = 0; iCell < iNrMotorInputs; iCell++) {
	double fDelta = clsStateArrayMotorInput[0][iCell] - clsStateArrayMotorInput[0][iCell + iNrMotorInputs];
/*
	cout << "iCellr: " << iCell  << endl;
	cout << "iCell + iNrCellsHor: " << iCell + iNrCellsHor << endl;
*/
	vecRcxMotorControls[iCell].motor_dir = 0;
	vecRcxMotorControls[iCell].motor_command = 0;

	/* motor on/off */
/*
  if(abs(fDelta)<fMotorThresold){
  motor_command = (PAR_SET_MOTORS_OFF | (0x01 << ii));
  } else {
  motor_command = (PAR_SET_MOTORS_ON | (0x01 << ii));
  }
*/

	/* motor direction */
	if(fDelta>0){
	    vecRcxMotorControls[iCell].motor_dir = (PAR_SET_MOTORS_FWD | (0x01 << iCell));
	} else {
	    vecRcxMotorControls[iCell].motor_dir = (PAR_SET_MOTORS_REV | (0x01 << iCell));
	}

	/* motor power */
	vecRcxMotorControls[iCell].iMotorPower = (int)(abs(fDelta)* double(MAX_MOTORPOWER));
//	cout << "vecRcxMotorControls[iCell].iMotorPower: " << vecRcxMotorControls[iCell].iMotorPower << endl;
	if(vecRcxMotorControls[iCell].iMotorPower>MAX_MOTORPOWER){
	    vecRcxMotorControls[iCell].iMotorPower = MAX_MOTORPOWER;
	}
        vecRcxMotorControls[iCell].motor_id = 0x01 << iCell;

	/* float */
	if(clsStateArrayFloatInput[0][iCell] > 0){
	    vecRcxMotorControls[iCell].motor_command |= (PAR_SET_MOTORS_FLOAT | (0x01 << iCell));
	} else {
	    vecRcxMotorControls[iCell].motor_command |= (PAR_SET_MOTORS_ON | (0x01 << iCell));
	}
	
    	/* flip */
	if(clsStateArrayFlipInput[0][iCell] > 0){
	    vecRcxMotorControls[iCell].motor_dir |= (PAR_SET_MOTORS_FLIP | (0x01 << iCell));

	}
/*
	fprintf(stderr, "RCX: Motor command 0x%X\n", vecRcxMotorControls[iCell].motor_command);
	fprintf(stderr, "RCX: Motor direction 0x%X\n", vecRcxMotorControls[iCell].motor_dir);
	fprintf(stderr, "RCX: Motor power 0x%X 0x%X\n", vecRcxMotorControls[iCell].motor_id, vecRcxMotorControls[iCell].iMotorPower);
	cout << "iMotorPower: " << vecRcxMotorControls[iCell].iMotorPower << endl;
*/
    }


	clsStateArraySensors[0] = vaSensorReadings;
	qmutexThread->unlock(); //new


	/* again the sendig is very slow, so we put it outside the mutex lock */
	for(int ii=0; ii<iNrMotorInputs; ii++){
	    rcx_set_motors_state(fd, vecRcxMotorControls[ii].motor_command);
	    rcx_set_motors_power(fd, vecRcxMotorControls[ii].motor_id, 2, vecRcxMotorControls[ii].iMotorPower);
	    rcx_set_motors_direction(fd, vecRcxMotorControls[ii].motor_dir);	
	}


};
    

void iqrcommon::moduleLegoMindStorm::cleanup(){
//    cout << "moduleLegoMindStorm::cleanup()" << endl;

    if(fd>=0){
	rcx_set_motors_state(fd, PAR_SET_MOTORS_FLOAT | 0x07);
	rcx_close(fd);
    }


}

int iqrcommon::moduleLegoMindStorm::get_sensor_type(string str) {
    if (!str.compare(SensorTypeRaw())) {
	return SENSOR_RAW_ID;
    }
    if (!str.compare(SensorTypeTouch())) {
	return SENSOR_TOUCH_ID;
    }
    if (!str.compare(SensorTypeTemp())) {
	return SENSOR_TEMP_ID;
    }
    if (!str.compare(SensorTypeLight())) {
	return SENSOR_LIGHT_ID;
    }
    if (!str.compare(SensorTypeRot())) {
	return SENSOR_ROT_ID;
    }
    return SENSOR_RAW_ID;
}


// Returns a sensor mode number depending on the input parameter string
// If unknown, then raw is returned
int iqrcommon::moduleLegoMindStorm::get_sensor_mode(string str) {
    if (!str.compare(SensorModeSet())) {
	return MODE_SET_ID;
    }
    if (!str.compare(SensorModeRaw())) {
	return MODE_RAW_ID;
    }
    if (!str.compare(SensorModeBoolean())) {
	return MODE_BOOLEAN_ID;
    }
    return MODE_RAW_ID;
}

    

