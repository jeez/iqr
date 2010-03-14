/****************************************************************************
 ** $Filename: subtypedAutoDialog.cpp
 ** $Id: subtypedAutoDialog.cpp,v 1.2 2004/01/19 11:21:47 ulysses Exp ulysses $
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Sun May 18 18:50:31 2003
 ** $Date: 2004/01/19 11:21:47 $
 **
 ** $Log: subtypedAutoDialog.cpp,v $
 ** Revision 1.2  2004/01/19 11:21:47  ulysses
 ** *** empty log message ***
 **
 ** Revision 1.1  2003/07/01 16:56:28  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>
#include <sstream>

#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>


#include "subtypedAutoDialog.hpp"
#include "autoWidget.hpp"

#include "autoGroup.hpp"
#include "autoGroupPatternForeach.hpp"
#include "autoGroupPatternTuples.hpp"
#include "ClsSelectorSparse.h"
#include "dialogButtons.hpp"

#include "ClsQLogWindow.h"

#include "memberWidget.h"

#include "item.hpp"

#include "tagLibrary.hpp"

//#define DEBUG_CLSSUBTYPEDAUTODIALOG
#ifdef DEBUG_CLSSUBTYPEDAUTODIALOG
static const bool bDebugsubtypedAutoDialog = true;
#else
static const bool bDebugsubtypedAutoDialog = false;
#endif

using namespace iqrcommon;
using namespace std;


iqrfe::ClsSubtypedAutoDialog::ClsSubtypedAutoDialog(ClsItem &_item, int _iType, string _strID, bool _bModal, QWidget *_pqwgtParent, const char *_pcName)
    : ClsDialog(_pqwgtParent, _pcName, _iType, _strID, _bModal, (Qt::WidgetAttribute)0), item(_item) {
    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::ClsSubtypedAutoDialog" << endl;
    }

    QHBoxLayout* qlayoutMain= new QHBoxLayout( this );
    qlayoutMain->setContentsMargins (0, 0, 0, 0);
    qlayoutMain->setSpacing(0);
    qlayoutMain->setMargin(0);

    qsplitter = new QSplitter(Qt::Horizontal, this /*, "main" */);
    qlayoutMain->addWidget ( qsplitter/*, 10,0 */);

    qframeLeftPane = new QFrame(qsplitter);



    // Create top-level layout manager for dialog.
    QVBoxLayout *qlayoutLeftPane = new QVBoxLayout(qframeLeftPane /*, DIALOG_BORDER, -1*/);

/* this is new for qt4, but does not seem to do anything.... */
    qlayoutLeftPane->setContentsMargins (0, 0, 0, 0);
    qlayoutLeftPane->setSpacing(0);
    qlayoutLeftPane->setMargin(0);
/* --------------------------------------------------------- */


    pAutoWidget = new ClsAutoWidget(_item, qframeLeftPane );
    qlayoutLeftPane->addWidget(pAutoWidget, WIDGET_V_STRETCH);
    qlayoutLeftPane->setStretchFactor (pAutoWidget, 0 );


    // Create buttons
    // Buttons depend on whether the dialog is modal or modeless.
    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));
    QWidget *pqwgtButtons = NULL;

    if (_bModal) {
	pqwgtButtons = new ClsModalButtons( qframeLeftPane /*, QString(_pcName)+" buttons"*/);

	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons( qframeLeftPane /*, QString(_pcName)+" buttons"*/);

	connect(pAutoWidget, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));

	// How should the Undo button be handled?
	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }



    list<string> lst = _item.getListOfMembers();
    list<string>::iterator it;
    for(it=lst.begin(); it!=lst.end(); ++it){
	if (bDebugsubtypedAutoDialog) {
	    cout << "MEMBER: " << *it << endl;
	}
	list<string> lst = item.getListOfMembersTypes(*it);
	string strValue= item.getMemberTypeByName(*it);
//	cout << "itemNameE: " << *it << endl;
	ClsItem *itemMember = item.getMemberByName(*it);
//	cout << "itemMember==NULL: " << (itemMember==NULL ? "yes" : "no") << endl;

	if(itemMember!=NULL){
	    if(!itemMember->isRuntimeEditable()){
		lstRuntimeDisable.push_back(*it);
		lstRuntimeDisable.unique();
	    }
	}


	ClsMemberWidget *clsMemberWidget = new ClsMemberWidget(qframeLeftPane, *it, _bModal, strValue, lst);
	/* use runtimeEditable in constructor */

	pair<string, ClsMemberWidget*> pairTemp(*it, clsMemberWidget);
	mapMemberWidgets.insert(pairTemp);
	qlayoutLeftPane->addWidget(clsMemberWidget, WIDGET_V_STRETCH);

	qlayoutLeftPane->setStretchFactor (clsMemberWidget, 0 );



	connect(clsMemberWidget, SIGNAL(toggleMemberEdit(string, string, unsigned int)), this, SLOT(editItemMember(string, string, unsigned int)));
	connect(this, SIGNAL(sigSubTypesChangable(bool)), clsMemberWidget, SLOT(slotSelectorEnabled(bool)));
	if(pqwgtButtons!=NULL){
	    connect(clsMemberWidget, SIGNAL(sigChanged()), pqwgtButtons, SLOT(enableApply()));
	}
    }

    qlayoutLeftPane->addWidget(pqwgtButtons, BUTTON_V_STRETCH);

    qsplitter->show();

    qlayoutMain->setSizeConstraint ( QLayout::SetFixedSize );


}

