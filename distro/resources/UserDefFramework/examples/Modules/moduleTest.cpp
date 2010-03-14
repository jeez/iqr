#include "moduleTest.hpp"

MAKE_MODULE_DLL_INTERFACE(iqrcommon::ClsModuleTest,
			  "test module 1")

iqrcommon::ClsModuleTest::ClsModuleTest() : 
    ClsModule() {
    addDoubleParameter("dummy Par0",
		       "Minimum potential",
		       0.0, 0.0,
		       1.0, 3,
		       "Minimum value of the membrane potential.",
		       "Params");

    addDoubleParameter("dummy Par1",
		       "Maximum potential",
		       1.0, 0.0,
		       1.0, 3,
		       "Maximum value of the membrane potential.",
		       "Params");


    /* add input from group */
    inputStateVariablePtr = addInputFromGroup("_nameIFG0", "IFG 0");

    /* add output to group */
    outputStateVariable = addOutputToGroup("_nameOTG0", "OTG 0");

}


void 
iqrcommon::ClsModuleTest::init(){
    if(inputStateVariablePtr->getTarget()->getStateArray().getWidth()<9 ){
        throw ModuleError(string("Module \"") + label() + "\": needs 9 cells for output");
    }

    if(outputStateVariable->getStateArray().getWidth()<10){
        throw ModuleError(string("Module \"") + label() + "\": needs 10 cells for input");
    }

};

void 
iqrcommon::ClsModuleTest::cleanup(){
};

void 
iqrcommon::ClsModuleTest::update(){
    /* input from group */
    StateArray &clsStateArrayInput = inputStateVariablePtr->getTarget()->getStateArray();

    /* output to group */
    StateArray &clsStateArrayOut = outputStateVariable->getStateArray();

    for(unsigned int ii=0; ii<clsStateArrayOut.getWidth(); ii++){
	clsStateArrayOut[0][ii] = ii;
    }
};



