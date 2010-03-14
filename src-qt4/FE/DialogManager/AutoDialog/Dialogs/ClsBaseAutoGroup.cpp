/****************************************************************************
 ** $Filename: ClsBaseAutoGroup.cpp
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 30 19:19:22 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>

#include <QtGui/qlayout.h>

#include "ClsBaseAutoGroup.h"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
//#include "memberWidget.h"

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace iqrcommon;

iqrfe::ClsBaseAutoGroup::ClsBaseAutoGroup(ClsItem &_item, QWidget *_pqwgtParent, const char *_pcName)
    : QGroupBox(_pqwgtParent), item(_item), strName(_pcName) 
{
    if (bDebugAutoGroup) {
	cout << "ClsBaseAutoGroup::ClsBaseAutoGroup" << endl;
    }

}

void iqrfe::ClsBaseAutoGroup::apply(){
//    if (bDebugAutoGroup) { 
	cout << "ClsBaseAutoGroup::apply()" << endl;
//    }
    
    if(pAutoWidget!=NULL){
	/* LOGGING STILL NEEDS TO BE IMPLEMENTED */
	ParameterWidgetMap::const_iterator itWidget;
	for (itWidget = pAutoWidget->getParameterWidgets().begin();
	     itWidget != pAutoWidget->getParameterWidgets().end();
	     ++itWidget) {

	    ClsParameterWidget *pWidget = itWidget->second;

	    if (pWidget->valueHasChanged()) {
		if (bDebugAutoGroup) {
		    cout << "ClsAutoGroup::apply:"
			 << " value in parameter widget "
			 << pWidget->getName()
			 << " changed to "
			 << pWidget->getValueAsString()
			 << endl;
		}


		bool bLogging = true;
		if(bLogging){
		    cout <<
			/* we need the parent information... */
			"type: " << item.Type() << "; " << 
//FIX			"ID: " << strID << "; " << 
			"parameter: " << pWidget->getName() << "; " << 
			"value: " << pWidget->getValueAsString() << endl;
		}




		// Get parameter object from item.
		ClsParameter *pParameter = item.getParameter(pWidget->getName());

		if (pParameter != 0) {
		    if (!isModal()) {
			// @@@ 10/01/02 Put the old value of the parameter
			// into an undo list.  Managing undo in the dialog
			// allows parameters from multiple items to be
			// managed easily.  Undo should only be provided
			// in modeless dialogs, where should the undo
			// information be stored?
		
			// Create undo object.
			// Add name of parameter and old value as a string.
			// Put object into undo buffer.
		    }
		
		    // The value in this widget has changed, pass it along
		    // to the item (and mark the change as applied in the
		    // parameter widget).
		    pWidget->apply(pParameter);
		}
	    }
	}
	
	list<ClsXRefHolderWidget*>::const_iterator itXRHW;
	for(itXRHW = pAutoWidget->getXRefHolderWidgets().begin();
	    itXRHW != pAutoWidget->getXRefHolderWidgets().end();
	    itXRHW++){
	    (*itXRHW)->apply();

	}

    }
    emit changed();    
};

void iqrfe::ClsBaseAutoGroup::subItemChanged(){
    if (bDebugAutoGroup) {
	cout << "ClsBaseAutoGroup::subItemChanged()" << endl;
    }
//    emit changed();
    emit sigSubItemChanged();
}
