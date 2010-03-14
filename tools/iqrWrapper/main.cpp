#include <qapplication.h>

#include "process.h"



int main( int argc, char **argv ) {
    QApplication a( argc, argv );
    QStringList qstringlstArgs;
    qstringlstArgs.append("catchsegv");
    qstringlstArgs.append("iqr");
    for ( int i = 1; i < a.argc(); i++ ){
	qstringlstArgs.append( a.argv()[i] );
	cout <<  a.argv()[i] << endl;
    }
    UicManager manager(qstringlstArgs);
    a.setMainWidget( &manager );
//    manager.show();
    return a.exec();
}
