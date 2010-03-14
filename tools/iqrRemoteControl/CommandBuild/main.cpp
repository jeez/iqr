#include <qapplication.h>
#include "CmdBuildImpl.h"

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    CmdBuildImpl cmdBuildImpl;
    a.setMainWidget( &cmdBuildImpl );
    cmdBuildImpl.show();

    return a.exec();
}
