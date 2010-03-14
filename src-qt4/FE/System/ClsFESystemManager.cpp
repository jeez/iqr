/****************************************************************************
 ** $Filename: ClsFESystemManager.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Feb 19 18:06:06 2002
 **
 **
 *****************************************************************************/


#include <map>
#include <string>
#include <time.h>
#include <stdexcept>

#include <stdexcept>
#include "ClsSysFileNode.h"
#include "ClsFESystemManager.h"
#include "ClsFESystemBuilder.h"
#include "ClsPortFinder.h"
#include "ClsFESerializer.h"
#include "tagLibrary.hpp"
#include "iqrUtils.h"


#include "ClsFESystem.h"
#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"
#include "ClsQLogWindow.h"


//#define DEBUG_CLSFESYSTEMMANAGER

ClsFESystemManager*  ClsFESystemManager::_instanceSystemManager = NULL;

void ClsFESystemManager::initializeSystemManager(QWidget* _parent){
    _instanceSystemManager = new ClsFESystemManager(_parent);
}


ClsFESystemManager* ClsFESystemManager::Instance(){
    if(_instanceSystemManager == NULL){
	_instanceSystemManager = new ClsFESystemManager();
    } 
    return _instanceSystemManager;
}


ClsFESystemManager::ClsFESystemManager (QWidget* _parent) : parent(_parent) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::ClsFESystemManager ()" << endl;
#endif

    clsFESystem = new ClsFESystem();


//    clsFESystemBuilder = new ClsFESystemBuilder(this) ;
    clsFESystemBuilder = new ClsFESystemBuilder();
    bSystemChanged = false;
    bSimulationRunning = false;

    iNewProcessCounter = 0;
    iNewGroupCounter = 0;

};

bool ClsFESystemManager::systemChanged()  {
    return bSystemChanged;
}

void ClsFESystemManager::slotSimulationRunning(bool b) {
    bSimulationRunning = b;
};


map<string, ClsFEProcess*> ClsFESystemManager::getMapFEProcesses() {
    return mapFEProcesses;
};


map<string, ClsFEGroup*> ClsFESystemManager::getMapFEGroups() {
    return mapFEGroups;
};

map<string, ClsFEConnection*> ClsFESystemManager::getMapFEConnections() {
    return mapFEConnections;
};


ClsInfoDiagramIcon ClsFESystemManager::getDiagramIcon(string strID){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getDiagramIcon(string strID)" << endl;
#endif
    return clsFESystemBuilder->getDiagramIcon(strID);
};

ClsInfoDiagramLine ClsFESystemManager::getDiagramLine(string strID){
    return clsFESystemBuilder->getDiagramLine( strID);
};


void ClsFESystemManager::setInfoSystem( ClsInfoSystem _clsInfoSystem) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::setInfoSystem( ClsInfoSystem _clsInfoSystem)" << endl;
#endif
//***    clsInfoSystem = _clsInfoSystem; //***

    clsFESystem = new ClsFESystem();
    clsFESystem->setSystemID(_clsInfoSystem.getSystemID());
    clsFESystem->setParameter(ClsTagLibrary::NameTag(), _clsInfoSystem.getSystemName( ) );
    clsFESystem->setParameter(ClsTagLibrary::NotesTag(), _clsInfoSystem.getNotes( ) );
    clsFESystem->setParameter(ClsTagLibrary::AuthorTag(), _clsInfoSystem.getAuthor( ) );
    clsFESystem->setParameter(ClsTagLibrary::DateTag(), _clsInfoSystem.getDate( ) );
    clsFESystem->setParameter(ClsTagLibrary::HostnameTag(), _clsInfoSystem.getHostname(  ) );
    clsFESystem->setParameter(ClsTagLibrary::PortTag(), iqrUtils::int2string(_clsInfoSystem.getPort( )) );
    clsFESystem->setParameter(ClsTagLibrary::CyclesPerSecondTag(), iqrUtils::int2string(_clsInfoSystem.getCyclesPerSecond( )) );
    clsFESystem->setParameter(ClsTagLibrary::SyncPlotsTag(), iqrUtils::bool2string(_clsInfoSystem.getSyncPlots( )) );

}


//***ClsInfoSystem ClsFESystemManager::getInfoSystem() {
//***    return clsInfoSystem;
//***};

ClsFESystem* ClsFESystemManager::getFESystem() {
    return clsFESystem;
};


string ClsFESystemManager::getSystemFileCont() {
    return strSystemFileCont;
};

void ClsFESystemManager::itemChanged(int iType, string strID) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::itemChanged(int iType, string strID)" << endl;
#endif

    if(iType==ITEM_GROUP){ /* we should find out which connections are affected */
	map<string, ClsFEConnection*>::iterator it;
	for(it=mapFEConnections.begin();it!=mapFEConnections.end();it++){
	    if(!strID.compare(it->second->getConnectionTargetID( )) || !strID.compare(it->second->getConnectionSourceID( ))){
		if(!bSimulationRunning){
		    it->second->calculateArbPattern();
		}
		emit sigItemChanged(ITEM_CONNECTION, it->second->getConnectionID()); /* potentially tricky !! */
	    }
	}
    } else if(iType==ITEM_CONNECTION){ 
	if(!bSimulationRunning){
	    getFEConnection( strID )->calculateArbPattern();
	}
    }

    bSystemChanged = true;
    emit sigItemChanged(iType, strID);
}

void ClsFESystemManager::closeSystem() {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::closeSystem()" << endl;
#endif

    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapFEProcesses.begin(); mapIteratorProcesses!=mapFEProcesses.end(); ++mapIteratorProcesses){
	delete mapIteratorProcesses->second;
    }
    mapFEProcesses.clear();

    map<string, ClsFEGroup*>::iterator itmapFEGroups;
    for (itmapFEGroups = mapFEGroups.begin(); itmapFEGroups != mapFEGroups.end(); ++itmapFEGroups){
	delete itmapFEGroups->second;
    }
    mapFEGroups.clear();

    map<string, ClsFEConnection*>::iterator itmapFEConnections;
    for (itmapFEConnections = mapFEConnections.begin(); itmapFEConnections != mapFEConnections.end(); ++itmapFEConnections){
	delete itmapFEConnections->second;
    }
    mapFEConnections.clear();


    while (queueDeletedFEProcesses.size()) {
	queueDeletedFEProcesses.pop();
    }

    while (queueDeletedFEConnections.size()) {
	delete queueDeletedFEConnections.front();
	queueDeletedFEConnections.pop();
    }

    while (queueDeletedFEGroups.size()) {
	delete queueDeletedFEGroups.front();
	queueDeletedFEGroups.pop();
    }

    /* clean out InfoSystem */
    bSystemChanged = false;
}


