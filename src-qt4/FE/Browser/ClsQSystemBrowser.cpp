/****************************************************************************
 ** $Filename: ClsQSystemBrowser.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Sep  6 23:54:24 2001
 ** $Date: 2002/12/08 17:33:42 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

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
#include <QHeaderView>

//#define DEBUG_CLSQSYSBROWSER

ClsQSystemBrowser::ClsQSystemBrowser( QWidget *_parent): QTreeWidget(_parent){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::ClsQSystemBrowser( QWidget *_parent): QTreeWidget(_parent, system browser)" << endl;
#endif
    parent = _parent;
//    setSortOrder (Qt::Ascending);

    sortItems( 0, Qt::AscendingOrder);
    setSortingEnabled(true);

    qlstvitemRootElement = NULL;
    qlstvitemConnections = NULL;
    
    setRootIsDecorated(true);
    setDragEnabled ( true );

    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);
 
    setColumnCount(2);
    QStringList qstrlstHeaders;
    qstrlstHeaders << "Name" << "ID";
    setHeaderLabels ( qstrlstHeaders );


    header()->setResizeMode( 0, QHeaderView::ResizeToContents );

};

void ClsQSystemBrowser::closeSystem() {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::closeSystem()" << endl;
#endif

    QTreeWidget::clear();    

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
    qlstvitemRootElement->setExpanded(true);

    qlstvitemConnections = new ClsQlstItemConnectionHeader( qlstvitemRootElement);
    qlstvitemConnections->setExpanded(false);


};
     

void ClsQSystemBrowser::addProcess(string strProcessID, string strProcessName) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addProcess(string strProcessID, string strProcessName)" << endl;
#endif
    ClsQlstItemProcess *qlstvitemProcess = new ClsQlstItemProcess( qlstvitemRootElement, strProcessID );
    qlstvitemProcess->setItemName(strProcessName.c_str());
    qlstvitemProcess->setExpanded(false);
    string strColor = ClsFESystemManager::Instance()->getFEProcess( strProcessID )->getColor(); 
    qlstvitemProcess->setColor(strColor); 
};


void ClsQSystemBrowser::addGroup(string strProcessID, string strGroupID, string strGroupName) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addGroup(string strProcessID, string strGroupID, string strGroupName,....)" << endl;
#endif



    /* UB: seems I just cannot get this code to work....  
    QList<QTreeWidgetItem *> qlst = findItems (strProcessID.c_str(), Qt::MatchExactly, 1 );
    cout << "COUNT: " << qlst.count() << endl;
    if(!qlst.isEmpty()){
	QTreeWidgetItem *qListViewItem = qlst.front();
	if(qListViewItem!=NULL){
	    cerr << "item != NULL" << endl;
	    if(dynamic_cast<ClsQBaseItem*>(qListViewItem)){
		ClsQBaseItem* clsQBaseItemProcess = dynamic_cast<ClsQBaseItem*>(qListViewItem);
		if(clsQBaseItemProcess!=NULL){
		    ClsQlstItemGroup *qlstvitemGroup = new ClsQlstItemGroup( clsQBaseItemProcess, strGroupID);
		    qlstvitemGroup->setItemName(strGroupName.c_str());
		}
	    }
	}
    } else {
	cerr << "--list empty" << endl;
    }
    */


    QTreeWidgetItemIterator it( this );
    while( *it ) {  
	string strItemID = (*it)->text(1).latin1();
	if(!strItemID.compare(strProcessID)){
	    
	    if(dynamic_cast<ClsQBaseItem*>(*it)){
		ClsQBaseItem* clsQBaseItemProcess = dynamic_cast<ClsQBaseItem*>(*it);
		if(clsQBaseItemProcess!=NULL){
		    ClsQlstItemGroup *qlstvitemGroup = new ClsQlstItemGroup( clsQBaseItemProcess, strGroupID);
		    qlstvitemGroup->setItemName(strGroupName.c_str());
		    string strColor = ClsFESystemManager::Instance()->getFEGroup( strGroupID )->getColor(); 
		    qlstvitemGroup->setColor(strColor); 
		}
	    }
	    break;
	}
	it++;
    }



};



