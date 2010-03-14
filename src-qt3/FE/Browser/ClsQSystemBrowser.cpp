/****************************************************************************
 ** $Filename: ClsQSystemBrowser.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ClsQSystemBrowser.cpp,v 1.3 2002/12/08 17:33:42 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Sep  6 23:54:24 2001
 ** $Date: 2002/12/08 17:33:42 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <qmotifstyle.h>
#include "ClsQSystemBrowser.h"
#include "ClsQlstItemProcess.h"
#include "ClsQlstItemGroup.h"
#include "ClsQlstItemConnection.h"
#include <ClsFESystemManager.h>
#include <ClsFEDataClient.h>
#include "ClsFEGroup.h"
#include "ClsFEProcess.h"
#include "ClsFEConnection.h"
#include <ClsDragDropDeEncoder.h>

//#define DEBUG_CLSQSYSBROWSER

ClsQSystemBrowser::ClsQSystemBrowser( QWidget *_parent): QListView(_parent, "system browser"){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::ClsQSystemBrowser( QWidget *_parent): QListView(_parent, system browser)" << endl;
#endif
    parent = _parent;
//    setSortOrder (Qt::Ascending);
    //   setSorting(-1);
    qlstvitemRootElement = NULL;
    qlstvitemConnections = NULL;
    
    setRootIsDecorated(true);

    setDragAutoScroll ( false );
    this->setStyle(new QMotifStyle);  

//    setSelectionMode(QListView::Extended);
    setSelectionMode(QListView::Single);
    setAllColumnsShowFocus(true);
 
    addColumn( "Name", -1 );
    addColumn( "ID", -1 );

    connect(this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint& , int ) ),
	    this, SLOT( slotRMB( QListViewItem *, const QPoint &, int ) ) );

};

void ClsQSystemBrowser::closeSystem() {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::closeSystem()" << endl;
#endif

    QListView::clear();    

    /* delete qlstvitemRootElement; BOOOOM */
    qlstvitemRootElement = NULL;
//    qlstvitemConnections = NULL;
}


void ClsQSystemBrowser::setRoot(string strID, string strName) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::setRoot(string _strID, string strName)" << endl;
#endif
    qlstvitemRootElement = new ClsQlstItemSystem( this, strID );
    qlstvitemRootElement->setItemName(strName.c_str());
    qlstvitemRootElement->setOpen(true);

    qlstvitemConnections = new ClsQlstItemConnectionHeader( qlstvitemRootElement);
    qlstvitemConnections->setOpen(false);
};
     

void ClsQSystemBrowser::addProcess(string strProcessID, string strProcessName) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addProcess(string strProcessID, string strProcessName)" << endl;
#endif
    ClsQlstItemProcess *qlstvitemProcess = new ClsQlstItemProcess( qlstvitemRootElement, strProcessID );
    qlstvitemProcess->setItemName(strProcessName.c_str());
    qlstvitemProcess->setOpen(false);
    
};


void ClsQSystemBrowser::addGroup(string strProcessID, string strGroupID, string strGroupName) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addGroup(string strProcessID, string strGroupID, string strGroupName,....)" << endl;
#endif

/*
    QListViewItemIterator it( this );
    for ( ; it.current(); ++it ) {  
	if(!it.current()->text(1).compare(strProcessID.c_str())){
	    break;
	}
    }
    
    ClsQlstItemGroup *qlstvitemGroup = new ClsQlstItemGroup( (ClsQBaseItem*)it.current(), strGroupID);
*/

    ClsQBaseItem* clsQBaseItemProcess = dynamic_cast<ClsQBaseItem*>(findItem ( strProcessID.c_str(), 1));
    if(clsQBaseItemProcess!=NULL){
	ClsQlstItemGroup *qlstvitemGroup = new ClsQlstItemGroup( clsQBaseItemProcess, strGroupID);
	qlstvitemGroup->setItemName(strGroupName.c_str());
    }
      
};



void ClsQSystemBrowser::addConnection(string strConnectionID, string strConnectionName, int iConnectionType) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addConnection(string strConnectionID, string strConnectionName)" << endl;
#endif
    ClsQlstItemConnection *qlstvitemConnection = new ClsQlstItemConnection( qlstvitemConnections, strConnectionID, iConnectionType );
    qlstvitemConnection->setItemName(strConnectionName.c_str());
    qlstvitemConnection->setOpen(false);
 
};