void ClsFESystemManager::buildSystem(){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::buildSystem()" << endl;
#endif

    try{
	clsFESystemBuilder->buildSystem( );
    }
    catch (ClsSysFileParserException &e){
	throw;
    }

#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::buildSystem():DONE" << endl;
#endif

    emit sigSystemChanged();
    emit systemBuildt();

};



// void ClsFESystemManager::addConnection(ClsInfoConnection clsInfoConnection){
// #ifdef DEBUG_CLSFESYSTEMMANAGER
//     cout << "ClsFESystemManager::addConnection(ClsInfoConnection clsInfoConnection)" << endl;
// #endif
//     pair<string, ClsInfoConnection> pairTemp(clsInfoConnection.getConnectionID(), clsInfoConnection);
//     mapConnections.insert(pairTemp);
// }

void ClsFESystemManager::addConnection(ClsInfoConnection clsInfoConnection){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::addConnection(ClsInfoConnection clsInfoConnection)" << endl;
#endif

    string strConnectionID = clsInfoConnection.getConnectionID( );
    string strConnectionName = clsInfoConnection.getConnectionName( );
    string strConnectionSourceID = clsInfoConnection.getConnectionSourceID( );
    string strConnectionTargetID = clsInfoConnection.getConnectionTargetID( );


#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << __FILE__ << ":strConnectionID:" << strConnectionID << endl;
    cout << __FILE__ << ":strConnectionSourceID:" << strConnectionSourceID << endl;
    cout << __FILE__ << ":strConnectionTargetID:" << strConnectionTargetID << endl;
#endif

    ClsSysFileNode clsSysFileNodeConnectionSubNodes = clsInfoConnection.getSysFileNodeConnectionSubNodes();

    ClsFEConnection*  clsFEConnection = new ClsFEConnection ( strConnectionID, strConnectionName, 
							      strConnectionSourceID, strConnectionTargetID);

    clsFEConnection->setConnectionSubNodes(clsSysFileNodeConnectionSubNodes);
    clsFEConnection->setNotes(clsInfoConnection.getNotes());
    clsFEConnection->setConnectionType(clsInfoConnection.getConnectionType());
    clsFEConnection->calculateArbPattern();

    pair<string, ClsFEConnection*> pairTemp(strConnectionID, clsFEConnection);
    mapFEConnections.insert(pairTemp);
}


string ClsFESystemManager::duplicateConnection(ClsInfoConnection clsInfoConnection, bool bUseNewGroups, bool bCopyOfPrefix){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::duplicateConnection(ClsInfoConnection clsInfoConnection)" << endl;
#endif
    string strConnectionID = clsInfoConnection.getConnectionID();
    string strConnectionName;
    if(bCopyOfPrefix){
	strConnectionName = "Copy of " + clsInfoConnection.getConnectionName();
    } else {
	strConnectionName = clsInfoConnection.getConnectionName();
    }
    clsInfoConnection.setConnectionName(strConnectionName);
    addConnection(clsInfoConnection);
    emit sigItemDuplicated(ClsFESystemManager::ITEM_CONNECTION, strConnectionID);
    return strConnectionID;

}


string ClsFESystemManager::createConnection(string strSourceID, string strTargetID, int iConnectionType) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::createConnection(string strSourceID, string strTargetID)" << endl;
#endif

    bSystemChanged = true;
    iNewConnectionCounter++;

    string strConnectionID =   ClsIDGenerator::Instance()->Next();//.latin1();
//    string strConnectionName = "New Connection " + iqrUtils::int2string(iNewConnectionCounter);

    string strConnectionName = "Connection " + getGroupName(strSourceID) + "->" + getGroupName(strTargetID);

    ClsFEConnection*  clsFEConnection = new ClsFEConnection ( strConnectionID, strConnectionName, strSourceID, strTargetID);
    clsFEConnection->setConnectionType(iConnectionType);

/* 2009/01/09
   try to define a default synapse... */
    string strDefaultSynapse = "synapseFixedWeight";
    clsFEConnection->createSynapse(strDefaultSynapse);
/* ---------- */
    clsFEConnection->calculateArbPattern();

    pair<string, ClsFEConnection*> pairTemp(strConnectionID, clsFEConnection);
    mapFEConnections.insert(pairTemp);
    emit sigItemAdded(ClsFESystemManager::ITEM_CONNECTION, strConnectionID);
    return strConnectionID;


}




void ClsFESystemManager::addProcess(ClsInfoPrc _clsInfoProcess){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::addProcess(ClsInfoPrc _clsInfoPrc)" << endl;
#endif

    string strProcessID =_clsInfoProcess.getPrcID();
    string strProcessName = _clsInfoProcess.getProcessName();
    string strHostname =_clsInfoProcess.getHostname();

    int iPort = _clsInfoProcess.getPort();

    ClsFEProcess *clsFEProcess = new ClsFEProcess(strProcessID, strProcessName, strHostname, iPort);
    clsFEProcess->setParameter(ClsTagLibrary::NotesTag(), _clsInfoProcess.getNotes() );
    clsFEProcess->setParameter(ClsTagLibrary::EnableModuleTag(), iqrUtils::bool2string(_clsInfoProcess.getEnableModule() ));
    clsFEProcess->setParameter(ClsTagLibrary::PathTag(), _clsInfoProcess.getPath());
    clsFEProcess->setParameter(ClsTagLibrary::ColorTag(), _clsInfoProcess.getColor());


    ClsSysFileNode clsSysFileNodeProcesssubNodes = _clsInfoProcess.getSysFileNodeProcessSubNodes();
    clsFEProcess->setProcessSubNodes(clsSysFileNodeProcesssubNodes);
    clsFEProcess->setNotes(_clsInfoProcess.getNotes());

    pair<string, ClsFEProcess*> pairTemp(strProcessID, clsFEProcess);
    mapFEProcesses.insert(pairTemp);

    const string strSearch = "New Process ";
    string::size_type idx;
    idx = strProcessName.find(strSearch);
    if (idx != string::npos){
	unsigned int ii = iqrUtils::string2int(strProcessName.substr(idx + strSearch.size()));
	iNewProcessCounter = (ii>iNewProcessCounter ? ii : iNewProcessCounter);
    }

};

