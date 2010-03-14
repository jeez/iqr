#include "example.hpp"
#include <qapplication.h>

int main( int argc, char **argv ) {

  QApplication a(argc,argv);	

  a.setMainWidget(new MainWin());

  a.exec();
} 
