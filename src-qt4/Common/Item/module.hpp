/*****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 **
 *****************************************************************************/

#ifndef MODULE_HPP
#define MODULE_HPP

#include <list>
#include <string>
#include <stdexcept>

#include "item.hpp"
#include "namedPtr.hpp"
#include "stateArray.hpp"
#include "stateVariableHolder.hpp"

#include "moduleIcon.hpp"
#include "xRefHolder.hpp"

using namespace std;

namespace iqrcommon {

    typedef ClsNamedPtr<ClsStateVariable> StateVariablePtr;
    typedef list<StateVariablePtr*>  StateVariablePtrList;

    class ClsModule : public ClsItem
    {
    public:
	ClsModule();
	virtual ~ClsModule();

	void setProcessID(string str);
	string getProcessID();

	virtual void initialize();

	list<string> getListOfXRefHolders();
	const ClsXRefHolder* getXRefHolder(string strName) const;
	
	const XRefList& getListInputsFromGroups() const;
	const XRefList& getListOutputsToGroups() const;

	void setXRefTargetByName(string strXRefHolderName, string strXRefName, string strTargetID);


	ClsXRef* getInputFromGroupRefByName(string _name);
	ClsXRef* getOutputToGroupRefByName(string _name);

	ClsXRef* getInputFromGroupRefByLabel(string _label);
	ClsXRef* getOutputToGroupRefByLabel(string _label);

	void setInputFromGroupByName(string _name, string _input);
	void setOutputToGroupByName(string _name, string _input);

	void setInputFromGroupByLabel(string _label, string _input);
	void setOutputToGroupByLabel(string _label, string _input);

	ClsStateVariable* getState(string _name); 

	StateVariablePtr* addInputFromGroup(string _name, string _label);
	ClsStateVariable* addOutputToGroup(string _name, string _label);

	StateVariablePtrList& getListInputFromGroupPtrs() {
	    return inputFromGroupPtrList;
	}

	void resetOutputToGroupStateHolder();

	virtual void init(){};
	virtual void update(){};
	virtual void cleanup(){};

	virtual void start(){};
	virtual void stop(){};
	virtual moduleIcon getIcon();

//	virtual const bool isThreaded(){ return false; };

    protected:
    private:
	// Disable copy constructor.
	ClsModule(const ClsModule&);

	string strProcessID;

	// Maps indexed by the input/output names might be useful here.
	ClsXRefHolder inputFromGroupXRefHolder;
	ClsXRefHolder outputToGroupXRefHolder;
	StateVariablePtrList inputFromGroupPtrList;
	ClsStateVariableHolder outputToGroupStateHolder;
    };



    class ModuleError : public runtime_error {
    public:
	/** 
	 * @param _strReason Reason for failure.
	 */
	ModuleError(const string& _strReason) : runtime_error(_strReason) {};
    };

};

/**
 * Macro which builds the shared object interface needed to
 * dynamically load module types.
 *
 * @param ModuleClass Name of the module class definition including
 * the namespace.
 * @param Human-readable name for the module class, e.g. for a module
 * class ClsModuleSimple, the name might be "Simple".  Users
 * will use this name to create objects of the module type.  The name
 * should be supplied inside "".
 */
#define MAKE_MODULE_DLL_INTERFACE(ModuleClass,Label) \
extern "C" { \
    const char* label() { return Label; } \
    iqrcommon::ClsModule* create() { return new ModuleClass; } \
    void destroy(const iqrcommon::ClsModule* _pModule) { delete _pModule; } \
}

#endif






