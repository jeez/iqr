/****************************************************************************
 ** $Filename: neuronRandomSpike.cpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Aug  9 18:38:39 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Implemetation of an IQR random spike neuron.
 **
 **               @@@ 14/08/01 This neuron can be combined with
 **               integrate&fire by adding a spontaneous activity
 **               parameter to the latter.
 **
 *****************************************************************************/

#include "neuronRandomSpike.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_NEURON_DLL_INTERFACE(iqrcommon::ClsNeuronRandomSpike, 
			  "Random spike")

iqrcommon::ClsNeuronRandomSpike::ClsNeuronRandomSpike()
    : ClsNeuron(),
      pActivity(0)
{    
    // Use base class function to create a parameter object, and store
    // the pointer to its value field (for efficient computation).
    pProbability = addDoubleParameter("probability", "Probability",
				      0.3, 0.0, 1.0, 4,
				      "Probability of a spike occuring\n"
				      "during a single timestep.");
    
    // Spike amplitude should be normalized to reduce parameter overload in models
    pSpikeAmplitude = addDoubleParameter("spkAmpl", "Spike amplitude",
					 1.0, 0.0, 1.0, 4,
					 "Amplitude of each spike.");
    
    pActivity = addOutputState("act", "Activity");
}

void 
iqrcommon::ClsNeuronRandomSpike::update()
{    
    // Get a local reference to the state array.
    StateArray &activity = pActivity->getStateArray();

    // Extract the parameter values.
    double probability = pProbability->getValue();
    double spikeAmpl   = pSpikeAmplitude->getValue();

    // Neurons of this type are active with a probability given by
    // the probability parameter.  The amplitude of their output
    // is given by spikeAmpl.
    activity.fillProbabilityMask(probability);
    activity[0] *= spikeAmpl;
}