void ClsQSystemBrowser::slotRMB( QListViewItem* Item, const QPoint & point, int ) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotRMB( QListViewItem* Item, const QPoint & point, int )" << endl;
#endif
    if(Item!=NULL){
	string strID = ((ClsQBaseItem*)(Item))->getID();
	if(dynamic_cast<ClsQlstItemSystem*> (Item)) {
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_SYSTEM, strID, point);
	}
	else if(dynamic_cast<ClsQlstItemProcess*> (Item)) {
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_PROCESS, strID, point);
	}
	else if ( dynamic_cast<ClsQlstItemGroup*> (Item)){
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_GROUP, strID, point);
	} 
	else if ( dynamic_cast<ClsQlstItemConnection*> (Item)){
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_CONNECTION, strID, point);
	} 
    }
};

ClsQBaseItem * ClsQSystemBrowser::getItem(string _strID) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQBaseItem* ClsQSystemBrowser::getItem(string _strID)" << endl;
#endif

//    cout << "strID: " << _strID << endl;
    QListViewItemIterator it( this );
    for ( ; it.current(); ++it ) {  
	ClsQBaseItem *clsQBaseItem = (ClsQBaseItem *)it.current();
	string strID = clsQBaseItem->getID();
//	cout << "strID: " << strID << endl;	
	if(!strID.compare(_strID)){
	    return clsQBaseItem;
	}
    }
    return NULL;
};


void ClsQSystemBrowser::startDrag(){ 
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::startDrag()" << endl;
#endif

    QTextDrag *d = new QTextDrag("", this );
    d->setSubtype("iqr-plot");
     
    if(dynamic_cast<ClsQlstItemGroup*>(currentItem())){
	string strParamList = "";
	string strRange = "";
//	cout << "ClsQSystemBrowser::groupName: " << ClsFESystemManager::Instance()->getGroupName(strID) << endl;
	string strMessage = "";
	/* add support for connections */
	strMessage = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_GROUP, dynamic_cast<ClsQlstItemGroup*>(currentItem())->getID(), strParamList, strRange);	
	d->setText(strMessage);
	d->dragCopy();
    }
    else if(dynamic_cast<ClsQlstItemConnection*>(currentItem())){
	string strMessage = "";
	/* add support for connections */
	strMessage = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_CONNECTION, dynamic_cast<ClsQlstItemConnection*>(currentItem())->getID(), "", "");
	d->setText(strMessage);
	d->dragCopy();
    }

};

void ClsQSystemBrowser::contentsMouseMoveEvent( QMouseEvent *e ){
    if(e->state() == LeftButton){
	startDrag();
    }
};


void ClsQSystemBrowser::slotSystemChanged(){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::SystemChanged()" << endl;
#endif

    string strSystemID = ClsFESystemManager::Instance()->getSystemID();
    string strSystemName = ClsFESystemManager::Instance()->getSystemName();

    setRoot(strSystemID.c_str(), strSystemName.c_str());

    map<string, ClsFEProcess*> mapProcesses =  ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
	string strProcessID = mapIteratorProcesses->second->getProcessID();
	string strProcessName = mapIteratorProcesses->second->getProcessName();
	addProcess(strProcessID, strProcessName);  
    }

    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEGroup*>::iterator mapIteratorGroups;
    for(mapIteratorGroups = mapGroups.begin(); mapIteratorGroups!=mapGroups.end(); ++mapIteratorGroups){
    	string strProcessID = mapIteratorGroups->second->getProcessID();
	string strGroupID = mapIteratorGroups->second->getGroupID();
	string strGroupName = mapIteratorGroups->second->getGroupName();
	addGroup(strProcessID,strGroupID,strGroupName);
    }

/* new 20050214 */
    map<string, ClsFEConnection*> mapConnections = ClsFESystemManager::Instance()->getMapFEConnections();
    map<string, ClsFEConnection*>::iterator mapIteratorConnections;
    for(mapIteratorConnections = mapConnections.begin(); mapIteratorConnections!=mapConnections.end(); ++mapIteratorConnections){
	string strConnectionID = mapIteratorConnections->second->getConnectionID();
	string strConnectionName = mapIteratorConnections->second->getConnectionName();
	int iConnectionType =  mapIteratorConnections->second->getConnectionTypeAsInt();
	addConnection(strConnectionID,strConnectionName, iConnectionType);
    }
      sort();
};




