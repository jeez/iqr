/****************************************************************************
 ** $Filename: stringParameter.hpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef STRINGPARAMETER_HPP
#define STRINGPARAMETER_HPP

// #include <string>
// using std::string;

#include "parameter.hpp"

// String parameters can be labels, or have single or multi-line
// edits.

namespace iqrcommon {
    

    class ClsStringParameter : public ClsParameter
    {
    public:
	ClsStringParameter(string _strName, 
			   string _strLabel,
			   string _strDescription = "", 
			   string _strCategory = "");
	ClsStringParameter(string _strName, 
			   string _strLabel,
			   string _strValue = "", 
			   bool   _bEditable = true,
			   bool   _bLong = false, 
			   string _strDescription = "", 
			   string _strCategory = "");
	
	ClsStringParameter(const ClsStringParameter &_source);

	~ClsStringParameter();

	ClsStringParameter& operator= (const ClsStringParameter &_source);

	string getValue() const 
	{
	    return strValue;
	}
	string getValueAsString() const
	{
	    return strValue;
	}
	void setValue(string _strValue);
	void setValueFromString(string _strValue);

	ClsStringParameter* makeCopy() const;

	bool isEditable() const {return bEditable;}
	bool isLong()     const {return bLong;}

    private:	
	// Disable default constructor.
	ClsStringParameter();
	
	string strValue;
	
	// Editable specifies whether the parameter should generate a
	// line edit-type widget (true) or a label widget (false).
	bool bEditable;
	
	// Long string specifies that a multiline edit should be used
	// to edit this string.  The default (in the constructor) is
	// false.
	bool bLong;
    };
};

#endif
