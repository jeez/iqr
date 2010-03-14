/****************************************************************************
 ** $Filename: pathParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#include <iostream>

#include "pathParameter.hpp"

#ifdef DEBUG_CLSPATHPARAMETER
static const bool bDebugPathParameter = true;
#else
static const bool bDebugPathParameter = false;
#endif

using namespace std;

iqrcommon::ClsPathParameter::ClsPathParameter(string _strName, 
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
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::ClsPathParameter: "
	     << getName()
	     << " is a short editable parameter, is empty" 
	     << endl;
    }
}

iqrcommon::ClsPathParameter::ClsPathParameter(string _strName, 
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
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::ClsPathParameter: "
	     << getName()
	     << " is "
	     << " and "
	     << (bEditable ? "editable" : "constant")
	     << " with value " 
	     << strValue
	     << endl;
    }
}

iqrcommon::ClsPathParameter::ClsPathParameter(const ClsPathParameter &_source)
    : ClsParameter(_source.getName(),
		   _source.getLabel(),
		   _source.getDescription(),
		   _source.getCategory()),
      strValue(_source.getValue()),
      bEditable(_source.isEditable())
{
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::ClsPathParameter: copied " 
	     << _source.getName()
	     << endl;
    }
}

iqrcommon::ClsPathParameter::~ClsPathParameter()
{    
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::~ClsPathParameter: "
 	     << getName()
	     << endl;
    }    
}

/** 
 * Assignment operator copies only the value and flags from the
 * source.
 *
 * The name, description, category, and controller widget paths
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  
 */
iqrcommon::ClsPathParameter& 
iqrcommon::ClsPathParameter::operator= (const ClsPathParameter &_source) {    
    if (this == &_source) {	
	return *this;	
    }
    
    // Copy the value.
    strValue = _source.getValue();
	
    // Copy the flags.
    bEditable   = _source.isEditable();
    
    return *this;	
}

void iqrcommon::ClsPathParameter::setValue(string _strValue) {
    strValue = _strValue;
    
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::setValue: "
	     << getName()
	     << " value set to "
	     << strValue
	     << endl;
    }
}   

void iqrcommon::ClsPathParameter::setValueFromString(string _strValue) {
    strValue = _strValue;
    
    if (bDebugPathParameter) {
	cout << "ClsPathParameter::setValueFromPath: "
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
iqrcommon::ClsPathParameter* 
iqrcommon::ClsPathParameter::makeCopy() const {    
    return new ClsPathParameter(*this);   
}
