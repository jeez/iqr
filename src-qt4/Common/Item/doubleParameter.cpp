/****************************************************************************
 ** $Filename: doubleParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 *****************************************************************************/

#include <cmath>
#include <iostream>
#include <sstream>

#include <Common/Helper/stringConversions.hpp>

#include "doubleParameter.hpp"

#ifdef DEBUG_CLSDOUBLEPARAMETER
static const bool bDebugDoubleParameter = true;
#else
static const bool bDebugDoubleParameter = false;
#endif

using namespace std;

iqrcommon::ClsDoubleParameter::ClsDoubleParameter(string _strName, 
						  string _strLabel,
						  string _strDescription,
						  string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel, 
		   _strDescription, 
		   _strCategory), 
      dValue(DEFAULT_VALUE()),
      dMinimum(DEFAULT_MINIMUM()),
      dMaximum(DEFAULT_MAXIMUM()),
      iPrecision(DEFAULT_PRECISION),
      strMinSetter(""),
      strMaxSetter("")
{
    if (bDebugDoubleParameter) {
	cout << "ClsDoubleParameter::ClsDoubleParameter: "
	     << getName()
	     << "has default value" 
	     << DEFAULT_VALUE()
	     << endl;
    }    
}

iqrcommon::ClsDoubleParameter::ClsDoubleParameter(string _strName, 
						  string _strLabel,
						  double _dValue, 
						  double _dMinimum, 
						  double _dMaximum, 
						  int _iPrecision, 
						  string _strDescription,
						  string _strCategory)
    : ClsParameter(_strName, 
		   _strLabel, 
		   _strDescription, 
		   _strCategory), 
      dValue(_dValue),
      dMinimum(_dMinimum),
      dMaximum(_dMaximum),
      iPrecision(_iPrecision),
      strMinSetter(""),
      strMaxSetter("")
{    
    if (bDebugDoubleParameter) {
	cout << "ClsDoubleParameter::ClsDoubleParameter: "
	     << getName()
	     << ", value is " 
	     << dValue
	     << endl;
    }    
}


iqrcommon::ClsDoubleParameter::ClsDoubleParameter(const ClsDoubleParameter &_source)
    : ClsParameter(_source.getName(),
		   _source.getLabel(),
		   _source.getDescription(),
		   _source.getCategory(),
 		   _source.getEnabler()),
      dValue(_source.getValue()),
      dMinimum(_source.getMinimum()),
      dMaximum(_source.getMaximum()),
      iPrecision(_source.getPrecision()),
      strMinSetter(_source.getMinSetter()),
      strMaxSetter(_source.getMaxSetter())
{
    if (bDebugDoubleParameter) {
	cout << "ClsDoubleParameter::ClsDoubleParameter: copied " 
	     << _source.getName()
	     << endl;
    }    
}

iqrcommon::ClsDoubleParameter::~ClsDoubleParameter()
{    
    if (bDebugDoubleParameter) {
	cout << "ClsDoubleParameter::~ClsDoubleParameter: "
 	     << getName()
	     << endl;
    }    
}

/** 
 * Assignment operator copies only the value, range and precision from
 * the source.
 *
 * The name, description, category, and controller widget strings
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  
 */
iqrcommon::ClsDoubleParameter& 
iqrcommon::ClsDoubleParameter::operator= (const ClsDoubleParameter &_source)
{    
    if (this == &_source) {
	return *this;
    }
    
    // Copy the value, range and precision only.
    dValue     = _source.getValue();
    dMinimum   = _source.getMinimum();
    dMaximum   = _source.getMaximum();
    iPrecision = _source.getPrecision();
    
    return *this;    
}

string 
iqrcommon::ClsDoubleParameter::getValueAsString() const 
{    
//     ostringstream buffer;
//     // @@@ 17/11/01 Is it possible to control the precision here?
//     buffer << dValue;   
//     return buffer.str();
    return double2string(dValue);
}

// There is no check for iMinimum <= iValue <= iMaximum.
void
iqrcommon::ClsDoubleParameter::setValue(double _dValue) 
{   
    dValue = _dValue;  
}

void 
iqrcommon::ClsDoubleParameter::setValueFromString(string _strValue) 
{    
//     istringstream buffer(_strValue);
    
//     // NOTE: This operation does not test whether the string contains
//     // a valid double.
//     buffer >> dValue;
    dValue = string2double(_strValue);
}

/** 
 * Set the min setter of a parameter.
 *
 * The min setter field is used during construction of a widget.  The
 * auto widget class searches for the min setter parameter (widget)
 * and connects this parameter's widget's minimum value to the min
 * setter's value.  The min setter must be a double-type parameter.
 *
 * @param *_pMinSetter Pointer to the min setter parameter.  If the
 * pointer is 0, this function does nothing.
 */
void 
iqrcommon::ClsDoubleParameter::setMinSetter(ClsDoubleParameter *_pMinSetter) 
{
    if (_pMinSetter != 0) {
	strMinSetter = _pMinSetter->getName();
    }
}

/** 
 * Set the max setter of a parameter.
 *
 * The max setter field is used during construction of a widget.  The
 * auto widget class searches for the max setter parameter (widget)
 * and connects this parameter's widget's maximum value to the max
 * setter's value.  The max setter must be a double-type parameter.
 *
 * @param *_pMaxSetter Pointer to the max setter parameter.  If the
 * pointer is 0, this function does nothing.
 */
void 
iqrcommon::ClsDoubleParameter::setMaxSetter(ClsDoubleParameter *_pMaxSetter)
{
    if (_pMaxSetter != 0) {
	strMaxSetter = _pMaxSetter->getName();
    }
}

/** 
 * Make a deep copy of objects of this class.
 *
 * @return A new double parameter object with the same attributes as
 * *this.
 */
iqrcommon::ClsDoubleParameter* 
iqrcommon::ClsDoubleParameter::makeCopy() const 
{
    return new ClsDoubleParameter(*this);
}
