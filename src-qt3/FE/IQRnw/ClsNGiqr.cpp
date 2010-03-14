#include "ClsNGiqr.h"

#include <ClsSysFileParserException.h>
#include "ClsQLogWindow.h"

ClsNQiqr::ClsNQiqr(){
    strSystemFileName = "";
    ClsFESystemManager::initializeSystemManager();
    ClsFEComputeEngine::initializeComputeEngine(NULL, &qmutexSysGUI, &qWaitCondition); 

    ClsQLogWindow::initializeLogger();
}


void ClsNQiqr::openSystem(string s){
	strSystemFileName = s;
	if(!buildSystem()){
	}
}

bool ClsNQiqr::buildSystem() {
#ifdef DEBUG_IQRMAINWINDOW
    cout << "ClsNQiqr::buildSystem()" << endl;
#endif
    ClsFESystemManager::Instance()->clsFESystemBuilder->readSystemFileFromDisk(strSystemFileName);
    try {
	ClsFESystemManager::Instance()->clsFESystemBuilder->parseSystemFile( );
    }
    catch (ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" in system file parser\n" + e.getMessage() ;
//	reportError(strError);
	return true;
    }

    try{
	ClsFESystemManager::Instance()->buildSystem( );
    }
    catch (ClsSysFileParserException &e){
	cerr << e.getMessage() << endl;
	string strError = (string)" reading system file\n" + e.getMessage() ;
//	reportError(strError);
	return true;
    }
#ifdef DEBUG_IQRMAINWINDOW
    cout << "done building system" << endl;
#endif
    return false;

};

int ClsNQiqr::startSimulation() {
#ifdef DEBUG_CLSNQIQR
    cout << "ClsNQiqr::startSimulation()" << endl;
#endif
    int iError = 0;

    if(ClsFESystemManager::Instance()->isValidSystem(false)){
	try{
	    ClsFESystemManager::Instance()->initSystem();
	}
	catch(runtime_error &e){
	    cerr << "System Error: " << e.what() << endl;
	    iError = -1;
	}
	
	if(!iError){
	    ClsFEComputeEngine::Instance()->prepare();
	    ClsFEComputeEngine::Instance()->start();
	}
	
    } else {
	iError = -1;
    }
    return iError;
}


void ClsNQiqr::stopSimulation() {
#ifdef DEBUG_CLSNQIQR
    cout << "ClsNQiqr::stopSimulation()" << endl;
#endif

    ClsFEComputeEngine::Instance()->stop();
    cout << "ClsSystemGUI::waiting..." << endl;
    qWaitCondition.wait();
    
    ClsFESystemManager::Instance()->cleanupSystem();
}
