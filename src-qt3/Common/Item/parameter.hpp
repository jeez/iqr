/****************************************************************************
 ** $Filename: parameter.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 20:43:05 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: base class for parameter types.  
 **
 **               This class provides a field for the parameter name
 **               and a virtual function declaration to return the
 **               value of the parameter as a string.  Parameters
 **               must be set in the constructor, but can be changed
 **               after the object has been created.
 **
 *****************************************************************************/

#ifndef PARAMETER_HPP
#define PARAMETER_HPP
    
#include <string>
using namespace std;

namespace iqrcommon {

//     using std::string;

    class ClsBoolParameter;

    class ClsParameter
    {
    public:
	// Category field is used to group parameters.  Description field
	// is used to build tooltips in parameter widgets.
	ClsParameter(string _strName, 
		     string _strLabel, 
		     string _strDescription,
		     string _strCategory = "", 
 		     string _strEnabler = "");
	virtual ~ClsParameter() {}
    
	virtual string getValueAsString() const = 0;
	// If the name and value are both strings (i.e. after a
	// message has been received), a cast can be avoided by
	// overriding this function in each parameter type.  Either a
	// boolean Success/Failure flag should be returned, or an
	// exception should be thrown if an error occurs (e.g. string
	// contained invalid data for parameter type).
	virtual void setValueFromString(string _strValue) = 0;
    
	virtual ClsParameter* makeCopy() const = 0;
    
	string getName() const 
	{
	    return strName;
	}    
	string getLabel() const 
	{
	    return strLabel;
	}
	string getDescription() const 
	{
	    return strDescription;
	}
	string getCategory() const 
	{
	    return strCategory;
	}
	string getEnabler() const 
	{
	    return strEnabler;
	}
	void setEnabler(ClsBoolParameter *_pEnabler);
    
    private:    
	// Hide default and copy constructors.
	ClsParameter();
	ClsParameter(const ClsParameter & );
	// All parameters must have a valid name.
	string strName;
    
	// Label is displayed by autoWidget.
	string strLabel;
    
	// Descriptions are used by autoWidget to construct tooltips
	// for parameter widgets.
	string strDescription;
    
	// Categories are used to group parameters in autoWidget.
	string strCategory;
    
	// Enabler contains the name of a boolean parameter object
	// which controls the state of this parameter.  This is used
	// by autoWidget to establish the connections between
	// parameter widgets.
	string strEnabler;    
    };
};

#endif

