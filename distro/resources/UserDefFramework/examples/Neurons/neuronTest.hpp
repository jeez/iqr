/* 
*/

#ifndef NEURONTEST_HPP
#define NEURONTEST_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    using namespace std;

    class ClsNeuronTest : public ClsNeuron
    {
    public:
	ClsNeuronTest();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronTest(const ClsNeuronTest&);

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
