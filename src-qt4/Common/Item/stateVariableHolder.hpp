/****************************************************************************
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef STATEVARIABLEHOLDER_HPP
#define STATEVARIABLEHOLDER_HPP

#include <list>
#include <string>
using namespace std;

#include "stateVariable.hpp"

namespace iqrcommon {

    typedef list<ClsStateVariable*> StateVariableList;

    class ClsStateVariableHolder
    {
    public:
	ClsStateVariableHolder();
	~ClsStateVariableHolder();
	
	ClsStateVariable* addStateVariable(string _name, 
					   string _label, 
					   unsigned int _minLengthHistory = 1);
	const StateVariableList& getListStates() const;

	// Access to state variables needed to set maximum state
	// array lengths during simulation initialization.
	ClsStateVariable* getState(string _name);

	void resize(unsigned int _nElements);
	void resetSize();

	void advance();
			
    private:
	// Disable copy constructor.
	ClsStateVariableHolder(const ClsStateVariableHolder &);
	
	StateVariableList lstStateVariables;
    };
}

#endif
