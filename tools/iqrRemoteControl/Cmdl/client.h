#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <qapplication.h>
#include <qsocket.h>
#include <qlistbox.h>
#include <qvariant.h>
#include <qwidget.h>

static const Q_UINT16 ipcPort = 54923;

using namespace std;

class client : public QWidget {
    Q_OBJECT

public:
    client (const QString& qstrMessage) {//, QObject * parent = 0): QWidget ( parent) {
	socket = new QSocket( this );
	connect(socket, SIGNAL(error(int)), this, SLOT(errorOccured(int)));
	connect(socket, SIGNAL(bytesWritten(int)), this, SLOT(messageSent(int)));
	socket->connectToHost( "localhost", ipcPort );
	sendPacket( qstrMessage );
    }

private slots:
void errorOccured(int){
    cout << "Error connecting to iqr" << endl;
    qApp->quit();
}

void messageSent(int iResult){
    if(iResult<0){
	    cerr << "Error sending message to iqr" << endl;
    } else {
	cout << iResult << " bytes sent to iqr" << endl;
    }
    qApp->quit();

}

private:
//    void sendPacket( const QVariant &v ) {
void sendPacket( QString qstrMessage ){

//--	QByteArray ba;
//--	QDataStream varDs( ba, IO_WriteOnly );
//--	varDs << v;
//--	QDataStream ds( socket );
//--	ds << (Q_UINT32) ba.size();
//--	socket->writeBlock( ba.data(), ba.size() );
//--	socket->writeBlock( ba.data(), ba.size() );
    socket->writeBlock ( qstrMessage.latin1(), qstrMessage.length());

    }
    
    QSocket *socket;
};


#endif
