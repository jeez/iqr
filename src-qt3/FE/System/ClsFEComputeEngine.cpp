#include <qevent.h>

#include "ClsFEComputeEngine.h"

#include <ClsFESystemManager.h>

#include "ClsFESystem.h"
#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"
#include "ClsFEDataSampler.h"
#include "ClsFEDataManager.h" //NEW 20050131

#include "ClsQCPSCustomEvent.h"

//#define DEBUG_CLSFECOMPUTEENGINE 

double fCPSCurrent; //NEW 2008/08/03

ClsFEComputeEngine* ClsFEComputeEngine::_instanceComputeEngine = NULL;

void ClsFEComputeEngine::initializeComputeEngine(QWidget* _parent, QMutex* _qmutexSysGUI, QWaitCondition* _qWaitCondition){
    _instanceComputeEngine = new ClsFEComputeEngine(_parent, _qmutexSysGUI, _qWaitCondition);
}


ClsFEComputeEngine* ClsFEComputeEngine::Instance(){
    return _instanceComputeEngine;
}



ClsFEComputeEngine::ClsFEComputeEngine(QWidget* _parent, QMutex* _qmutexSysGUI, QWaitCondition* _qWaitCondition) : 
    parent(_parent), qmutexSysGUI(_qmutexSysGUI), qWaitCondition(_qWaitCondition) {
    clsFEDataSampler = NULL;
    iSamplerInterval = 1;
//--    iSamplerCount = 0;
    iRoundCounter = 0;
//--    iSamplerUpdatesCounter = 0;
    stopped = FALSE;
    bSyncPlots = false;

}

void ClsFEComputeEngine::setDataSampler(ClsFEDataSampler* _clsFEDataSampler, int iInterval/*, int iCount*/){
#ifdef DEBUG_CLSFECOMPUTEENGINE 
    cout << "ClsFEComputeEngine::setDataSampler(ClsFEDataSampler* _clsFEDataSampler, int iInterval)" << endl;
#endif
    clsFEDataSampler = _clsFEDataSampler;
    iSamplerInterval = iInterval;
}


void ClsFEComputeEngine::prepare(bool _bSyncPlots ) {
#ifdef DEBUG_CLSFECOMPUTEENGINE 
    cout << "ClsFEComputeEngine::prepare( )" << endl;
#endif

    bSyncPlots = _bSyncPlots;

    for(miGroup = ClsFESystemManager::Instance()->mapFEGroups.begin(); miGroup!=ClsFESystemManager::Instance()->mapFEGroups.end(); ++miGroup){
//	    cout << "prepare group: " << miGroup->first << endl;
//	    miGroup->second->setMutex(&mutex);
	miGroup->second->setMutex(qmutexSysGUI);
    }
    
    /* loop here over process to start the (potentially) threaded modules */
    for(miProcess = ClsFESystemManager::Instance()->mapFEProcesses.begin(); miProcess!=ClsFESystemManager::Instance()->mapFEProcesses.end(); ++miProcess){
	/* 20040227 */
	miProcess->second->setMutex(qmutexSysGUI);
	/* --------- */
	miProcess->second->startModule();
    }
    
//	cout << "exit ClsFEComputeEngine::prepare( )" << endl;
//--	iSamplerUpdatesCounter = 0;

}

