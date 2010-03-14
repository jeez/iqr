#ifndef REMOTECTRLIMPL_H
#define REMOTECTRLIMPL_H

#include <qtimer.h>

#include "ui_remotectrl.h"

using namespace Ui;


static const Q_UINT16 iDefaultPort = 54923;
class Q3Socket;

class RemoteCtrlImpl : public QDialog, public Ui::RemoteCtrl {
    Q_OBJECT

public:
    RemoteCtrlImpl(  );

private slots:
    void sendFile();
    void sendText();
    void sendEntry();
    void errorOccured(int);
    void slotTimer();
    void connClosed();
    void connOk();
    void slotRetrieve(Q3ListBoxItem*);
    void slotCommand(QString);
    void slotConnect();
    void socketError(int);

private:
    void sendPacket( QString qstrMessage );

    Q3Socket *socket;
    QTimer *timer;
    QString qstrHostname;
    unsigned int iPort;
};

#endif // REMOTECTRLIMPL_H


//// Local Variables:
//// mode: c++
//// End:
