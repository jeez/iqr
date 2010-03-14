/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov  2 23:08:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Apical dendrite synapse model.
 **
 *****************************************************************************/

#ifndef SYNAPSEAPICALSHUNT_HPP
#define SYNAPSEAPICALSHUNT_HPP

#include <Common/Item/synapse.hpp>

namespace iqrcommon {

    class ClsSynapseApicalShunt : public ClsSynapse
    {
    public:
	ClsSynapseApicalShunt();

	void update();

    private:
	// Hide copy constructor.
	ClsSynapseApicalShunt(const ClsSynapseApicalShunt&);

	// Feedback inputs
	ClsStateVariable *pApicalFeedback;
	ClsStateVariable *pApicalShunt;

	// Pointer to output state.
	ClsStateVariable *pPostsynapticPotential;
    };
}

#endif