void iqrfe::ClsSubtypedAutoDialog::editItemMember(string strName, string strValue, unsigned int state){
    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::editItemMember(string strName, bool state)" << endl;
    }

    if(state == ClsMemberWidget::SHOW){
	if (bDebugsubtypedAutoDialog) {
	    cout << "SHOW AUTOGROUP" << endl;
	}
	ClsItem *itemMember = item.getMemberByName(strName);
//	cout << "itemMember==NULL: " << (itemMember==NULL ? "yes" : "no") << endl;

	if(itemMember!=NULL){
//      cout << "++itemNameE: " << strName << endl; //pattern
// 	cout << "itemMember->Type(): " << itemMember->Type() << endl;
	    if(!itemMember->isRuntimeEditable()){
		lstRuntimeDisable.push_back(strName);
		lstRuntimeDisable.unique();
	    }
	    string strItemMemberType = itemMember->Type();

	    ClsBaseAutoGroup *clsAutoGroup;

	    if(!strItemMemberType.compare(ClsTagLibrary::ConnectionPatternMapped())){
//	    cout << "pattern mapped" << endl;
		clsAutoGroup = new ClsAutoGroupPatternForeach(item, *itemMember, false, qsplitter, strName.c_str(), strValue, false);
	    }
	    else if(!strItemMemberType.compare(ClsTagLibrary::ConnectionPatternForeach())){
//	    cout << "pattern foreach" << endl;
		clsAutoGroup = new ClsAutoGroupPatternForeach(item, *itemMember, false, qsplitter, strName.c_str(), strValue, true);
	    }
	    else if(!strItemMemberType.compare(ClsTagLibrary::ConnectionPatternTuples())){
//	    cout << "pattern tuples" << endl;
		clsAutoGroup = new ClsAutoGroupPatternTuples(item, *itemMember, false, qsplitter, strName.c_str(), strValue);
	    }
	    else if(!strItemMemberType.compare(ClsTagLibrary::SparseTopologyTag())){
		clsAutoGroup = new ClsSelectorSparse(*itemMember, false, qsplitter, strName.c_str(), strValue);
	    }
	    else {
		if (bDebugsubtypedAutoDialog) {
		    cout << "anonymous ItemMemberType" << endl;
		}
		clsAutoGroup = new ClsAutoGroup(*itemMember, false, qsplitter, strName.c_str(), strValue);
	    }


	    connect(clsAutoGroup, SIGNAL(autoGroupClosed(string, string, unsigned int)), this, SLOT(editItemMember(string, string, unsigned int)));
	    connect(clsAutoGroup, SIGNAL(changed()), this, SLOT(slotAutoGroupChanged()));

	    pair<string, ClsBaseAutoGroup*> pairTemp(strName, clsAutoGroup);
	    mapAutoGroups.insert(pairTemp);
	    clsAutoGroup->show();
	}
    }
    else {
	if (bDebugsubtypedAutoDialog) {
	    cout << "HIDE AUTOGROUP" << endl;
	}
	map<string, ClsBaseAutoGroup*>::iterator it = mapAutoGroups.find(strName);
	if(it!=mapAutoGroups.end()){
	    delete it->second;
	    mapAutoGroups.erase(it);
	} else {
	    cout << "AutoGroup not found: " << strName << endl;
	}

	map<string, ClsMemberWidget*>::iterator itMemberWidgets = mapMemberWidgets.find(strName);
	if(itMemberWidgets!=mapMemberWidgets.end()){
	    itMemberWidgets->second->toggleEdit(true);
	}

    }

};



