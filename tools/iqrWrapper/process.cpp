#include <iostream>

#include <qobject.h>
#include <qprocess.h>
#include <qvbox.h>
#include <qtextview.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qdatetime.h>


#include <stdlib.h>

#include "process.h"
#include "iqrCRWimpl.h"

using namespace std;


UicManager::UicManager(QStringList qstringlstArgs) {



    proc = new QProcess( this );
    proc->setArguments(qstringlstArgs);

    connect( proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout()));
    connect( proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
    connect( proc, SIGNAL(processExited()), this, SLOT(appEnded()));

    if ( !proc->start() ) {
	// error handling
	QMessageBox::critical( 0,
		tr("Fatal error"),
		tr("Could not start the uic command."),
		tr("Quit") );
	exit( -1 );
    }
}

void UicManager::readFromStdout() {
//    cout << proc->readStdout() << endl;
    
    qstrCout = proc->readLineStdout();
    cout << qstrCout << endl;
    qstrCoutSumm.append(qstrCout + "\n");
}

void UicManager::readFromStderr() {
    qstrCerr = proc->readLineStderr();
    cerr << qstrCerr << endl;
    qstrCerrSumm.append(qstrCerr + "\n");
}


void UicManager::appEnded(){
    if(!qstrCoutSumm.find("*** Segmentation fault")){
	cout << "we have a Segmentation fault" << endl;
/*
out=`hostname`$out
mail -s "$out" ulysses@ini.phys.ethz.ch
*/

    QDateTime qdt = QDateTime::currentDateTime();
//    cout << qdt.toString("yyyy/MM/dd_hh:mm") << endl;

    QString qstrSender = "<Ulysses Bernardet> ulysses@ini.phys.ethz.ch";
    QString qstrSubject = "iqr segfault ";
    qstrSubject.append(qdt.toString("yyyy/MM/dd hh:mm"));


    iqrCRWimpl *crw = new iqrCRWimpl(qstrSubject, qstrCoutSumm, this);
    crw->show();
    } else {
	qApp->quit();
    }
}


