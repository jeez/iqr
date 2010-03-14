/****************************************************************************
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/
#ifdef WIN32
#include <algorithm>
#endif

#include <iostream>
using namespace std;

#include <Common/Helper/deleteObject.hpp>
using namespace effective_stl;

#include "stateVariableHolder.hpp"

#ifdef DEBUG_CLSSTATEVARIABLEHOLDER
static const bool bDebugStateVariableHolder = true;
#else
static const bool bDebugStateVariableHolder = false;
#endif

iqrcommon::ClsStateVariableHolder::ClsStateVariableHolder() 
{
    if (bDebugStateVariableHolder) {
	cout << "ClsStateVariableHolder::ClsStateVariableHolder"
	     << endl;
    }    
}
    
iqrcommon::ClsStateVariableHolder::~ClsStateVariableHolder() 
{    
    if (bDebugStateVariableHolder) {
	cout << "ClsStateVariableHolder::~ClsStateVariableHolder: "
	     << endl;
    }

    // Delete the state variables explicitly.
    for_each(lstStateVariables.begin(), lstStateVariables.end(), DeleteObject());
}

/** 
 * Add a new state variable to the list of state variables.
 *
 * Currently this function supports only double state variables.  This
 * function would need to be rewritten/overloaded if more types are to
 * be supported.
 *
 * @param _name Name of the state variable.
 * @param _label Human-readable label for the state variable.
 * @param _iMinLength Minimum length of the state array buffer.
 */
iqrcommon::ClsStateVariable*
iqrcommon::ClsStateVariableHolder::addStateVariable(string _name, 
						    string _label, 
						    unsigned int _minLengthHistory)
{
    ClsStateVariable *pState 
	= new ClsStateVariable(_name, _label, _minLengthHistory);
    lstStateVariables.push_back(pState);
    return pState;
}

/** 
 * @return Reference to the list of state variables.
 */
const iqrcommon::StateVariableList& 
iqrcommon::ClsStateVariableHolder::getListStates() const
{
    return lstStateVariables;
}

/** 
 * Get the specified state variable.
 *
 * This function searches the list of state variables for the
 * specified name.  If the name is found in a state variable object, a
 * pointer to that state variable is returned; if the name is not
 * found, a NULL pointer is returned.
 *
 * @return Pointer to the specified state variable.  WARNING: If the
 * name is not found, a NULL pointer is returned.
 * @param _strStateName Name of desired state variable.
 */
iqrcommon::ClsStateVariable*
iqrcommon::ClsStateVariableHolder::getState(string _name)
{
    ClsStateVariable *pState = 0;

    // Find state variable in list.
    StateVariableList::const_iterator itState = lstStateVariables.begin();
    while (pState == 0 && itState != lstStateVariables.end()) {
	if ((*itState)->getName() == _name) {
	    pState = *itState;
	} else {
	    ++itState;
	}
    }
    
    return pState;
}

/** 
 * Set the number of elements in the state arrays of all state
 * variables.
 *
 * This function should be called before a simulation starts.
 * 
 * @param _nElements Number of elements for each state array.
 */
void 
iqrcommon::ClsStateVariableHolder::resize(unsigned int _nElements)
{
    iqrcommon::StateVariableList::iterator itState;
    for (itState = lstStateVariables.begin(); 
	 itState != lstStateVariables.end();
	 ++itState) {
	
	ClsStateVariable *pState = *itState;
	
	if (bDebugStateVariableHolder) {
	    cout << "Creating state array for variable "
		 << pState->getName()
		 << endl;
	}

	pState->setSize(_nElements);
    }
}

/** 
 * Resets the size of the state arrays in the state variables to their
 * default settings.
 *
 * This function should be called after a simulation terminates.
 */
void 
iqrcommon::ClsStateVariableHolder::resetSize()
{
//    cout << "ClsStateVariableHolder::resetSize()" << endl;
//    cout << "lstStateVariables.size(): " << lstStateVariables.size() << endl;
    iqrcommon::StateVariableList::iterator itState;
    for (itState = lstStateVariables.begin(); 
	 itState != lstStateVariables.end();
	 ++itState) {
	
 	ClsStateVariable *pState = *itState;
	
// 	cout << "pState == NULL? " << (pState==NULL ? "yes" : "no") << endl;
// 	cout << "pState->getName(): " << pState->getName() << endl;
 	pState->resetSize();
	
    }
//    cout << __FILE__ << ":" << __LINE__ << endl;
}

/** 
 * Advance the current pointer in all the state arrays.
 *
 * The state variable objects store the state arrays, so this function
 * iterates along the state variable list.  If performance is a
 * problem, the group could store a local list of state array
 * pointers; this would reduce the number of pointer dereferences
 * which need to be performed.
 */
void 
iqrcommon::ClsStateVariableHolder::advance()
{
    StateVariableList::iterator itState;
    for (itState = lstStateVariables.begin();
	 itState != lstStateVariables.end(); ++itState) {

	(*itState)->advance();
    }
}
