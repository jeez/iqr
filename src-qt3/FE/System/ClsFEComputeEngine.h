/****************************************************************************
 ** $Filename: ClsFEComputeEngine.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jul 10 00:45:22 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSFECOMPUTEENGINE_H
#define CLSFECOMPUTEENGINE_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <iostream>
#include <string>
#include <sys/time.h>

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

    signals:


private:

    ClsFEComputeEngine::ClsFEComputeEngine(QWidget* _parent, QMutex* _qmutexSysGUI, QWaitCondition* _qWaitCondition);
    static ClsFEComputeEngine* _instanceComputeEngine;


    int iRoundCounter;

    QWidget* parent;

    QMutex *qmutexSysGUI;

    bool stopped;

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
