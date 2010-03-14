/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author$ 
 ** 
 ** Created: Wed Jan 15 14:29:30 2003
 ** Time-stamp: <Sun Nov 09 2003 17:52:44 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef NEURONPYRAMIDAPICALSHUNT_HPP
#define NEURONPYRAMIDAPICALSHUNT_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    class ClsNeuronPyramidApicalShunt : public ClsNeuron
    {
    public:	
	ClsNeuronPyramidApicalShunt();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronPyramidApicalShunt(const ClsNeuronPyramidApicalShunt&);
	
	ClsDoubleParameter *pVmPrs, *pThreshold, *pVmReset;
	
	// Pointers to state variables.	
	ClsStateVariable *pVmembrane, *pActivity;

	// Pointers to feedback states.
	ClsStateVariable *pShunt, *pBackpropogation;
    };
}

#endif
