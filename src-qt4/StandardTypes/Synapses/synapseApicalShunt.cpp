/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Oct  7 22:28:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Implemetation of fixed weight synapse.
 **
 *****************************************************************************/

#include "synapseApicalShunt.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_SYNAPSE_DLL_INTERFACE(iqrcommon::ClsSynapseApicalShunt, 
			  "Apical shunt")

iqrcommon::ClsSynapseApicalShunt::ClsSynapseApicalShunt()
    : ClsSynapse()
{    
    // Add parameters for weight, learning window, etc.

    pApicalFeedback = addFeedbackInput("apicalFeedback", 
				       "Apical dendrite backpropogation");
    pApicalShunt = addFeedbackInput("apicalShunt", "Apical dendrite shunt");


    pPostsynapticPotential = addOutputState("psp", "Postsynaptic potential");
}

void 
iqrcommon::ClsSynapseApicalShunt::update()
{    
    // For clarity, create a local reference to the state array.  This
    // has the performance advantage that the state array pointer need
    // only be dereferenced once.
    StateArray &synIn    = getInputState()->getStateArray();
    //StateArray &apicalFB = pApicalFeedback->getStateArray();
    StateArray &shunt    = pApicalShunt->getStateArray();
    StateArray &psp      = pPostsynapticPotential->getStateArray();

    psp[0] = synIn[0] * shunt[0];
}