string ClsFESystemManager::duplicateProcess(ClsInfoPrc clsInfoProcess) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::duplicateProcess(ClsInfoPrc clsInfoProcess)" << endl;
#endif

    string strProcessName = "Copy of " + clsInfoProcess.getProcessName();
    string strProcessID =  clsInfoProcess.getPrcID(); 
    clsInfoProcess.setProcessName(strProcessName);
    addProcess(clsInfoProcess);
    emit sigItemDuplicated(ClsFESystemManager::ITEM_PROCESS, strProcessID);
    return strProcessID;
}

string ClsFESystemManager::createProcess() {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::createProcess()" << endl;
#endif
    iNewProcessCounter++;
    bSystemChanged = true;
    string strProcessID =   ClsIDGenerator::Instance()->Next();//.latin1();
    string strProcessName = "New Process " + iqrUtils::int2string(iNewProcessCounter);
    ClsFEProcess*  clsFEProcess = new ClsFEProcess (strProcessID, strProcessName, "", 0);

    pair<string, ClsFEProcess*> pairTemp(strProcessID, clsFEProcess);
    mapFEProcesses.insert(pairTemp);
    emit sigItemAdded(ClsFESystemManager::ITEM_PROCESS, strProcessID);
    return strProcessID;
}




void ClsFESystemManager::addGroup(ClsInfoGroup clsInfoGroup){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::addGroup(ClsInfoGroup clsInfoGroup)" << endl;
#endif


    string strGroupID = clsInfoGroup.getGroupID();
    string strGroupName = clsInfoGroup.getGroupName();
    string strProcessID = clsInfoGroup.getProcessID();


#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << __FILE__ << ":strProcessID:" << strProcessID << endl;
    cout << __FILE__ << ":strGroupID:" << strGroupID << endl;
    cout << __FILE__ << ":strGroupName:" << strGroupName << endl;
#endif

    ClsSysFileNode clsSysFileNodeGroupSubNodes = clsInfoGroup.getSysFileNodeGroupSubNodes();
    ClsFEGroup* clsFEGroup = new ClsFEGroup (strGroupID, strGroupName, strProcessID);

    clsFEGroup->setGroupSubNodes(clsSysFileNodeGroupSubNodes);
    clsFEGroup->setNotes(clsInfoGroup.getNotes());

    clsFEGroup->setParameter(ClsTagLibrary::ColorTag(), clsInfoGroup.getColor());


    pair<string, ClsFEGroup*> pairTemp(strGroupID, clsFEGroup);
    mapFEGroups.insert(pairTemp);

    const string strSearch = "New Group ";
    string::size_type idx;
    idx = strGroupName.find(strSearch);
    if (idx != string::npos){
	unsigned int ii = iqrUtils::string2int(strGroupName.substr(idx + strSearch.size()));
	iNewGroupCounter = (ii>iNewGroupCounter ? ii : iNewGroupCounter);
    }
}


string ClsFESystemManager::duplicateGroup(ClsInfoGroup clsInfoGroup, bool bCopyOfPrefix){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::duplicateGroup(ClsInfoGroup clsInfoGroup)" << endl;
#endif

    string strGroupName;
    if(bCopyOfPrefix){
	strGroupName = "Copy of " + clsInfoGroup.getGroupName();
    } else {
	strGroupName = clsInfoGroup.getGroupName();
    }

    string strGroupID = clsInfoGroup.getGroupID(); 
    clsInfoGroup.setGroupName(strGroupName);
    addGroup(clsInfoGroup);
    emit sigItemDuplicated(ClsFESystemManager::ITEM_GROUP, strGroupID);
    return strGroupID;
}

string ClsFESystemManager::createGroup(string strProcessID ){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::createGroup(string strProcessID )" << endl;
#endif
    iNewGroupCounter++;
    bSystemChanged = true;
    string strGroupID =   ClsIDGenerator::Instance()->Next();//.latin1();
    string strGroupName = "New Group " + iqrUtils::int2string(iNewGroupCounter);
    ClsFEGroup*  clsFEGroup = new ClsFEGroup (strGroupID, strGroupName, strProcessID);
    string strDefaultNeuron = "neuronLinearThreshold";
    clsFEGroup->createNeuron(strDefaultNeuron);

    pair<string, ClsFEGroup*> pairTemp(strGroupID, clsFEGroup);
    mapFEGroups.insert(pairTemp);
    emit sigItemAdded(ClsFESystemManager::ITEM_GROUP, strGroupID);
    return strGroupID;
}


void ClsFESystemManager::newSystem(){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::newSystem()" << endl;
#endif

    string strSystemID = ClsIDGenerator::Instance()->Next();//.latin1();
    time_t  tCurrent = time(NULL);;

    string strDate = ctime(&tCurrent);
    clsFESystem->setSystemID(strSystemID);
    clsFESystem->setParameter(ClsTagLibrary::NameTag(),  "new system" );
    clsFESystem->setParameter(ClsTagLibrary::NotesTag(), "" );

#ifdef _WINDOWS
    clsFESystem->setParameter(ClsTagLibrary::AuthorTag(), getenv("USERNAME") );
#else
    clsFESystem->setParameter(ClsTagLibrary::AuthorTag(), getenv("USER") );
#endif


    clsFESystem->setParameter(ClsTagLibrary::DateTag(), strDate  );

    string strHostName =  "localhost";

    /* with debian sarge, we get back a NULL pointer for getenv("HOSTNAME")! */
#ifdef _WINDOWS
    char *cpHostName = getenv("COMPUTERNAME");
#else
    char *cpHostName = getenv("HOSTNAME");
#endif

    if(cpHostName!=NULL){
	strHostName = cpHostName ;
    } 

    clsFESystem->setParameter(ClsTagLibrary::HostnameTag(), strHostName );
    clsFESystem->setParameter(ClsTagLibrary::PortTag(), "54923" );
    clsFESystem->setParameter(ClsTagLibrary::SyncPlotsTag(), "false" );
    clsFESystem->setParameter(ClsTagLibrary::CyclesPerSecondTag(), "0" );

    emit systemBuildt();
}









