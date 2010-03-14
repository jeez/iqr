/****************************************************************************
 ** $Filename: widgetFactory.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sat Dec 29 14:47:07 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>

#include "widgetFactory.hpp"

#include "boolParameter.hpp"
#include "boolWidget.hpp"

#include "doubleParameter.hpp"
#include "doubleWidget.hpp"

#include "intParameter.hpp"
#include "intWidget.hpp"

#include "optionsParameter.hpp"
#include "optionsWidget.hpp"

//#include "subtypeWidget.hpp"


#include "stringParameter.hpp"
#include "labelWidget.hpp"
#include "stringWidget.hpp"
#include "longStringWidget.hpp"

using namespace iqrcommon;
using iqrfe::ClsParameterWidget;

/** 
 * Get a parameter widget for the supplied parameter object.
 *
 * This function uses dynamic_cast to identify the type of the
 * parameter.  This is inefficient, but removes the need for the
 * parameter objects to be able to build widgets for themselves.  This
 * insulates the compute engine from the widget set libraries.
 *
 * WARNING: if the type of the parameter cannot be identified, or a 0
 * pointer is received, this function returns 0.  The caller must
 * check the return value.
 *
 * Management of the new widget passes to the caller, which must
 * delete the widget.
 *
 * @return Pointer to the new parameter widget.
 * @param *_pParameter Pointer to the parameter object.
 * @param *_pqwgtParent Pointer to the parent of the new widget.
 */
iqrfe::ClsParameterWidget*
iqrfe::ClsWidgetFactory::createWidget(ClsParameter *_pParameter, 
				      QWidget *_pqwgtParent)
{
    if (_pParameter == 0) {
	// ERROR: No parameter object was supplied.
	return 0;
    }

    ClsParameterWidget *pWidget = 0;

    // dynamic_cast is safe here because failure returns just a 0
    // pointer.  If references to the parameter objects are used
    // instead, a catch block for bad_cast must be included.
    ClsBoolParameter *pBool = dynamic_cast<ClsBoolParameter*>(_pParameter);
    if (pBool != 0) {
	// The parameter is a boolean parameter.
	pWidget = new ClsBoolWidget(*pBool,
				    _pqwgtParent,
				    pBool->getName().c_str());
    }
    
    ClsDoubleParameter *pDouble = dynamic_cast<ClsDoubleParameter*>(_pParameter);
    if (pDouble != 0) {
	// The parameter is a double parameter.
	pWidget = new ClsDoubleWidget(*pDouble,
				      _pqwgtParent,
				      pDouble->getName().c_str());
    }
    
    ClsIntParameter *pInt = dynamic_cast<ClsIntParameter*>(_pParameter);
    if (pInt != 0) {
	// The parameter is an integer parameter.
	pWidget = new ClsIntWidget(*pInt,
				   _pqwgtParent,
				   pInt->getName().c_str());
    }
    
    ClsOptionsParameter *pOpt = dynamic_cast<ClsOptionsParameter*>(_pParameter);
    if (pOpt != 0) {
	// The parameter is an options parameter.

	pWidget = new ClsOptionsWidget(*pOpt,
				       _pqwgtParent,
				       pOpt->getName().c_str());
/*
	pWidget = new ClsSubtypeWidget(*pOpt,
				       _pqwgtParent,
				       pOpt->getName().c_str());
*/


    }
    
    ClsStringParameter *pString = dynamic_cast<ClsStringParameter*>(_pParameter);
    if (pString != 0) {
	// The parameter is a string parameter.
	if (pString->isEditable()) {
	    if (pString->isLong()) {
		// Parameter is a long editable string, use a
		// multiline edit widget.
		pWidget = new ClsLongStringWidget(*pString,
						  _pqwgtParent,
						  pString->getName().c_str());
	    } else {
		// Parameter is a short editable string, use a single
		// line edit widget.
		pWidget = new ClsStringWidget(*pString,
					      _pqwgtParent,
					      pString->getName().c_str());
	    }
	} else {
	    // Parameter is a constant string, use a label widget.
	    pWidget = new ClsLabelWidget(*pString,
					 _pqwgtParent,
					 pString->getName().c_str());
	}
    }
    
    // File parameter.

    // Group parameter.

    // Return the widget pointer, or 0 if the parameter widget was not
    // identified.
    return pWidget;
}
