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
 ** $Description: Simple synapse model where all synapses have the identical 
 **               fixed weight.  Presynaptic output is multiplied by the 
 **               weight to give the output
 **
 *****************************************************************************/

#ifndef SYNAPSEUNIFORMFIXEDWEIGHT_HPP
#define SYNAPSEUNIFORMFIXEDWEIGHT_HPP

#include <Common/Item/synapse.hpp>

namespace iqrcommon {

    class ClsSynapseUniformFixedWeight : public ClsSynapse
    {
    public:
	ClsSynapseUniformFixedWeight();

	void update();

    private:
	// Hide copy constructor.
	ClsSynapseUniformFixedWeight(const ClsSynapseUniformFixedWeight&);

	ClsDoubleParameter *pWeight;

	// Pointer to output state variable.
	ClsStateVariable *pPostsynapticPotential;
    };
}

#endif
