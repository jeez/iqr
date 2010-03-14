/****************************************************************************
 ** $Filename: parameter.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Aug 14 20:50:04 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: implementation of base parameter class.
 **
 *****************************************************************************/

#include "parameter.hpp"
#include "boolParameter.hpp"

#ifdef DEBUG_CLSPARAMETER
static const bool bDebugParameter = true;
#else
static const bool bDebugParameter = false;
#endif

iqrcommon::ClsParameter::ClsParameter(string _strName, 
				      string _strLabel,
				      string _strDescription,
				      string _strCategory, 
				      string _strEnabler) 
    : strName(_strName),
      strLabel(_strLabel),
      strDescription(_strDescription),
      strCategory(_strCategory),
      strEnabler(_strEnabler)
{
}

/** 
 * Set the enabler of a parameter.
 *
 * The enabler field is used during construction of a widget.  The
 * auto widget class searches for the enabler parameter (widget) and
 * connects this parameter's widget's enabled state to the enabler.
 * The enabler must be a bool-type parameter.
 *
 * @param *_pEnabler Pointer to the enabler parameter.  If the pointer
 * is 0, this function does nothing.
 */
void
iqrcommon::ClsParameter::setEnabler(ClsBoolParameter *_pEnabler)
{
    if (_pEnabler != 0) {
	strEnabler = _pEnabler->getName();
    }
}


// @@@ 14/08/01 More functions will follow when the comm handler
// interface is implemented.
