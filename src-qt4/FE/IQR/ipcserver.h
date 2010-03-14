#ifndef IPCSERVER_H
#define IPCSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class IpcServer : public QObject{
Q_OBJECT
public:
    IpcServer( Q_UINT16 port, QObject *parent );

signals:
    void receivedText( const QString& );

private slots:
    void newConnection(  );
    void dataReceived( );

private:
    QTcpServer* tcpServer;
    QTcpSocket* clientConnection;
};

#endif // IPCSERVER_H

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
 
