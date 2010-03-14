/****************************************************************************
 ** $Filename: subtypedAutoDialog.cpp
 ** $Id: subtypedAutoDialog.cpp,v 1.2 2004/01/19 11:21:47 ulysses Exp ulysses $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
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

#include <qlayout.h>
#include <qlabel.h>


#include "subtypedAutoDialog.hpp"
#include "autoWidget.hpp"

#include "autoGroup.hpp"
#include "autoGroupPatternForeach.hpp"
#include "autoGroupPatternTuples.hpp"
#include "ClsSelectorSparse.h"
#include "dialogButtons.hpp"

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
    : ClsDialog(_pqwgtParent, _pcName, _iType, _strID, _bModal, 0), item(_item)


{
    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::ClsSubtypedAutoDialog" << endl;
    }

//    setSizePolicy(QSizePolicy(QSizePolicy::Fixed ,QSizePolicy::Fixed));
//    setSizePolicy(QSizePolicy(QSizePolicy::Minimum ,QSizePolicy::Minimum));
//    setSizePolicy(QSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum));
//    setSizePolicy(QSizePolicy(QSizePolicy::Preferred ,QSizePolicy::Preferred));
//    setSizePolicy(QSizePolicy(QSizePolicy::Expanding ,QSizePolicy::Expanding));
//    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding ,QSizePolicy::MinimumExpanding));
//    setSizePolicy(QSizePolicy(QSizePolicy::Ignored ,QSizePolicy::Ignored));

//    setCaption(_pcName);

    qsplitter = new QSplitter(QSplitter::Horizontal, this, "main" );
//    qsplitter->setOpaqueResize( true );
    qframeMain = new QFrame(qsplitter);

//    qframeMain->setBackgroundColor(red);
//    this->setBackgroundColor(green);
//    qframeMain->setFixedWidth(245);


//    qsplitter->setResizeMode( qframeMain, QSplitter::KeepSize );


    // Create top-level layout manager for dialog.
    QVBoxLayout *pqlayMainAuto = new QVBoxLayout(qframeMain, DIALOG_BORDER, -1, "auto dialog layout");

    // Construct an autowidget based on the item's parameter
    // list.
    pAutoWidget = new ClsAutoWidget(_item, qframeMain, QCString(_pcName)+" widget");
//    pqlayMainAuto->setResizeMode(QLayout::Fixed);

    pqlayMainAuto->addWidget(pAutoWidget, WIDGET_V_STRETCH);


/////////////////
    // Create buttons
    // Buttons depend on whether the dialog is modal or modeless.
    QString  qstrButtonName(tr("%1 %2").arg(_pcName).arg("buttons"));
    QWidget *pqwgtButtons = NULL;

    if (_bModal) {
	pqwgtButtons = new ClsModalButtons( qframeMain, QCString(_pcName)+" buttons");

	// OK in modal dialog applies changes and closes dialog.
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));

	// Cancel closes dialog without applying changes.
	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
	pqwgtButtons = new ClsModelessButtons( qframeMain, QCString(_pcName)+" buttons");

	// @@@ 21/05/01 There might be problems with the state of the
	// apply button.  It should always be disabled when the dialog
	// is opened.
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


	ClsMemberWidget *clsMemberWidget = new ClsMemberWidget(qframeMain, *it, _bModal, strValue, lst);
	/* use runtimeEditable in constructor */

	pair<string, ClsMemberWidget*> pairTemp(*it, clsMemberWidget);
	mapMemberWidgets.insert(pairTemp);
	pqlayMainAuto->addWidget(clsMemberWidget, WIDGET_V_STRETCH);
	connect(clsMemberWidget, SIGNAL(toggleMemberEdit(string, string, unsigned int)), this, SLOT(editItemMember(string, string, unsigned int)));
	connect(this, SIGNAL(sigSubTypesChangable(bool)), clsMemberWidget, SLOT(slotSelectorEnabled(bool)));
	if(pqwgtButtons!=NULL){
	    connect(clsMemberWidget, SIGNAL(sigChanged()), pqwgtButtons, SLOT(enableApply()));
	}
    }

    pqlayMainAuto->addWidget(pqwgtButtons, BUTTON_V_STRETCH);
    qsplitter->show();


    QHBoxLayout* qboxlayoutMain= new QHBoxLayout( this );
    qboxlayoutMain->addWidget ( qsplitter, 10,0 );
    qboxlayoutMain->setResizeMode (QLayout::Fixed);

//    resize(100, 300);



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

/*
      int iWidth = qframeMain->sizeHint ().width();
      map<string, ClsBaseAutoGroup*>::iterator it;
      for(it=mapAutoGroups.begin(); it!=mapAutoGroups.end(); ++it){
      iWidth += it->second->sizeHint().width();
      }

      qsplitter->refresh();

      setMaximumWidth(iWidth);
*/


/*
  int iWidth =0;
  QValueList<int> list = qsplitter->sizes();
  QValueList<int>::Iterator it = list.begin();
  while( it != list.end() ) {
  iWidth += *it;
  ++it;
  }

  cout << "iWidth: " << iWidth << endl;
  qsplitter->setMaximumWidth(iWidth);

  cout << "qsplitter->size().width(): " << qsplitter->size().width() << endl;
*/

//    setMaximumWidth(iWidth);

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
    if (bDebugsubtypedAutoDialog) {
	cout << "ClsSubtypedAutoDialog::apply" << endl;
    }


    /* take care of the memberwidgets */
    map<string, ClsMemberWidget*>::iterator it;
    for(it = mapMemberWidgets.begin(); it!=mapMemberWidgets.end(); ++it){
	if(it->second->changed()){
	    string strName = it->second->getName();
	    string strValue = it->second->getValue();
	    it->second->setValue(strValue);

//	    cout << "MemberName: " << strName << endl;
//	    cout << "MemberValue: " << strValue << endl;
	    item.setMemberTypeByName( strName, strValue);
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
