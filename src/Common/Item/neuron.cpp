/****************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Jun 15 12:20:44 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "neuron.hpp"

#ifdef DEBUG_CLSNEURON
static const bool bDebugNeuron = true;
#else
static const bool bDebugNeuron = false;
#endif

iqrcommon::ClsNeuron::ClsNeuron() 
    : ClsItem(),
      pOutput(0)
{
    if (bDebugNeuron) {
	cout << "ClsNeuron::ClsNeuron"
	     << endl;
    }    

    // (Almost) all neuron types handle excitatory, inhibitory and
    // modulatory input.

    pExcitation = addStateVariable("excIn", "Excitatory input");
    pInhibition = addStateVariable("inhIn", "Inhibitory input");
    pModulation = addStateVariable("modIn", "Modulatory input");
}
    
iqrcommon::ClsNeuron::~ClsNeuron() 
{    
    if (bDebugNeuron) {
	cout << "ClsNeuron::~ClsNeuron: "
	     << endl;
    }
}

// @@@ 3/9/2003 [jmb] The following three functions could be combined
// into a single function with the definition, only the connection types 
// have to be available:
//
// void
// iqrcommon::ClsNeuron::addInput(ClsBaseConnection::CONNECTION_TYPE type, 
//                                StateArray* _pInput)
// {
//     if (_pInput == 0) {
// 	// Ignore invalid input
// 	return;
//     }
//
//     switch (type) {
//         case CONN_EXCITATORY:
//             lstExcInputs.push_back(_pInput);
//             break;
//         case CONN_INHIBITORY:
//             lstInhInputs.push_back(_pInput);
//             break;
//         case CONN_MODULATORY:
//             lstModInputs.push_back(_pInput);
//             break;
//         default:
//             break;
//     }
// }
void
iqrcommon::ClsNeuron::addExcitatoryInput(StateArray *_pExcInput)
{
    if (_pExcInput == 0) {
	// Ignore invalid input
	return;
    }

    lstExcInputs.push_back(_pExcInput);

    if (bDebugNeuron) {
	cout << "Added excitatory input, neuron has "
	     << lstExcInputs.size()
	     << " excitatory input arrays"
	     << endl;
    }    
}

void
iqrcommon::ClsNeuron::addInhibitoryInput(StateArray *_pInhInput)
{
    if (_pInhInput == 0) {
	// Ignore invalid input
	return;
    }

    lstInhInputs.push_back(_pInhInput);

    if (bDebugNeuron) {
	cout << "Added inhibitory input, neuron has "
	     << lstInhInputs.size()
	     << " inhibitory input arrays"
	     << endl;
    }
}

void
iqrcommon::ClsNeuron::addModulatoryInput(StateArray *_pModInput)
{
    if (_pModInput == 0) {
	// Ignore invalid input
	return;
    }

    lstModInputs.push_back(_pModInput);

    if (bDebugNeuron) {
	cout << "Added modulatory input, neuron has "
	     << lstModInputs.size()
	     << " modulatory input arrays"
	     << endl;
    }
}

/**
 * Clear input pointer lists.
 */
void
iqrcommon::ClsNeuron::removeAllInputs()
{
    lstExcInputs.clear();
    lstInhInputs.clear();
    lstModInputs.clear();
	
}

/** 
 * Gets the output state, allowing output connections access to the
 * data.
 *
 * @return Pointer to the output state variable.  WARNING: If
 * addOutputState has not been called, this function returns a NULL
 * pointer.
 */
iqrcommon::ClsStateVariable* 
iqrcommon::ClsNeuron::getOutputState()
{
    return pOutput;
}

/** 
 * Gets the named feedback state, allowing input connections access
 * to the data.
 *
 * @return Pointer to the output state variable.  WARNING: If the
 * named state variable is not found, this function returns 0.
 */
iqrcommon::ClsStateVariable* 
iqrcommon::ClsNeuron::getFeedbackState(string _name)
{
    return getState(_name);
}

/**
 * Get a list of names of feedback states which can be used in the
 * update functions of synapses in input connections.
 *
 * @return Copy of list of feedback state names.
 */
list<string>
iqrcommon::ClsNeuron::getListFeedbackStateNames() const
{
    list<string> namesFeedbackStates(lstNamesFeedbackStates);
    // The output state can be used for feedback.  It isn't included
    // in the feedback as the author of a user-defined type might,
    // incorrectly, set the output state twice.  If speed is an issue,
    // this policy can be altered.
    namesFeedbackStates.push_front(nameOutputState);
    return namesFeedbackStates;
}

/** 
 * Set the number of elements in the component and input states.
 *
 * This function should be called before a simulation starts.
 * 
 * @param _nElements Number of elements for each state array.
 */
void 
iqrcommon::ClsNeuron::resize(unsigned int _nElements)
{
    stateVariableHolder.resize(_nElements); /* Problem */
    pExcitation->setSize(_nElements);
    pInhibition->setSize(_nElements);
    pModulation->setSize(_nElements);
}

/** 
 * Resets the size of the component and input state arrays to their
 * default settings.
 *
 * This function should be called after a simulation terminates.
 */
void 
iqrcommon::ClsNeuron::resetSize()
{
    stateVariableHolder.resetSize();
    pExcitation->resetSize();
    pInhibition->resetSize();
    pModulation->resetSize();    

}

const iqrcommon::StateVariableList& 
iqrcommon::ClsNeuron::getListStates() const
{
    return stateVariableHolder.getListStates();
}

