/*****************************************************************************
 ** $Header: /home/ulysses/Code/iqr421_Redesign/Common/Item/synapse.hpp,v 1.1 2005/03/19 15:56:59 ulysses Exp ulysses $
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: 
 ** $Date: 2005/03/19 15:56:59 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef SYNAPSE_HPP
#define SYNAPSE_HPP

#include <list>
#include <string>
using namespace std;

#include "item.hpp"
#include "stateArray.hpp"
#include "stateVariableHolder.hpp"

#include "xRefHolder.hpp"

namespace iqrcommon {

    class ClsSynapse : public ClsItem 
    {
    public:	
	ClsSynapse();
	virtual ~ClsSynapse();

	void setConnectionID(string str);
	string getConnectionID();
	
	// Connection handles connection masks, therefore...
	// 1. Parent connections needs access to input state to pass data
	// from presynaptic group into synapse.
	ClsStateVariable* getInputState();	

 	// 2. Parent connections needs access to feedback input states to
	// pass feedback data from postsynaptic group into synapse.
//      ClsStateVariable* getFeedbackInputState(string _name);

	// 3. Parent connections needs access to output data in order to
	// push data into postsynaptic group.
	ClsStateVariable* getOutputState();


 	virtual void initialize() {}
	virtual void update() = 0;
	virtual void reset() {}

	void resize(unsigned int _nElements);
	void resetSize();

	const StateVariableList& getListStates() const;

	// Access to state variables needed to set maximum state
	// array lengths during simulation initialization.
	ClsStateVariable* getState(string _name);
	
	const XRefList& getListFeedbackInputs() const;


	list<string> getListOfXRefHolders();
	const ClsXRefHolder* getXRefHolder(string strName) const;


	// Access to XRefs needed to establish feedback.
	ClsXRef* getFeedbackInputRefByName(string _name);
	ClsXRef* getFeedbackInputRefByLabel(string _label);

	void setFeedbackInputByName(string _name, string _input);
	void setFeedbackInputByLabel(string _label, string _input);

	void advance();
	
    protected:
	ClsStateVariable* addStateVariable(string _name, string _label, 
					   unsigned int _minLengthHistory = 1);
	ClsStateVariable* addOutputState(string _name, string _label,
					 unsigned int _minLengthHistory = 1);
	ClsStateVariable* addFeedbackInput(string _name, string _label,
					   unsigned int _minLengthHistory = 1);

    private:
	// Disable copy constructor.
	ClsSynapse(const ClsSynapse&);
	
	ClsStateVariableHolder stateVariableHolder;

	ClsStateVariable *pInput, *pOutput;

	ClsXRefHolder feedbackXRefHolder;

	string strConnectionID;
	string nameOutputState;
    };
};

/**
 * Macro which builds the shared object interface needed to
 * dynamically load synapse types.
 *
 * @param SynapseClass Name of the synapse class definition including
 * the namespace.
 * @param Human-readable name for the synapse class, e.g. for a synapse
 * class ClsSynapseSimple, the name might be "Simple".  Users
 * will use this name to create objects of the synapse type.  The name
 * should be supplied inside "".
 */
#define MAKE_SYNAPSE_DLL_INTERFACE(SynapseClass,Label) \
extern "C" { \
    const char* label() { return Label; } \
    iqrcommon::ClsSynapse* create() { return new SynapseClass; } \
    void destroy(const iqrcommon::ClsSynapse* _pSynapse) { delete _pSynapse; } \
}

#endif
