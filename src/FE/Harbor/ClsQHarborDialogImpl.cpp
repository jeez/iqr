#include <string>


#include <qcombobox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>

#include <ClsFESystemManager.h>
#include <ClsFEGroup.h>
#include <ClsFEConnection.h>
#include <neuron.hpp>


#include "ClsQHarborDialogImpl.h"

//#include "moc_ClsQHarborDialogImpl.cxx"

ClsQHarborDialogImpl::ClsQHarborDialogImpl(string strItemType, string strID, string strItemName, 
					   string strSubItemName, list<string> lstParams,  int _iIndex,
					   QWidget* parent, const char* name, bool modal, WFlags f1) : 
    ClsQHarborDialog(parent, name, modal, f1), iIndex(_iIndex)  {
    setAcceptDrops(false);
    connect(pbOK ,SIGNAL(clicked()), SLOT(slotOK()));
    connect(pbCancel ,SIGNAL(clicked()), SLOT(slotDoItemCanceled()));

    setCaption("Add " + strItemType);
    lblID->setText(strID);
    lblName->setText(strItemName);
    gboxSubtype->setTitle(strSubItemName);
    
    for(list<string>::iterator it= lstParams.begin();it!=lstParams.end();it++){
	qcomboParamList->insertItem(*it);
    }
}



void ClsQHarborDialogImpl::slotOK(){
//    cout << "ClsQHarborDialogImpl::slotOK()" << endl;
    string strParamLabel = qcomboParamList->currentText();
    emit sigDoItem(strParamLabel, iIndex);
    close();
}


void ClsQHarborDialogImpl::slotDoItemCanceled(){
//    cout << "ClsQHarborDialogImpl::slotDoItemCanceled()" << endl;
    emit sigDoItemCanceled(iIndex);
    close();
}



