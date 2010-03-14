#include <iostream>
#include "remotectrlimpl.h"

#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3socket.h>
#include <q3filedialog.h>
#include <qcolordialog.h>
#include <qimage.h>
#include <q3listbox.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qfile.h>
#include <q3textstream.h>
#include <qmessagebox.h>

#include "CmdBuildImpl.h"


using namespace std;

RemoteCtrlImpl::RemoteCtrlImpl( ) {

    setupUi(this); 
    
    iPort = iDefaultPort;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()));

    socket = new Q3Socket( this );
    connect(socket, SIGNAL(error(int)), this, SLOT(errorOccured(int)));

    connect(socket, SIGNAL(connected()), this, SLOT(connOk()));
    connect(socket, SIGNAL(connectionClosed()), this, SLOT(connClosed()));
    connect(socket, SIGNAL(error(int)), this, SLOT(socketError(int)));

    qstrHostname = "localhost";
    leHostname->setText(qstrHostname);
    spbPort->setValue(iPort);

    connect( sFile, SIGNAL(clicked()), SLOT(sendFile()) );
    connect( sText, SIGNAL(clicked()), SLOT(sendText()) );
    connect( sEntry, SIGNAL(clicked()), SLOT(sendEntry()) );
    connect( pbConnect, SIGNAL(clicked()), this, SLOT(slotConnect()));


    connect(listBoxHistory, SIGNAL(doubleClicked(Q3ListBoxItem*)), SLOT(slotRetrieve(Q3ListBoxItem*)));

    connect(cmdBuildImpl, SIGNAL(sigCommand(QString)), SLOT(slotCommand(QString)));

    listBoxHistory->clear();
    socket->connectToHost( qstrHostname, iPort );
}

void RemoteCtrlImpl::errorOccured( int ){
//    cout << "RemoteCtrlImpl::errorOccured( int )" << endl;
//    cout << "host: " << qstrHostname << ", port: " << iPort << endl;
    lblConnected->setText("Not connected");
    lblConnected->setPaletteForegroundColor(Qt::red);
    timer->start(2000, true);
}

void RemoteCtrlImpl::slotTimer(  ){
//    cout << "RemoteCtrlImpl::slotTimer()" << endl;
    socket->connectToHost( qstrHostname, iPort );
}


void RemoteCtrlImpl::connOk(  ){
//    cout << "RemoteCtrlImpl::connOk()" << endl;
//    cout << "host: " << qstrHostname << ", port: " << iPort << endl;
    QString qstrPort;
    qstrPort = qstrPort.setNum(iPort);
    lblConnected->setText("Connected to " + qstrHostname + ":" + qstrPort);
    lblConnected->setPaletteForegroundColor(Qt::green);
    timer->stop();
}

void RemoteCtrlImpl::connClosed(  ){
//    cout << "RemoteCtrlImpl::connClosed()" << endl;
//    cout << "host: " << qstrHostname << ", port: " << iPort << endl;
    lblConnected->setText("Not connected");
    lblConnected->setPaletteForegroundColor(Qt::red);
    timer->start(3000, true);
}

void RemoteCtrlImpl::sendPacket( QString qstrMessage ) {
    socket->writeBlock ( qstrMessage.latin1(), qstrMessage.length());
}

void RemoteCtrlImpl::sendFile() {
    QString qstrFilename = Q3FileDialog::getOpenFileName( QString::null,
	    "All Files (*)", this );

     QFile f2( qstrFilename );
     if(!f2.open( QIODevice::ReadOnly | QIODevice::Text )){
	 cerr << "error opening file" << endl;
	 QMessageBox::warning( this, "Error", "Could not open file \"" + qstrFilename + "\"");
	 return;
     }

     Q3TextStream t( &f2 );

     while ( !t.atEnd() ) {
	 sendPacket(  t.readLine() );
     }
}

void RemoteCtrlImpl::sendText() {
    listBoxHistory->insertItem(textToSend->text(),0);
    sendPacket( textToSend->text() );
    textToSend->clear();
}


void RemoteCtrlImpl::sendEntry() {
//    cout << "RemoteCtrlImpl::sendEntry()" << endl;
//    cout << "listBoxHistory->currentItem(): " << listBoxHistory->currentItem() << endl;
    if(listBoxHistory->currentItem() >= 0){
	sendPacket( listBoxHistory->currentText () );
    }
}


void RemoteCtrlImpl::slotRetrieve(Q3ListBoxItem* qlbi){
    textToSend->setText(qlbi->text());

}

void RemoteCtrlImpl::slotCommand(QString qstr){
    textToSend->setText(qstr);

}

void RemoteCtrlImpl::slotConnect(){
//    cout << "RemoteCtrlImpl::slotConnect()" << endl;
    qstrHostname = leHostname->text();
    iPort = spbPort->value();
//    cout << "host: " << qstrHostname << ", port: " << iPort << endl;
    socket->connectToHost( qstrHostname, iPort );
}


void RemoteCtrlImpl::socketError(int iError){

    if (iError == Q3Socket::ErrConnectionRefused){
	cerr << "ErrConnectionRefused" << endl;
    }
    else if (iError == Q3Socket::ErrHostNotFound){
	cerr << "ErrHostNotFound" << endl;
    }
    else if (iError == Q3Socket::ErrSocketRead){
	cerr << "ErrSocketRead" << endl;
    }
    else {
	cerr << "Unknown Error occured: " << iError << endl;
    }
}