void ClsQSystemBrowser::addConnection(string strConnectionID, string strConnectionName, int iConnectionType) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::addConnection(string strConnectionID, string strConnectionName)" << endl;
#endif
    ClsQlstItemConnection *qlstvitemConnection = new ClsQlstItemConnection( qlstvitemConnections, strConnectionID, iConnectionType );
    qlstvitemConnection->setItemName(strConnectionName.c_str());
    qlstvitemConnection->setExpanded(false);
 
};




ClsQBaseItem * ClsQSystemBrowser::getItem(string _strID) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQBaseItem* ClsQSystemBrowser::getItem(string _strID)" << endl;
#endif

//    cout << "strID: " << _strID << endl;
    QTreeWidgetItemIterator it( this );
    while( *it ) {  
	ClsQBaseItem *clsQBaseItem = dynamic_cast<ClsQBaseItem *>(*it);
	string strID = clsQBaseItem->getID();
//	cout << "strID: " << strID << endl;	
	if(!strID.compare(_strID)){
	    return clsQBaseItem;
	}
	it++;
    }
    return NULL;
};


void ClsQSystemBrowser::startDrag(Qt::DropActions ){ 
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::startDrag()" << endl;
#endif

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    if(dynamic_cast<ClsQlstItemGroup*>(currentItem())){
	string strParamList = "";
	string strRange = "";
	string strMessage = "";
	strMessage = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_GROUP, dynamic_cast<ClsQlstItemGroup*>(currentItem())->getID(), strParamList, strRange);	
	mimeData->setData("text/iqr-plot", strMessage.c_str());

	drag->setMimeData(mimeData);
	
	Qt::DropAction dropAction = drag->exec();
	
    }
    else if(dynamic_cast<ClsQlstItemConnection*>(currentItem())){
	string strMessage = "";
	strMessage = ClsDragDropDeEncoder::encode(ClsFESystemManager::ITEM_CONNECTION, dynamic_cast<ClsQlstItemConnection*>(currentItem())->getID(), "", "");
	mimeData->setData("text/iqr-plot", strMessage.c_str());

	drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec();

    }
};




void ClsQSystemBrowser::slotDiagItemActivated(int iType, string strID){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotItemActivated(int iType, string strID)" << endl;
    cout << "iType: " << iType << endl;
#endif
    
    if(iType == ClsFESystemManager::ITEM_SYSTEM){
      //	cout << "SYSTEM" << endl;
	setCurrentItem(qlstvitemRootElement);
    } else if(iType == ClsFESystemManager::ITEM_PROCESS){
      //	cout << "PROCESS" << endl;
	QTreeWidgetItemIterator it( this );
	while( *it ) {  
	    if(dynamic_cast<ClsQlstItemProcess*>((*it))){
		string strProcessID = dynamic_cast<ClsQlstItemProcess*>((*it))->getProcessID();
		if(!strProcessID.compare(strID)){
		    setCurrentItem ( *it);
		    return;
		}
	    }
	    it++;
	}
    }
}

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
//FIX    sort();
    sortItems( 0, Qt::AscendingOrder);

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
	string strColor = "";	
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
	    strColor = ClsFESystemManager::Instance()->getFEProcess( strID )->getColor(); 
	    if(strName.length() > 0){
//		cout << "strName: " << strName << endl;
		clsQBaseItem->setItemName(strName);
		clsQBaseItem->setColor(strColor); 
	    }

	}
	else if(iType == ClsFESystemManager::ITEM_GROUP){
//	    cout << "\tITEM_GROUP" << endl;
	    strName = ClsFESystemManager::Instance()->getGroupName(strID );
	    strColor = ClsFESystemManager::Instance()->getFEGroup( strID )->getColor(); 


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
		    clsQBaseItem->setColor(strColor); 
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
//FIX  sort();
    sortItems( 0, Qt::AscendingOrder);

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
//FIX      sort();
    sortItems( 0, Qt::AscendingOrder);

}