void ClsFESystemManager::serializeSystem(){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::serializeSystem()" << endl;
#endif

/*
  note about port numbers:
  As discussed 20011002 the port numbers are generated automatically. This means, that in
  the process of serializing the system the ports for the individual Prcs have to be assigned,
  as well as the port number for the FE. No port number information
  from the saved system file is used anymore... Numbers: all Prcs get a consecutive port
  number starting from the (random) port number of the FE Port range (from /etc/services)"
  "The Dynamic and/or Private Ports are those from 49152 through 65535"
  The idea of using a random portnumber from within the range is to avoid conflicts with other Prcs/FEs
  running on the same machine
*/

#define MAXNRPRCS 100.
    int iPortFE = 49152 + (int)(((double)rand() / (double)RAND_MAX) * (65535. - 49152. -MAXNRPRCS));

#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "portFE: " << iPortFE << endl;
#endif

    char caHostname[255];
    string strHostname;
    gethostname(caHostname, 255);
    strHostname =  caHostname;

    clsFESystem->setParameter(ClsTagLibrary::HostnameTag(), strHostname );
    clsFESystem->setParameter(ClsTagLibrary::PortTag(), iqrUtils::int2string(iPortFE));



/* here the ports for the prcs should be "published" */
    ClsPortFinder clsPortFinder;
    clsPortFinder.setScanRange(iPortFE + 1,65534); // for why iPortFE+1 as the lower boundary see comment above


    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapFEProcesses.begin(); mapIteratorProcesses!=mapFEProcesses.end(); ++mapIteratorProcesses){
	int iPort;
	try{
	    iPort = clsPortFinder.getPortByHostName(mapIteratorProcesses->second->getHostname());
	}
	catch(ClsPortFinderException& e) {
	    throw;
	}
#ifdef DEBUG_CLSFESYSTEMMANAGER
	cout << "HostName: " << mapIteratorProcesses->second->getHostname() << " gets: " << iPort << endl;
#endif
	mapIteratorProcesses->second->setPort( iPort);
	emit  sigPrcPortChanged( mapIteratorProcesses->second->getProcessID(),  iPort);
    }
};


ClsFEProcess* ClsFESystemManager::getFEProcess( string strID ) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getFEProcess( string strID )" << endl;
#endif
    if(mapFEProcesses.find(strID) != mapFEProcesses.end()){
	return mapFEProcesses.find(strID)->second;
    } else {
	return NULL;
    }
}

ClsFEGroup* ClsFESystemManager::getFEGroup( string strID ) {
    if(mapFEGroups.find(strID) != mapFEGroups.end()){
	return (mapFEGroups.find(strID)->second);
    } else {
	return NULL;
    }
};

ClsFEConnection* ClsFESystemManager::getFEConnection( string strID ) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second);
    } else {
	return NULL;
    }
};

list<ClsFEGroup*> ClsFESystemManager::getFEGroupsByName(string strName){
    list<ClsFEGroup*> lst;
    map<string, ClsFEGroup*>::iterator itmapFEGroups;    
    for (itmapFEGroups = mapFEGroups.begin(); itmapFEGroups != mapFEGroups.end(); ++itmapFEGroups){
	if(!strName.compare(itmapFEGroups->second->getGroupName())){
	    lst.push_back(itmapFEGroups->second);
	}
    }
    return lst;
}

list<ClsFEConnection*> ClsFESystemManager::getFEConnectionsByName(string strName){
    list<ClsFEConnection*> lst;
    map<string, ClsFEConnection*>::iterator itmapFEConnections;    
    for (itmapFEConnections = mapFEConnections.begin(); itmapFEConnections != mapFEConnections.end(); ++itmapFEConnections){
	if(!strName.compare(itmapFEConnections->second->getConnectionName())){
	    lst.push_back(itmapFEConnections->second);
	}
    }
    return lst;
}


list<ClsFEProcess*> ClsFESystemManager::getFEProcessesByName(string strName){
    list<ClsFEProcess*> lst;
    map<string, ClsFEProcess*>::iterator itmapFEProcesses;    
    for (itmapFEProcesses = mapFEProcesses.begin(); itmapFEProcesses != mapFEProcesses.end(); ++itmapFEProcesses){
	if(!strName.compare(itmapFEProcesses->second->getProcessName())){
	    lst.push_back(itmapFEProcesses->second);
	}
    }
    return lst;
}


string ClsFESystemManager::getGroupName(string strID ) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getGroupName(string strID )" << endl;
    cout << "strID: " << strID << endl;
#endif

    if(mapFEGroups.find(strID) != mapFEGroups.end()){
//	cout << "FOUND: " << (mapFEGroups.find(strID)->second)->getGroupName() << endl;

	return (mapFEGroups.find(strID)->second)->getGroupName();
    } else {
//	cout << "NOT FOUND" << endl;
	return "";
    }
}


string ClsFESystemManager::getSystemID() {
//***    return clsInfoSystem.getSystemID();
    return clsFESystem->getSystemID();

}

string ClsFESystemManager::getSystemName() {
//***    return clsInfoSystem.getSystemName();
    return clsFESystem->getSystemName();

}

string ClsFESystemManager::getSystemHostName() {
    return clsFESystem->getSystemHostName();
}

int ClsFESystemManager::getSystemPort() {
    return clsFESystem->getSystemPort();
}


bool ClsFESystemManager::getSyncPlots() {
    return clsFESystem->getSyncPlots();
}

int ClsFESystemManager::getCyclesPerSecond() {
    return clsFESystem->getCyclesPerSecond();
}


string ClsFESystemManager::getProcessName(string strID ) {

    if(mapFEProcesses.find(strID) != mapFEProcesses.end()){
	return (mapFEProcesses.find(strID)->second)->getProcessName();
    } else {
	return "";
    }
}


