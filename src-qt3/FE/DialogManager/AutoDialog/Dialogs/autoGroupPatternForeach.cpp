/****************************************************************************
 ** $Filename: autoGroupPatternForeach.cpp
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



#include "autoGroupPatternForeach.hpp"
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
#include "ClsPatternSelectorList.h"
#include "ClsPatternSelectorRegion.h"
#include "ClsPatternSelectorAll.h"
#include "ClsBaseTopology.h"

#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace std;
using namespace iqrcommon;


iqrfe::ClsAutoGroupPatternForeach::ClsAutoGroupPatternForeach(ClsItem& _itemConnection, ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue, bool bShowList)
    : ClsBaseAutoGroup(_item, _pqwgtParent, _pcName), itemConnection(_itemConnection) {

    if (bDebugAutoGroup) {
    cout << "ClsAutoGroupPatternForeach::ClsAutoGroupPatternForeach(...)" << endl;
    }

    string strTitle = _pcName;
    strTitle.append(" : ");
    strTitle.append(strValue);

    setTitle(strTitle.c_str());

//    setBackgroundColor(red);

    QVBoxLayout *pqlayTop = new QVBoxLayout(this, 14, -1, "auto dialog layout");


    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));

    clsPatternForeach = static_cast<ClsPatternForeach*>(&_item);

    QHBoxLayout *pqlaySelectors = new QHBoxLayout(pqlayTop, 5, "selectors");
// SOURCE----------------------------------------------
    qgroupboxSource = new QGroupBox(this);
    qgroupboxSource->setTitle("Source");
    pqlaySelectors->addWidget(qgroupboxSource , WIDGET_V_STRETCH);

    QVBoxLayout *pqlaySourceGB = new QVBoxLayout(qgroupboxSource, 14, -1, "source gb layout");
    pqlaySourceGB->setAutoAdd (true);

    qcomboSourcePopulation = new QComboBox(false, qgroupboxSource, "combo box");
    connect(qcomboSourcePopulation, SIGNAL(activated(const QString &)), this, SLOT(sourceTypeChanged(const QString &)));
    qcomboSourcePopulation->insertItem(ClsTagLibrary::SelectorAll());
    qcomboSourcePopulation->insertItem(ClsTagLibrary::SelectorRegion());
    if(bShowList){
	qcomboSourcePopulation->insertItem(ClsTagLibrary::SelectorList());
    }


    qwidgetstackSourceSelectors = new QWidgetStack(qgroupboxSource);


    ClsBaseSubPopulation *clsSubPopulationSource = clsPatternForeach->getSourcePopulation();


    string strPopulationSource = "";
    if(clsSubPopulationSource!=NULL){
	strPopulationSource = clsSubPopulationSource->Type();
    }

    if(strPopulationSource.length()==0){
	strPopulationSource = ClsTagLibrary::SelectorAll();
    }

//    cout << "strPopulationSource: " << strPopulationSource << endl;
    setSourceSelector(strPopulationSource);
//----------------------------------------------
// TARGET----------------------------------------------
    qgroupboxTarget = new QGroupBox(this);
    qgroupboxTarget->setTitle("Target");
    pqlaySelectors->addWidget(qgroupboxTarget , WIDGET_V_STRETCH);

    QVBoxLayout *pqlayTargetGB = new QVBoxLayout(qgroupboxTarget, 14, -1, "target gb layout");
    pqlayTargetGB->setAutoAdd (true);

    qcomboTargetPopulation = new QComboBox(false, qgroupboxTarget, "combo box");
    connect(qcomboTargetPopulation, SIGNAL(activated(const QString &)), this, SLOT(targetTypeChanged(const QString &)));
    qcomboTargetPopulation->insertItem(ClsTagLibrary::SelectorAll());
    qcomboTargetPopulation->insertItem(ClsTagLibrary::SelectorRegion());
    if(bShowList){
	qcomboTargetPopulation->insertItem(ClsTagLibrary::SelectorList());
    }


    qwidgetstackTargetSelectors = new QWidgetStack(qgroupboxTarget);

    ClsBaseSubPopulation *clsSubPopulationTarget = clsPatternForeach->getTargetPopulation();
    string strPopulationTarget;
    if(clsSubPopulationTarget!=NULL){
	strPopulationTarget = clsSubPopulationTarget->Type();
    }

    if(strPopulationTarget.length()==0){
	strPopulationTarget = ClsTagLibrary::SelectorAll();
    }

//    cout << "strPopulationTarget: " << strPopulationTarget << endl;
    setTargetSelector(strPopulationTarget);

//----------------------------------------------
    pAutoWidget = new ClsAutoWidget(_item, this, QCString(_pcName)+" widget"); //@@@
    pqlayTop->addWidget(pAutoWidget); //@@@



    if (_bModal) {
	pqwgtButtons = new ClsModalButtons(this, QCString(_pcName)+" buttons");

	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons(this, QCString(_pcName)+" buttons");

	connect(pAutoWidget, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));

//	connect(this, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
	connect(this, SIGNAL(sigSubItemChanged()), pqwgtButtons, SLOT(enableApply()));


	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));
	// How should the Undo button be handled?
	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }

    pqlayTop->addWidget(pqwgtButtons, BUTTON_V_STRETCH);
}

iqrfe::ClsAutoGroupPatternForeach::~ClsAutoGroupPatternForeach() {
    if (bDebugAutoGroup) {
    cout << "ClsAutoGroupPatternForeach::~ClsAutoGroupPatternForeach" << endl;
    }
}

void iqrfe::ClsAutoGroupPatternForeach::apply() {
    if (bDebugAutoGroup) {
    cout << "ClsAutoGroupPatternForeach::apply" << endl;
    }


//    ClsBaseSubPopulation *clsSubPopulationSource;
    ClsPatternBaseSelector* clsPatternBaseSelectorSource = dynamic_cast<ClsPatternBaseSelector*>(qwidgetstackSourceSelectors->visibleWidget());
//    clsSubPopulationSource = clsPatternBaseSelectorSource->getSubPopulation();
    clsPatternForeach->setSourcePopulation(clsPatternBaseSelectorSource->getSubPopulation());

    ClsPatternBaseSelector* clsPatternBaseSelectorTarget = dynamic_cast<ClsPatternBaseSelector*>(qwidgetstackTargetSelectors->visibleWidget());
    clsPatternForeach->setTargetPopulation( clsPatternBaseSelectorTarget->getSubPopulation());

    ClsBaseAutoGroup::apply();


}


void iqrfe::ClsAutoGroupPatternForeach::closeEvent ( QCloseEvent *  ) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternForeach::closeEvent ( QCloseEvent * e )" << endl;
    }
    emit autoGroupClosed(strName, "", ClsMemberWidget::HIDE);

}

void iqrfe::ClsAutoGroupPatternForeach::sourceTypeChanged(const QString &qstrType) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternForeach::sourceTypeChanged(const QString &qstrType)" << endl;
    }
    setSourceSelector(qstrType.latin1());
//    qcomboSourcePopulation->setCurrentText(qstrType);
//    emit changed();
    emit sigSubItemChanged();
}

void iqrfe::ClsAutoGroupPatternForeach::targetTypeChanged(const QString &qstrType) {
    setTargetSelector(qstrType.latin1());
//    qcomboTargetPopulation->setCurrentText(qstrType);
//    emit changed();
    emit sigSubItemChanged();
}

void iqrfe::ClsAutoGroupPatternForeach::setSourceSelector(string strType) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternForeach::setSourceSelector(string strType)" << endl;
    }

    string strSourceGroupID = static_cast<ClsFEConnection*>(&itemConnection)->getConnectionSourceID();
    ClsBaseTopology *clsBaseTopologySource = ClsFESystemManager::Instance()->getFEGroup( strSourceGroupID )->getTopology();

    setSelector(qgroupboxSource, strType, mapSelectorSourceStack, qwidgetstackSourceSelectors, clsBaseTopologySource);

    ClsBaseSubPopulation *clsSubPopulationSource = clsPatternForeach->getSourcePopulation();

    ClsPatternBaseSelector* clsPatternBaseSelectorSource = dynamic_cast<ClsPatternBaseSelector*>(qwidgetstackSourceSelectors->visibleWidget());
    if(clsSubPopulationSource!=NULL){
	string strCurrentType = clsSubPopulationSource->Type();
	if(!strType.compare(strCurrentType)){
	    clsPatternBaseSelectorSource->setSubPopulation(clsSubPopulationSource);
	}
    }

    qcomboSourcePopulation->setCurrentText(strType.c_str());

}

void iqrfe::ClsAutoGroupPatternForeach::setTargetSelector(string strType) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternForeach::setTargetSelector(string strType)" << endl;
    }


    string strTargetGroupID = static_cast<ClsFEConnection*>(&itemConnection)->getConnectionTargetID();
    ClsBaseTopology *clsBaseTopologyTarget = ClsFESystemManager::Instance()->getFEGroup( strTargetGroupID )->getTopology();

    setSelector(qgroupboxTarget, strType, mapSelectorTargetStack, qwidgetstackTargetSelectors, clsBaseTopologyTarget);

    ClsBaseSubPopulation *clsSubPopulationTarget = clsPatternForeach->getTargetPopulation();

    ClsPatternBaseSelector* clsPatternBaseSelectorTarget = dynamic_cast<ClsPatternBaseSelector*>(qwidgetstackTargetSelectors->visibleWidget());
    if(clsSubPopulationTarget!=NULL){
	string strCurrentType = clsSubPopulationTarget->Type();
	if(!strType.compare(strCurrentType)){
	    clsPatternBaseSelectorTarget->setSubPopulation(clsSubPopulationTarget);
	}
    }

    qcomboTargetPopulation->setCurrentText(strType.c_str());
}


void iqrfe::ClsAutoGroupPatternForeach::setSelector(QWidget *qwParent,
						    string _strType,
						    map<string, int> &_map,
						    QWidgetStack *qws,
						    ClsBaseTopology *clsBaseTopology) {
    if (bDebugAutoGroup) {
	cout << "ClsAutoGroupPatternForeach::setSelector(...)" << endl;
    }

    int iIndex;
    map<string, int>::iterator it;
    if ((it = _map.find(_strType)) != _map.end()) {
	if (bDebugAutoGroup) {
	    cout << "reusing selector" << endl;
	}
	qws->raiseWidget(it->second);
    } else {
	if (bDebugAutoGroup) {
	    cout << "instantiating new selector strType:" << _strType << endl;
	}
	if(!_strType.compare(ClsTagLibrary::SelectorList())){
	    iIndex = qws->addWidget(new ClsPatternSelectorList(qwParent, this, clsBaseTopology, "source"));
	}
	else if(!_strType.compare(ClsTagLibrary::SelectorRegion())){
	    iIndex = qws->addWidget(new ClsPatternSelectorRegion(qwParent, this, clsBaseTopology, "source"));
	}
	else if(!_strType.compare(ClsTagLibrary::SelectorAll())){
	    iIndex = qws->addWidget( new ClsPatternSelectorAll(qwParent, this, "source"));
	}
	else {
	    iIndex = qws->addWidget(new ClsPatternSelectorRegion(qwParent, this, clsBaseTopology, "source"));
	}

	pair <string, int> pairTemp(_strType, iIndex);
	_map.insert(pairTemp);
	qws->raiseWidget(iIndex);
    }

//    cout << "end:setSelector()" << endl;

}

// void iqrfe::ClsAutoGroupPatternForeach::subItemChanged() {
//     cout << "ClsAutoGroupPatternForeach::subItemChanged()" << endl;
//     emit changed();
// }



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