void ClsQSystemBrowser::slotItemChanged(int iType, string strID ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotItemChanged(int iType, string strID )" << endl;
#endif
    /* to do:
       for all items:
       - check name

       for groups:
       - check if parent changed
    */

    ClsQBaseItem *clsQBaseItem = getItem(strID);
    if(clsQBaseItem!=NULL){
	string strName = "";
	if(iType == ClsFESystemManager::ITEM_SYSTEM){
//	    cout << "\tITEM_SYSTEM" << endl;
	    strName = ClsFESystemManager::Instance()->getSystemName();
	    if(strName.length() > 0){
//		cout << "strName: " << strName << endl;
		clsQBaseItem->setItemName(strName);
	    }
	}
	else if(iType == ClsFESystemManager::ITEM_PROCESS){
//	    cout << "\tITEM_PROCESS" << endl;
	    strName = ClsFESystemManager::Instance()->getProcessName( strID );
	    if(strName.length() > 0){
//		cout << "strName: " << strName << endl;
		clsQBaseItem->setItemName(strName);
	    }

	}
	else if(iType == ClsFESystemManager::ITEM_GROUP){
//	    cout << "\tITEM_GROUP" << endl;
	    strName = ClsFESystemManager::Instance()->getGroupName(strID );
/* check if the parent process of the group has changed */
	    string strNewProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strID);
	    string strOriginalProcessID = dynamic_cast<ClsQlstItemGroup*>(clsQBaseItem)->getProcessID();
   
/* if it changed, the delete the group, and recreate */
	    if(strName.length() > 0 && strNewProcessID.length() > 0 ){
		if(!strOriginalProcessID.compare(strNewProcessID)){
		    slotItemDeleted(iType, strID );
		    addGroup(strNewProcessID, strID, strName);
		} else {
		    clsQBaseItem->setItemName(strName);
		}
	    }
	}
	else if(iType == ClsFESystemManager::ITEM_CONNECTION){
//	    cout << "\tITEM_CONNECTION" << endl;
	    strName = ClsFESystemManager::Instance()->getConnectionName( strID );
	    int iConnectionType =  ClsFESystemManager::Instance()->getConnectionTypeAsInt(strID);
	    if(strName.length() > 0){
//		cout << "strName: " << strName << endl;
		clsQBaseItem->setItemName(strName);
		dynamic_cast<ClsQlstItemConnection*>(clsQBaseItem)->setConnectionType(iConnectionType);
	    }

	}
    }
  sort();
};


void ClsQSystemBrowser::slotItemAdded(int iType, string strID ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotItemAdded(int iType, string strID )" << endl;
#endif
    
    if(iType == ClsFESystemManager::ITEM_PROCESS){
 	string strName = ClsFESystemManager::Instance()->getProcessName( strID );
	addProcess(strID, strName);
    }
    else if(iType == ClsFESystemManager::ITEM_GROUP){
	string strName = ClsFESystemManager::Instance()->getGroupName(strID );
 	string strProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strID);
	addGroup( strProcessID, strID, strName);
    }
    else if(iType == ClsFESystemManager::ITEM_CONNECTION){
 	string strName = ClsFESystemManager::Instance()->getConnectionName( strID );
	int iConnectionType =  ClsFESystemManager::Instance()->getConnectionTypeAsInt(strID);
	addConnection(strID, strName, iConnectionType);
    }
      sort();
}

void ClsQSystemBrowser::slotItemDeleted(int /* iType */, string strID ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotItemDeleted(int iType, string strID )" << endl;
#endif

    /* THIS CAUSED CRASHES FOR SOME REASON...
     ClsQBaseItem *clsQBaseItem = getItem(strID);
     if(clsQBaseItem!=NULL){
 	delete clsQBaseItem;
     }
   sort();
    */
    
/* THIS WORKS... */
    QListViewItem *qListViewItem = findItem (strID, 1);
    if(qListViewItem!=NULL){
	delete qListViewItem;
    }
};


void ClsQSystemBrowser::slotItemUnDeleted(int iType, string strID ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::NOT IMPLEMENTED:";
    cout << "ClsQSystemBrowser::slotItemUnDeleted(int iType, string strID )" << endl;
#endif
    iType = 0; strID = "";
  sort();
};



void ClsQSystemBrowser::contentsMouseDoubleClickEvent ( QMouseEvent * e ) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::contentsMouseDoubleClickEvent ( QMouseEvent * e )" << endl;
#endif
    
    QListViewItem *item = itemAt(e->pos());
    if(dynamic_cast<ClsQlstItemProcess*> (item) || dynamic_cast<ClsQlstItemSystem*> (item)) {
	string strID = ((ClsQBaseItem*)(item))->getID();
	emit sigShowBlockDiagram(strID);
    } 
    
};

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
