#ifndef STARTUP_H
#define STARTUP_H

#include <qobject.h>

class Q3Socket;
class RemoteCtrlImpl;

class StartUp : public QObject
{
    Q_OBJECT

public:
    StartUp();
    ~StartUp();

private slots:

private:
//    QSocket *socket;
    RemoteCtrlImpl *remoteCtrl;
};

#endif // STARTUP_H
