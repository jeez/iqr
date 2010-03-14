/****************************************************************************
 ** $Filename: neuronLinearThreshold.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Dec 09 23:42:53 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef NEURONLINEARTHRESHOLD_HPP
#define NEURONLINEARTHRESHOLD_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    class ClsNeuronLinearThreshold : public ClsNeuron
    {
    public:
	ClsNeuronLinearThreshold();
	
	void update();

    private:	
	// Hide copy constructor.
	ClsNeuronLinearThreshold(const ClsNeuronLinearThreshold &);

	// Pointers to parameter objects.  The objects are managed by
	// the base class, the pointers are only needed to provide
	// rapid access to these objects during the performance
	// critical update function.
	ClsDoubleParameter *pExcGain, *pInhGain;
	ClsDoubleParameter *pVmPrs;
	ClsBoolParameter   *pClipVm;
	ClsDoubleParameter *pVmMin, *pVmMax;
	ClsDoubleParameter *pProbability, *pThreshold;

	// Pointers to state variables.	
	ClsStateVariable *pVmembrane, *pActivity;	
    };
}

#endif


