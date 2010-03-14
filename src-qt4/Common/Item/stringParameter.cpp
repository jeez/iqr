/****************************************************************************
 ** $Filename: stringParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#include <iostream>

#include "stringParameter.hpp"

#ifdef DEBUG_CLSSTRINGPARAMETER
static const bool bDebugStringParameter = true;
#else
static const bool bDebugStringParameter = false;
#endif

using namespace std;

iqrcommon::ClsStringParameter::ClsStringParameter(string _strName, 
						  string _strLabel, 
						  string _strDescription, 
						  string _strCategory)
    : ClsParameter(_strName,
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      strValue(""),
      bEditable(true),
      bLong(false)
{
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::ClsStringParameter: "
	     << getName()
	     << " is a short editable parameter, is empty" 
	     << endl;
    }
}

iqrcommon::ClsStringParameter::ClsStringParameter(string _strName, 
						  string _strLabel, 
						  string _strValue, 
						  bool   _bEditable, 
						  bool   _bLong, 
						  string _strDescription, 
						  string _strCategory)
    : ClsParameter(_strName,
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      strValue(_strValue),
      bEditable(_bEditable),
      bLong(_bLong)
{
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::ClsStringParameter: "
	     << getName()
	     << " is "
	     << (bLong ? "long" : "short")
	     << " and "
	     << (bEditable ? "editable" : "constant")
	     << " with value " 
	     << strValue
	     << endl;
    }
}

iqrcommon::ClsStringParameter::ClsStringParameter(const ClsStringParameter &_source)
    : ClsParameter(_source.getName(),
		   _source.getLabel(),
		   _source.getDescription(),
		   _source.getCategory()),
      strValue(_source.getValue()),
      bEditable(_source.isEditable()),
      bLong(_source.isLong())
{
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::ClsStringParameter: copied " 
	     << _source.getName()
	     << endl;
    }
}

iqrcommon::ClsStringParameter::~ClsStringParameter()
{    
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::~ClsStringParameter: "
 	     << getName()
	     << endl;
    }    
}

/** 
 * Assignment operator copies only the value and flags from the
 * source.
 *
 * The name, description, category, and controller widget strings
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  
 */
iqrcommon::ClsStringParameter& 
iqrcommon::ClsStringParameter::operator= (const ClsStringParameter &_source)
{    
    if (this == &_source) {	
	return *this;	
    }
    
    // Copy the value.
    strValue = _source.getValue();
	
    // Copy the flags.
    bEditable   = _source.isEditable();
    bLong = _source.isLong();
    
    return *this;	
}

void 
iqrcommon::ClsStringParameter::setValue(string _strValue)
{
    strValue = _strValue;
    
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::setValue: "
	     << getName()
	     << " value set to "
	     << strValue
	     << endl;
    }
}   

void 
iqrcommon::ClsStringParameter::setValueFromString(string _strValue)
{
    strValue = _strValue;
    
    if (bDebugStringParameter) {
	cout << "ClsStringParameter::setValueFromString: "
	     << getName()
	     << " value set to "
	     << strValue
	     << endl;
    }
}   

/** 
 * Create a copy of the parameter.
 *
 * This function uses the copy constructor.
 *
 * @return Pointer to the new copy of the parameter.
 */
iqrcommon::ClsStringParameter* 
iqrcommon::ClsStringParameter::makeCopy() const
{    
    return new ClsStringParameter(*this);   
}
