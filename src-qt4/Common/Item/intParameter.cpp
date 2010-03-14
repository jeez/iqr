/****************************************************************************
 ** $Filename: intParameter.cpp
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: implementation of integer parameter class.
 **
 *****************************************************************************/

#include <iostream>
#include <sstream>

#include <Common/Helper/stringConversions.hpp>

#include "intParameter.hpp"

#ifdef DEBUG_CLSINTPARAMETER
static const bool bDebugIntParameter = true;
#else
static const bool bDebugIntParameter = false;
#endif

using namespace std;

iqrcommon::ClsIntParameter::ClsIntParameter(string _strName, 
				      string _strLabel,
				      string _strCategory, 
				      string _strDescription) 
    : ClsParameter(_strName, 
		   _strLabel, 
		   _strCategory, 
		   _strDescription),
      iValue(DEFAULT_VALUE),
      iMinimum(DEFAULT_MINIMUM),
      iMaximum(DEFAULT_MAXIMUM),
      strMinSetter(""),
      strMaxSetter("")
{
}

iqrcommon::ClsIntParameter::ClsIntParameter(string _strName, 
				      string _strLabel,
				      int _iValue,
				      int _iMinimum,
				      int _iMaximum, 
				      string _strCategory, 
				      string _strDescription) 
    : ClsParameter(_strName, 
		   _strLabel,
		   _strCategory, 
		   _strDescription),
      iValue(_iValue),
      iMinimum(_iMinimum),
      iMaximum(_iMaximum),
      strMinSetter(""),
      strMaxSetter("")
{
}

iqrcommon::ClsIntParameter::ClsIntParameter(const ClsIntParameter &_source) 
    : ClsParameter(_source.getName(), 
		   _source.getLabel(),
		   _source.getCategory(),
		   _source.getDescription()),
      iValue(_source.getValue()),
      iMinimum(_source.getMinimum()),
      iMaximum(_source.getMaximum()),
      strMinSetter(_source.getMinSetter()),
      strMaxSetter(_source.getMaxSetter())
{
}

iqrcommon::ClsIntParameter::~ClsIntParameter()
{
    if (bDebugIntParameter) {
	cout << "ClsIntParameter::~ClsIntParameter"
	     << endl;
    }
}

/** 
 * Assignment operator copies only the value and range from the
 * source.
 *
 * The name, description, category, and controller widget strings
 * remain unchanged.
 *
 * @return Reference to this parameter.
 * @param &_source Source parameter from which to copy the value.  
 */
iqrcommon::ClsIntParameter&
iqrcommon::ClsIntParameter::operator= (const ClsIntParameter &_source)
{
    if (this == &_source) {
	return *this;
    }
    
    // Copy the value and range only.
    iValue   = _source.getValue();
    iMinimum = _source.getMinimum();
    iMaximum = _source.getMaximum();
    
    return *this;    
}

string 
iqrcommon::ClsIntParameter::getValueAsString() const 
{    
//     ostringstream buffer;
//     buffer << iValue;
//     return buffer.str();    
    return int2string(iValue);
}

// @@@ 11/12/01 There is no check here for minimum <= value <= maximum.
void
iqrcommon::ClsIntParameter::setValue(int _iValue) 
{
    iValue = _iValue;  
}
    
void
iqrcommon::ClsIntParameter::setValueFromString(string _strValue) 
{
//     istringstream buffer(_strValue);

//     // NOTE: This operation does not test whether the string contains
//     // a valid integer.
//     buffer >> iValue;
    iValue = string2int(_strValue);
}

void 
iqrcommon::ClsIntParameter::setRange(int _iMinimum, int _iMaximum) 
{
    if (iMinimum < iMaximum) {
	iMinimum = _iMinimum;
	iMaximum = _iMaximum;
    }
    if (iValue < iMinimum) {
	iValue = iMinimum;
    } else if (iValue > iMaximum) {
	iValue = iMaximum;
    }
}

/** 
 * Set the min setter of a parameter.
 *
 * The min setter field is used during construction of a widget.  The
 * auto widget class searches for the min setter parameter (widget)
 * and connects this parameter's widget's minimum value to the min
 * setter's value.  The min setter must be an int-type parameter.
 *
 * @param *_pMinSetter Pointer to the min setter parameter.  If the
 * pointer is 0, this function does nothing.
 */
void 
iqrcommon::ClsIntParameter::setMinSetter(ClsIntParameter *_pMinSetter) 
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
 * setter's value.  The max setter must be an int-type parameter.
 *
 * @param *_pMaxSetter Pointer to the max setter parameter.  If the
 * pointer is 0, this function does nothing.
 */
void 
iqrcommon::ClsIntParameter::setMaxSetter(ClsIntParameter *_pMaxSetter)
{
    if (_pMaxSetter != 0) {
	strMaxSetter = _pMaxSetter->getName();
    }
}

/** 
 * Make a deep copy of objects of this class.
 *
 * @return A new int parameter object with the same attributes as
 * *this.
 */
iqrcommon::ClsIntParameter* 
iqrcommon::ClsIntParameter::makeCopy() const 
{
    return new ClsIntParameter(*this);   
}
