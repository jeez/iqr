/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Oct  7 22:28:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Implemetation of fixed weight synapse.
 **
 *****************************************************************************/

#include "synapseFixedWeight.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_SYNAPSE_DLL_INTERFACE(iqrcommon::ClsSynapseFixedWeight, 
			  "Fixed weight")

iqrcommon::ClsSynapseFixedWeight::ClsSynapseFixedWeight()
    : ClsSynapse()
{    
    // Use base class function to create a parameter object, and store
    // the pointer to its value field (for efficient computation).
    pWeight = addDoubleParameter("weight",
				 "Weight",
				 1.0,
				 -1.0,	// negative weights allow inversion
				 1.0,
				 5,
				 "Uniform synaptic weight.");

    pPostsynapticPotential = addOutputState("psp", "Postsynaptic potential");
}

void 
iqrcommon::ClsSynapseFixedWeight::update()
{    
    // For clarity, create a local reference to the state array.  This
    // has the performance advantage that the state array pointer need
    // only be dereferenced once.
    StateArray &synIn = getInputState()->getStateArray();
    StateArray &psp   = pPostsynapticPotential->getStateArray();

    // Also extract the parameter values for speed and clarity.
    // Again, this ensures that the pointer dereferencing happens only
    // once.
    double weight = pWeight->getValue();

    psp[0] = weight * synIn[0];
}