string ClsFESystemManager::getConnectionID(string strID ) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionID();
    } else {
	return "";
    }
}

string ClsFESystemManager::getConnectionName(string strID ) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionName();
    } else {
	return "";
    }
}

string ClsFESystemManager::getConnectionLongName(string strID ) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionLongName();
    } else {
	return "";
    }
}

int ClsFESystemManager::getConnectionTypeAsInt(string strID ) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionTypeAsInt();
    } else {
	return ClsBaseConnection::CONN_EXCITATORY;
    }
}


string ClsFESystemManager::getConnectionSourceID(string strID) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionSourceID();
    } else {
	return "";
    }
}

string ClsFESystemManager::getConnectionTargetID(string strID) {
    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	return (mapFEConnections.find(strID)->second)->getConnectionTargetID();
    } else {
	return "";
    }
}

string ClsFESystemManager::getGroupProcessID(string strID ) {
    if(mapFEGroups.find(strID) != mapFEGroups.end()){
	return (mapFEGroups.find(strID)->second)->getProcessID();
    } else {
	return "";
    }
}


list<string> ClsFESystemManager::getConnectionsByInvolvedGroups(const list<string> & lstGroups){
    list<string> lstConn;
    map<string, ClsFEConnection*>::iterator it;
    for(it=mapFEConnections.begin();it!=mapFEConnections.end();it++){
	if(find(lstGroups.begin(), lstGroups.end(), it->second->getConnectionSourceID())!= lstGroups.end() &&
	   find(lstGroups.begin(), lstGroups.end(), it->second->getConnectionTargetID())!= lstGroups.end()){
	    lstConn.push_back(it->first);
	}
    }
    return lstConn;
}


int ClsFESystemManager::setParameterByItemID(int iItemType, string strItemID, string strParamID, string strValue){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::setParameterByItemID(int iItemType, string strItemID, string strParamID, string strValue)" << endl;
#endif
    int iReturn = -1;
    ClsItem* item = NULL;
    if(iItemType==ITEM_GROUP){
//	cout << "iItemType==ITEM_GROUP" << endl;
	item = ClsFESystemManager::Instance()->getFEGroup( strItemID );
	if(item!=NULL){
//	    cout << "item!=NULL" << endl;
	    if(item->getParameter(strParamID)!=NULL){
		item->setParameter(strParamID, strValue);
		if(!bSimulationRunning){
		    emit sigItemChanged(ITEM_GROUP, strItemID);
		}
		iReturn = 0;
	    } 
	}
    } 
    else if (iItemType==ITEM_NEURON){ 
//	cout << "iItemType==ITEM_NEURON" << endl;
	item = ClsFESystemManager::Instance()->getFEGroup( strItemID );
	if(item!=NULL){
//	    cout << "item!=NULL" << endl;
	    iReturn = dynamic_cast<ClsFEGroup*>(item)->setNeuronParameter(strParamID, strValue);
		if(!bSimulationRunning){
		    emit sigItemChanged(ITEM_GROUP, strItemID);
		}
	}
    }
    else if (iItemType==ITEM_CONNECTION){
//	cout << "iItemType==ITEM_CONNECTION" << endl;
	item = ClsFESystemManager::Instance()->getFEConnection( strItemID );
	if(item!=NULL){
//	    cout << "item!=NULL" << endl;
	    if(item->getParameter(strParamID)!=NULL){
		item->setParameter(strParamID, strValue);
		if(!bSimulationRunning){
		    emit sigItemChanged(ITEM_CONNECTION, strItemID);
		}
		iReturn = 0;
	    } 
	}
    }
    else if (iItemType==ITEM_SYNAPSE){ 
//	cout << "iItemType==ITEM_SYNAPSE" << endl;
	item = ClsFESystemManager::Instance()->getFEConnection( strItemID );
	if(item!=NULL){
//	    cout << "item!=NULL" << endl;
	    iReturn = dynamic_cast<ClsFEConnection*>(item)->setSynapseParameter(strParamID, strValue);
	    if(!bSimulationRunning){
		emit sigItemChanged(ITEM_CONNECTION, strItemID);
	    }
	}
    }
    else if(iItemType==ITEM_PROCESS){
//	cout << "iItemType==ITEM_PROCESS" << endl;
	item = ClsFESystemManager::Instance()->getFEProcess( strItemID );
	if(item!=NULL){
//	    cout << "item!=NULL" << endl;
	    if(item->getParameter(strParamID)!=NULL){
		item->setParameter(strParamID, strValue);
		if(!bSimulationRunning){
		    emit sigItemChanged(ITEM_PROCESS, strItemID);
		}
		iReturn = 0;
	    } 
	}
    } else if(iItemType==ITEM_MODULE){
//	cout << "iItemType==ITEM_MODULE" << endl;
	item = ClsFESystemManager::Instance()->getFEProcess( strItemID );
	if(item!=NULL){
	    dynamic_cast<ClsFEProcess*>(item)->setModuleParameter(strParamID, strValue);
	    if(!bSimulationRunning){
		emit sigItemChanged(ITEM_MODULE, strItemID);
	    }
	}
	iReturn = 0;
     }

 
    return iReturn;
}

