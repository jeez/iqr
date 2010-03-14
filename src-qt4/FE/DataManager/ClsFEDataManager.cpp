/****************************************************************************
 ** $Filename: ClsFEDataManager.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Sat Sep 15 21:00:43 2001
 ** $Date: 2004/02/02 16:48:03 $
 **
 ** $log:
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <list>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "ClsFEDataManager.h"
#include <ClsFEDataClient.h>
#include <ClsFEGroupPlot.h>
#include <ClsFETimePlot.h>
#include <ClsFEDataSampler.h>

#include "ClsQLogWindow.h"
#include "ClsFEConnectionDiagram.h"

#include <ClsFEMessageOut.h>

#include <ClsFESystemManager.h>

#include <iqrUtils.h>

#include "ConfigTagLibrary.h"
#include <ClsFEDataSink.h>

#include "ClsDataClientConfigReader.h"
#include "ClsDataClientConfigWriter.h"
#include "ClsFEGroup.h"
#include "diagramTypes.h"

//#define DEBUG_CLSFEDATAMANAGER

ClsFEDataManager*  ClsFEDataManager::_instanceDataManager = NULL;

void ClsFEDataManager::initializeDataManager(QWidget *_parent, const char * name, QMutex* _qmutexSysGUI){
    _instanceDataManager = new ClsFEDataManager(_parent, name, _qmutexSysGUI);
}


ClsFEDataManager* ClsFEDataManager::Instance(){
    return _instanceDataManager;
}



ClsFEDataManager::ClsFEDataManager(QWidget *_parent, const char * name, QMutex* _qmutexSysGUI) :
    QObject ( _parent, name ), parent(_parent), qmutexSysGUI(_qmutexSysGUI) {
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::ClsFEDataManager(...)" << endl;
#endif
    iDataClientCounter = 0;

#ifdef SINGLE_DATASAMPLER
    clsFEDataSampler = NULL;
    strDataSamplerID = "";
#endif

    bSyncPlots = false;

    qtimerPaceMaker = new QTimer( this );
    connect( qtimerPaceMaker, SIGNAL(timeout()), this, SLOT(updateClients()) );

};


void ClsFEDataManager::DataClientCreate(int iClientType , string strSystemElementID, string strParamName, string strRange){
#ifdef  DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::DataClientCreate(int iClientType , string strSystemElementID, string strParamName, int iRange)" << endl;
#endif



    string strBaseID =   ClsIDGenerator::Instance()->Next();//.latin1();

    string strClientID = "";
    strParamName = "";


    if(iClientType  == ClsFEDataClient::CLIENT_SPACEPLOT){
	string strPrcID = "";
	ClsFEGroup *clsFEGroup = NULL;
	if(strSystemElementID.size() > 0){
	    clsFEGroup = ClsFESystemManager::Instance()->getFEGroup(strSystemElementID);

	}

	if(clsFEGroup!=NULL){
	    string strValid = clsFEGroup->validate();
	    if(strValid.size()>0){
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strValid);
	    }
	    else {
		strPrcID = clsFEGroup->getProcessID();
//		strClientID = "GroupPlot:" + strPrcID + (string)":" + strSystemElementID + (string)":" + iqrUtils::int2string(iDataClientCounter);
		strClientID = "GroupPlot:" + strBaseID + (string)":" + iqrUtils::int2string(iDataClientCounter);

		ClsFEBasePlot* clsFEBasePlot = new ClsFEGroupPlot(this, qmutexSysGUI, strClientID, clsFEGroup);
		pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
		mapPlots.insert(pairTemp);
		connect(dynamic_cast<ClsFEPlotFramework*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			this, SLOT( slotPlotClosed(string)));
	    }
	}
    }
    else if(iClientType  == ClsFEDataClient::CLIENT_TIMEPLOT){
	string strPrcID = "";
	ClsFEGroup *clsFEGroup = NULL;
	if(strSystemElementID.size() > 0){
	    clsFEGroup = ClsFESystemManager::Instance()->getFEGroup(strSystemElementID);
	}

	if(clsFEGroup!=NULL){
	    string strValid = clsFEGroup->validate();
	    if(strValid.size()>0){
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strValid);
	    }
	    else {
		strPrcID = clsFEGroup->getProcessID();
//		strClientID = "TimePlot:" + strPrcID + (string)":" + strSystemElementID + (string)":" + iqrUtils::int2string(iDataClientCounter);
		strClientID = "TimePlot:" + strBaseID + (string)":" + iqrUtils::int2string(iDataClientCounter);

		ClsFEBasePlot* clsFEBasePlot = new ClsFETimePlot(this, qmutexSysGUI, strClientID);
		
		if(strSystemElementID.size() > 0){
		    string strOutputStateName = clsFEGroup->getNeuronOutput()->getName(); 
		    list<string> lstSelectedStates; 
		    lstSelectedStates.push_back(strOutputStateName); 
		    dynamic_cast<ClsFETimePlot*>(clsFEBasePlot)->addStateVariableDisplay(clsFEGroup, lstSelectedStates, strRange);
		}
		pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
		mapPlots.insert(pairTemp);
		connect(dynamic_cast<ClsFEPlotFramework*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			this, SLOT( slotPlotClosed(string)));
	    }
	}
	
    }
    else if(iClientType  == ClsFEDataClient::CLIENT_DATASAMPLER){
	string strPrcID = "";
	ClsFEGroup *clsFEGroup = NULL;
	if(strSystemElementID.size() > 0){
	    clsFEGroup = ClsFESystemManager::Instance()->getFEGroup(strSystemElementID);
	    strPrcID = clsFEGroup->getProcessID();
	}

//	strClientID = "DataSampler:" + strPrcID + (string)":" + strSystemElementID + (string)":" + iqrUtils::int2string(iDataClientCounter);
	strClientID = "DataSampler:" + strBaseID + (string)":" + iqrUtils::int2string(iDataClientCounter);


#ifdef SINGLE_DATASAMPLER
	if(clsFEDataSampler!= NULL){
	    clsFEDataSampler->raise();
	} else {
	    clsFEDataSampler = new ClsFEDataSampler(this, qmutexSysGUI, strClientID);
	    strDataSamplerID = strClientID;
	    connect(clsFEDataSampler, SIGNAL(sigPlotClosed(string)),
		    this, SLOT( slotPlotClosed(string)));
	}
#else
	ClsFEDataSampler* clsFEDataSampler = new ClsFEDataSampler(this, strClientID);
	pair<string, ClsFEDataSampler*> pairTemp(strClientID, clsFEDataSampler);
	mapDataSamplers.insert(pairTemp);
	connect(dynamic_cast<ClsFEPlotFramework*>(clsFEDataSampler), SIGNAL(sigPlotClosed(string)),
		this, SLOT( slotPlotClosed(string)));
#endif
    }

    else if(iClientType== diagramTypes::DIAGRAM_CONNECTION){

	string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strSystemElementID);
	    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strSystemElementID);

//	    strClientID = "ConnectionDiagram:" + strSystemElementID + (string)":" + iqrUtils::int2string(iDataClientCounter);
	    strClientID = "ConnectionDiagram:" + strBaseID + (string)":" + iqrUtils::int2string(iDataClientCounter);


	    if(strSourceID.length()<=0 && strTargetID.length()<=0){
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, 
						  "Cannot create diagram.\n"
						  "Source and Target Groups do not exist." );
	    }
	    else if(strSourceID.length()<=0){
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, 
						  "Cannot create diagram.\n"
						  "Source Group does not exist.");
		
	    } else if(strTargetID.length()<=0){
		ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, 
						  "Cannot create diagram.\n"
						  "Target Group does not exist.");
	    } else {

		ClsFEConnection *clsFEConnection = NULL;
		if(strSystemElementID.size() > 0){
		    clsFEConnection = ClsFESystemManager::Instance()->getFEConnection(strSystemElementID);
		}
		
		if(clsFEConnection!=NULL){
		    
		    string strValid = clsFEConnection->validate();
		    if(strValid.size()>0){
			ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strValid);
		    }
		    else {
			
			
			
			ClsFEBasePlot *clsFEBasePlot = new ClsFEConnectionDiagram(this, qmutexSysGUI, strClientID, strSystemElementID);
			dynamic_cast<ClsFEConnectionDiagram*>(clsFEBasePlot)->createDiagram();
			dynamic_cast<ClsFEConnectionDiagram*>(clsFEBasePlot)->addStateVariableDisplay(clsFEConnection);
			dynamic_cast<ClsFEConnectionDiagram*>(clsFEBasePlot)->show();
			pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
			mapPlots.insert(pairTemp);
			connect(dynamic_cast<ClsFEPlotFramework*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			this, SLOT( slotPlotClosed(string)));
		    }
		}
	    }
    }


//     pair<QWidget*, ClsFEDataClient*> pairTemp1(qwidget, clsFEDataClientTemp);
//     pair<string, pair<QWidget*, ClsFEDataClient*> > pairTemp2(strClientID, pairTemp1);
//     mapPlots.insert(pairTemp2);



    iDataClientCounter++;
};


ClsDataSinkCopying* ClsFEDataManager::createDataSink(ClsItem* clsItem, string strParamName, string strRange ) {
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::createDataSink( string strGroupID, string strParamName, string strRange )" << endl;
#endif

/* send new data request to the ClsFECommHandler */
//	  MESSAGE_SET_VALUE,
//	  MESSAGE_REQ_PERM,
//	  MESSAGE_REQ_VOL,
//	  MESSAGE_CANCEL_REQ


