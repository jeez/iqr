#include <iostream>
#include <unistd.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <qmovie.h>

#include "dlgAboutIqr.h"

dlgAboutIqr::dlgAboutIqr(QWidget* parent) : QDialog(parent) {
    setupUi(this);

#ifdef _WINDOWS  
  char *cwd = getcwd (NULL, 0);
  string strAppDir = cwd;
#else
  string strAppDir;
  char * pcIQR_HOME = NULL;
  pcIQR_HOME = getenv ("IQR_HOME");
  if (pcIQR_HOME!=NULL){
      strAppDir = pcIQR_HOME;
  } else {
      strAppDir = "/usr/lib/iqr/";
  }
#endif  


    QString qstrVersion = "";
    QString qstrFilename = string(strAppDir + "/iqr.version").c_str();

    QFile qfile (qstrFilename);
    if(qfile.exists()){
        qfile.open( QIODevice::ReadOnly);
	char buf[1024];
        qfile.readLine(buf, sizeof(buf) );
        qfile.close();
        qtxteditVersion->setText(buf );
    } else {
        cerr << "iqr.version not found" << endl;
    }

    QString qstrMovieFile = string(strAppDir + "/iqr-logo-lg.gif").c_str();
    qlblMovie->setMovie(new QMovie (qstrMovieFile));


    if(qlblMovie->movie()!=NULL){
	qlblMovie->movie()->setPaused(false);
    }

}
