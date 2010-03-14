/****************************************************************************
 ** $Header$
 **
 ** $Author$ 
 ** 
 ** Created: Wed Jan 15 14:34:55 2003
 ** Time-stamp: <Sat Jan 31 2004 21:35:40 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#include "neuronPyramidApicalShunt.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_NEURON_DLL_INTERFACE(iqrcommon::ClsNeuronPyramidApicalShunt, 
			  "Pyramidal with apical shunt")

iqrcommon::ClsNeuronPyramidApicalShunt::ClsNeuronPyramidApicalShunt()
    : ClsNeuron(),
      pVmembrane(0),
      pActivity(0)
{    
    // Membrane persistence.
    pVmPrs = addDoubleParameter("vmPrs",
				"Membrane persistence",
				0.0, 
				0.0,
				1.0,
				4, 
				"Proportion of the membrane potential\n"
				"which remains after one time step\n"
				"if no input arrives.",
				"Membrane");

    pThreshold = addDoubleParameter("threshold",
				    "Threshold potential",
				    0.0, 
				    0.0,
				    1.0, 
				    4, 
				    "Membrane potential threshold for"
				    " output of a spike.",
				    "Membrane");
    
    pVmReset = addDoubleParameter("vmReset",
				  "Membrane potential reset",
				  0.0, 
				  0.0,
				  1.0, 
				  4, 
				  "Membrane potential reduction after a spike.",
				  "Membrane");
    
    // Add state variables.
    pVmembrane = addStateVariable("vm", "Membrane potential");
    pActivity  = addOutputState("act", "Activity", 2);

    pShunt = addFeedbackState("shunt", "Apical shunt");
    pBackpropogation = addFeedbackState("backprop", 
					"Backpropogating spikes");
}

void
iqrcommon::ClsNeuronPyramidApicalShunt::update()
{
    // Get local references to the state arrays
    StateArray &excitation = getExcitatoryInput();
    StateArray &inhibition = getInhibitoryInput();
    StateArray &modulation = getModulatoryInput();

    StateArray &vm         = pVmembrane->getStateArray();
    StateArray &activity   = pActivity->getStateArray();

    StateArray &shunt      = pShunt->getStateArray();
    StateArray &backprop   = pBackpropogation->getStateArray();

    // Get parameter values from parameter objects
    double vmPrs       = pVmPrs->getValue();
    double threshold   = pThreshold->getValue();
    double vmReset     = pVmReset->getValue();

    // Calculate membrane potential
    vm[0] *= vmPrs;
    vm[0] += excitation[0];
    vm[0] -= inhibition[0];

    // Calculate activity, all neurons at threshold or above produce a spike
    activity[0] = 0.0;
    activity[0][vm[0] >= threshold] = 1.0;
	
    // All spikes reduce the membrane potential
    vm[0] -= activity[0] * vmReset;

    // Backpropogation, use an extra parameter to attenuate the amplitude
    backprop[0] = activity[1];

    // Shunt
    shunt[0] = 1.0 / (1.0 - modulation[0]);
}