/*
    cout << "strRange: " << strRange << endl;
    cout << "strParamName: " << strParamName << endl;
    cout << "group name: " << clsItem << endl;
*/

    ClsDataSinkCopying *clsDataSink = NULL;
    StateArray* pStateArray = NULL;


    if(dynamic_cast<ClsFEGroup*>(clsItem)){
//	cout << "got group" << endl;
	ClsNeuron *pNeuron = dynamic_cast<ClsFEGroup*>(clsItem)->getNeuron();

	try{
	    pStateArray = &(pNeuron->getState(strParamName)->getStateArray());
	} catch(...) {
	    cerr << "Unknown state variable" << endl;
	}
	string strGroupID = dynamic_cast<ClsFEGroup*>(clsItem)->getGroupID();

//	cout << "strGroupID : " << strGroupID << endl;

	if(strRange.length()>0){
	    clsDataSink = new ClsDataSinkCopying(strGroupID, strParamName, pStateArray, strRange);
 	} else {
	    clsDataSink = new ClsDataSinkCopying(strGroupID, strParamName, pStateArray);
	}



    } else if(dynamic_cast<ClsFEConnection*>(clsItem)){
//	cout << "got connection" << endl;
	string strConnectionID = dynamic_cast<ClsFEConnection*>(clsItem)->getConnectionID();
	ClsSynapse *pSynapse = dynamic_cast<ClsFEConnection*>(clsItem)->getSynapse();
	try{
	    pStateArray = &(pSynapse->getState(strParamName)->getStateArray());
	} catch(...) {
	    cerr << "Unknown state variable" << endl;
	}
	clsDataSink = new ClsDataSinkCopying(strConnectionID, strParamName, pStateArray);
    }



/* not needed if no network
   ClsFEMessageOut clsFEMessageOut ( ClsProtocol::PROTO_CMD_REQUEST_PERMANENT, strGroupID, strParamName, clsDataSink );
   Mediator->addDataRequest(strPrcID, clsFEMessageOut); //KILL
*/

    return clsDataSink;
};


//////////////



void ClsFEDataManager::slotItemChanged(int iType, string strID ) {
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::slotItemChanged()" << endl;
#endif

    if(iType == ClsFESystemManager::ITEM_GROUP){
	map<string, ClsFEBasePlot*>::iterator it;
	for(it=mapPlots.begin(); it != mapPlots.end(); it++){
	    it->second->groupChanged(strID);
	}

#ifdef SINGLE_DATASAMPLER
	if(clsFEDataSampler!=NULL){
	    clsFEDataSampler->groupChanged(strID);
	}
#endif
    } else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	map<string, ClsFEBasePlot*>::iterator it;
	for(it=mapPlots.begin(); it != mapPlots.end(); it++){
	    it->second->connectionChanged(strID);
	}
    }

}


void ClsFEDataManager::slotItemDeleted(int iType, string strID ) {
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::slotItemDeleted(int iType, string strID )" << endl;
#endif

    if(iType == ClsFESystemManager::ITEM_GROUP){
	map<string, ClsFEBasePlot*>::iterator mapIteratorPlots;
	for(mapIteratorPlots = mapPlots.begin(); mapIteratorPlots!=mapPlots.end(); ++mapIteratorPlots){
	    mapIteratorPlots->second->groupDeleted(strID);
	}

#ifdef SINGLE_DATASAMPLER
	if(clsFEDataSampler!=NULL){
	    clsFEDataSampler->groupDeleted(strID);
	}
#endif
    } else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	map<string, ClsFEBasePlot*>::iterator mapIteratorPlots;
	for(mapIteratorPlots = mapPlots.begin(); mapIteratorPlots!=mapPlots.end(); ++mapIteratorPlots){
	    mapIteratorPlots->second->connectionDeleted(strID);
	}
    }
}





void ClsFEDataManager::cancelDataRequest( ClsDataSinkCopying* /* clsFEDataSink */, string /* strGroupID */,
					  string /* strParamName */, string /* strRange*/  ) {
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::cancelDataRequest( ClsDataSinkCopying* clsFEDataSink, string strGroupID, string strParamName, string strRange)" << endl;
#endif
/*
  ClsFEGroup* clsFEGroup = ClsFESystemManager::Instance()->getFEGroup(strGroupID);
  string strPrcID = ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getProcessID();
  ClsFEMessageOut clsFEMessageOut ( ClsFEMessageOut::MESSAGE_CANCEL_REQ , strPrcID, strGroupID, strParamName, clsFEDataSink );
  Mediator->addDataRequest(clsFEMessageOut); //KILL
*/

};

void ClsFEDataManager::customEvent(QEvent *e){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::customEvent(QEvent *e)" << endl;
#endif
    if(e->type() == EVENT_UPDATE){
	updateClients();
    }

}

void ClsFEDataManager::updateClients(){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::updateClients()" << endl;
#endif

    if(!bPaused){
	map<string, ClsFEBasePlot*>::iterator mapIteratorPlots;
	for(mapIteratorPlots = mapPlots.begin(); mapIteratorPlots!=mapPlots.end(); ++mapIteratorPlots){
	    mapIteratorPlots->second->update();
	}
    }

};


void ClsFEDataManager::start(bool _bSyncPlots){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::start()" << endl;
#endif

    bPaused = false;
    bSyncPlots = _bSyncPlots;

    map<string, ClsFEBasePlot*>::iterator mapIteratorPlots;
    for(mapIteratorPlots = mapPlots.begin(); mapIteratorPlots!=mapPlots.end(); ++mapIteratorPlots){
	mapIteratorPlots->second->init();
    }

    startDataSampler();

    if(!bSyncPlots){
	qtimerPaceMaker->start(100, FALSE); 
    }
};

void ClsFEDataManager::stop(){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::stop()" << endl;
#endif

    stopDataSampler();

    if(!bSyncPlots){
	qtimerPaceMaker->stop(); 
    }
    bPaused = false;
};


void ClsFEDataManager::pause(bool b){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::pause(bool b)" << endl;
#endif

    bPaused = b;
};



void ClsFEDataManager::startDataSampler(bool bForce){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::startDataSampler()" << endl;
#endif

#ifdef SINGLE_DATASAMPLER
    if(clsFEDataSampler!=NULL){
/* this should be handeled in the DataSampler
	clsFEDataSampler->init();
*/
	if(clsFEDataSampler->isAutoStart() || bForce)	{
	    clsFEDataSampler->slotStartSampling();
	}
    }
#else
    map<string, ClsFEDataSampler*>::iterator it;
    for(it=mapDataSamplers.begin(); it!=mapDataSamplers.end(); it++){
	if(it->second->isAutoStart() || bForce) {
	    it->second->slotStartSampling();
	}
    }
#endif
}

void ClsFEDataManager::stopDataSampler(bool bForce){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::stopDataSampler()" << endl;
#endif
#ifdef SINGLE_DATASAMPLER
    if(clsFEDataSampler!=NULL){
	if(clsFEDataSampler->isAutoStart() || bForce) {
	    clsFEDataSampler->slotStopSampling();
	}
    }
#else
    map<string, ClsFEDataSampler*>::iterator it;
    for(it=mapDataSamplers.begin(); it!=mapDataSamplers.end(); it++){
	if(it->second->isAutoStart() || bForce)	{
	    it->second->slotStopSampling();
	}
    }
#endif

}

void ClsFEDataManager::slotPlotClosed(string strID){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::slotPlotClosed(string strID)" << endl;
#endif


    if(!strID.compare(strDataSamplerID)){
	clsFEDataSampler = NULL;
    } else {
	map<string, ClsFEBasePlot*>::iterator it;
	it = mapPlots.find(strID);
	if(it!=mapPlots.end()){
	    mapPlots.erase(it);
	}
    }
//    cout << "mapPlots.size(): " << mapPlots.size() << endl;
}

void ClsFEDataManager::closeAllPlots(){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::closeAllPlots()" << endl;
#endif
//TEMP    map<string, ClsFEBasePlot*>::iterator itPlots;
//TEMP    for(itPlots = mapPlots.begin(); itPlots != mapPlots.end(); itPlots++){
//TEMP	if(itPlots->second!=NULL){
//TEMP	    itPlots->second->close();
//TEMP	}
//TEMP    }

#ifdef SINGLE_DATASAMPLER 
    if(clsFEDataSampler!=NULL){
	clsFEDataSampler->close();
    }
#else
    map<string, ClsFEDataSampler*>::iterator itDataSampler;
    for(itDataSampler = mapDataSamplers.begin(); itDataSampler != mapDataSamplers.end(); itDataSampler++){
	if(itDataSampler->second!=NULL){
	    itDataSampler->second->close();
	}
    }

#endif



}

void ClsFEDataManager::saveConfig(){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::saveConfig()" << endl;
#endif

    QString qstrFileName = "";
    bool bValidFile = false;

    while(!bValidFile){
//zzz	qstrFileName = QFileDialog::getSaveFileName(
//zzz	    "",
//zzz	    "configuration (*.conf)",
//zzz	    0,
//zzz	    "save file dialog"
//zzz	    "Choose a filename to save under" );
//zzz
	qstrFileName = QFileDialog::getSaveFileName(
	    0, /*parent*/
	    "save file dialog"
	    "Choose a filename to save under",  /*caption*/
	    "", /* dir? */
	    "configuration (*.conf)" /* filter */
	    );


	/* append default extension here */
	QString qstrExtension( ".conf" );
	QString qstrCurrentTail = qstrFileName.right(qstrExtension.length());
	if(qstrCurrentTail.compare(qstrExtension)){
	    qstrFileName.append(qstrExtension);
	} 
	/* ------------------------------ */




	QFileInfo qFileInfo(qstrFileName );
	QString qstrPath = qFileInfo.dirPath(true);

	qFileInfo.setFile (qstrPath);
	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( 0, "iqr",
						 "No permission to write in this directory\n",
						 "Retry", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return;
	    }
	} else {
	    bValidFile = true;
	}
    }



    
    if(qstrFileName.length()<=0){
	return;
    }



    if ( QFile::exists(qstrFileName) ) {
/*      warning on overwrite */
	int iReturn = QMessageBox::warning( 0, "iqr",
					    "A file with the seleteced name exists\n",
					    "Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
	    return;
	}
    }


    string strFileName = qstrFileName.latin1();

    list<ClsDataClientConfig> lstConfigs;

    map<string, ClsFEBasePlot*>::iterator it;
    for(it=mapPlots.begin(); it != mapPlots.end(); it++){
//	it->second->print();
//	    cout << "====================" << endl;
	ClsDataClientConfig clsDataClientConfig = it->second->getConfig();

	string strType = "";
	if(dynamic_cast<ClsFEGroupPlot*>(it->second)){
	    strType = ConfigTagLibrary::PlotTypeGroup();
	} else if(dynamic_cast<ClsFETimePlot*>(it->second)){
	    strType = ConfigTagLibrary::PlotTypeTime();
	} else if(dynamic_cast<ClsFEConnectionDiagram*>(it->second)){
	    strType = ConfigTagLibrary::PlotTypeConnection();
/*	} else if(dynamic_cast<>()){
	strType = ConfigTagLibrary::PlotTypeCorrelation(); */
	} else {
	    strType = "UnknownType";
	}
	clsDataClientConfig.setType(strType);
	lstConfigs.push_back(clsDataClientConfig);
    }

#ifdef SINGLE_DATASAMPLER
    if(clsFEDataSampler!=NULL){
	ClsDataClientConfig clsDataClientConfigDataSampler = clsFEDataSampler->getConfig();
	clsDataClientConfigDataSampler.setType(ConfigTagLibrary::DataSampler());
	lstConfigs.push_back(clsDataClientConfigDataSampler);
    }
#endif


    ClsDataClientConfigWriter clsDataClientConfigWriter;
    if(!clsDataClientConfigWriter.saveConfig(strFileName, lstConfigs)){
	string strError = "Error writing configuration\n" ;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_WARNING, strError);
    }


};

void ClsFEDataManager::applyConfig(string strFileName){
#ifdef DEBUG_CLSFEDATAMANAGER
    cout << "ClsFEDataManager::applyConfig()" << endl;
#endif

    bool bError = false;

    if(strFileName.size() <= 0){
//zzz	QString qstrFileName = QFileDialog::getOpenFileName(
//zzz	    "",
//zzz	    "configuration (*.conf)",
//zzz	    0,
//zzz	    "open file dialog"
//zzz	    "Choose a file" );
//zzz
	QString qstrFileName = QFileDialog::getOpenFileName(
	    0, /*parent*/
	    "open file dialog"
	    "Choose a file", /*caption*/
	    "", /*dir*/
	    "configuration (*.conf)" /*filter*/
	    );

	if(qstrFileName.length() > 0){
	    strFileName = qstrFileName.latin1();
	} else {
	    return;
	}
    }

    if(strFileName.size() > 0){
	ClsDataClientConfigReader clsDataClientConfigReader;

	list<ClsDataClientConfig> lstConfigs;
	try {
	    lstConfigs = clsDataClientConfigReader.getDataClientConfig(strFileName);
	}
	catch (ClsDataClientConfigReaderException &e) {
	    string strError = (string)"Error reading configuration\n" + e.getMessage() ;
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strError);
	}


	if(!bError){
	    list<ClsDataClientConfig>::iterator itConfigs;
	    for(itConfigs=lstConfigs.begin(); itConfigs != lstConfigs.end(); itConfigs++){
		string strType =  (*itConfigs).getType();
//		cout << "strType: " << strType << endl;
		int iClientType = -99;

		if(!strType.compare(ConfigTagLibrary::PlotTypeGroup())){
		    iClientType = ClsFEDataClient::CLIENT_SPACEPLOT;
		}
		else if(!strType.compare(ConfigTagLibrary::PlotTypeTime())){
		    iClientType = ClsFEDataClient::CLIENT_TIMEPLOT;
		}
		else if (!strType.compare(ConfigTagLibrary::PlotTypeConnection())){
		    iClientType = diagramTypes::DIAGRAM_CONNECTION;
		}
		else if (!strType.compare(ConfigTagLibrary::DataSampler())){
		    iClientType = ClsFEDataClient::CLIENT_DATASAMPLER;
		}

//		string strCaption = (*itConfigs).getCaption();

		string strClientID = (*itConfigs).getID();

		pair<int,int> pPosition =  (*itConfigs).getPosition();
		pair<int,int> pGeometry =  (*itConfigs).getGeometry();



		if(iClientType  == ClsFEDataClient::CLIENT_SPACEPLOT){
/*		    strClientID = "GroupPlot:" + strPrcID + (string)":" + strGroupID + (string)":" + iqrUtils::int2string(iDataClientCounter); */
		    ClsFEBasePlot* clsFEBasePlot = new ClsFEGroupPlot(this, qmutexSysGUI, strClientID);
		    clsFEBasePlot->setConfig((*itConfigs));
		    clsFEBasePlot->resize(pGeometry.first, pGeometry.second);
		    clsFEBasePlot->move(pPosition.first, pPosition.second);

		    pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
		    mapPlots.insert(pairTemp);
		    connect(dynamic_cast<ClsFEPlotFramework*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			    this, SLOT( slotPlotClosed(string)));
		} else if(iClientType  == ClsFEDataClient::CLIENT_TIMEPLOT){
/*		    strClientID = "TimePlot:" + strPrcID + (string)":" + strGroupID + (string)":" + iqrUtils::int2string(iDataClientCounter); */
		    ClsFEBasePlot* clsFEBasePlot = new ClsFETimePlot(this, qmutexSysGUI,  strClientID);
		    clsFEBasePlot->setConfig((*itConfigs));
		    clsFEBasePlot->resize(pGeometry.first, pGeometry.second);
		    clsFEBasePlot->move(pPosition.first, pPosition.second);

		    pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
		    mapPlots.insert(pairTemp);
		    connect(dynamic_cast<ClsFEPlotFramework*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			    this, SLOT( slotPlotClosed(string)));
		} else if(iClientType== diagramTypes::DIAGRAM_CONNECTION){
//		    cout << "CREATE CONNECTION DIAGRAM" << endl;
		    ClsFEBasePlot *clsFEBasePlot = new ClsFEConnectionDiagram(this, qmutexSysGUI, strClientID, "");
		    clsFEBasePlot->setConfig((*itConfigs));
		    clsFEBasePlot->resize(pGeometry.first, pGeometry.second);
		    clsFEBasePlot->move(pPosition.first, pPosition.second);

		    pair<string, ClsFEBasePlot*> pairTemp(strClientID, clsFEBasePlot);
		    mapPlots.insert(pairTemp);
		    connect(dynamic_cast<ClsFEConnectionDiagram*>(clsFEBasePlot), SIGNAL(sigPlotClosed(string)),
			    this, SLOT( slotPlotClosed(string)));
		} else if(iClientType  == ClsFEDataClient::CLIENT_DATASAMPLER){
		    if(clsFEDataSampler!= NULL){
			clsFEDataSampler->raise();
		    } else {
			clsFEDataSampler = new ClsFEDataSampler(this, qmutexSysGUI, strClientID);
			strDataSamplerID = strClientID;
			connect(clsFEDataSampler, SIGNAL(sigPlotClosed(string)),
				this, SLOT( slotPlotClosed(string)));
		    }
		    clsFEDataSampler->setConfig((*itConfigs));
		    clsFEDataSampler->resize(pGeometry.first, pGeometry.second);
		    clsFEDataSampler->move(pPosition.first, pPosition.second);
		}
	    }
	}



    }

};


//	cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
