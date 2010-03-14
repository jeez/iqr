#include <iostream>
#include "CmdBuildImpl.h"
#include <qfile.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

using namespace std;

CmdBuildImpl::CmdBuildImpl( QWidget *parent, const char *name ) : CmdBuild( parent, name ) {

    connect(comboCmd, SIGNAL(activated(const QString&)), SLOT(slotComboCmd(const QString&)));
    connect(comboItemType, SIGNAL(activated(const QString&)), SLOT(slotComboItemType(const QString&)));
    connect(comboItemIdentifierType, SIGNAL(activated(const QString&)), SLOT(slotComboIdType(const QString&)));
    connect(comboItemIdentifier, SIGNAL(activated(const QString&)), SLOT(slotComboItemIdentifier(const QString&)));

    connect(cmdGenerate, SIGNAL(clicked()), SLOT(slotGenerate()));
    connect( pbOpenFile, SIGNAL(clicked()), SLOT(slotOpenFile()) );

    comboCmd->insertItem("start");
    comboCmd->insertItem("stop");
    comboCmd->insertItem("quit");
    comboCmd->insertItem("param");

    comboItemIdentifierType->insertItem("");
    comboItemIdentifierType->insertItem("name");
    comboItemIdentifierType->insertItem("id");


    comboItemType->insertItem("PROCESS");
    comboItemType->insertItem("GROUP");
    comboItemType->insertItem("NEURON");
    comboItemType->insertItem("CONNECTION");
    comboItemType->insertItem("SYNAPSE");

    comboItemType->setEnabled(false);
    comboItemIdentifierType->setEnabled(false);
    comboItemIdentifier->setEnabled(false);
    comboParamName->setEnabled(false);
    leValue->setEnabled(false);

     clsSystemDB = new ClsSystemDB();
//     clsSystemDB->openFile(fileName);

//    clsSystemDB->getListItemIDs(ClsSystemDB::ITEM_GROUP);
//    clsSystemDB->getItemParameters(ClsSystemDB::ITEM_GROUP, "name", "G7");
//    clsSystemDB->getItemParameters(ClsSystemDB::ITEM_GROUP, "id", "L-6648-1107513181-2053999932");
//    clsSystemDB->getItemParameters(ClsSystemDB::ITEM_PROCESS, "name", "New Process 1");
//    clsSystemDB->getSubItemParameters(ClsSystemDB::ITEM_NEURON, "id", "L-6648-1107513181-2053999932");

}

void CmdBuildImpl::slotComboCmd(const QString & qstr){
//    cout << qstr << endl;
    if(!qstr.compare("param")){
	comboItemType->setEnabled(true);
	comboItemIdentifierType->setEnabled(true);
	comboItemIdentifier->setEnabled(true);
	comboParamName->setEnabled(true);
	leValue->setEnabled(true);
    } else {
	comboItemType->setEnabled(false);
	comboItemIdentifierType->setEnabled(false);
	comboItemIdentifier->setEnabled(false);
	comboParamName->setEnabled(false);
	leValue->setEnabled(false);
    }
	
}

void CmdBuildImpl::slotComboItemType(const QString&){
    comboItemIdentifierType->setCurrentText("");
    comboItemIdentifier->clear();
    comboParamName->clear();
}


void CmdBuildImpl::slotComboIdType(const QString& ){
//    cout << "CmdBuildImpl::slotComboIdType(const QString& qstr)" << endl;

    comboItemIdentifier->clear();
//    cout << "qstr: " << qstr  << endl;
    int iItemType = -1;

    QString qstrType = comboItemType->currentText(); 

    if(!qstrType.compare("PROCESS")){
	iItemType = ClsSystemDB::ITEM_PROCESS;
    }
    else if(!qstrType.compare("GROUP") || !qstrType.compare("NEURON")){
	iItemType = ClsSystemDB::ITEM_GROUP;
    }
    else if(!qstrType.compare("CONNECTION") || !qstrType.compare("SYNAPSE")){
	iItemType = ClsSystemDB::ITEM_CONNECTION;
    }

    QStringList qlst;
    QString qstrItemIdentifierType = comboItemIdentifierType->currentText();
    if(!qstrItemIdentifierType.compare("id")){
	qlst = clsSystemDB->getListItemIDs(iItemType);
    } else if(!qstrItemIdentifierType.compare("name")){
	qlst = clsSystemDB->getListItemNames(iItemType);
    }
    

    QStringList::Iterator it;
    for(it=qlst.begin(); it!=qlst.end(); it++){
//	cout << (*it) << endl;
	comboItemIdentifier->insertItem(*it);
    }
}



void CmdBuildImpl::slotComboItemIdentifier(const QString& qstr){
    QString qstrIdentifier = qstr;
//    cout << "qstrIdentifier: " << qstrIdentifier << endl;
    comboParamName->clear();

    QString qstrIdType = comboItemIdentifierType->currentText(); 
    QStringList qlst;

    QString qstrType = comboItemType->currentText(); 
    if(!qstrType.compare("PROCESS")){
	qlst = clsSystemDB->getItemParameters(ClsSystemDB::ITEM_PROCESS, qstrIdType, qstrIdentifier);
    }
    else if(!qstrType.compare("GROUP")){
	qlst = clsSystemDB->getItemParameters(ClsSystemDB::ITEM_GROUP, qstrIdType, qstrIdentifier);
    }
    else if(!qstrType.compare("NEURON")){
	qlst = clsSystemDB->getSubItemParameters(ClsSystemDB::ITEM_NEURON, qstrIdType, qstrIdentifier);
    }
    else if(!qstrType.compare("CONNECTION")){
	qlst = clsSystemDB->getItemParameters(ClsSystemDB::ITEM_CONNECTION, qstrIdType, qstrIdentifier);
    }
    else if(!qstrType.compare("SYNAPSE")){
	qlst = clsSystemDB->getSubItemParameters(ClsSystemDB::ITEM_SYNAPSE, qstrIdType, qstrIdentifier);
    }


    QStringList::Iterator it;
    for(it=qlst.begin(); it!=qlst.end(); it++){
//	cout << (*it) << endl;
	comboParamName->insertItem(*it);
    }
}


void CmdBuildImpl::slotGenerate(){

    QString qstrCmd;
    
    qstrCmd.append("cmd:");
    qstrCmd.append( comboCmd->currentText());
    qstrCmd.append(";itemType:");
    qstrCmd.append(comboItemType->currentText());

    QString qstrItemIdentifierType = comboItemIdentifierType->currentText();
    if(!qstrItemIdentifierType.compare("id")){
    qstrCmd.append(";itemID:");
    } else if(!qstrItemIdentifierType.compare("name")){
    qstrCmd.append(";itemName:");
    }
    qstrCmd.append(comboItemIdentifier->currentText());
    qstrCmd.append(";paramID:");
    qstrCmd.append(comboParamName->currentText());
    qstrCmd.append(";value:");
    qstrCmd.append(leValue->text());
    qstrCmd.append(";");

    leCommand->setText(qstrCmd);



}


void CmdBuildImpl::slotOpenFile() {
    QString qstrFilename = QFileDialog::getOpenFileName( QString::null,
	    "iqr system (*.iqr)", this );

    if(!clsSystemDB->openFile(qstrFilename)){
	setCaption("iqr command builder: " + qstrFilename);
    } else {
	setCaption("iqr command builder");
    }
}
CmdBuildImpl::~CmdBuildImpl() {
}

