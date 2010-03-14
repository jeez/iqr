/****************************************************************************
 ** $Filename: colorParameter.hpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef COLORPARAMETER_HPP
#define COLORPARAMETER_HPP

// #include <string>
// using std::string;

#include "parameter.hpp"

// Color parameters can be labels, or have single or multi-line
// edits.

namespace iqrcommon {
    

    class ClsColorParameter : public ClsParameter
    {
    public:
	ClsColorParameter(string _strName, 
			 string _strLabel,
			 string _strDescription = "", 
			 string _strCategory = "");
	ClsColorParameter(string _strName, 
			 string _strLabel,
			 string _strValue = "", 
			 bool _bEditable = true,
			 string _strDescription = "", 
			 string _strCategory = "");
	
	ClsColorParameter(const ClsColorParameter &_source);

	~ClsColorParameter();

	ClsColorParameter& operator= (const ClsColorParameter &_source);

	string getValue() const { return strValue; }
	string getValueAsString() const { return strValue; }
	void setValue(string _strValue);
	void setValueFromString(string _strValue);

	ClsColorParameter* makeCopy() const;

	bool isEditable() const {return bEditable;}

    private:	
	// Disable default constructor.
	ClsColorParameter();
	
	string strValue;
	
	// Editable specifies whether the parameter should generate a
	// line edit-type widget (true) or a label widget (false).
	bool bEditable;
	
    };
};

#endif
