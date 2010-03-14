/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <iostream>
#include <qfile.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <qmovie.h>

using namespace std;
void dlgAboutIqr::init(){
  /* read /opt/iqr/iqr.version */	
    QString qstrVersion = "";
   QString qstrFilename = "/usr/lib/iqr/iqr.version";
   QFile qfile (qstrFilename);
   if(qfile.exists()){	
	qfile.open( IO_ReadOnly);
	qfile.readLine( qstrVersion, 255 );   
	qfile.close();
	qtxtVersion->setText(qstrVersion );
    } else {
	cerr << "iqr.version not found" << endl;
    }
    
    QMovie movie ("/usr/lib/iqr/iqr-logo-lg.gif");
    qlblMovie->setMovie(movie);

    
     QTimer *timer = new QTimer( this );
        connect( timer, SIGNAL(timeout()), this, SLOT(scrollText()) );
        timer->start( 100, FALSE);
	
}




void dlgAboutIqr::scrollText() {
    qtxtAbout->scrollBy(0,1);
//    cout << qtxtAbout->contentsY() << endl;
    int iContHeight = qtxtAbout->contentsHeight () - qtxtAbout->height();
//    cout << "iContHeight: " << iContHeight << endl;
    if(qtxtAbout->contentsY()>=iContHeight){
//	cout << "---" << endl;
	qtxtAbout->scrollBy(0,-5000);
    }	
}
