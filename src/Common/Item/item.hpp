/****************************************************************************
 ** $Filename: item.hpp
 ** $Header$
 **
 ** Copyright (C) 2000 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Wed Dec 13 15:23:08 2000
 ** $Date$
 **
 ** $Keywords:
 ** $Description: This class forms the basis of all model classes
 **               which must save data in the system file.
 **
 **               It provides a list to hold the items parameter
 **               objects, which should be created in the
 **               derived-class' constructor, plus a member function
 **               to return a list of these parameters.  
 **
 **               A list is used in preference to a map in order to
 **               preserve the creation order of the programmer (of
 **               either IQR core items or user-defined modules,
 **               neuron and synapse types).
 **
 *****************************************************************************/

#ifndef ITEM_HPP
#define ITEM_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <list>
#include <iostream>

using namespace std;

#include "parameterList.hpp"
#include "boolParameter.hpp"
#include "doubleParameter.hpp"
#include "intParameter.hpp"
#include "optionsParameter.hpp"
#include "stringParameter.hpp"
#include "xRefHolder.hpp"

namespace iqrcommon {

//     class ClsParameter;
//     class ClsBoolParameter;
//     class ClsDoubleParameter;
//     class ClsIntParameter;
//     class ClsOptionsParameter;
//     class ClsStringParameter;

//     class ParameterNotFound : public runtime_error
//     {
//     public:
// 	/** 
// 	 * Constructor for parameter not found exception class.
// 	 *
// 	 * This class derives from std::runtime_error, and is thrown
// 	 * by item objects when the requested parameter is not found
// 	 * in the parameter list/map.  The what() message is used to
// 	 * return the exception details.
// 	 *
// 	 * @param &_strName Name of parameter which could not be
// 	 * found.
// 	 */
// 	ParameterNotFound(const string &_strName)
// 	    : runtime_error(string("unknown parameter ") + _strName)
// 	{}
//     };

    class ClsItem
    {
    public:
	ClsItem();
	virtual ~ClsItem();


	/* stuff for items that have members items */
	virtual list<string> getListOfMembers() {
	    list<string> lst;
	    return lst;
	};

	virtual ClsItem* getMemberByName(string /*strName*/){
	    cout << "ClsItem::getMemberByName(string strName)" << endl;
	    
	    return NULL;
	}; 

	virtual void setMemberTypeByName(string /*strName*/, string /*strValue*/){};
	virtual string getMemberTypeByName(string /*strName*/){return "";};

	virtual list<string> getListOfMembersTypes(string /*strName*/) {
	    cout << "ClsItem::getListOfMembersTypes(string strName)" << endl;
	    list<string> lst;
	    return lst;
	};

//	virtual string Type() { return ""; };
	virtual string Type() { return strType; };
	


	virtual list<string> getListOfXRefHolders(){
	    list<string> lst;
	    return lst;
	};
	virtual const ClsXRefHolder* getXRefHolder(string /* strName */) const { 
	    cout << "ClsItem::getXRefHolder(string)" << endl;
	    return NULL; };


	
	/* ------------------------------ */
	

	
	// Add/remove functions insulate derived classes from the internal
	// parameter storage mechanism.

	// Parameter objects are created and managed by this class.
	// Derived classes should use these member functions to add
	// parameters to the list.  The operators new and delete for
	// parameter objects should only be called by this class.
	ClsBoolParameter* addBoolParameter(string _strName, 
					   string _strLabel,
					   string _strDescription = "",
					   string _strCategory = "");

	ClsBoolParameter* addBoolParameter(string _strName, 
					   string _strLabel,
					   bool _bValue, 
					   string _strDescription = "",
					   string _strCategory = "");

	ClsDoubleParameter* addDoubleParameter(string _strName, 
					       string _strLabel,
					       string _strDescription = "",
					       string _strCategory = "");

	ClsDoubleParameter* addDoubleParameter(string _strName, 
					       string _strLabel,
					       double  _dValue, 
					       double  _dMinimum, 
					       double  _dMaximum, 
					       int     _iPrecision,
					       string _strDescription = "",
					       string _strCategory = "");

	ClsIntParameter* addIntParameter(string _strName, 
					 string _strLabel,
					 string _strDescription = "",
					 string _strCategory = "");
	
	ClsIntParameter* addIntParameter(string _strName, 
					 string _strLabel,
					 int    _iValue, 
					 int    _iMinimum, 
					 int    _iMaximum, 
					 string _strDescription = "",
					 string _strCategory = "");

	ClsOptionsParameter* addOptionsParameter(string _strName, 
						 string _strLabel, 
						 string _strDescription = "",
						 string _strCategory = "");
	ClsOptionsParameter* addOptionsParameter(string _strName, 
						 string _strLabel,
						 bool   _bReadOnly, 
						 string _strDescription = "", 
						 string _strCategory = "");
	
	
	ClsStringParameter* addStringParameter(string _strName, 
					       string _strLabel,
					       string _strDescription = "", 
					       string _strCategory = "");
	ClsStringParameter* addStringParameter(string _strName, 
					       string _strLabel,
					       string _strValue, 
					       bool   _bEditable,
					       bool   _bLong, 
					       string _strDescription = "", 
					       string _strCategory = "");
	// @@@ 17/11/01 Add other parameter types here (file, group).

	void removeParameter(string _strName);

	// Export a list of the parameter objects.
	const ParameterList& getListParameters() const;

	ClsParameter* getParameter(string _strName);
	// @@@ 29/11/01 setParameter could be virtual here.  A module
	// may need to take special actions when specific parameter
	// values are changed at run-time.  In this case, setParameter
	// should not be used by the front-end.
	void setParameter(string _strName, string _strValue);

	void setParameters(const ParameterList &_lstParameters);
	bool isRuntimeEditable() { return bRuntimeEditable; };
	


    protected:
	string strType;
	bool bRuntimeEditable;

    private:
	void addParameterToList(ClsParameter *_pParam);
	void addParameterToMap(ClsParameter *_pParam);

	// The parameter list is used to manage the parameter objects.
	// Use of a list preserves the order of creation, which is
	// desirable for constructing the user interface.
	ParameterList lstParameters;

	// The parameter map provides convenient access for setting
	// parameter values.
	typedef map<string,ClsParameter* const> ParameterMap;
	ParameterMap mapParameters;

	
    };
};

#endif
