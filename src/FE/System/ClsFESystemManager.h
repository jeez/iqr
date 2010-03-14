/****************************************************************************
 ** $Filename: ClsFESystemManager.h
 ** $Id: ClsFESystemManager.h,v 1.3 2003/10/06 15:03:30 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Feb  5 13:40:01 2002
 ** $Date: 2003/10/06 15:03:30 $
 **
 ** $Log: ClsFESystemManager.h,v $
 ** Revision 1.3  2003/10/06 15:03:30  ulysses
 ** before making SystemManager a Singleton
 **
 ** Revision 1.2  2003/04/04 17:29:06  ulysses
 ** before removal of the ClsInfoGroup and ClsInfoPrc data structure usage
 **
 ** Revision 1.1  2003/04/03 17:04:43  ulysses
 ** Initial revision
 **
 ** Revision 1.2  2002/12/08 17:34:14  ulysses
 ** After separating system building, managing and serializing
 **
 ** Revision 1.1  2002/02/20 13:52:43  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


//#define DEBUG_CLSFESYSTEMMANAGER

#ifndef CLSFESYSTEMMANAGER_H
#define CLSFESYSTEMMANAGER_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <string>
#include <queue>

#include <qobject.h>

//#include <ClsItemProcess.h>

#include "ClsInfoSystem.h"
#include "ClsInfoPrc.h"
#include <ClsInfoGroup.h>
#include "ClsInfoConnection.h"

#include "ClsInfoDiagramLine.h"
#include "ClsInfoDiagramIcon.h"

#ifdef OLDITEM
#include "Item.20021216/Neuron/neuronManager.hpp"
#else
#include "neuronManager.hpp"
using iqrfe::initializeNeuronManager;
using iqrfe::NeuronManager;

#endif

#include "ClsStorage.h"
#include <idGenerator.hpp>

class ClsFESystemBuilder;


namespace iqrcommon {
    class ClsSysFileNode;
}




class ClsFESystem;
class ClsFEProcess;
class ClsFEGroup;
class ClsFEConnection;


using namespace iqrcommon;
using namespace std;

class ClsFESystemManager : public QObject {
    Q_OBJECT

public:

    
    static ClsFESystemManager* Instance();
    static void initializeSystemManager(QWidget* _parent = NULL);

    enum POPUP_ITEM{
	ITEM_GROUP,
	ITEM_PROCESS,
	ITEM_CONNECTION,
	ITEM_MODULE,
	ITEM_NEURON,
	ITEM_SYNAPSE,
	ITEM_SYSTEM,
	ITEM_GROUPSWITHCONNECTIONS
    };

    void buildSystem();
    void closeSystem();
    bool systemChanged();
    void systemSaved(){
	bSystemChanged = false;
    }

    void initSystem();
    void cleanupSystem();

    string validateConnections();
    bool isValidSystem(bool bConfirm);
    list<pair<int, string> > getListInvalidItems(){ return lstInvalidItems;};

/* operations for adding stuff */
    void newSystem();

    void addGroup(ClsInfoGroup _clsInfoGroup); /* this should be the final impl */
    string duplicateGroup(ClsInfoGroup clsInfoGroup, bool bCopyOfPrefix = true);
    string createGroup(string strPrcID);

    void addProcess(ClsInfoPrc _clsInfoPrc);
    string duplicateProcess(ClsInfoPrc clsInfoProcess);
    string createProcess();


    void addConnection(ClsInfoConnection clsInfoConnection);
    string duplicateConnection(ClsInfoConnection clsInfoConnection, bool bUseNewGroups=false, bool bCopyOfPrefix = true);
    string createConnection(string strSourceID, string strTargetID, int iConnectionType);