void ClsQSystemBrowser::slotItemDeleted(int /* iType */, string strID ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::slotItemDeleted(int iType, string strID )" << endl;
#endif

    
    /* UB: seems I just cannot get this code to work....   
    QList<QTreeWidgetItem *> qlst = findItems (strID.c_str(), Qt::MatchExactly, 1 );
    if(!qlst.isEmpty()){
	QTreeWidgetItem *qListViewItem = qlst.front();
	if(qListViewItem!=NULL){
	    delete qListViewItem;
	}
    } else {
	cerr << "--list empty" << endl;
    }
    */

    QTreeWidgetItem *qListViewItem = NULL;
    QTreeWidgetItemIterator it( this );
    while( *it ) {  
	string strItemID = (*it)->text(1).latin1();
	if(!strItemID.compare(strID)){
	    qListViewItem = (*it);
	    break;
	}
	it++;
    }
    
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
//FIX  sort();
    sortItems( 0, Qt::AscendingOrder);

};



void ClsQSystemBrowser::mouseDoubleClickEvent ( QMouseEvent * e ) {
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::contentsMouseDoubleClickEvent ( QMouseEvent * e )" << endl;
#endif
    
    QTreeWidgetItem *item = itemAt(e->pos());
    if(dynamic_cast<ClsQlstItemProcess*> (item) || dynamic_cast<ClsQlstItemSystem*> (item)) {
	string strID = ((ClsQBaseItem*)(item))->getID();
	emit sigShowBlockDiagram(strID);
    } 
    
};


void ClsQSystemBrowser::contextMenuEvent ( QContextMenuEvent * event ){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::contextMenuEvent ( QContextMenuEvent * e )" << endl;
#endif
    
    QTreeWidgetItem* item = itemAt(event->pos());
    if (item) {
	string strID = ((ClsQBaseItem*)(item))->getID();

	if(dynamic_cast<ClsQlstItemSystem*> (item)) {
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_SYSTEM, strID, event->globalPos());
	}
	else if(dynamic_cast<ClsQlstItemProcess*> (item)) {
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_PROCESS, strID, event->globalPos());
	}
	else if ( dynamic_cast<ClsQlstItemGroup*> (item)){
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_GROUP, strID, event->globalPos());
	} 
	else if ( dynamic_cast<ClsQlstItemConnection*> (item)){
	    emit sysbrowserRightClick(ClsFESystemManager::ITEM_CONNECTION, strID, event->globalPos());
	} 
    }
}


int ClsQSystemBrowser::getWidth (){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::getWidth ()" << endl;
#endif 
    return header()->sectionSizeHint ( 0 );
}

void ClsQSystemBrowser::setFilter(QString qstrFilter){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::setFilter(QString qstrFilter)" << endl;
#endif

    QTreeWidgetItemIterator it( this );
    while( *it ) {  
	if(dynamic_cast<ClsQlstItemGroup*>((*it)) || dynamic_cast<ClsQlstItemConnection*>((*it))){
	    QString qstrItemText = (*it)->text(0);
	    if(!qstrItemText.contains(qstrFilter, Qt::CaseSensitive)){
		(*it)->setHidden(true);
	    } else {
		(*it)->setHidden(false);
	    }
	}
	it++;
    }

}

void ClsQSystemBrowser::removeFilter(){
#ifdef DEBUG_CLSQSYSBROWSER
    cout << "ClsQSystemBrowser::removeFilter()" << endl;
#endif

    QTreeWidgetItemIterator it( this );
    while( *it ) {  
	(*it)->setHidden(false);
	it++;
    }

}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
