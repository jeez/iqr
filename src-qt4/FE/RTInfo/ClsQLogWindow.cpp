/****************************************************************************
 ** $Filename: ClsQLogWindow.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 *****************************************************************************/

#include <sstream>
#include <fstream>

#include "ClsQLogWindow.h"

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QHBoxLayout>

#include "ClsFESystemManager.h"
#include "ClsFEDialogManager.h"

ClsQLogWindow* ClsQLogWindow::_instanceQLogWindow = NULL;

void ClsQLogWindow::initializeLogger(QWidget *parent){
    _instanceQLogWindow = new ClsQLogWindow(parent);
}


ClsQLogWindow* ClsQLogWindow::Instance(){
    if(_instanceQLogWindow == NULL){
////	_instanceQLogWindow = new ClsQLogWindow(0);
    } 
    return _instanceQLogWindow;
}


void ClsQLogWindow::report(int iTarget, int iType, string strMessage){
    if(iTarget == TARGET_LOG ){
	appendText(strMessage);
    } else if(iTarget == TARGET_MESSAGE){
	if(iType == TYPE_INFO){
	    QMessageBox::information( this, "IQR", strMessage.c_str(), "close", 0 );
	} else if(iType == TYPE_WARNING){
	    QMessageBox::warning( this, "IQR", strMessage.c_str(), "close", 0 );
	} else if(iType == TYPE_ERROR){
	    QMessageBox::critical( this, "IQR", strMessage.c_str(), "close", 0 );
	} 
    } else if(iTarget == TARGET_BOTH){

    }
}

void ClsQLogWindow::reportSysChange(int iItemType, string strID, string strMessage){
    cout << "ClsQLogWindow::reportSysChange(int iItemType, string strID, string strMessage)" << endl;
    string strType = ClsFESystemManager::Instance()->getTypeAsString(iItemType);
    string strName = ClsFESystemManager::Instance()->getItemName(strID, iItemType);
    
    stringstream ssLog;
    ssLog << 
	"CHANGED: " << 
	"type: " << strType << "; " << 
	"name: \"" << strName << "\"; " << 
	"ID: " << strID << "; " << 
	strMessage;
    appendText(ssLog.str());
    
}



ClsQLogWindow::ClsQLogWindow(QWidget *parent) : QDialog ( parent) {
    QBoxLayout *qvblt = new QVBoxLayout( this );

    qtxtLog = new QPlainTextEdit(this);
    qvblt->addWidget( qtxtLog );

    QBoxLayout *qhblt = new QHBoxLayout( qvblt );
    qhblt->addStretch(1);

    qbtnSave = new QPushButton ( "save", this ) ;
    qhblt->addWidget( qbtnSave);
    connect(qbtnSave, SIGNAL(clicked()), this, SLOT(saveLog()));

    qbtnClose = new QPushButton ( "close", this ) ;
    qhblt->addWidget( qbtnClose);
    connect(qbtnClose, SIGNAL(clicked()), this, SLOT(hide()));

    qhblt->addSpacing(1);
    qbtnClear = new QPushButton ( "clear", this ) ;
    qhblt->addWidget( qbtnClear);
    connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clearText()));
    qhblt->addStretch(1);

    resize( 250, 400 );
};
     


void ClsQLogWindow::appendText( string strText) {
    string str = strText; // + (string)"\n";
    qtxtLog->appendPlainText(str.c_str());
};     

void ClsQLogWindow::setText( string strText ) {
    qtxtLog->setPlainText(strText.c_str());
};     

void ClsQLogWindow::clearText() {
    qtxtLog->setPlainText("");
};     


void ClsQLogWindow::saveLog() {
    cout << "ClsQLogWindow::saveLog()" << endl;
    string strFileName = ClsFEDialogManager::Instance()->createSaveDialog("Log file(*.log)", ".log");
    if(strFileName.size()>0){
	ofstream out(strFileName.c_str());
	out << qtxtLog->toPlainText().toStdString() << endl;
    }

};     


