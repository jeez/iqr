/****************************************************************************
 ** $Filename: ClsFEComputeEngine.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jul 10 00:45:22 2003
 **
 *****************************************************************************/

#ifndef CLSFECOMPUTEENGINE_H
#define CLSFECOMPUTEENGINE_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <iostream>
#include <string>
//#ifdef LINUX
#include <sys/time.h>
//#endif
#include <qapplication.h>
#include <qthread.h>
#include <qsemaphore.h>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qwidget.h>

//#include "ClsGroupManipPattern.h"

//using namespace std

#if defined(QT_NO_THREAD)
#  error Thread support not enabled.
#endif

using namespace std;

class ClsFEGroup;
class ClsFEProcess;
class ClsFEConnection;
class ClsFESystemManager; 
class ClsFEDataSampler;



class ClsFEComputeEngine : public QThread {
	
public:

    static ClsFEComputeEngine* Instance();
    static void initializeComputeEngine(QWidget* _parent = NULL, QMutex* _qmutexSysGUI = NULL, QWaitCondition* _qWaitCondition = NULL);




    void setDataSampler(ClsFEDataSampler* _clsFEDataSampler, int iInterval /*, int iCount*/);
    void prepare(bool _bSyncPlots);
    void run();
    void stop();
    void pause(bool b);

    signals:


private:

    ClsFEComputeEngine::ClsFEComputeEngine(QWidget* _parent, QMutex* _qmutexSysGUI, QWaitCondition* _qWaitCondition);
    static ClsFEComputeEngine* _instanceComputeEngine;


    int iRoundCounter;

    QWidget* parent;

    QMutex *qmutexSysGUI;

    bool stopped;
    bool paused;

    map<string, ClsFEGroup*>::iterator miGroup;
    map<string, ClsFEProcess*>::iterator miProcess;
    map<string, ClsFEConnection*>::iterator miConnection;

    ClsFEDataSampler* clsFEDataSampler;
    int iSamplerInterval;
    QWaitCondition* qWaitCondition;
    bool bSyncPlots;


};


#endif /* CLSFECOMPUTEENGINE_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
