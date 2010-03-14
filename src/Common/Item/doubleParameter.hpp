/****************************************************************************
 ** $Filename: doubleParameter.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 20:57:35 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef DOUBLEPARAMETER_HPP
#define DOUBLEPARAMETER_HPP

// #include <string>

#include "parameter.hpp"

namespace iqrcommon {
    
//     using std::string;

    class ClsDoubleParameter : public ClsParameter
    {
    public:	
	ClsDoubleParameter(string _strName,
			   string _strLabel,
			   string _strDescription = "",
			   string _strCategory = "");
	ClsDoubleParameter(string _strName, 
			   string _strLabel,
			   double _dValue, 
			   double _dMinimum, 
			   double _dMaximum, 
			   int    _iPrecision,
			   string _strDescription = "",
			   string _strCategory = "");

	ClsDoubleParameter(const ClsDoubleParameter &_source);

	~ClsDoubleParameter();

	ClsDoubleParameter& operator= (const ClsDoubleParameter &_source);

	double getValue() const 
	{
	    return dValue;
	}
	string getValueAsString() const;
	void   setValue(double _dValue);
 	void   setValueFromString(string _strValue);
			
	ClsDoubleParameter* makeCopy() const;
	
	double getMinimum() const {return dMinimum;}
	double getMaximum() const {return dMaximum;}

	int  getPrecision() const {return iPrecision;}
	void setPrecision(int _iPrecision) {iPrecision = _iPrecision;}
	
	string getMinSetter() const {return strMinSetter;}
	string getMaxSetter() const {return strMaxSetter;}
	void   setMinSetter(ClsDoubleParameter *_pMinSetter);
	void   setMaxSetter(ClsDoubleParameter *_pMaxSetter);
	
    private:
	// Disable default constructor.
	ClsDoubleParameter();
	
	static const double DEFAULT_VALUE  (){return 0.0;};
	static const double DEFAULT_MINIMUM(){return 0.0;};
	static const double DEFAULT_MAXIMUM(){return 1.0;};
	
	// Precision specifies number of decimal places.
	static const int DEFAULT_PRECISION = 3;
	
	double dValue, dMinimum, dMaximum;
	int    iPrecision;
	
	string strMinSetter, strMaxSetter;	
    };    
};

#endif