int ClsFESystemManager::setParameterByItemName(int iItemType, string strItemName, string strParamID, string strValue){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::setParameterByItemName(int iItemType, string strItemName, string strParamID, string strValue)" << endl;
#endif
    int iReturn = -1;
        if(iItemType==ITEM_GROUP || iItemType==ITEM_NEURON){ 
	    list<ClsFEGroup*> lst = getFEGroupsByName(strItemName);
	    list<ClsFEGroup*>::iterator it;
	    for(it=lst.begin(); it!=lst.end(); it++){
		string strItemID = (*it)->getGroupID();
//		cout << "strItemID" << endl;
		if(iItemType==ITEM_GROUP){
		    if((*it)->getParameter(strParamID)!=NULL){
			(*it)->setParameter(strParamID, strValue);
			iReturn = 0;
		    } 
		} 
		else if(iItemType==ITEM_NEURON){
		    iReturn = dynamic_cast<ClsFEGroup*>(*it)->setNeuronParameter(strParamID, strValue);
		}
		emit sigItemChanged(ITEM_GROUP, strItemID);
	    }
	}
	else if(iItemType==ITEM_CONNECTION || iItemType==ITEM_SYNAPSE){
	    list<ClsFEConnection*> lst = getFEConnectionsByName(strItemName);
	    list<ClsFEConnection*>::iterator it;
	    for(it=lst.begin(); it!=lst.end(); it++){
		string strItemID = (*it)->getConnectionID();
//		cout << "strItemID" << endl;
		if(iItemType==ITEM_CONNECTION){
		    if((*it)->getParameter(strParamID)!=NULL){
			(*it)->setParameter(strParamID, strValue);
			iReturn = 0;
		    } 
		} 
		else if(iItemType==ITEM_SYNAPSE){
		    iReturn = dynamic_cast<ClsFEConnection*>(*it)->setSynapseParameter(strParamID, strValue);
		}
		emit sigItemChanged(ITEM_CONNECTION, strItemID);
	    }
	}
	else if(iItemType==ITEM_PROCESS || iItemType==ITEM_MODULE){
	    list<ClsFEProcess*> lst = getFEProcessesByName(strItemName);
	    list<ClsFEProcess*>::iterator it;
	    for(it=lst.begin(); it!=lst.end(); it++){
		string strItemID = (*it)->getProcessID();
//		cout << "strItemID" << endl;
		if(iItemType==ITEM_PROCESS){
		    if((*it)->getParameter(strParamID)!=NULL){
			(*it)->setParameter(strParamID, strValue);
			iReturn = 0;
		    } 
		} 
		else if(iItemType==ITEM_SYNAPSE){
		    dynamic_cast<ClsFEProcess*>(*it)->setModuleParameter(strParamID, strValue);
		}
		emit sigItemChanged(ITEM_PROCESS, strItemID);
	    }

	}
	return iReturn;
}


list<string> ClsFESystemManager::getXRefValueList(int iOwnerType, string strOwnerParentID){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getXRefValueList(int iOwnerType, string strOwnerParentID)" << endl;
#endif
    list<string> lst;
    if(iOwnerType == ClsFESystemManager::ITEM_MODULE){
	map<string, ClsFEGroup*>::iterator itmapFEGroups;
	for (itmapFEGroups = mapFEGroups.begin(); itmapFEGroups != mapFEGroups.end(); ++itmapFEGroups){
	    if(!strOwnerParentID.compare((itmapFEGroups->second)->getProcessID())) {
		lst.push_back(itmapFEGroups->second->getGroupName());
	    }
	}
    } else if(iOwnerType == ClsFESystemManager::ITEM_CONNECTION){
	string strTargetGroupID = getFEConnection(strOwnerParentID)->getConnectionTargetID();
	ClsFEGroup* clsFEGroup = getFEGroup(strTargetGroupID);
	if(clsFEGroup!=NULL){
	    ClsNeuron* pNeuron = clsFEGroup->getNeuron();
	    if(pNeuron != NULL){
		list<string>lstTemp;
		lstTemp = pNeuron->getListFeedbackStateNames();
		list<string>::iterator it;
		for(it=lstTemp.begin(); it!=lstTemp.end(); it++){
		    if(pNeuron->getFeedbackState(*it)!=NULL){
			lst.push_back(pNeuron->getFeedbackState(*it)->getLabel());
		    }
		}
	    } else {
		cerr << "pNeuron is NULL" << endl;
	    }
	}
    }
    return lst;
}

string ClsFESystemManager::getXRefTargetIDByName(int iOwnerType, string strOwnerParentID, string strXRefTargetName){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getXRefTargetIDByName(int iOwnerType, string strOwnerParentID, string strXRefTargetName)" << endl;
#endif
    string strXRefTargetID = "";
    if(strXRefTargetName.size()>0){
	if(iOwnerType == ClsFESystemManager::ITEM_MODULE){
	    map<string, ClsFEGroup*>::iterator itmapFEGroups;
	    for (itmapFEGroups = mapFEGroups.begin(); itmapFEGroups != mapFEGroups.end(); ++itmapFEGroups){
		if(!strOwnerParentID.compare(itmapFEGroups->second->getProcessID()) && 
		   !strXRefTargetName.compare(itmapFEGroups->second->getGroupName())) {
		    strXRefTargetID = itmapFEGroups->second->getGroupID();
		    return strXRefTargetID;
		}
	    }
	} else if(iOwnerType == ClsFESystemManager::ITEM_CONNECTION){
	    /* in this case it's slightliy confusing:
	       XRefName <-> state->Label
	       XRefID <-> state->name
	    */

	    string strTargetGroupID = getFEConnection(strOwnerParentID)->getConnectionTargetID();
	    ClsFEGroup* clsFEGroup = getFEGroup(strTargetGroupID);
	    if(clsFEGroup!=NULL){
		ClsNeuron* pNeuron = clsFEGroup->getNeuron();
		if(pNeuron != NULL){
		    list<string> lst = pNeuron->getListFeedbackStateNames();
		    list<string>::iterator it;
		    for(it=lst.begin(); it!=lst.end(); it++){
			string strStateLabel = pNeuron->getFeedbackState(*it)->getLabel();
			if(!strXRefTargetName.compare(strStateLabel)){
			    return pNeuron->getFeedbackState(*it)->getName();
			}
		    }
		}
	    }
	}
    }
    return strXRefTargetID;
}


