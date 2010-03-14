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

#ifndef SYNAPSEFIXEDWEIGHT_HPP
#define SYNAPSEFIXEDWEIGHT_HPP

#include <Common/Item/synapse.hpp>

namespace iqrcommon {

    class ClsSynapseFixedWeight : public ClsSynapse
    {
    public:
	ClsSynapseFixedWeight();

	void update();

    private:
	// Hide copy constructor.
	ClsSynapseFixedWeight(const ClsSynapseFixedWeight&);

	ClsDoubleParameter *pWeight;

	// Pointers to state variables.
	ClsStateVariable *pPostsynapticPotential;
    };
}

#endif
