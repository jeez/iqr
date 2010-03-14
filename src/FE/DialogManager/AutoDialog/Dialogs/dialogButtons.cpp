/****************************************************************************
 ** $Filename: dialogButtons.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Jun 19 15:33:03 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>

#include <qlayout.h>

#include "dialogButtons.hpp"

//#define DEBUG_DIALOGBUTTONS

#ifdef DEBUG_DIALOGBUTTONS
static const bool bDebugDialogButtons = true;
#else
static const bool bDebugDialogButtons = false;
#endif

// @@@ 19/06/01 Font size should be taken into account here.
static const int BUTTON_BORDER    = 0; //15
static const int BUTTON_SPACE     = 5; //15
static const int BUTTON_H_STRETCH = 10;
static const int BUTTON_V_STRETCH = 0;
static const int NO_STRETCH       = 0;

using namespace std;

iqrfe::ClsModalButtons::ClsModalButtons(QWidget *_pqwgtParent, 
				      const char *_pcName)
    : QWidget(_pqwgtParent, _pcName)
{
    if (bDebugDialogButtons) {
	cout << "ClsModalButtons::ClsModalButtons"
	     << endl;
    }

    QHBoxLayout *pqlayButtons = new QHBoxLayout(this, 
						BUTTON_BORDER, 
						BUTTON_SPACE);
    pqlayButtons->addStretch(BUTTON_H_STRETCH);

    // Modal dialogs need OK and cancel.  OK should apply changes,
    // cancel does not.
    pqpbOK = new QPushButton("OK", this, "OK button");
    pqlayButtons->addWidget(pqpbOK, NO_STRETCH);
    connect(pqpbOK, SIGNAL(clicked()), this, SIGNAL(ok()));
    pqpbOK->setFocus();
    
//     pqlayButtons->addSpace(5);
    
    pqpbCancel = new QPushButton("Cancel", this, "Cancel button");
    pqlayButtons->addWidget(pqpbCancel, NO_STRETCH);
    connect(pqpbCancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
    

    pqlayButtons->addStretch(BUTTON_H_STRETCH);

    setFixedHeight(sizeHint().height());
}


iqrfe::ClsModelessButtons::ClsModelessButtons(QWidget *_pqwgtParent,
					    const char *_pcName)
    : QWidget(_pqwgtParent, _pcName)
{
    if (bDebugDialogButtons) {
	cout << "ClsModelessButtons::ClsModelessButtons"
	     << endl;
    }

    QHBoxLayout *pqlayButtons = new QHBoxLayout(this, 
						BUTTON_BORDER,
						BUTTON_SPACE);
     
    // Put buttons flush right.
    pqlayButtons->addStretch(BUTTON_H_STRETCH);
    
    // Modeless dialogs need apply, undo and close buttons.
    pqpbApply = new QPushButton("Apply", this, "Apply button");
    pqpbApply->setEnabled(false); // Nothing to apply yet.
    pqlayButtons->addWidget(pqpbApply);
//    connect(pqpbApply, SIGNAL(clicked()), this, SIGNAL(apply()));
    connect(pqpbApply, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(pqpbApply, SIGNAL(clicked()), this, SLOT(disableApply()));

//     pqlayButtons->addSpace(5);
    
    pqpbUndo = new QPushButton("Undo", this, "Undo button");
    // Only enable undo button when undo information is available.
    pqpbUndo->setEnabled(false);
    pqlayButtons->addWidget(pqpbUndo);
    connect(pqpbUndo, SIGNAL(clicked()), this, SIGNAL(undo()));
    pqpbUndo->hide();    


//     pqlayButtons->addSpace(5);
    
    pqpbClose = new QPushButton("Close", this, "Close button");
    pqlayButtons->addWidget(pqpbClose);
    connect(pqpbClose, SIGNAL(clicked()), this, SIGNAL(close()));
/* probably no good:    pqpbClose->setFocus(); */
    
    pqpbApply->setFocus(); 
    setFixedHeight(sizeHint().height());
}

/** 
 * Enable the apply button.
 */
void 
iqrfe::ClsModelessButtons::enableApply()
{
    pqpbApply->setEnabled(true);
}

/** 
 * Disable the apply button.
 */
void 
iqrfe::ClsModelessButtons::disableApply()
{
    pqpbApply->setEnabled(false);
    /* probably no good: pqpbClose->setFocus(); */
}


/** 
 * wrapper for emit apply.
 */
void iqrfe::ClsModelessButtons::applyPressed() {
    pqpbClose->setFocus();
    emit(apply());
}


/** 
 * Enable the undo button.
 */
void 
iqrfe::ClsModelessButtons::enableUndo()
{
    pqpbUndo->setEnabled(true);
}

/** 
 * Disable the undo button.
 */
void 
iqrfe::ClsModelessButtons::disableUndo()
{
    pqpbUndo->setEnabled(false);
}

/** 
 * Disable the apply button when the dialog is opened.
 *
 * @param *e Show event details received from Qt.
 */
void 
iqrfe::ClsModelessButtons::showEvent(QShowEvent *e)
{
    // Is this slot being called by the application or the window
    // manager?
    if (!e->spontaneous()) {
	// Dialog is being opened, disable the apply button.  This
	// assumes that the parameter values in the dialog are
	// up-to-date.
	disableApply();
    }
}
