#include "ipcserver.h"

#include <qsocket.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpalette.h>
#include <qapplication.h>

#include "ipcsocket.h"

IpcServer::IpcServer( Q_UINT16 port, QObject *parent ) :
//    QServerSocket( 0x7f000001, port, 1, parent ) {
    QServerSocket( port, 1, parent ) {
}

void IpcServer::newConnection( int socket ) {
    IpcSocket *s = new IpcSocket( this );
    s->setSocket( socket );
    connect( s, SIGNAL(receivedText(const QString&)), SIGNAL(receivedText(const QString&)) );
}