void ClsFEComputeEngine::run( ) {
#ifdef DEBUG_CLSFECOMPUTEENGINE
	cout << "ClsFEComputeEngine::run( ) " << endl;
#endif


	struct timeval sTimevalCurrent;
	struct timezone sTimezone;
	double fDeltaTime, fArrivalTime;
	long int iSleep;
	
	gettimeofday(&sTimevalCurrent, &sTimezone);
	double fLastUpdate = sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1000000.;
	double fStartTime = fLastUpdate;
	double fCurrentTime = fLastUpdate;
	double fCPS = ClsFESystemManager::Instance()->getCyclesPerSecond();
	static const int iSpeedMeasureInterval = 100;


	for(;;){

#ifdef DEBUG_CLSFECOMPUTEENGINE
	cout << "updating groups:" << endl;
#endif
	for(miGroup = ClsFESystemManager::Instance()->mapFEGroups.begin(); miGroup!=ClsFESystemManager::Instance()->mapFEGroups.end(); ++miGroup){
#ifdef DEBUG_CLSFECOMPUTEENGINE
//	    cout << "\tupdate: " << miGroup->second->getGroupID() << endl;
#endif
	    miGroup->second->update();
	}

//                       /* apply perturb patterns (could also happen in the group update loop... */
// 	mutex.lock(); /* prevent setting the pattern at this point in time (will this schedule requests??? )*/
// 	for(miGroupManipPattern = mapGroupManipPattern.begin(); miGroupManipPattern!=mapGroupManipPattern.end(); ++miGroupManipPattern){
// //	    cout << "miGroupManipPattern->second.getInterval(): " << miGroupManipPattern->second.getInterval() << endl;
// 	    if( miGroupManipPattern->second.bDoUpdate){
// 		if( iRoundCounter%miGroupManipPattern->second.getInterval() == 0){
// 		    if( miGroupManipPattern->second.getMode() == ClsGroupManipPattern::MODE_ADD  ){
// 			cout << "miGroupManipPattern->second[0]: " << endl;
// 			cout << miGroupManipPattern->second[0] << endl;
// 		    }
// 		    else if( miGroupManipPattern->second.getMode() == ClsGroupManipPattern::MODE_MULTIPLY  ){
// 			cout << "miGroupManipPattern->second[0]: " << endl;
// 			cout << miGroupManipPattern->second[0] << endl;		    
// 		    } else {
// 			cout << "miGroupManipPattern->second[0]: " << endl;
// 			cout << miGroupManipPattern->second[0] << endl;		    
// 		    }
// 			miGroupManipPattern->second.advance();
// 		} 
// 	    }
// 	}
// 	mutex.unlock();

//	cout << "==============================================================" << endl;



	for(miProcess = ClsFESystemManager::Instance()->mapFEProcesses.begin(); miProcess!=ClsFESystemManager::Instance()->mapFEProcesses.end(); ++miProcess){
#ifdef DEBUG_CLSFECOMPUTEENGINE
//	    cout << "\tupdate: " << miProcess->second->getProcessID() << endl;
#endif
	    miProcess->second->update();
	}


#ifdef DEBUG_CLSFECOMPUTEENGINE
	cout << "updating  connections" << endl;
#endif
	for(miConnection = ClsFESystemManager::Instance()->mapFEConnections.begin(); miConnection!=ClsFESystemManager::Instance()->mapFEConnections.end(); ++miConnection){
#ifdef DEBUG_CLSFECOMPUTEENGINE
	    cout << "\t" << miConnection->second->getConnectionID() << endl;
#endif
	    miConnection->second->update();
	}
	
	if(clsFEDataSampler!=NULL && iSamplerInterval > 0){
//	    cout << "iSamplerInterval: " << iSamplerInterval << endl;
	    if(iRoundCounter%iSamplerInterval == 0 ){
//		    clsFEDataSampler->saveData();
		    QCustomEvent *qceDS = new QCustomEvent(ClsFEDataSampler::EVENT_SAVEDATA);
		    QApplication::postEvent( clsFEDataSampler, qceDS ); 
	    }
	}

	
	// NEW 20050131 -> try avoiding to reinstantiate the event at every update loop...
	if(bSyncPlots){
	    QCustomEvent *qceDM = new QCustomEvent(ClsFEDataManager::EVENT_UPDATE);
	    QApplication::postEvent( ClsFEDataManager::Instance(), qceDM ); 
	}



	iRoundCounter++;
#ifdef DEBUG_CLSFECOMPUTEENGINE
	cout << "iRoundCounter: " << iRoundCounter << endl;
#endif

	if((iRoundCounter%iSpeedMeasureInterval)==0){
//	    cout << "----------" << endl << "iRoundCounter: " << iRoundCounter << endl;
	    gettimeofday(&sTimevalCurrent, &sTimezone);
	    fCurrentTime = sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1e6;
	    fDeltaTime = fCurrentTime-fLastUpdate;
	    fLastUpdate = fCurrentTime;

//	    cout << "fDeltaTime (sec): " << fDeltaTime << endl;

	    if(fDeltaTime>0.0000000000000001){
		/* double 2008/08/03 */ fCPSCurrent = (double)iSpeedMeasureInterval / fDeltaTime;
		/* post the update speed to SystemGUI */
		ClsQCPSCustomEvent *qce = new ClsQCPSCustomEvent(fCPSCurrent);
		QApplication::postEvent( parent, qce ); 
	    }
	}
	


	qmutexSysGUI->lock();
	if (stopped) {
	    stopped = FALSE;
	    qmutexSysGUI->unlock();
	    break;
	}
	qmutexSysGUI->unlock();


	if(fCPS>0){
	    /* jitter correction: */
	    fArrivalTime = fStartTime + (double)iRoundCounter / fCPS;

	    /* no jitter correction: */
//	    fArrivalTime = fCurrentTime + 1. / fCPS;

	    gettimeofday(&sTimevalCurrent, &sTimezone);
	    fCurrentTime = sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1e6;
	    iSleep = (int)((fArrivalTime-fCurrentTime)*1e6);
//	    cout << "fArrivalTime: " << fStartTime + (double)iRoundCounter / fCPS << endl;
//	    cout << "fCurrentTime: " << sTimevalCurrent.tv_sec + (double)sTimevalCurrent.tv_usec/1e6 << endl;
//	    cout << "iSleep: " << iSleep << endl;
	    if(iSleep>0){
		usleep(iSleep);
	    }
	}
    }


    /* loop here over process to stop the (potentially) threaded modules */
    for(miProcess = ClsFESystemManager::Instance()->mapFEProcesses.begin(); miProcess!=ClsFESystemManager::Instance()->mapFEProcesses.end(); ++miProcess){
	miProcess->second->stopModule();
    }

    cerr << "exit run()" << endl;
    qWaitCondition->wakeAll();

}


void ClsFEComputeEngine::stop() {
#ifdef DEBUG_CLSFECOMPUTEENGINE 
    cout << "ClsFEComputeEngine::stop()" << endl;
#endif
    qmutexSysGUI->lock();
    stopped = TRUE;
    qmutexSysGUI->unlock();

    iRoundCounter = 0;

}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
 


