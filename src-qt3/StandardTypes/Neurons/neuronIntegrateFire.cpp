/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Wed Jan 15 14:34:55 2003
 ** Time-stamp: <Sat Jan 31 2004 21:30:39 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#include "neuronIntegrateFire.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_NEURON_DLL_INTERFACE(iqrcommon::ClsNeuronIntegrateFire, 
			  "Integrate & fire")

iqrcommon::ClsNeuronIntegrateFire::ClsNeuronIntegrateFire()
    : ClsNeuron(),
      pVmembrane(0),
      pActivity(0)
{    
    pExcGain = addDoubleParameter("excGain", "Excitatory gain",
				  1.0, 0.0, 10.0, 4, 
				  "Gain of excitatory inputs.\n"
				  "The inputs are summed before\n"
				  "being multiplied by this gain.",
				  "Input");
    
    pInhGain = addDoubleParameter("inhGain", "Inhibitory gain",
				  1.0, 0.0, 10.0, 4, 
				  "Gain of inhibitory inputs.\n"
				  "The inputs are summed before\n"
				  "being multiplied by this gain.",
				  "Input");
    
    // This neuron type doesn't use modulatory input, so don't include
    // ModGain.
    
    // Membrane persistence.
    pVmPrs = addDoubleParameter("vmPrs", "Membrane persistence",
				0.0, 0.0, 1.0, 4, 
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
				0.0, 0.0, 1.0, 4, 
				"Minimum value of the membrane potential.",
				"Clip");
    
    pVmMax = addDoubleParameter("vmMax", "Maximum potential",
				1.0, 0.0, 1.0, 4, 
				"Maximum value of the membrane potential.",
				"Clip");
    
    pVmMin->setEnabler(pClipVm);
    pVmMax->setEnabler(pClipVm);
    pVmMin->setMaxSetter(pVmMax);
    pVmMax->setMinSetter(pVmMin);

    pProbability = addDoubleParameter("probability", "Probability",
				      1.0, 0.0, 1.0, 4,
				      "Probability of output occuring\n"
				      "during a single timestep.",
				      "Membrane");
    
    pBaseThreshold = addDoubleParameter("threshold", "Threshold potential",
				    0.0, 0.0, 1.0, 4, 
				    "Membrane potential threshold for"
				    " output of a spike.",
				    "Membrane");

    // Spikes should be normalized to reduce parameter overload in models.
    pSpikeAmplitude = addDoubleParameter("spkAmpl", "Spike amplitude",
					 1.0, 0.0, 1.0, 4,
					 "Amplitude of output spikes.",
					 "Spike");
    
    pVmReset = addDoubleParameter("vmReset", "Membrane potential reset",
				  0.0, 0.0, 1.0, 4, 
				  "Membrane potential reduction after a spike.",
				  "Membrane");
    
    // Add state variables.
    pVmembrane = addStateVariable("vm", "Membrane potential");
    pActivity  = addOutputState("act", "Activity");
}

void
iqrcommon::ClsNeuronIntegrateFire::update()
{
    // For clarity, create local references to the state arrays.  This
    // has the performance advantage that the state array pointers need
    // only be dereferenced once.
    StateArray &excitation = getExcitatoryInput();
    StateArray &inhibition = getInhibitoryInput();
    StateArray &vm         = pVmembrane->getStateArray();
    StateArray &activity   = pActivity->getStateArray();

    // Get parameter values from parameter objects.
    double excGain     = pExcGain->getValue();
    double inhGain     = pInhGain->getValue();
    double vmPrs       = pVmPrs->getValue();
    bool   clipVm      = pClipVm->getValue();
    double vmMin       = pVmMin->getValue();
    double vmMax       = pVmMax->getValue();
    double probability = pProbability->getValue();
    double threshold   = pBaseThreshold->getValue();
    double spikeAmpl   = pSpikeAmplitude->getValue();
    double vmReset     = pVmReset->getValue();

    //1) new 2005/06/03: this way we can plot the exc/ihn with the grain applied
    excitation[0] *= excGain;
    inhibition[0] *= inhGain;



    // Calculate membrane potential
    vm[0] *= vmPrs;
    vm[0] += excitation[0];// see 1 * excGain;
    vm[0] -= inhibition[0];// see 1 * inhGain;
    
    if (clipVm) {
	// Clip the membrane potential
	vm[0][vm[0] < vmMin] = vmMin;
	vm[0][vm[0] > vmMax] = vmMax;
    }

    activity.fillProbabilityMask(probability);
    // All neurons at threshold or above produce a spike.
/* this did not take probability into account:    
   activity[0][vm[0] >= threshold] = spikeAmpl; */
    activity[0][vm[0] >= threshold] *= activity[0][vm[0] >= threshold];

    activity[0][vm[0] <  threshold] = 0.0;

    // All spikes reduce the membrane potential.
    // If spikeAmpl is not a free parameter, the check against 0 is not necessary here.
    if (spikeAmpl != 0.0) {
	vm[0] -= vmReset * activity[0]/spikeAmpl;
    }
}
