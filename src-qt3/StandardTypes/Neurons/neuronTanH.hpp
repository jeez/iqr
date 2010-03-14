/* WARNING: 
   THIS IS JUST A PLACEHOLDER FOR THIS NEURON TYPE!
   (it's actually just a copy of neuronRandomSpike)
*/

#ifndef NEURONTANH_HPP
#define NEURONTANH_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    using namespace std;

    class ClsNeuronTanH : public ClsNeuron
    {
    public:	
	ClsNeuronTanH();
	~ClsNeuronTanH();

	void update();

    private:
	// Hide copy constructor.
	ClsNeuronTanH(const ClsNeuronTanH&);

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