/* deleting stuff */
    void deleteItem(int iType, string strID);

    void deleteProcess(string strID);
    void deleteGroup(string strID);
    void deleteConnection(string strID);

    ClsFEProcess* getFEProcess( string strID );

    ClsFEGroup* getFEGroup( string strID );
    ClsFEGroup* getFEGroupByName( string strName, string strProcessID );
    ClsFEConnection* getFEConnection( string strID );

    list<ClsFEGroup*> getFEGroupsByName(string strName);
    list<ClsFEConnection*> getFEConnectionsByName(string strName);
    list<ClsFEProcess*> getFEProcessesByName(string strName); 


    map<string, ClsFEProcess*> getMapFEProcesses();
    map<string, ClsFEGroup*> getMapFEGroups();
    map<string, ClsFEConnection*> getMapFEConnections();


    string getSystemID();
    string getSystemName();
    string getSystemHostName();
    int getSystemPort();
    int getCyclesPerSecond();
    bool getSyncPlots();

    string getProcessName(string strID );
    string getGroupName(string strID );
    string getConnectionID(string strID );
    string getConnectionName(string strID );
    string getConnectionLongName(string strID );
    int getConnectionTypeAsInt(string strID );
    string getConnectionSourceID(string strID);
    string getConnectionTargetID(string strID);
    string getGroupProcessID(string strID );

    list<string> getConnectionsByInvolvedGroups(const list<string> & lstGroups);

    int setParameterByItemID(int iItemType, string strItemID, string strParamID, string strValue);
    int setParameterByItemName(int iItemType, string strItemName, string strParamID, string strValue);

    list<string> getXRefValueList(int iOwnerType, string strOwnerParentID);
    string getXRefTargetIDByName(int iOwnerType, string strOwnerParentID, string strXRefTargetName);
    string getXRefTargetNameByID(int iOwnerType, string strOwnerParentID, string strXRefTargetID);

    ClsInfoDiagramIcon getDiagramIcon(string strID);
    ClsInfoDiagramLine getDiagramLine(string strID);

    void setInfoSystem( ClsInfoSystem _clsInfoSystem);


    ClsFESystem* getFESystem();

    string getSystemFileCont();

    void itemChanged(int iType, string strID);

    int getTotalItemCount(){
	return mapFEGroups.size() + mapFEProcesses.size() + mapFEConnections.size();
    }

    void emitLoadProgress(string s){
	emit sigLoadProgress(s);
    }

    void emitItemDuplicated(int iType, string strID){
	emit sigItemDuplicated(iType, strID);
    }

public slots:
    void slotSimulationRunning(bool);

signals:
/* these should be collapsed into 1 sigItemChanged(int iType, string strID) function */
    void systemBuildt();
    void sigSystemChanged();
    void sigProcessChanged(string strPrcID);
    void sigGroupChanged(string strGroupID);
    void sigConnectionChanged(string strConnectionID);

    void sigItemChanged(int iType, string strID); /* should replace the above 4 functions */


    void sigItemAdded(int iType, string strID);
    void sigItemDuplicated(int iType, string strID);
    void sigItemDeleted(int iType, string strID);
    void sigItemUnDeleted(int iType, string strID);

    void sigPrcPortChanged( string strPrcID, int iPort);
    void sigAdvanceProgress();
    void sigLoadProgress(string s);

private:

    QWidget* parent;
    ClsFESystemManager (QWidget* _parent = NULL);
    static ClsFESystemManager* _instanceSystemManager;

    void serializeSystem();

    bool bSimulationRunning;

    map<string, ClsFEProcess*> mapFEProcesses;
    map<string, ClsFEConnection*> mapFEConnections;
    map<string, ClsFEGroup*> mapFEGroups;

    /* containers for deleted objects */

    queue<ClsStorage<ClsFEProcess*, ClsFEGroup*> > queueDeletedFEProcesses;
    queue< ClsFEGroup*> queueDeletedFEGroups;
    queue< ClsFEConnection*> queueDeletedFEConnections;

    list<pair<int, string> > lstInvalidItems;

    string strSystemFileCont;
    ClsFESystemBuilder *clsFESystemBuilder;


/* neurons related */
#ifdef OLDITEM
    NeuronManager neuronManager;
#endif
/* -------------- */


    ClsFESystem* clsFESystem;
    bool bSystemChanged;

    unsigned int iNewProcessCounter;
    unsigned int iNewGroupCounter;
    unsigned int iNewConnectionCounter;

    friend class iqrMainWindow;
    friend class ClsFEComputeEngine;
    friend class ClsNQiqr;
};


#endif /* CLSFESYSTEMMANAGER_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
