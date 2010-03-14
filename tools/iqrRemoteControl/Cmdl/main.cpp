#include <iostream>
#include <qapplication.h>
#include <qlistbox.h>

#include "client.h"

using namespace std;

int main( int argc, char **argv ) {
    QApplication a( argc, argv );

   
    if(a.argc()==2){
	QString qstrMessage = a.argv()[1];
	client c(qstrMessage);
	a.setMainWidget( &c );
	return a.exec();
    } else {
	cerr << "usage: " << a.argv()[0] << " <command>" << endl;
	return -1;
    }
}


    
