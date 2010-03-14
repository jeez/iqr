/****************************************************************************
 ** $Filename: autoGroupPatternTuples.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jun  6 16:18:12 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>



#include "autoGroupPatternTuples.hpp"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
#include "memberWidget.h"

#include "boolParameter.hpp"
#include "doubleParameter.hpp"
#include "intParameter.hpp"
#include "optionsParameter.hpp"
#include "stringParameter.hpp"

#include "ClsFESystemManager.h"
#include "ClsFEConnection.h"
#include "ClsFEGroup.h"
#include "ClsPatterns.h"
#include "ClsPatternSelectorTuples.h"

//#define DEBUG_CLSAUTODIALOG

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace iqrcommon;

/*
iqrfe::ClsAutoGroupPatternTuples::ClsAutoGroupPatternTuples(ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : QGroupBox(_pqwgtParent, _pcName), item(_item), strName(_pcName)
*/


iqrfe::ClsAutoGroupPatternTuples::ClsAutoGroupPatternTuples(ClsItem& _itemConnection, ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : ClsBaseAutoGroup(_item, _pqwgtParent, _pcName), itemConnection(_itemConnection)

{
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternTuples::ClsAutoGroupPatternTuples" << endl;
    }


    clsPatternTuples = static_cast<ClsPatternTuples*>(&_item);

    clsPatternSelector = NULL;

    string strTitle = _pcName;
    strTitle.append(" : ");
    strTitle.append(strValue);
    
    setTitle(strTitle.c_str());

//    setBackgroundColor(red);     
    
    // Create top-level layout manager for dialog.
    QVBoxLayout *pqlayTop = new QVBoxLayout(this, 14, -1, "auto dialog layout");

    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));

//    ClsPatternForeach &tt = static_cast<ClsPatternForeach&>(_item);
//    cout << "getTest: " << tt.getTest() << endl;


    string strSourceGroupID = static_cast<ClsFEConnection*>(&itemConnection)->getConnectionSourceID();
    string strTargetGroupID = static_cast<ClsFEConnection*>(&itemConnection)->getConnectionTargetID();
    
    ClsBaseTopology *clsBaseTopologySource = ClsFESystemManager::Instance()->getFEGroup( strSourceGroupID )->getTopology();
    ClsBaseTopology *clsBaseTopologyTarget = ClsFESystemManager::Instance()->getFEGroup( strTargetGroupID )->getTopology();

    clsPatternSelector = new ClsPatternSelectorTuples(this, this, clsBaseTopologySource, clsBaseTopologyTarget, "source");    
    pqlayTop->addWidget(clsPatternSelector);
    


    setTuples();
    

    if (_bModal) {
	pqwgtButtons = new ClsModalButtons(this, QCString(_pcName)+" buttons");
	
	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons(this, QCString(_pcName)+" buttons");

//	connect(this, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
	connect(this, SIGNAL(sigSubItemChanged()), pqwgtButtons, SLOT(enableApply()));

	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));
	
	// How should the Undo button be handled?
	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }

    pqlayTop->addWidget(pqwgtButtons, BUTTON_V_STRETCH);

//******    connect(clsPatternSelector, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply())); //****

}

iqrfe::ClsAutoGroupPatternTuples::~ClsAutoGroupPatternTuples() {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternTuples::~ClsAutoGroupPatternTuples" << endl;
    }
}

void iqrfe::ClsAutoGroupPatternTuples::setTuples() {
//    cout << "ClsAutoGroupPatternTuples::setTuples()" << endl;
    

    ClsTuples* clsTuples = clsPatternTuples->getTuples();
    
//    cout << "clsTuples is NULL: " << (clsTuples == NULL ? "yes" : "no" ) << endl;
    

    if(clsTuples!=NULL){
//	clsTuples->print();
	
	clsPatternSelector->setSubPopulation(clsTuples);
    }
    
    

}

void iqrfe::ClsAutoGroupPatternTuples::apply() {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternTuples::apply" << endl;
    }

    clsPatternTuples->setTuples( clsPatternSelector->getSubPopulation());
    emit changed();    
}


void iqrfe::ClsAutoGroupPatternTuples::closeEvent ( QCloseEvent *  ) {
//    cout << "ClsAutoGroupPatternTuples::closeEvent ( QCloseEvent * e )" << endl;
    emit autoGroupClosed(strName, "", ClsMemberWidget::HIDE);
    
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
