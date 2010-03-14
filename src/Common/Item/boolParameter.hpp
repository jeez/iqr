/****************************************************************************
 ** $Filename: boolParameter.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 20:59:11 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef BOOLPARAMETER_HPP
#define BOOLPARAMETER_HPP

// #include <string>
// using namespace std;

#include "parameter.hpp"

namespace iqrcommon {

    class ClsBoolParameter : public ClsParameter
    {
    public:
	ClsBoolParameter(string _strName, 
			 string _strLabel, 
			 string _strDescription = "",
			 string _strCategory = "");
	ClsBoolParameter(string _strName, 
			 string _strLabel, 
			 bool   _bValue, 
			 string _strDescription = "",
			 string _strCategory = "");

	ClsBoolParameter(const ClsBoolParameter &_source);

	~ClsBoolParameter();

	ClsBoolParameter& operator= (const ClsBoolParameter &_source);

	bool getValue() const 
	{
	    return bValue;
	}
	string getValueAsString() const;
	void setValue(bool _bValue);
	void setValueFromString(string _strValue);    

	ClsBoolParameter* makeCopy() const;

    private:
	// Disable default constructor.
	ClsBoolParameter();

	static const bool DEFAULT_VALUE = false;

	bool bValue;
    };

};

#endif

