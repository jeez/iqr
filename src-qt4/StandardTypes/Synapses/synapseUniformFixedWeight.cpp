/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Oct  7 22:28:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Implemetation of uniform fixed weight synapse.
 **
 *****************************************************************************/

#include <cstdlib>
using namespace std;

#include "synapseUniformFixedWeight.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_SYNAPSE_DLL_INTERFACE(iqrcommon::ClsSynapseUniformFixedWeight, 
			  "Uniform fixed weight")

iqrcommon::ClsSynapseUniformFixedWeight::ClsSynapseUniformFixedWeight()
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
				 "Uniform weight for all synapses.");

    pPostsynapticPotential = addOutputState("psp", "Postsynaptic potential");
}

void 
iqrcommon::ClsSynapseUniformFixedWeight::update()
{    
    // For clarity, create a local reference to the state array.  This
    // has the performance advantage that the state array pointer need
    // only be dereferenced once.
    StateArray &synIn = getInputState()->getStateArray();
    StateArray &psp   = pPostsynapticPotential->getStateArray();

    psp[0] = pWeight->getValue() * synIn[0];
}
