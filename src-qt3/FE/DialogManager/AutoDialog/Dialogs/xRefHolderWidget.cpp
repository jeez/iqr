#include <qlayout.h>

#include "xRefHolderWidget.hpp"

#include "ClsFESystemManager.h"
#include "xRefWidget.hpp"


#define NO_STRETCH 0
#define WIDGET_STRETCH 10
#define TAB_V_STRETCH 10

#define TAB_BORDER 5

#ifdef DEBUG_CLSXREFHOLDERWIDGET
static const bool bDebugXRefHolderWidget = true;
#else
static const bool bDebugXRefHolderWidget = false;
#endif


#include "ClsFEConnection.h"
#include "ClsFEProcess.h"
#include "module.hpp"
#include "synapse.hpp"

iqrfe::ClsXRefHolderWidget::ClsXRefHolderWidget(ClsItem& _item, const ClsXRefHolder* xRefHolder, QWidget *_pqwgtParent, const char *_pcName) :
    QWidget(_pqwgtParent, _pcName) {

//    dynamic_cast<ClsFEConnection*>(&_item);
    iOwnerType = -1;
    strOwnerParentID = "";

    if(dynamic_cast<ClsSynapse*>(&_item)){
	if (bDebugXRefHolderWidget) {
	    cout << "got Synapse" << endl;
	}
	iOwnerType = ClsFESystemManager::ITEM_CONNECTION;
	strOwnerParentID = dynamic_cast<ClsSynapse*>(&_item)->getConnectionID();
    } else if(dynamic_cast<ClsModule*>(&_item)){
	if (bDebugXRefHolderWidget) {
	    cout << "got Module" << endl;
	}
	iOwnerType = ClsFESystemManager::ITEM_MODULE;
	strOwnerParentID = dynamic_cast<ClsModule*>(&_item)->getProcessID();
    }

    if (bDebugXRefHolderWidget) {
	cout << "strOwnerParentID: " << strOwnerParentID << endl;
    }
    if(strOwnerParentID.length()>0){
	list<string> lstXRefValues = ClsFESystemManager::Instance()->getXRefValueList(iOwnerType, strOwnerParentID);
	if (bDebugXRefHolderWidget) {
	    cout << "lstXRefValues.size(): " << lstXRefValues.size() << endl;
	}
	QVBoxLayout *pqlayTabLayout;
	pqlayTabLayout = new QVBoxLayout(this, TAB_BORDER);
	ClsXRefWidget* clsXRefWidget;
	XRefList xRefList = xRefHolder->getListXRefs();

	if (bDebugXRefHolderWidget) {
	    cout << "xRefHolder->getName(): " << xRefHolder->getName() << endl;
	    cout << "xRefList.size(): " << xRefList.size() << endl;
	}


	XRefList::iterator itXRef;
	for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	    string strXRefName = (*itXRef)->getName();
	    string strXRefLabel = (*itXRef)->getLabel();
	    ClsXRef* pXRef = (*itXRef);
	    string strXRefTargetName = ClsFESystemManager::Instance()->getXRefTargetNameByID(iOwnerType, strOwnerParentID, pXRef->getTarget());
	    clsXRefWidget = new ClsXRefWidget(pXRef, lstXRefValues, strXRefTargetName, this);
	    lstXRefWidgets.push_back(clsXRefWidget);
	    connect(clsXRefWidget, SIGNAL(changed()), this, SIGNAL(changed()));
	    pqlayTabLayout->addWidget(clsXRefWidget);//, WIDGET_STRETCH);
	}
	pqlayTabLayout->addStretch(TAB_V_STRETCH);
    }
}

void iqrfe::ClsXRefHolderWidget::apply(){
    if (bDebugXRefHolderWidget) {
	cout << "ClsXRefHolderWidget::apply()" << endl;
    }

    list<ClsXRefWidget*>::iterator it;
    for(it = lstXRefWidgets.begin(); it!= lstXRefWidgets.end(); it++){
	// valueHasChanged()
	if((*it)->valueHasChanged()){
//	    cout << "VALUE: " << (*it)->getValue() << endl;
	    string strXRefTargetName = (*it)->getValue();
	    string strXRefTargetID = ClsFESystemManager::Instance()->getXRefTargetIDByName(iOwnerType, strOwnerParentID, strXRefTargetName);
	    (*it)->xRef->setTarget(strXRefTargetID);
	}
    }
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
