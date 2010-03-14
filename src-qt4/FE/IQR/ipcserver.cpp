#include "ipcserver.h"

#include <iostream>

#include <QVariant>
#include <QImage>
#include <QPalette>
#include <QApplication>


IpcServer::IpcServer( Q_UINT16 port, QObject *parent ) {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    tcpServer->listen ( QHostAddress::Any, port );


}

void IpcServer::newConnection( ) {
    clientConnection = tcpServer->nextPendingConnection();
    connect( clientConnection, SIGNAL(readyRead( )), this, SLOT(dataReceived()) );
}


void IpcServer::dataReceived(  ) {
    Q_ULONG bytesAvail = clientConnection->bytesAvailable();
    QByteArray s;
    s.resize( bytesAvail );
    s = clientConnection->read( bytesAvail  );
    emit receivedText( s );
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
 