/** 
 * Gets the named state.
 *
 * @return Pointer to the state variable.  WARNING: If the
 * named state variable is not found, this function returns 0.
 */
iqrcommon::ClsStateVariable*
iqrcommon::ClsNeuron::getState(string _name)
{
    return stateVariableHolder.getState(_name);
}


/**
 * Prepares for the next timestep.
 */
void
iqrcommon::ClsNeuron::advance()
{
    stateVariableHolder.advance();
}
	
/**
 * Add a state variable.
 *
 * This function may be called repeatedly during construction of
 * derived classes to add states with unique names.
 *
 * @param _name Name of the state variable.
 * @param _label Human-readable label for the state variable.
 * @param _iMinLength Minimum length of the state array buffer.
 */
iqrcommon::ClsStateVariable* 
iqrcommon::ClsNeuron::addStateVariable(string _name, 
				       string _label, 
				       unsigned int _minLengthHistory)
{
    return stateVariableHolder.addStateVariable(_name, _label,
						_minLengthHistory);
}

/** 
 * Add the output state, which provides the information transmitted
 * from a group via its output connections.
 *
 * This function should be called exactly once during the construction
 * of a neuron type.  If repeated calls are made, only the last state
 * will be used as the output.  WARNING: If this function is not
 * called, the name of the output site remains undefined and a runtime
 * error will occur when getOutputState is called.
 *
 * @return Pointer to the newly created state variable.
 * @param _name Name of the state.
 * @param _label Human-readable label of the state.
 * @param _minLengthHistory Minimum length of the history.
 */
iqrcommon::ClsStateVariable* 
iqrcommon::ClsNeuron::addOutputState(string _name, 
				     string _label, 
				     unsigned int _minLengthHistory)
{
    // Keep track of the output state here - use the last state as the
    // output.
    nameOutputState = _name;
    pOutput = addStateVariable(_name, _label, _minLengthHistory);
    return pOutput;
}

/**
 * Add a new feedback state.
 *
 * Feedback states can be used in the update functions of synapses in
 * input connections.
 *
 * @return Pointer to the newly created state variable.
 * @param _name Name of the state.
 * @param _label Human-readable label of the state.
 * @param _minLengthHistory Minimum length of the history.
 */
iqrcommon::ClsStateVariable*
iqrcommon::ClsNeuron::addFeedbackState(string _name,
				       string _label, 
				       unsigned int _minLengthHistory)
{
    lstNamesFeedbackStates.push_back(_name);
    return addStateVariable(_name, _label, _minLengthHistory);
}

/** 
 * Calculate the total excitatory input.
 *
 * This default function sums all excitatory inputs on a
 * neuron-by-neuron basis and returns a reference to the result.  The
 * individual inputs are stored in a list of state arrays created by
 * the incoming connections using addExcitatoryInput.
 *
 * @return Reference to the total excitatory input.
 */
iqrcommon::StateArray&
iqrcommon::ClsNeuron::getExcitatoryInput()
{
    StateArray &excitation = pExcitation->getStateArray();
    excitation[0] = 0.0;

    // Iterate through the list of input state arrays, summing the
    // values in the totalExcitation state array.
    InputList::const_iterator itExcInput;
    for (itExcInput = lstExcInputs.begin();
	 itExcInput != lstExcInputs.end();
	 ++itExcInput) {

	// The list of inputs stores pointers to the input arrays.
	// Extract the input array from the iterator...
	StateArray &exc = *(*itExcInput);
	// ...and add the input to the total.
 	excitation[0] += exc[0];
	
    }

    return excitation;
}

/** 
 * Calculate the total inhibitory input.
 *
 * This default function sums all inhibitory inputs on a
 * neuron-by-neuron basis and returns a reference to the result.  The
 * individual inputs are stored in a list of state arrays created by
 * the incoming connections using addInhibitoryInput.
 *
 * @return Reference to the total inhibitory input.
 */
iqrcommon::StateArray&
iqrcommon::ClsNeuron::getInhibitoryInput()
{
    StateArray &inhibition = pInhibition->getStateArray();
    inhibition[0] = 0.0;

    // Iterate through the list of input state arrays, summing the
    // values in the inhibition state array.
    InputList::const_iterator itInhInput;
    for (itInhInput = lstInhInputs.begin();
	 itInhInput != lstInhInputs.end();
	 ++itInhInput) {

	// The list of inputs stores pointers to the input arrays.
	// Extract the input array from the iterator...
	StateArray &inh = *(*itInhInput);
	// ...and add the input to the total.
 	inhibition[0] += inh[0];
	
    }

    return inhibition;
}

/** 
 * Calculate the total modulatory input.
 *
 * This default function sums all modulatory inputs on a
 * neuron-by-neuron basis and returns a reference to the result.  The
 * individual inputs are stored in a list of state arrays created by
 * the incoming connections using addModulatoryInput.
 *
 * @return Reference to the total modulatory input.
 */
iqrcommon::StateArray&
iqrcommon::ClsNeuron::getModulatoryInput()
{
    StateArray &modulation = pModulation->getStateArray();
    modulation[0] = 0.0;

    // Iterate through the list of input state arrays, summing the
    // values in the modulation state array.
    InputList::const_iterator itModInput;
    for (itModInput = lstModInputs.begin();
	 itModInput != lstModInputs.end();
	 ++itModInput) {

	// The list of inputs stores pointers to the input arrays.
	// Extract the input array from the iterator...
	StateArray &mod = *(*itModInput);
	// ...and add the input to the total.
 	modulation[0] += mod[0];	
    }

    return modulation;
}
