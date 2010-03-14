/****************************************************************************
 ** $Header$
 **
 ** $Author$ 
 ** 
 ** Created: Wed Jan 15 14:29:30 2003
 ** Time-stamp: <Sun Nov 09 2003 17:22:49 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef NEURONINTEGRATEFIRE_HPP
#define NEURONINTEGRATEFIRE_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    class ClsNeuronIntegrateFire : public ClsNeuron
    {
    public:	
	ClsNeuronIntegrateFire();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronIntegrateFire(const ClsNeuronIntegrateFire&);

	// Pointers to parameter objects.  The objects are managed by
	// the base class, the pointers are only needed to provide
	// rapid access to these objects during the performance
	// critical update function.
	ClsDoubleParameter *pExcGain, *pInhGain;
	ClsDoubleParameter *pVmPrs;
	ClsBoolParameter   *pClipVm;
	ClsDoubleParameter *pVmMin, *pVmMax;
	ClsDoubleParameter *pProbability, *pBaseThreshold;
	ClsDoubleParameter *pSpikeAmplitude, *pVmReset;

	// Pointers to state variables.	
	ClsStateVariable *pVmembrane, *pActivity;	
    };
}

#endif
