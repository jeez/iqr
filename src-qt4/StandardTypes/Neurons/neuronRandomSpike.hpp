/****************************************************************************
 ** $Filename: neuronRandomSpike.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Aug  9 18:36:20 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Definition of an IQR random spike neuron.
 **
 *****************************************************************************/

#ifndef NEURONRANDOMSPIKE_HPP
#define NEURONRANDOMSPIKE_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    class ClsNeuronRandomSpike : public ClsNeuron
    {
    public:
	ClsNeuronRandomSpike();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronRandomSpike(const ClsNeuronRandomSpike&);

	// Pointers to parameter values.  The parameter values should
	// not be changed by this class, so the pointers are marked
	// const.  The values may be changed by the ClsItem without
	// passing through this class, should the pointers are marked
	// volatile?
	ClsDoubleParameter *pProbability, *pSpikeAmplitude;

	// Pointers to state variables.
	ClsStateVariable *pActivity;	
    };
}

#endif
