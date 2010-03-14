#include <iostream>
#include "CmdBuildImpl.h"
#include <qfile.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <q3filedialog.h>
#include <qlabel.h>
#include <QDateTime>

using namespace std;

CmdBuildImpl::CmdBuildImpl( QWidget *parent, const char *name ) :  QDialog(parent) /*CmdBuild( parent, name )*/ {

    setupUi(this); 


    qstrFilename = "";
 
    connect(comboCmd, SIGNAL(activated(const QString&)), SLOT(slotComboCmd(const QString&)));
    connect(comboItemType, SIGNAL(activated(const QString&)), SLOT(slotComboItemType(const QString&)));
    connect(comboItemIdentifierType, SIGNAL(activated(const QString&)), SLOT(slotComboIdType(const QString&)));
    connect(comboItemIdentifier, SIGNAL(activated(const QString&)), SLOT(slotComboItemIdentifier(const QString&)));

    connect(cmdGenerate, SIGNAL(clicked()), SLOT(slotGenerate()));
    connect( pbOpenFile, SIGNAL(clicked()), SLOT(slotOpenFile()) );
    connect( pbReload, SIGNAL(clicked()), SLOT(slotReloadFile()) );


    comboCmd->insertItem("start");
    comboCmd->insertItem("stop");
    comboCmd->insertItem("quit");
    comboCmd->insertItem("startsampler");
    comboCmd->insertItem("stopsampler");
    comboCmd->insertItem("param");

    lblFileChanged->hide();

    comboItemIdentifierType->insertItem("");
    comboItemIdentifierType->insertItem("name");
    comboItemIdentifierType->insertItem("id");


//    comboItemType->insertItem("");
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

     qtimerFilewatch = new QTimer(this);
     connect( qtimerFilewatch, SIGNAL(timeout()), SLOT(slotCheckFile()) );


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
    
    comboItemIdentifier->insertItem("");
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


    comboParamName->insertItem("");
    QStringList::Iterator it;
    for(it=qlst.begin(); it!=qlst.end(); it++){
//	cout << (*it) << endl;
	comboParamName->insertItem(*it);
    }
}


void CmdBuildImpl::slotGenerate(){

    QString qstrCmd;
    
    qstrCmd.append("cmd:");
    QString qstr = comboCmd->currentText();
    qstrCmd.append( qstr);
    if(!qstr.compare("param")){
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
    }
    qstrCmd.append(";");

//    leCommand->setText(qstrCmd);

    emit sigCommand(qstrCmd);



}

void CmdBuildImpl::slotOpenFile(QString _qstrFilename) {
    if(_qstrFilename.length()<=0){
	qstrFilename = Q3FileDialog::getOpenFileName( QString::null, "iqr system (*.iqr)", this );
    } else {
	qstrFilename = _qstrFilename;
    }
    if(!clsSystemDB->openFile(qstrFilename)){
	leFilename->setText("File: \n" + qstrFilename);
	pbReload->setEnabled(true);
	qfinfoCurrentFile.setFile(qstrFilename);
	lblFileChanged->hide();
	qtimerFilewatch->start(800, false);
    } else {
	qstrFilename = "";
	leFilename->setText("File: <none>");
    }
}


void CmdBuildImpl::slotReloadFile() {
    slotOpenFile(qstrFilename);
}

void CmdBuildImpl::slotCheckFile() {
//    cout << "CmdBuildImpl::slotCheckFile()" << endl;
    if(qstrFilename.length()>0){
	QFileInfo qfinfo(qstrFilename);
	if(qfinfoCurrentFile.lastModified () !=  qfinfo.lastModified ()){
//	    cout << "file changed" << endl;
	    qtimerFilewatch->stop();
	    lblFileChanged->show();
	}
    }
}



CmdBuildImpl::~CmdBuildImpl() {
}




