/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef NEURON_HPP
#define NEURON_HPP

#include <list>
#include <string>
using namespace std;

#include "item.hpp"
#include "stateArray.hpp"
#include "stateVariableHolder.hpp"

namespace iqrcommon {

    class ClsNeuron : public ClsItem
    {
    public:
	ClsNeuron();
	virtual ~ClsNeuron();

	// The functions below allow input connections to add their
	// data to the input
	void addExcitatoryInput(StateArray *_pExcInput);
	void addInhibitoryInput(StateArray *_pInhInput);
	void addModulatoryInput(StateArray *_pModInput);
	
	void removeAllInputs();

	ClsStateVariable* getOutputState();
	ClsStateVariable* getFeedbackState(string _name);
	list<string> getListFeedbackStateNames() const;

 	virtual void initialize() {}
	virtual void update() = 0;
	virtual void reset() {}

	void resize(unsigned int _nElements);
	void resetSize();

	const StateVariableList& getListStates() const;

	// Access to state variables needed to set maximum state
	// array lengths during simulation initialization.
	ClsStateVariable* getState(string _name);

	void advance();
	
   protected:
	ClsStateVariable* addStateVariable(string _name, 
					   string _label, 
					   unsigned int _minLengthHistory = 1);
	ClsStateVariable* addOutputState(string _name, 
					 string _label,
					 unsigned int _minLengthHistory = 1);
	ClsStateVariable* addFeedbackState(string _name, 
					   string _label,
					   unsigned int _minLengthHistory = 1);

	virtual StateArray& getExcitatoryInput();
	virtual StateArray& getInhibitoryInput();
	virtual StateArray& getModulatoryInput();
	
    private:
	// Disable copy constructor.
	ClsNeuron(const ClsNeuron&);
	
	ClsStateVariableHolder stateVariableHolder;

	typedef list<StateArray*> InputList;
	InputList lstExcInputs, lstInhInputs, lstModInputs;	
	ClsStateVariable *pExcitation, *pInhibition, *pModulation;
	ClsStateVariable *pOutput;

	string nameOutputState;
	list<string> lstNamesFeedbackStates;
    };
}

/**
 * Macro which builds the shared object interface needed to
 * dynamically load neuron types.
 *
 * @param NeuronClass Name of the neuron class definition including
 * the namespace.
 * @param Human-readable name for the neuron class, e.g. for a neuron
 * class ClsNeuronRandomSpike, the name might be "Random spike".  Users
 * will use this name to create objects of the neuron type.  The name
 * should be supplied inside "".
 */
#define MAKE_NEURON_DLL_INTERFACE(NeuronClass,Label) \
extern "C" { \
    const char* label() { return Label; } \
    iqrcommon::ClsNeuron* create() { return new NeuronClass; } \
    void destroy(const iqrcommon::ClsNeuron* _pNeuron) { delete _pNeuron; } \
}

#endif
