/****************************************************************************
 ** $Header$
 **
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

    class ClsSynapseTest : public ClsSynapse
    {
    public:
	ClsSynapseTest();

	void update();

    private:
	// Hide copy constructor.
	ClsSynapseTest(const ClsSynapseTest&);

	ClsDoubleParameter *pWeight;

	// Pointers to state variables.
	ClsStateVariable *pPostsynapticPotential;

	// Feedback inputs
	ClsStateVariable *pApicalFeedback;
	ClsStateVariable *pApicalShunt;


    };
}

#endif
