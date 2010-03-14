/****************************************************************************
 ** $Filename: colorParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#include <iostream>

#include "colorParameter.hpp"

#ifdef DEBUG_CLSCOLORPARAMETER
static const bool bDebugColorParameter = true;
#else
static const bool bDebugColorParameter = false;
#endif

using namespace std;

iqrcommon::ClsColorParameter::ClsColorParameter(string _strName, 
					      string _strLabel, 
					      string _strDescription, 
					      string _strCategory)
    : ClsParameter(_strName,
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      strValue(""),
      bEditable(true)
{
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::ClsColorParameter: "
	     << getName()
	     << " is a short editable parameter, is empty" 
	     << endl;
    }
}

iqrcommon::ClsColorParameter::ClsColorParameter(string _strName, 
					      string _strLabel, 
					      string _strValue, 
					      bool _bEditable, 
					      string _strDescription, 
					      string _strCategory)
    : ClsParameter(_strName,
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      strValue(_strValue),
      bEditable(_bEditable)
{
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::ClsColorParameter: "
	     << getName()
	     << " is "
	     << " and "
	     << (bEditable ? "editable" : "constant")
	     << " with value " 
	     << strValue
	     << endl;
    }
}

iqrcommon::ClsColorParameter::ClsColorParameter(const ClsColorParameter &_source)
    : ClsParameter(_source.getName(),
		   _source.getLabel(),
		   _source.getDescription(),
		   _source.getCategory()),
      strValue(_source.getValue()),
      bEditable(_source.isEditable())
{
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::ClsColorParameter: copied " 
	     << _source.getName()
	     << endl;
    }
}

iqrcommon::ClsColorParameter::~ClsColorParameter()
{    
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::~ClsColorParameter: "
 	     << getName()
	     << endl;
    }    
}

/** 
 * Assignment operator copies only the value and flags from the
 * source.
 *
 * The name, description, category, and controller widget colors
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  
 */
iqrcommon::ClsColorParameter& 
iqrcommon::ClsColorParameter::operator= (const ClsColorParameter &_source) {    
    if (this == &_source) {	
	return *this;	
    }
    
    // Copy the value.
    strValue = _source.getValue();
	
    // Copy the flags.
    bEditable   = _source.isEditable();
    
    return *this;	
}

void iqrcommon::ClsColorParameter::setValue(string _strValue) {
    strValue = _strValue;
    
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::setValue: "
	     << getName()
	     << " value set to "
	     << strValue
	     << endl;
    }
}   

void iqrcommon::ClsColorParameter::setValueFromString(string _strValue) {
    strValue = _strValue;
    
    if (bDebugColorParameter) {
	cout << "ClsColorParameter::setValueFromColor: "
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
iqrcommon::ClsColorParameter* 
iqrcommon::ClsColorParameter::makeCopy() const {    
    return new ClsColorParameter(*this);   
}
