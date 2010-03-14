/****************************************************************************
 ** $Filename: pathParameter.hpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#ifndef PATHPARAMETER_HPP
#define PATHPARAMETER_HPP

// #include <string>
// using std::string;

#include "parameter.hpp"

// Path parameters can be labels, or have single or multi-line
// edits.

namespace iqrcommon {
    

    class ClsPathParameter : public ClsParameter
    {
    public:
	ClsPathParameter(string _strName, 
			 string _strLabel,
			 string _strDescription = "", 
			 string _strCategory = "");
	ClsPathParameter(string _strName, 
			 string _strLabel,
			 string _strValue = "", 
			 bool _bEditable = true,
			 string _strDescription = "", 
			 string _strCategory = "");
	
	ClsPathParameter(const ClsPathParameter &_source);

	~ClsPathParameter();

	ClsPathParameter& operator= (const ClsPathParameter &_source);

	string getValue() const { return strValue; }
	string getValueAsString() const { return strValue; }
	void setValue(string _strValue);
	void setValueFromString(string _strValue);

	ClsPathParameter* makeCopy() const;

	bool isEditable() const {return bEditable;}

    private:	
	// Disable default constructor.
	ClsPathParameter();
	
	string strValue;
	
	// Editable specifies whether the parameter should generate a
	// line edit-type widget (true) or a label widget (false).
	bool bEditable;
	
    };
};

#endif
