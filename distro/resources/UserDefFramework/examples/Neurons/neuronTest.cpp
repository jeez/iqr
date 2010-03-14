/* 
*/

#include "neuronTest.hpp"

#ifdef DEBUG_CLSNEURONTEST
static const bool bDebugNeuronTest = true;
#else
static const bool bDebugNeuronTest = false;
#endif

// Interface for dynamic loading is built using a macro.
MAKE_NEURON_DLL_INTERFACE(iqrcommon::ClsNeuronTest, 
			  "TestNeuron")

iqrcommon::ClsNeuronTest::ClsNeuronTest()
    : ClsNeuron(),
      pVmembrane(0),
      pActivity(0)
{    

    pExcGain = addDoubleParameter("excGain", "Excitatory gain",
				  1.0, 0.0, 1.0, 3, 
				  "Gain of excitatory inputs.\n"
				  "The inputs are summed before\n"
				  "being multiplied by this gain.",
				  "Input");
    
    pInhGain = addDoubleParameter("inhGain", "Inhibitory gain",
				  1.0, 0.0, 1.0, 3, 
				  "Gain of inhibitory inputs.\n"
				  "The inputs are summed before\n"
				  "being multiplied by this gain.",
				  "Input");
    
    // This neuron type doesn't use modulatory input, so don't include
    // ModGain.
    
    // Membrane persistence.
    pVmPrs = addDoubleParameter("vmPrs", "Membrane persistence",
				0.0, 0.0, 1.0, 3, 
				"Proportion of the membrane potential\n"
				"which remains after one time step\n"
			     "if no input arrives.",
				"Membrane");
    
    // Membrane clip parameters
    pClipVm = addBoolParameter("clipVm", "Clip potential", 
			       false, 
			       "Restrict the range of the membrane potential",
			       "Clip");
    
    pVmMin = addDoubleParameter("vmMin", "Minimum potential",
				0.0, 0.0, 1.0, 3, 
				"Minimum value of the membrane potential.",
				"Clip");
    
    pVmMax = addDoubleParameter("vmMax", "Maximum potential",
				1.0, 0.0, 1.0, 3, 
				"Maximum value of the membrane potential.",
				"Clip");
    
    pVmMin->setEnabler(pClipVm);
    pVmMax->setEnabler(pClipVm);
    pVmMin->setMaxSetter(pVmMax);
    pVmMax->setMinSetter(pVmMin);

   
    pMidpoint = addDoubleParameter("midpoint", "Midpoint",
				    0.0, 0.0, 1.0, 3, 
				   "Midpoint of the Test",
				   "Test");
    
    pSlope = addDoubleParameter("slope", "Slope",
				1.0, 1.0, 1.0, 3, 
				"Slope of the test",
				"Test");
    
    pVmembrane = addStateVariable("vm", "Membrane potential", 2);
    pActivity = addOutputState("act", "Activity");
}


void 
iqrcommon::ClsNeuronTest::update()
{    
    StateArray &excitation = getExcitatoryInput();
    StateArray &inhibition = getInhibitoryInput();
    StateArray &vm         = pVmembrane->getStateArray();
    StateArray &activity   = pActivity->getStateArray();

    double excGain     = pExcGain->getValue();
    double inhGain     = pInhGain->getValue();
    double vmPrs       = pVmPrs->getValue();
    bool   clipVm      = pClipVm->getValue();
    double vmMin       = pVmMin->getValue();
    double vmMax       = pVmMax->getValue();
    double midpoint   = pMidpoint->getValue();
    double slope       = pSlope->getValue();


    // Calculate membrane potential
    vm[0] = vm[1] * vmPrs;
    vm[0] += excitation[0] * excGain;
    vm[0] -= inhibition[0] * inhGain;
    
    if (clipVm) {
	// Clip the membrane potential
	vm[0][vm[0] < vmMin] = vmMin;
	vm[0][vm[0] > vmMax] = vmMax;
    }


    // Calculate activity = vm for active cells
    activity[0] = 0.5*(1+tanh(2*slope * (vm[0] - midpoint)) ); 


}