string ClsFESystemManager::getXRefTargetNameByID(int iOwnerType, string strOwnerParentID, string strXRefTargetID){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::getXRefTargetNameByID(int iOwnerType, string strOwnerParentID, string strXRefTargetID)" << endl;
    cout << "strXRefTargetID: " << strXRefTargetID << endl;
#endif
    string strXRefTargetName = "";
    if(strXRefTargetID.size()>0){
	if(iOwnerType == ClsFESystemManager::ITEM_MODULE){
	    if(!strOwnerParentID.compare(getGroupProcessID(strXRefTargetID))){ /* just double checking... */
		return getGroupName(strXRefTargetID);
	    }
	} else if(iOwnerType == ClsFESystemManager::ITEM_CONNECTION){
	    /* in this case it's slightliy confusing:
	       XRefName <-> state->Label
	       XRefID <-> state->name
	    */
	    string strTargetGroupID = getFEConnection(strOwnerParentID)->getConnectionTargetID();
	    ClsFEGroup* clsFEGroup = getFEGroup(strTargetGroupID);
	    if(clsFEGroup!=NULL){
		ClsNeuron* pNeuron = clsFEGroup->getNeuron();
		if(pNeuron != NULL){
		    strXRefTargetName = pNeuron->getFeedbackState(strXRefTargetID)->getLabel();
		}
	    }
	}
    }
    return strXRefTargetName;
}



// ClsInfoConnection* ClsFESystemManager::getInfoConnection( string strID ) {
// #ifdef DEBUG_CLSFESYSTEMMANAGER
//     cout << "ClsFESystemManager::getInfoConnection( string strID )" << endl;
// #endif
//     if(mapConnections.find(strID) != mapConnections.end()){
//	return &(mapConnections.find(strID)->second);
//     } else {
//	return NULL;
//     }
// };

void ClsFESystemManager::deleteItem(int iType, string strID) {
//#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << " ClsFESystemManager::deleteItem(int iType, string strID)" << endl;
//#endif

    bool bLogging = true;
    if(bLogging){
	stringstream ssLog;
	ssLog << "DELETED: " <<
	    "type: " << getTypeAsString(iType) << "; " << 
	    "name: \"" << getItemName(strID, iType) << "\"; " << 
	    "ID: " << strID;
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_LOG, ClsQLogWindow::TYPE_INFO, ssLog.str());
    }
    

    bSystemChanged = true;

    if (iType== ClsFESystemManager::ITEM_PROCESS){
	deleteProcess(strID);
    }
    else if (iType== ClsFESystemManager::ITEM_GROUP){
	deleteGroup(strID);
    }
    else if (iType== ClsFESystemManager::ITEM_CONNECTION){
	deleteConnection(strID);
    }
};

void ClsFESystemManager::deleteProcess(string strProcessID) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::deleteProcess(string strProcessID)" << endl;
#endif
    ClsStorage<ClsFEProcess*, ClsFEGroup*> clsStorage;
    
    /* remove Connections!!!! */
    
    if(mapFEProcesses.find(strProcessID) != mapFEProcesses.end()){
	ClsFEProcess *clsFEProcess = mapFEProcesses.find(strProcessID)->second;
	clsStorage.addParent(clsFEProcess);
	mapFEProcesses.erase(mapFEProcesses.find(strProcessID));

	map<string, ClsFEGroup*>::iterator itmapFEGroups;
	for (itmapFEGroups = mapFEGroups.begin(); itmapFEGroups != mapFEGroups.end(); ++itmapFEGroups){
	    if(!strProcessID.compare((itmapFEGroups->second)->getProcessID())) {
		ClsFEGroup* clsFEGroup = itmapFEGroups->second;
		string strGroupID = clsFEGroup->getGroupID();

		list<string> lstConnections;
		map<string, ClsFEConnection*>::iterator itC;
		/* compile a list of connection that source from or target one of the groups in the process
		   we have to go via creating the list, because we cannot delete the connections from the map immediately
		   as this screws up the deleteConnection function...
		*/
		for ( itC = mapFEConnections.begin(); itC != mapFEConnections.end(); ++itC ) {
		    string strConnectionID =  itC->second->getConnectionID();
		    string strSourceID = itC->second->getConnectionSourceID();
		    string strTargetID = itC->second->getConnectionTargetID();
		    if(!strGroupID.compare(strSourceID) || !strGroupID.compare(strTargetID)){
			lstConnections.push_back(strConnectionID);
		    }
		}
		lstConnections.unique();
		/* delete all the connections in the list */
		list<string>::iterator itList;
		for(itList = lstConnections.begin(); itList != lstConnections.end(); ++itList){
#ifdef DEBUG_CLSFESYSTEMMANAGER
		    cout << "to delete: " << *itList << endl;
#endif
		    deleteConnection(*itList);
		}
		clsStorage.addChild(clsFEGroup);
		
		/* warning: these two statements have to be in the right order */
		emit sigItemDeleted(ClsFESystemManager::ITEM_GROUP, strGroupID); 
		mapFEGroups.erase(itmapFEGroups); 
	    }
	}
	queueDeletedFEProcesses.push(clsStorage);
	emit sigItemDeleted(ClsFESystemManager::ITEM_PROCESS, strProcessID);
    }
};



/**
 * remove the group form the map and put it in the deleted items container (vector?)
 *
 * @param strID
 */
void ClsFESystemManager::deleteGroup(string strID) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::deleteGroup(string strID)" << endl;
#endif

//    queue< ClsFEGroup*> queueDeletedFEGroups;

    /*
      - get the pointer form the map
      - put pointer in queue
      - delete iterator from map
    */
    if(mapFEGroups.find(strID) != mapFEGroups.end()){
	ClsFEGroup *clsFEGroup = mapFEGroups.find(strID)->second;
	queueDeletedFEGroups.push(clsFEGroup);
	emit sigItemDeleted(ClsFESystemManager::ITEM_GROUP, strID);
	mapFEGroups.erase(mapFEGroups.find(strID));
	map<string, ClsFEConnection*>::iterator it;
	for(it=mapFEConnections.begin();it!=mapFEConnections.end();it++){
	    if(!strID.compare(it->second->getConnectionSourceID())){
		it->second->setConnectionSourceID("");
	    }
	    if(!strID.compare(it->second->getConnectionTargetID())){
		it->second->setConnectionTargetID("");
	    }
	}
    }
}

void ClsFESystemManager::deleteConnection(string strID) {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::deleteConnection(string strID)::strID: " << strID << endl;
#endif

    if(mapFEConnections.find(strID) != mapFEConnections.end()){
	ClsFEConnection *clsFEConnection = mapFEConnections.find(strID)->second;
	queueDeletedFEConnections.push(clsFEConnection);
	emit sigItemDeleted(ClsFESystemManager::ITEM_CONNECTION, strID);
	mapFEConnections.erase(mapFEConnections.find(strID));
    }
}


