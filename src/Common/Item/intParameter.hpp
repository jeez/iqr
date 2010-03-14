/****************************************************************************
 ** $Filename: intParameter.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 20:39:57 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: 
 **
 *****************************************************************************/

#ifndef INTPARAMETER_HPP
#define INTPARAMETER_HPP

#include <string>

#include "parameter.hpp"

namespace iqrcommon {

    using namespace std;

    class ClsIntParameter : public ClsParameter
    {	
    public:	
	ClsIntParameter(string _strName, 
			string _strLabel, 
			string _strDescription = "",
			string _strCategory = "");
	ClsIntParameter(string _strName,
			string _strLabel,
			int _iValue,
			int _iMinimum,
			int _iMaximum, 
			string _strDescription = "", 
			string _strCategory = "");
	
	ClsIntParameter(const ClsIntParameter &_source);

	~ClsIntParameter();
	
	ClsIntParameter& operator= (const ClsIntParameter &_source);
	
	int getValue() const 
	{
	    return iValue;
	}
	string getValueAsString() const;
	void setValue(int _iValue);
	void setValueFromString(string _strValue);
	
	int  getMinimum() const {return iMinimum;}
	int  getMaximum() const {return iMaximum;}
	void setRange(int _iMinimum, int _iMaximum);
	
	// SetRange(...) should only be called from a parameter widget.
	// There is no check for iMinimum <= iMaximum.
	
	string getMinSetter() const {return strMinSetter;}
	string getMaxSetter() const {return strMaxSetter;}
	void setMinSetter(ClsIntParameter *_pMinSetter);
	void setMaxSetter(ClsIntParameter *_pMaxSetter);
	
	ClsIntParameter* makeCopy() const;
	
    private:	
	// Disable default constructor.
	ClsIntParameter();

	static const int DEFAULT_VALUE   =     0;
	static const int DEFAULT_MINIMUM = -1000;
	static const int DEFAULT_MAXIMUM =  1000;
	
	int iValue, iMinimum, iMaximum;
	
	string strMinSetter, strMaxSetter;	
    };
};

#endif
