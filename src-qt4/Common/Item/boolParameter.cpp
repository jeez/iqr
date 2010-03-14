/****************************************************************************
 ** $Filename: boolParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#include <iostream>

#include <Common/Helper/stringConversions.hpp>

#include "boolParameter.hpp"

#ifdef DEBUG_CLSBOOLPARAMETER
static const bool bDebugBoolParameter = true;
#else
static const bool bDebugBoolParameter = false;
#endif

using namespace std;

iqrcommon::ClsBoolParameter::ClsBoolParameter(string _strName, 
					string _strLabel, 
					string _strDescription, 
					string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      bValue(DEFAULT_VALUE)
{
    if (bDebugBoolParameter) {
	cout << "ClsBoolParameter::ClsBoolParameter: "
	     << getName()
	     << " has default value " 
	     << (DEFAULT_VALUE ? "true" : "false")
	     << endl;
    }
}

iqrcommon::ClsBoolParameter::ClsBoolParameter(string _strName, 
					      string _strLabel, 
					      bool   _bValue, 
					      string _strDescription, 
					      string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel, 
		   _strDescription,
		   _strCategory),
      bValue(_bValue)
{
    if (bDebugBoolParameter) {
	cout << "ClsBoolParameter::ClsBoolParameter: "
	     << getName()
	     << ", value is " 
	     << (bValue ? "true" : "false")
	     << endl;
    }
}

iqrcommon::ClsBoolParameter::ClsBoolParameter(const ClsBoolParameter &_source)
    : ClsParameter(_source.getName(), 
		   _source.getLabel(), 
		   _source.getDescription(), 
		   _source.getCategory()),
      bValue(_source.getValue())
{
    if (bDebugBoolParameter) {
	cout << "ClsBoolParameter::ClsBoolParameter: copied " 
	     << _source.getName()
	     << endl;
    }
}

iqrcommon::ClsBoolParameter::~ClsBoolParameter()
{
    if (bDebugBoolParameter) {
	cout << "ClsBoolParameter::~ClsBoolParameter: "
 	     << getName()
	     << endl;
    }
}

/** 
 * Assignment operator copies only the value from the source.
 *
 * The name, description, category, and controller widget strings
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.
 */
iqrcommon::ClsBoolParameter& 
iqrcommon::ClsBoolParameter::operator= (const ClsBoolParameter &_source)
{    
    if (this == &_source) {
	return *this;
    }
    
    // Copy the value only.
    bValue = _source.getValue();

    return *this;
}

/** 
 * Return the value of the parameter as a string.
 *
 * This is needed for saving and printing operations.
 *
 * @return "true" if the state of the parameter is true, otherwise
 * false.
 */
string 
iqrcommon::ClsBoolParameter::getValueAsString() const 
{
    return bool2string(bValue);
}

void 
iqrcommon::ClsBoolParameter::setValue(bool _bValue) 
{
    bValue = _bValue;
}

void 
iqrcommon::ClsBoolParameter::setValueFromString(string _strValue) 
{
    bValue = string2bool(_strValue);
}

/** 
 * Create a copy of the parameter.
 *
 * This function uses the copy constructor.
 *
 * @return Pointer to the new copy of the parameter.
 */
iqrcommon::ClsBoolParameter* 
iqrcommon::ClsBoolParameter::makeCopy() const
{
    return new ClsBoolParameter(*this);
}