/* TEMPORARY */



void ClsFESystemManager::initSystem() {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::initSystem()" << endl;
#endif



    map<string, ClsFEGroup*>::iterator miFEGroups;
    for (miFEGroups = mapFEGroups.begin(); miFEGroups != mapFEGroups.end(); ++miFEGroups){
	try{
	    miFEGroups->second->init();
	}
	catch(runtime_error &e){
	    cerr << "Group Error: " << e.what() << endl;
	    throw;
	}
	emit sigAdvanceProgress();
    }

    map<string, ClsFEProcess*>::iterator miFEProcesses;
    for (miFEProcesses = mapFEProcesses.begin(); miFEProcesses != mapFEProcesses.end(); ++miFEProcesses){
	try{
	    miFEProcesses->second->init();
	}
	catch(runtime_error &e){
	    cerr << "Process Error: " << e.what() << endl;
	    throw;
	}
	emit sigAdvanceProgress();
    }

    map<string, ClsFEConnection*>::iterator miConnection;
    for(miConnection = mapFEConnections.begin(); miConnection!=mapFEConnections.end(); ++miConnection){
	try{
	    miConnection->second->init();
	}
	catch(runtime_error &e){
	    cerr << "Connection Error: " << e.what() << endl;
	    throw;
	}
	emit sigAdvanceProgress();
    }
}

void ClsFESystemManager::cleanupSystem() {
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::cleanupSystem()" << endl;
#endif

#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::cleanup:: groups" << endl;
#endif
    map<string, ClsFEGroup*>::iterator miFEGroups;
    for (miFEGroups = mapFEGroups.begin(); miFEGroups != mapFEGroups.end(); ++miFEGroups){
	cout << "cleaning up group with id: " << miFEGroups->first << endl;
	miFEGroups->second->cleanup();
    }

#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::cleanup:: processes" << endl;
#endif
    map<string, ClsFEProcess*>::iterator miFEProcesses;
    for (miFEProcesses = mapFEProcesses.begin(); miFEProcesses != mapFEProcesses.end(); ++miFEProcesses){
	cout << "cleaning up process with id: " << miFEProcesses->first << endl;
	miFEProcesses->second->cleanup();
    }
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::cleanup:: connections" << endl;
#endif
    map<string, ClsFEConnection*>::iterator miConnection;
    for(miConnection = mapFEConnections.begin(); miConnection!=mapFEConnections.end(); ++miConnection){
	cout << "cleaning up connection with id: " << miConnection->first << endl;
	miConnection->second->cleanup();
    }

}


string ClsFESystemManager::validateConnections(){
    string strValidationResult = "";
    map<string, ClsFEConnection*>::iterator miConnection;
    for(miConnection = mapFEConnections.begin(); miConnection!=mapFEConnections.end(); ++miConnection){
	string str = miConnection->second->validate();
	if(str.size()>0){
	    strValidationResult.append(str);
	    strValidationResult.append("\n");
	    pair<int, string> pairTemp(ITEM_CONNECTION, miConnection->second->getConnectionID());
	    lstInvalidItems.push_back(pairTemp);
	}
    }
    return strValidationResult;
}

bool ClsFESystemManager::isValidSystem(bool bConfirm){
#ifdef DEBUG_CLSFESYSTEMMANAGER
    cout << "ClsFESystemManager::validateSystem()" << endl;
#endif

    lstInvalidItems.clear();


    string strValidationResult = "";

    map<string, ClsFEGroup*>::iterator miFEGroups;
    for (miFEGroups = mapFEGroups.begin(); miFEGroups != mapFEGroups.end(); ++miFEGroups){
	string str = miFEGroups->second->validate();
	if(str.size()>0){
	    strValidationResult.append(str);
	    strValidationResult.append("\n");
	    pair<int, string> pairTemp(ITEM_GROUP, miFEGroups->second->getGroupID());
	    lstInvalidItems.push_back(pairTemp);

	}
    }
    
    
    map<string, ClsFEProcess*>::iterator miFEProcesses;
    for (miFEProcesses = mapFEProcesses.begin(); miFEProcesses != mapFEProcesses.end(); ++miFEProcesses){
	string str = miFEProcesses->second->validate();
	if(str.size()>0){
	    strValidationResult.append(str);
	    strValidationResult.append("\n");
	    pair<int, string> pairTemp(ITEM_PROCESS, miFEProcesses->second->getProcessID());
	    lstInvalidItems.push_back(pairTemp);

	}
    }

    string strTemp = validateConnections();
//    map<string, ClsFEConnection*>::iterator miConnection;
//    for(miConnection = mapFEConnections.begin(); miConnection!=mapFEConnections.end(); ++miConnection){
//	string strTemp = miConnection->second->validate();
	if(strTemp.size()>0){
	    strValidationResult.append(strTemp);
	    strValidationResult.append("\n");
	}
//    }


    if(strValidationResult.size()>0){
	ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_ERROR, strValidationResult);
	return false;
    } else {
	if(bConfirm){
	    ClsQLogWindow::Instance()->report(ClsQLogWindow::TARGET_MESSAGE, ClsQLogWindow::TYPE_INFO, "System valid");
	}
	return true;
    }
    return true;
}


string ClsFESystemManager::getItemName(string strID, int iType){
    
    switch(iType){
    case ITEM_GROUP:
	return getGroupName(strID );
    case ITEM_PROCESS:
	return getProcessName(strID);
    case ITEM_CONNECTION:
	return getConnectionName(strID );
    case ITEM_SYSTEM:
	return  getSystemName();
    default:
	return "";
    };
}

string ClsFESystemManager::getTypeAsString(int iType){
    switch (iType){
    case ITEM_GROUP:
	return "GROUP";	  
    case ITEM_PROCESS:
	return "PROCESS";	  
    case ITEM_CONNECTION:
	return "CONNECTION";  
    case ITEM_MODULE:
	return "MODULE";	  
    case ITEM_NEURON:
	return "NEURON";	  
    case ITEM_SYNAPSE:
	return "SYNAPSE";	  
    case ITEM_SYSTEM:      
	return "SYSTEM";      
    default:
	return "";
    }
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:






