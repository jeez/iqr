#ifndef CLSQCPSCUSTOMEVENT_H
#define CLSQCPSCUSTOMEVENT_H

#include <qapplication.h>

class ClsQCPSCustomEvent : public QCustomEvent {
public:
    ClsQCPSCustomEvent( double _fCPS ) : QCustomEvent(QEvent::User ), fCPS(_fCPS) {
    }
    double getCPS() { return fCPS; }
private:
    double fCPS;
};


#endif
