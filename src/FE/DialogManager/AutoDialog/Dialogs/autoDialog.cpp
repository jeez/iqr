/****************************************************************************
 ** $Filename: autoDialog.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 **
 ** $CreateDate: Tue Feb  6 00:24:01 2001
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
 *****************************************************************************/

#include <iostream>

#include <qlayout.h>

#include "autoDialog.hpp"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoDialog = true;
#else
static const bool bDebugAutoDialog = false;
#endif

using namespace iqrcommon;

iqrfe::ClsAutoDialog::ClsAutoDialog(ClsItem &_item, int _iType, string _strID, bool _bModal, QWidget *_pqwgtParent, const char *_pcName)
    : ClsDialog(_pqwgtParent, _pcName, _iType, _strID, _bModal, Qt::WDestructiveClose), item(_item)
{
    if (bDebugAutoDialog) {
	cout << "ClsAutoDialog::ClsAutoDialog" << endl;
    }

    // Create top-level layout manager for dialog.
    QVBoxLayout *pqlayTop = new QVBoxLayout(this, DIALOG_BORDER, -1, "auto dialog layout");

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

iqrfe::ClsAutoDialog::~ClsAutoDialog()
{
    if (bDebugAutoDialog) {
	cout << "ClsAutoDialog::~ClsAutoDialog" << endl;
    }
}

void iqrfe::ClsAutoDialog::apply() {
    if (bDebugAutoDialog) {
	cout << "ClsAutoDialog::apply" << endl;
    }
    // For each parameter widget in autowidget:
    //     if value has changed, set parameter value in item.

    ParameterWidgetMap::const_iterator itWidget;
    for (itWidget = pAutoWidget->getParameterWidgets().begin();
	 itWidget != pAutoWidget->getParameterWidgets().end();
	 ++itWidget) {

	ClsParameterWidget *pWidget = itWidget->second;

	if (pWidget->valueHasChanged()) {
	    if (bDebugAutoDialog) {
		cout << "ClsAutoDialog::apply:"
		     << " value in parameter widget "
		     << pWidget->getName()
		     << " changed to "
		     << pWidget->getValueAsString()
		     << endl;
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
    emit sigApplied(getType(), getID());
}
