/****************************************************************************
 ** $Filename: autoGroup.cpp
 ** $Header$
 **
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

#include <QtGui/qlayout.h>

#include "autoGroup.hpp"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
#include "memberWidget.h"

//#define DEBUG_CLSAUTODIALOG

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace iqrcommon;

iqrfe::ClsAutoGroup::ClsAutoGroup(ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : ClsBaseAutoGroup(_item, _pqwgtParent, _pcName) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroup::ClsAutoGroup" << endl;
    }

//    cout << "TYPE: " << item.Type() << endl;
    string strTitle = _pcName;
    strTitle.append(" : ");
    strTitle.append(strValue);
    setTitle(strTitle.c_str());
    

    // Create top-level layout manager for dialog.
    QVBoxLayout *pqlayTop = new QVBoxLayout(this /*, 14, -1, "auto dialog layout"*/);
    pqlayTop->setContentsMargins (0, 0, 0, 0);
    pqlayTop->setSpacing(0);
    pqlayTop->setMargin(0);


    // Construct an autowidget based on the item's parameter
    // list.
    pAutoWidget = new ClsAutoWidget(_item, this /*, QString(_pcName)+" widget"*/);
    pqlayTop->addWidget(pAutoWidget /*, WIDGET_V_STRETCH*/);
    
    /////////////////
    // Create buttons
    // Buttons depend on whether the dialog is modal or modeless.
    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));
    QWidget *pqwgtButtons;

    if (_bModal) {
	pqwgtButtons = new ClsModalButtons(this /*, QString(_pcName)+" buttons"*/);
	
	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons(this /*, QString(_pcName)+" buttons"*/);
	
	connect(pAutoWidget, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));
	
	// How should the Undo button be handled?
	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }
    pqlayTop->addWidget(pqwgtButtons, BUTTON_V_STRETCH);
}

iqrfe::ClsAutoGroup::~ClsAutoGroup() {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroup::~ClsAutoGroup" << endl;
    }
}


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
