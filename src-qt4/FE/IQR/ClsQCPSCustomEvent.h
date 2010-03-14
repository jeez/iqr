#ifndef CLSQCPSCUSTOMEVENT_H
#define CLSQCPSCUSTOMEVENT_H

#include <qapplication.h>

class ClsQCPSCustomEvent : public QEvent {
public:
    ClsQCPSCustomEvent( double _fCPS ) : QEvent(QEvent::User ), fCPS(_fCPS) {
    }
    double getCPS() { return fCPS; }
private:
    double fCPS;
};


#endif
