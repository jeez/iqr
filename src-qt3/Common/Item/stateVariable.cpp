/****************************************************************************
 ** $Header: /home/iqrdev/iqrCode/Common/Item/stateVariable.cpp,v 1.1 2002/08/06 12:50:46 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Mon Dec  3 16:04:11 2001
 ** $Date: 2002/08/06 12:50:46 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>
// using std::cout;
// using std::endl;
using namespace std;

#include "stateVariable.hpp"

#ifdef DEBUG_CLSSTATEVARIABLE
static const bool bDebugStateVariable = true;
#else
static const bool bDebugStateVariable = false;
#endif

/**
 * Constuctor for state variable.
 *
 * The underlying state array is created automatically with the
 * default sizes.  For use in a user interface, this imposes an
 * overhead.  However, it streamlines the computationally active
 * functions by removing the need for pointer checks.
 *
 * @param _name Name of the state variable.
 * @param _minLengthHistory Minimum length of history needed in number
 * of timesteps.  This class guarantees that the underlying state
 * array will have at least this many timesteps worth of history
 * during simulation, allowing for the construction of, e.g. filters.
 */
iqrcommon::ClsStateVariable::ClsStateVariable(string _name, 
					      string _label,
					      unsigned int _minLengthHistory)
    : name(_name),
      label(_label),
      minLengthHistory(_minLengthHistory)
{
    if (bDebugStateVariable) {
	cout << "ClsStateVariable::ClsStateVariable: "
	     << name
	     << endl;
    }
    stateArray.setDepth(minLengthHistory);
}

/** 
 * Destructor for state variable.
 */
iqrcommon::ClsStateVariable::~ClsStateVariable()
{
    if (bDebugStateVariable) {
	cout << "ClsStateVariable::~ClsStateVariable: "
	     << name
	     << endl;
    }
}

/**
 * @return Name of the state variable.
 */
string 
iqrcommon::ClsStateVariable::getName() const 
{
    return name;
}	

/**
 * @return Human-readable label of the state variable.
 */
string 
iqrcommon::ClsStateVariable::getLabel() const 
{
    return label;
}	

/** 
 * Get the state array for a state variable.
 *
 *  WARNING: If the state array has not been created in advance, this
 * function returns a NULL pointer.
 *
 * @return Pointer to the state array.
 */
iqrcommon::StateArray&
iqrcommon::ClsStateVariable::getStateArray()
{
    return stateArray;
}

const iqrcommon::StateArray&
iqrcommon::ClsStateVariable::getStateArray() const
{
    return stateArray;
}

void
iqrcommon::ClsStateVariable::setNumberElements(unsigned int _nElements)
{
    stateArray.setWidth(_nElements);
}

void
iqrcommon::ClsStateVariable::setLengthHistory(unsigned int _lengthHistory)
{
    if (_lengthHistory > stateArray.getDepth()) {
	// This function can only extend the history.  To shorten the
	// history, call resetSize.
	stateArray.setDepth(_lengthHistory);
    }
}

void
iqrcommon::ClsStateVariable::setSize(unsigned int _nElements, 
				     unsigned int _lengthHistory)
{
    // Ensure that the new history length is not shorter than the
    // current value.
    unsigned int lengthHistory = _lengthHistory;
    if (lengthHistory < stateArray.getDepth()) {
	lengthHistory = stateArray.getDepth();
    }
    
    // Don't check the number of elements, this is taken care of by
    // the state array.
    stateArray.setSize(_nElements, lengthHistory);
}

void
iqrcommon::ClsStateVariable::resetSize()
{
    stateArray.resetSize();
    stateArray.setDepth(minLengthHistory);
}

/** 
 * Advance the current index in the state array.
 */
void
iqrcommon::ClsStateVariable::advance()
{
    stateArray.advance();
}
