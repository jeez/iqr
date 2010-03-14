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
					   QWidget* parent, const char* name, bool modal, Qt::WindowFlags f1) : iIndex(_iIndex),  
														QDialog(parent)
{
    
    setupUi(this);

    setAcceptDrops(false);
    connect(pbOK ,SIGNAL(clicked()), SLOT(slotOK()));
    connect(pbCancel ,SIGNAL(clicked()), SLOT(slotDoItemCanceled()));

    string strTitle = "Add " + strItemType;
    setWindowTitle(QString(strTitle.c_str()));
    lblID->setText(strID.c_str());
    lblName->setText(strItemName.c_str());
    gboxSubtype->setTitle(strSubItemName.c_str());

    
    for(list<string>::iterator it= lstParams.begin();it!=lstParams.end();it++){
	qcomboParamList->insertItem(-1, QString((*it).c_str()));
    }
}



void ClsQHarborDialogImpl::slotOK(){
//    cout << "ClsQHarborDialogImpl::slotOK()" << endl;
    string strParamLabel = qcomboParamList->currentText().toStdString();
    emit sigDoItem(strParamLabel, iIndex);
    close();
}


void ClsQHarborDialogImpl::slotDoItemCanceled(){
//    cout << "ClsQHarborDialogImpl::slotDoItemCanceled()" << endl;
    emit sigDoItemCanceled(iIndex);
    close();
}



