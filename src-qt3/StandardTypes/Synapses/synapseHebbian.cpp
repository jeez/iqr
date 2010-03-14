#include "synapseHebbian.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_SYNAPSE_DLL_INTERFACE(iqrcommon::ClsSynapseHebbian, 
			  "Hebbian")

iqrcommon::ClsSynapseHebbian::ClsSynapseHebbian() : ClsSynapse() {    
    pPostSynapticActivity = addFeedbackInput("postact", "Post Synaptic Activity");

    pSynapticPotential = addOutputState("synpot", "Synaptic potential");

    pWeight = addStateVariable("weight", "Synaptic Weight");
    
    
    pLR = addDoubleParameter("lrate",
			     "Learning Rate",
			     .1,
			     0.0,	
			     1.0,
			     6,
			     "Learning Rate");
    
    pDecay = addDoubleParameter("decay",
				"Decay",
				.1,
				0.0,	
				1.0,
				6,
				"Decay");
}

void iqrcommon::ClsSynapseHebbian::update() {    
    StateArray &synIn = getInputState()->getStateArray();

    StateArray &postact = pPostSynapticActivity->getStateArray();
    StateArray &synpot = pSynapticPotential->getStateArray();

    StateArray &weight = pWeight->getStateArray();

    double learningRate = pLR->getValue();
    double decay = pDecay->getValue();

/* */
    weight[0] = weight[0] + postact[0]*synIn[0]*learningRate;


/* with decay 
    weight[0] = weight[0] + postact[0]*synIn[0]*learningRate - 1./decay * weight[0];
*/
  
    synpot[0] = weight[0] * synIn[0];

}




