/******************************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: 
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *******************************************************************************************/

#include <iostream>
using namespace std;

#include "synapse.hpp"
#include "tagLibrary.hpp"

#ifdef DEBUG_CLSSYNAPSE
static const bool bDebugSynapse = true;
#else
static const bool bDebugSynapse = false;
#endif

iqrcommon::ClsSynapse::ClsSynapse()
    : ClsItem(),
      pOutput(0)
{
    if (bDebugSynapse) {
	cout << "ClsSynapse::ClsSynapse"
	     << endl;
    }    
    
    // Every synapse has one input.
    pInput = addStateVariable("axnlIn", "Axonal input");

    feedbackXRefHolder.setName(ClsTagLibrary::FeedbackTag());
    feedbackXRefHolder.setCategory("Feedback");
}

iqrcommon::ClsSynapse::~ClsSynapse()
{
    if (bDebugSynapse) {
	cout << "ClsSynapse::~ClsSynapse"
	     << endl;
    }    
}

void 
iqrcommon::ClsSynapse::setConnectionID(string str){
    strConnectionID = str;
}

string 
iqrcommon::ClsSynapse::getConnectionID(){
    return strConnectionID;
}


iqrcommon::ClsStateVariable*
iqrcommon::ClsSynapse::getInputState()
{
    return pInput;
}

// Access to output state is needed by connection, which pushes data
// into postsynaptic group.
iqrcommon::ClsStateVariable* 
iqrcommon::ClsSynapse::getOutputState()
{
    return pOutput;
}



void 
iqrcommon::ClsSynapse::resize(unsigned int _nElements)
{
    stateVariableHolder.resize(_nElements);
    pInput->setSize(_nElements);
    // Also resize each feedback input state array
}

void
iqrcommon::ClsSynapse::resetSize()
{
    stateVariableHolder.resetSize();
    pInput->resetSize();

    // Also reset size of each feedback input state array
}

const iqrcommon::StateVariableList& 
iqrcommon::ClsSynapse::getListStates() const
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
iqrcommon::ClsSynapse::getState(string _name)
{
    return stateVariableHolder.getState(_name);
}

/**
 * Get the list of feedback inputs.
 */
const iqrcommon::XRefList& 
iqrcommon::ClsSynapse::getListFeedbackInputs() const
{
    return feedbackXRefHolder.getListXRefs();
}


list<string> 
iqrcommon::ClsSynapse::getListOfXRefHolders(){
    list<string> lst;
    if(feedbackXRefHolder.getXRefsCount()>0){
	lst.push_back(feedbackXRefHolder.getName());
    }
    return lst;
}

const iqrcommon::ClsXRefHolder*
iqrcommon::ClsSynapse::getXRefHolder(string /*strName*/) const {
    return &feedbackXRefHolder;
}


/** 
 * Gets the named feedback input reference.
 *
 * @return Pointer to the cross reference.  WARNING: If the name is
 * not found, this function returns 0.
 */
iqrcommon::ClsXRef*
iqrcommon::ClsSynapse::getFeedbackInputRefByName(string _name)
{
    return feedbackXRefHolder.getXRefByName(_name);
}

iqrcommon::ClsXRef*
iqrcommon::ClsSynapse::getFeedbackInputRefByLabel(string _label)
{
    return feedbackXRefHolder.getXRefByLabel(_label);
}

// These functions are no-ops if the named XRef is not found.
void 
iqrcommon::ClsSynapse::setFeedbackInputByName(string _name, string _input)
{
    feedbackXRefHolder.setXRefTargetByName(_name, _input);
}

void 
iqrcommon::ClsSynapse::setFeedbackInputByLabel(string _label, string _input)
{
    feedbackXRefHolder.setXRefTargetByLabel(_label, _input);
}


/**
 * Prepares for the next timestep.
 */
void
iqrcommon::ClsSynapse::advance()
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
iqrcommon::ClsSynapse::addStateVariable(string _name, 
					string _label, 
					unsigned int _minLengthHistory)
{
    return stateVariableHolder.addStateVariable(_name, _label,
						_minLengthHistory);
}

/** 
 * Add the output state.
 *
 * This function should be called exactly once during the
 * construction.  If repeated calls are made, only the last state will
 * be used as the output.  WARNING: If this function is not called,
 * the name of the output site remains undefined and a runtime error
 * will occur when getOutputState or getOutput are called.
 *
 * @return Pointer to the newly created state variable.
 * @param _name Name of the state.
 * @param _label Human-readable label of the state.
 * @param _minLengthHistory Minimum length of the history.
 */
iqrcommon::ClsStateVariable* 
iqrcommon::ClsSynapse::addOutputState(string _name, 
				      string _label, 
				      unsigned int _minLengthHistory)
{
    // Keep track of the output state here - use the last state as the
    // output.
    nameOutputState = _name;
    pOutput = addStateVariable(_name, _label, _minLengthHistory);
    return pOutput;
}

iqrcommon::ClsStateVariable* 
iqrcommon::ClsSynapse::addFeedbackInput(string _name, 
					string _label,
					unsigned int _minLengthHistory)
{
    feedbackXRefHolder.addXRef(_name, _label);
    return stateVariableHolder.addStateVariable(_name, _label, _minLengthHistory);
}
 