iqrfe::ClsSubtypedAutoDialog::~ClsSubtypedAutoDialog() {
    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::~ClsSubtypedAutoDialog" << endl;
    }

    map<string, ClsMemberWidget*>::iterator it;
    for(it = mapMemberWidgets.begin(); it!=mapMemberWidgets.end(); ++it){
	delete it->second;
    }

    delete pAutoWidget;
    delete qsplitter;

}

void iqrfe::ClsSubtypedAutoDialog::apply() {
//    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::apply" << endl;
//    }

	bool bLogging = true;
	
    /* take care of the memberwidgets */
    map<string, ClsMemberWidget*>::iterator it;
    for(it = mapMemberWidgets.begin(); it!=mapMemberWidgets.end(); ++it){
	if(it->second->changed()){
	    string strName = it->second->getName();
	    string strValue = it->second->getValue();
	    it->second->setValue(strValue);

	    // cout << "MemberName: " << strName << endl; 
	    // cout << "MemberValue: " << strValue << endl;
	    item.setMemberTypeByName( strName, strValue);

	    if(bLogging){
		stringstream ssLog;
		ssLog <<
		    "parameter: " << strName << "; " << 
		    "value: \"" << strValue << "\"";
		ClsQLogWindow::Instance()->reportSysChange(iType, strID, ssLog.str());
	    }


	}
    }
    /* ------------------------------ */

    // For each parameter widget in autowidget:
    //     if value has changed, set parameter value in item.
    ParameterWidgetMap::const_iterator itWidget;
    for (itWidget = pAutoWidget->getParameterWidgets().begin();
	 itWidget != pAutoWidget->getParameterWidgets().end();
	 ++itWidget) {

	ClsParameterWidget *pWidget = itWidget->second;

	if (pWidget->valueHasChanged()) {
	    if (bDebugsubtypedAutoDialog) { 
		cout << "ClsSubtypedAutoDialog::apply:"
		     << " value in parameter widget "
		     << pWidget->getName()
		     << " changed to "
		     << pWidget->getValueAsString()
		     << endl;
	    }

	    if(bLogging){
		stringstream ssLog;
		ssLog <<
		    "parameter: " << pWidget->getName() << "; " << 
		    "value: \"" << pWidget->getValueAsString() << "\"";
		ClsQLogWindow::Instance()->reportSysChange(iType, strID, ssLog.str());
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

void iqrfe::ClsSubtypedAutoDialog::slotAutoGroupChanged() {
    emit sigApplied(getType(), getID());
}


void iqrfe::ClsSubtypedAutoDialog::slotSubTypesChangable(bool b) {
    if (bDebugsubtypedAutoDialog) {
	cout << "iqrfe::ClsSubtypedAutoDialog::slotSubTypesChangable(bool b)" << endl;
    }

/*
  - check for itemMember.isRuntimeeditable()
  - where should this happen? here or in the memberwidget???
  - we have to take the previous state into account
  -
*/

    list<string>::iterator itLstRTD;
    for(itLstRTD = lstRuntimeDisable.begin(); itLstRTD != lstRuntimeDisable.end(); itLstRTD++){
	string strName = *itLstRTD;
	/* closing all open AutoGroups */

	if(!b){ /* we're running */
	    map<string, ClsBaseAutoGroup*>::iterator itAutoGroups = mapAutoGroups.find(strName);
	    if(itAutoGroups!=mapAutoGroups.end()){
		delete itAutoGroups->second;
		mapAutoGroups.erase(itAutoGroups);
	    }
	}

	/* change from "hide" to edit */
	map<string, ClsMemberWidget*>::iterator itMemberWidgets = mapMemberWidgets.find(strName);
	if(itMemberWidgets!=mapMemberWidgets.end()){
	    if(!b){
		itMemberWidgets->second->toggleEdit(true);
	    }
	    itMemberWidgets->second->slotEditEnabled(b);
	}
    }
    emit sigSubTypesChangable(b);
}


void iqrfe::ClsSubtypedAutoDialog::closeEvent( QCloseEvent* e) {
    if (bDebugsubtypedAutoDialog) {
	cout << "iqrfe::ClsSubtypedAutoDialog::closeEvent( QCloseEvent* ce )" << endl;
    }
    
    emit sigDialogClosed(getID());
    e->accept();
}

void iqrfe::ClsSubtypedAutoDialog::reject(){ 
    if (bDebugsubtypedAutoDialog) {
	cout << "iqrfe::ClsSubtypedAutoDialog::reject()" << endl;
    }
	close();
};	

//cout << __FILE__ << ":" << __LINE__ << endl;


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
