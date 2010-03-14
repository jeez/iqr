/****************************************************************************
 ** $Filename: autoGroup.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard, Ulysses Bernardet
 ** $Author: Mark Blanchard, Ulysses Bernardet
 ** 
 ** $CreateDate$
 ** $Date$
 **
 ** $Keywords:
 ** $Description: this code is modified from Ulysses' original 
 **               autodialog source.
 **
 **               Primarily, it has been Qt-ized, and the naming
 **               standardized with the other IQR FE sources.
 **
 **               For modal dialogs, supply a parent widget; dialog
 **               will appear above parent.  For modeless dialogs,
 **               don't supply a parent widget.
 **
 **               This is a modified version of Mark's autoDialog class
 **
 *****************************************************************************/

#include <iostream>

#include <qlayout.h>

#include "autoGroup.hpp"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
#include "memberWidget.h"

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace iqrcommon;

/*
iqrfe::ClsAutoGroup::ClsAutoGroup(ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : QGroupBox(_pqwgtParent, _pcName), item(_item), strName(_pcName)
*/

iqrfe::ClsAutoGroup::ClsAutoGroup(ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : ClsBaseAutoGroup(_item, _pqwgtParent, _pcName)


{
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroup::ClsAutoGroup" << endl;
    }

//    cout << "TYPE: " << item.Type() << endl;
    string strTitle = _pcName;
    strTitle.append(" : ");
    strTitle.append(strValue);
    
    setTitle(strTitle.c_str());
    

    // Create top-level layout manager for dialog.
    QVBoxLayout *pqlayTop = new QVBoxLayout(this, 14, -1, "auto dialog layout");

    // Construct an autowidget based on the item's parameter
    // list.
    pAutoWidget = new ClsAutoWidget(_item, this, QCString(_pcName)+" widget");
    pqlayTop->addWidget(pAutoWidget, WIDGET_V_STRETCH);
    
    /////////////////
    // Create buttons
    // Buttons depend on whether the dialog is modal or modeless.
    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));
    QWidget *pqwgtButtons;

    if (_bModal) {
	pqwgtButtons = new ClsModalButtons(this, QCString(_pcName)+" buttons");
	
	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons(this, QCString(_pcName)+" buttons");
	
	// @@@ 21/05/01 There might be problems with the state of the
	// apply button.  It should always be disabled when the dialog
	// is opened.
	connect(pAutoWidget, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));
	
	// How should the Undo button be handled?
	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }

    pqlayTop->addWidget(pqwgtButtons, BUTTON_V_STRETCH);
}

iqrfe::ClsAutoGroup::~ClsAutoGroup()
{
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroup::~ClsAutoGroup" << endl;
    }
}


// void iqrfe::ClsAutoGroup::apply() {
//     if (bDebugAutoGroup) {
// 	cout << "ClsAutoGroup::apply" << endl;
//     }
//     // For each parameter widget in autowidget:
//     //     if value has changed, set parameter value in item.
    
//     ParameterWidgetMap::const_iterator itWidget;
//     for (itWidget = pAutoWidget->getParameterWidgets().begin();
// 	 itWidget != pAutoWidget->getParameterWidgets().end();
// 	 ++itWidget) {

// 	ClsParameterWidget *pWidget = itWidget->second;

// 	if (pWidget->valueHasChanged()) {
// 	    if (bDebugAutoGroup) {
// 		cout << "ClsAutoGroup::apply:"
// 		     << " value in parameter widget "
// 		     << pWidget->getName()
// 		     << " changed to "
// 		     << pWidget->getValueAsString()
// 		     << endl;
// 	    }
// 	    // Get parameter object from item.
// 	    ClsParameter *pParameter = item.getParameter(pWidget->getName());

// 	    if (pParameter != 0) {
// 		if (!isModal()) {
// 		    // @@@ 10/01/02 Put the old value of the parameter
// 		    // into an undo list.  Managing undo in the dialog
// 		    // allows parameters from multiple items to be
// 		    // managed easily.  Undo should only be provided
// 		    // in modeless dialogs, where should the undo
// 		    // information be stored?
		
// 		    // Create undo object.
// 		    // Add name of parameter and old value as a string.
// 		    // Put object into undo buffer.
// 		}
		
// 		// The value in this widget has changed, pass it along
// 		// to the item (and mark the change as applied in the
// 		// parameter widget).
// 		pWidget->apply(pParameter);
// 	    }
// 	}
//     }
// }


void iqrfe::ClsAutoGroup::closeEvent ( QCloseEvent *  ) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroup::closeEvent ( QCloseEvent * e )" << endl;
    }
    emit autoGroupClosed(strName, "", ClsMemberWidget::HIDE);
    
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
