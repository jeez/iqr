#include "startup.h"
#include "remotectrlimpl.h"



#include <q3socket.h>
#include <qlabel.h>



StartUp::StartUp()
{
    remoteCtrl = 0;

//    socket = new QSocket( this );
//    socket->connectToHost( "localhost", ipcPort );
    remoteCtrl = new RemoteCtrlImpl(  );
    remoteCtrl->show();

}

StartUp::~StartUp() {
    delete remoteCtrl;
}


