/* 
*/

#ifndef NEURONSIGMOID_HPP
#define NEURONSIGMOID_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    using namespace std;

    class ClsNeuronSigmoid : public ClsNeuron
    {
    public:
	ClsNeuronSigmoid();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronSigmoid(const ClsNeuronSigmoid&);

	ClsDoubleParameter *pExcGain, *pInhGain;
	ClsDoubleParameter *pVmPrs;
	ClsBoolParameter   *pClipVm;
	ClsDoubleParameter *pVmMin, *pVmMax;
	ClsDoubleParameter *pMidpoint, *pSlope;


	// Pointers to state variables.
	ClsStateVariable *pVmembrane, *pActivity;	
    };
}

#endif
