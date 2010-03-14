#include <stdlib.h>
#include <iostream>
#include <map>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QApplication>
#include <QFrame>
#include <QImage>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPrinter>
#include <QRegion>
#include <QToolTip>



#include <ClsFESystemManager.h>
#include "ClsFEDiagramManager.h"
#include <ClsSysFileParserException.h>

#include "ClsBlockDiagram.h"
#include "ClsDiagItemProcess.h"
#include "ClsDiagItemGroup.h"
#include "ClsDiagItemPhantom.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagHyperConnection.h"

#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"
#include "tagLibrary.hpp"
#include "parameter.hpp"

#include <zoomIn.xpm>
#include <zoomOut.xpm>
#include <connectionExcitatory_16x13.xpm>
#include <connectionInhibitory_16x13.xpm>
#include <connectionModulatory_16x13.xpm>
#include <newProcessIcon_16x13.xpm>
#include <newGroupIcon_16x13.xpm>
#include <infoIcon_16x13.xpm>
#include <iconSplitVertical_16x13.xpm>
#include <iconSplitHorizontal_16x13.xpm>
#include <iconNoSplit_16x13.xpm>


//#define DEBUG_CLSBLOCKDIAGRAM

#define CANVAS_DEFAULT_X 3200
#define CANVAS_DEFAULT_Y 2400

#define PHANTOMGROUP_WIDTH 72
pair<QPointF, int> findNearestPoint(QPointF qp0, QRectF qrect){

//      --- D ---
//      |       |
//      A       C
//      |       |
//      --- B ---

    QPointF qpA(qrect.left(), qrect.center().y());
    QPointF qpB(qrect.center().x(), qrect.bottom());
    QPointF qpC(qrect.right(), qrect.center().y());
    QPointF qpD(qrect.center().x(), qrect.top());

    double distA = sqrt((double)(pow((double)(qpA.x() - qp0.x()), 2) + pow((double)(qpA.y() - qp0.y()), 2)));
    double distB = sqrt((double)(pow((double)(qpB.x() - qp0.x()), 2) + pow((double)(qpB.y() - qp0.y()), 2)));
    double distC = sqrt((double)(pow((double)(qpC.x() - qp0.x()), 2) + pow((double)(qpC.y() - qp0.y()), 2)));
    double distD = sqrt((double)(pow((double)(qpD.x() - qp0.x()), 2) + pow((double)(qpD.y() - qp0.y()), 2)));


    double dMin = std::min(min(distA, distB), min(distC, distD));
    QPointF qpMin = (dMin == distA ? qpA : (dMin == distB ? qpB : (dMin == distC ? qpC : qpD) ));

    int iOrientation = (dMin == distA ? ClsDiagConnection::WEST_EAST :
			(dMin == distB ? ClsDiagConnection::SOUTH_NORTH :
			 (dMin == distC ? ClsDiagConnection::EAST_WEST :
			  ClsDiagConnection::NORTH_SOUTH) ));


    pair<QPointF, int> pairTemp(qpMin, iOrientation);
    return pairTemp;
}


ClsBlockDiagram*  ClsBlockDiagram::_instanceBlockDiagram = NULL;

void ClsBlockDiagram::initializeBlockDiagram(QWidget* _toplevel){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::initializeBlockDiagram(QWidget* _toplevel)" << endl;
#endif
    _instanceBlockDiagram = new ClsBlockDiagram(_toplevel);
}


ClsBlockDiagram* ClsBlockDiagram::Instance(){
    return _instanceBlockDiagram;
}


ClsBlockDiagram::ClsBlockDiagram( QWidget* _parent) : QFrame(_parent, 0 ,0) {
    clsDiagItemAPSource = NULL;

    iAddState = -99;


    qframeToolbar = new QFrame(this);
    QHBoxLayout *qboxlayoutTB= new QHBoxLayout( qframeToolbar );
    qboxlayoutTB->setContentsMargins ( 0, 0, 0, 0 );
    qboxlayoutTB->setSpacing ( 0);
    
    qsplitter = new QSplitter(this); /* splitter for multiple diagram views */
    qsplitter->setContentsMargins ( 0, 0, 0, 0 );

    clsQDiagramView00 = new ClsSubDiagram( qsplitter, this, DIAG_00, 0);
    clsQDiagramView01 = new ClsSubDiagram( qsplitter, this, DIAG_01, 0);
    clsQDiagramViewActive = clsQDiagramView00;
    clsQDiagramView00->mark(true);


    connect(clsQDiagramView00, SIGNAL(sigGotFocus(int,int,string)), this, SLOT(slotDiagViewGotFocus(int,int,string)));
    connect(clsQDiagramView01, SIGNAL(sigGotFocus(int,int,string)), this, SLOT(slotDiagViewGotFocus(int,int,string)));

    connect(clsQDiagramView00, SIGNAL(sigDiagItemActivated(int,string)), this, SIGNAL(sigDiagItemActivated(int,string)));
    connect(clsQDiagramView01, SIGNAL(sigDiagItemActivated(int,string)), this, SIGNAL(sigDiagItemActivated(int,string)));

//--
    connect(clsQDiagramView00, SIGNAL(sigDiagItemActivated(int,string)), this, SLOT(slotDiagViewActivated(int,string)));
    connect(clsQDiagramView01, SIGNAL(sigDiagItemActivated(int,string)), this, SLOT(slotDiagViewActivated(int,string)));
//ALT    connect(this,SIGNAL(sigDiagViewActivated(int,string)), this, SLOT(slotDiagViewActivated(int,string))); 


    qpbtnZoomIn = new QPushButton ( QIcon(QPixmap(zoomIn_xpm)), "", qframeToolbar, "ZoomIn" );
    connect(qpbtnZoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));

    qpbtnZoomOut = new QPushButton ( QIcon(QPixmap(zoomOut_xpm)), "", qframeToolbar, "ZoomOut" );
    connect(qpbtnZoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));

    qpbtnNewProcess = new QPushButton( QIcon(QPixmap(newProcessIcon)), "", qframeToolbar, "NewProcess" );
    connect(qpbtnNewProcess, SIGNAL(clicked(bool)),this, SLOT(slotAddNode(bool)));

    qpbtnNewGroup = new QPushButton( QIcon(QPixmap(newGroupIcon)), "", qframeToolbar, "NewGroup" );
    connect(qpbtnNewGroup, SIGNAL(clicked(bool)),this, SLOT(slotAddNode(bool)));

    qpbtnNewConnectionExcitatory = new QPushButton( QIcon(QPixmap(connectionExcitatory)), "", qframeToolbar, "NewConnExc" );
    connect(qpbtnNewConnectionExcitatory, SIGNAL(clicked(bool)),this, SLOT(slotAddConnectionExcitatory(bool)));

    qpbtnNewConnectionModulatory = new QPushButton( QIcon(QPixmap(connectionModulatory)), "", qframeToolbar, "NewConnMod" );
    connect(qpbtnNewConnectionModulatory, SIGNAL(clicked(bool)),this, SLOT(slotAddConnectionModulatory(bool)));

    qpbtnNewConnectionInhibitory = new QPushButton( QIcon(QPixmap(connectionInhibitory)), "", qframeToolbar, "NewConnInh" );
    connect(qpbtnNewConnectionInhibitory, SIGNAL(clicked(bool)),this, SLOT(slotAddConnectionInhibitory(bool)));




    QToolTip::add(qpbtnZoomIn, "Zoom In");
    QToolTip::add(qpbtnZoomOut, "Zoom Out");
    QToolTip::add(qpbtnNewProcess, "Add Process");
    QToolTip::add(qpbtnNewGroup, "Add Group");
    QToolTip::add(qpbtnNewConnectionExcitatory, "Add Excitatory Connection");
    QToolTip::add(qpbtnNewConnectionModulatory, "Add Modulatory Connection");
    QToolTip::add(qpbtnNewConnectionInhibitory, "Add Inhibitory Connection");
  

    qpbtnZoomIn->setFlat(true);
    qpbtnZoomOut->setFlat(true);
    qpbtnNewProcess->setFlat(true);
    qpbtnNewGroup->setFlat(true);
    qpbtnNewConnectionExcitatory->setFlat(true);
    qpbtnNewConnectionModulatory->setFlat(true);
    qpbtnNewConnectionInhibitory->setFlat(true);
 

    qpbtnZoomIn->setMaximumSize (QSize(22,22));
    qpbtnZoomOut->setMaximumSize (QSize(22,22));
    qpbtnNewProcess->setMaximumSize (QSize(22,22));
    qpbtnNewGroup->setMaximumSize (QSize(22,22));
    qpbtnNewConnectionExcitatory->setMaximumSize (QSize(22,22));
    qpbtnNewConnectionModulatory->setMaximumSize (QSize(22,22));
    qpbtnNewConnectionInhibitory->setMaximumSize (QSize(22,22));


    qpbtnNewProcess->setFocusPolicy(Qt::NoFocus);
    qpbtnNewGroup->setFocusPolicy(Qt::NoFocus);
    qpbtnNewConnectionExcitatory->setFocusPolicy(Qt::NoFocus);
    qpbtnNewConnectionModulatory->setFocusPolicy(Qt::NoFocus);
    qpbtnNewConnectionInhibitory->setFocusPolicy(Qt::NoFocus);

    
    qboxlayoutTB->addWidget(qpbtnZoomIn);
    qboxlayoutTB->addWidget(qpbtnZoomOut);
    qboxlayoutTB->addWidget(qpbtnNewProcess);
    qboxlayoutTB->addWidget(qpbtnNewGroup);
    qboxlayoutTB->addWidget(qpbtnNewConnectionExcitatory);
    qboxlayoutTB->addWidget(qpbtnNewConnectionModulatory);
    qboxlayoutTB->addWidget(qpbtnNewConnectionInhibitory);


    qboxlayoutTB->insertStretch (7, 1 ) ;


    qpbtnItemInfo = new QPushButton( QIcon(QPixmap(infoIcon)), "", qframeToolbar, "ItemInfo" );
    qpbtnItemInfo->setToggleButton(true);

    qpbtnSplitVertical = new QPushButton( QIcon(QPixmap(iconSplitVertical)), "", qframeToolbar, "SplitVer" );
    connect(qpbtnSplitVertical, SIGNAL(clicked()),this, SLOT(slotSplitVertical()));

    qpbtnSplitHorizontal = new QPushButton( QIcon(QPixmap(iconSplitHorizontal)), "", qframeToolbar, "SplitHor" );
    connect(qpbtnSplitHorizontal, SIGNAL(clicked()),this, SLOT(slotSplitHorizontal()));

    qpbtnNoSplit = new QPushButton( QIcon(QPixmap(iconNoSplit)), "", qframeToolbar, "NoSplit" );
    connect(qpbtnNoSplit, SIGNAL(clicked()),this, SLOT(slotNoSplit()));

    QToolTip::add(qpbtnItemInfo, "Get item information");
    QToolTip::add(qpbtnSplitVertical, "Split Vertically");
    QToolTip::add(qpbtnSplitHorizontal, "Split Horizontally");
    QToolTip::add(qpbtnNoSplit, "No Spliting");

    qpbtnItemInfo->setMaximumSize (QSize(24,24));
    qpbtnSplitVertical->setMaximumSize (QSize(24,24));
    qpbtnSplitHorizontal->setMaximumSize (QSize(24,24));
    qpbtnNoSplit->setMaximumSize (QSize(24,24));

    qpbtnItemInfo->setFlat(true);
    qpbtnSplitVertical->setFlat(true);
    qpbtnSplitHorizontal->setFlat(true);

    qpbtnNoSplit->setFlat(true);

    qboxlayoutTB->addWidget(qpbtnItemInfo);
    qboxlayoutTB->addWidget(qpbtnSplitVertical);
    qboxlayoutTB->addWidget(qpbtnSplitHorizontal);
    qboxlayoutTB->addWidget(qpbtnNoSplit);

/* the main block diagram layout */
    qboxlayoutMain= new QVBoxLayout( this );

    qboxlayoutMain->setContentsMargins ( 0, 0, 0, 0 );
    qboxlayoutMain->setSpacing ( 0);

    qboxlayoutMain->addWidget ( qframeToolbar, 0,0 );
    qboxlayoutMain->addWidget ( qsplitter, 10,0 );
/* ---------------- */

    printer = 0;

    clsQDiagramView00->show();
    clsQDiagramView01->hide();

    bSimulationRunning = false;
    slotDiagViewActivated(ClsFESystemManager::ITEM_SYSTEM, "");
}




ClsScene* ClsBlockDiagram::getCanvas(QString qstr) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getCanvas(QString qstr): " << qstr.toStdString() << endl;
#endif
    if(qmapCanvases.find(qstr)!=qmapCanvases.end()){
	ClsScene *clsSceneTemp = (qmapCanvases.find(qstr)).data();
	return clsSceneTemp;
    } else {
//	cout << "ClsBlockDiagram::CANVAS NOT FOUND" << endl;
	return 0;
    }
    return 0;
}

ClsScene* ClsBlockDiagram::getSystemCanvas(){
    return qmapCanvases[ClsFESystemManager::Instance()->getSystemID().c_str()];
}



void ClsBlockDiagram::slotSystemChanged(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::SystemChanged()" << endl;
#endif

/* Effectively this is called if there is a new system
   therefore we also want to:
   - clear the canvas
   - reset the canvas view (i.e. remove all tabs etc
*/
    string strSystemID = ClsFESystemManager::Instance()->getSystemID();
    string strSystemName = ClsFESystemManager::Instance()->getSystemName();

    clsQDiagramView00->addTab(ClsFESystemManager::ITEM_SYSTEM, strSystemName, strSystemID );
    clsQDiagramView01->addTab(ClsFESystemManager::ITEM_SYSTEM, strSystemName, strSystemID );

    setRoot(strSystemID.c_str(), strSystemName.c_str());

    map<string, ClsFEProcess*> mapProcesses =  ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
	string strProcessID = mapIteratorProcesses->second->getProcessID();
	string strProcessName = mapIteratorProcesses->second->getProcessName();
	ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFESystemManager::Instance()->getDiagramIcon(strProcessID);
	addProcess(strProcessID, strProcessName, clsInfoDiagramIcon);
    }

    map<string, ClsFEGroup*> mapGroups = ClsFESystemManager::Instance()->getMapFEGroups();
    map<string, ClsFEGroup*>::iterator mapIteratorGroups;
    for(mapIteratorGroups = mapGroups.begin(); mapIteratorGroups!=mapGroups.end(); ++mapIteratorGroups){
	string strProcessID = mapIteratorGroups->second->getProcessID();
//	cout <<  __FILE__ << ":strProcessID:" << strProcessID << endl;
	string strGroupID = mapIteratorGroups->second->getGroupID();
//	cout << __FILE__ <<  ":strGroupID:" << strGroupID << endl;
	string strGroupName = mapIteratorGroups->second->getGroupName();
//	cout << __FILE__ << ":strGroupName:" << strGroupName << endl;
	ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFESystemManager::Instance()->getDiagramIcon(strGroupID);
	addGroup(strProcessID,strGroupID,strGroupName, clsInfoDiagramIcon);
    }

    /* group module I/O icons */
    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
	string strProcessID = mapIteratorProcesses->second->getProcessID();
	setGroupModuleIOIcons(strProcessID);
    }
    /* --------------- */

    map<string, ClsFEConnection*> mapConnections = ClsFESystemManager::Instance()->getMapFEConnections();
    map<string, ClsFEConnection*>::iterator mapIteratorConnections;
    for(mapIteratorConnections = mapConnections.begin(); mapIteratorConnections!=mapConnections.end(); ++mapIteratorConnections){
	string strConnectionID = mapIteratorConnections->second->getConnectionID();
	ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strConnectionID);
	addConnection(strConnectionID, clsInfoDiagramLine);
    }

   
    ClsScene *clsSceneTemp = (qmapCanvases.find(strSystemID.c_str())).data();
    if(clsSceneTemp!=NULL){
	clsQDiagramView00->setScene(clsSceneTemp);
	clsQDiagramView01->setScene(clsSceneTemp);
    }

};


void ClsBlockDiagram::slotItemAdded(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemAdded(int iType, string strID )" << endl;
#endif

    if(iType==ClsFESystemManager::ITEM_PROCESS){
	ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFESystemManager::Instance()->getDiagramIcon(strID);
	string strProcessName = ClsFESystemManager::Instance()->getProcessName(strID);
	addProcess(strID, strProcessName, clsInfoDiagramIcon);
    }
    else if(iType==ClsFESystemManager::ITEM_GROUP){
	ClsInfoDiagramIcon clsInfoDiagramIcon = ClsFESystemManager::Instance()->getDiagramIcon(strID);
	string strGroupName = ClsFESystemManager::Instance()->getGroupName(strID);
	string strProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strID);
	addGroup(strProcessID,strID,strGroupName, clsInfoDiagramIcon);
    }
}


void ClsBlockDiagram::slotItemDuplicated(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemDuplicated(int iType, string strID )" << endl;
#endif

    slotItemAdded( iType,  strID );

    if(iType==ClsFESystemManager::ITEM_CONNECTION){
	ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strID);
	string strConnectionID = ClsFESystemManager::Instance()->getConnectionID(strID);
	addConnection(strConnectionID, clsInfoDiagramLine);
    }

}


void ClsBlockDiagram::slotItemChanged(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemChanged(int iType, string strID )" << endl;
    cout << "STRID: " << strID << endl;
#endif

    string strName = "";
    if(iType==ClsFESystemManager::ITEM_SYSTEM){
	strName = ClsFESystemManager::Instance()->getSystemName( );
	clsQDiagramView00->renameTab(strID, strName );
	clsQDiagramView01->renameTab(strID, strName );

    }
    else if(iType==ClsFESystemManager::ITEM_PROCESS){
	strName = ClsFESystemManager::Instance()->getProcessName( strID );
	if(qmapNodes.find(strID) != qmapNodes.end()){
	    qmapNodes.find(strID).data()->setName(strName);
	    
	    qmapNodes.find(strID).data()->setColor(ClsFESystemManager::Instance()->getFEProcess(strID)->getColor());

	    resetGroupModuleIOIcons(strID);

	    if(ClsFESystemManager::Instance()->getFEProcess(strID)->hasModule()){
		moduleIcon mi = ClsFESystemManager::Instance()->getFEProcess(strID)->getModuleIcon();
		bool bModuleEnabled = ClsFESystemManager::Instance()->getFEProcess(strID)->getEnableModules();
		dynamic_cast<ClsDiagItemProcess*>(qmapNodes.find(strID).data())->setModuleIcon(mi);
		dynamic_cast<ClsDiagItemProcess*>(qmapNodes.find(strID).data())->setModuleEnabled(bModuleEnabled);
		setGroupModuleIOIcons(strID);
	    } else {
		dynamic_cast<ClsDiagItemProcess*>(qmapNodes.find(strID).data())->removeModuleIcon();
	    }


	}
	clsQDiagramView00->renameTab(strID, strName );
	clsQDiagramView01->renameTab(strID, strName );

    }
    else if (iType==ClsFESystemManager::ITEM_GROUP){
	strName = ClsFESystemManager::Instance()->getGroupName( strID );
	if(qmapNodes.find(strID) != qmapNodes.end()){
	    qmapNodes.find(strID).data()->setName(strName);
	    qmapNodes.find(strID).data()->setColor(ClsFESystemManager::Instance()->getFEGroup(strID)->getColor());
	}

	QList<ClsDiagItemPhantom*> values = qmapPhantomNodes.values(strID);
	for (int i = 0; i < values.size(); ++i){
	    values.at(i)->setName(strName);
	    values.at(i)->setColor(ClsFESystemManager::Instance()->getFEGroup(strID)->getColor());
	}
	
    }
    else if (iType==ClsFESystemManager::ITEM_CONNECTION){
	int iConnectionType= ClsFESystemManager::Instance()->getFEConnection( strID )->getConnectionTypeAsInt();
	if(qmapHyperConnections.find(strID) != qmapHyperConnections.end()){
	    qmapHyperConnections.find(strID).data()->setConnectionType(iConnectionType);
	}
    }
}







void ClsBlockDiagram::slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType, int iCanvasConnectionType){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType)" << endl;
    cout << "strConnectionID: " << strConnectionID << endl;
#endif

    string strOldGroupID = "";
    string strSourceID = "";
    string strTargetID = "";
  


    if(qmapHyperConnections.find(strConnectionID) != qmapHyperConnections.end()){
	if(iEndType == ClsDiagConnection::CONNECTION_SOURCE){
//	    cout << "CONNECTION SOURCE" << endl;
 	    strSourceID = strGroupID;
 	    strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
	} else {
//	    cout << "CONNECTION TARGET" << endl;
 	    strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
 	    strTargetID = strGroupID;
	}
    }
}


void ClsBlockDiagram::deleteProcess(string strProcessID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::deleteProcess(string strProcessID):" << strProcessID << endl;
#endif

    if(qmapNodes.find(strProcessID) != qmapNodes.end()){
	ClsDiagItem *clsDiagItemProcess = qmapNodes.find(strProcessID).data();

	QGraphicsScene* qCanvasProcess = getCanvas( strProcessID.c_str());

	ClsStorage<ClsDiagItem*, ClsDiagItem*> clsStorage;
	clsStorage.addParent(clsDiagItemProcess);

	QMap<string, ClsDiagItem*>::Iterator it;
	for ( it = qmapNodes.begin(); it != qmapNodes.end(); ++it ) {
	    ClsDiagItem *clsDiagItem = it.data();

	    /* we have to get the parent of the node via the canvas,
	       because in the systemmanager the item is already deleted... */
	    QGraphicsScene* qCanvasNode = clsDiagItem->scene();
	    if(qCanvasNode == qCanvasProcess){
		clsStorage.addChild(clsDiagItem);
		string strNodeID = clsDiagItem->getID();
		clsDiagItem->Hide();


		list<string> lstConnections;
		QMap<string, ClsDiagHyperConnection*>::Iterator itC;
		/* compile a list of connection that source from or target one of the groups in the process
		   we have to go via creating the list, because we cannot delete the connections from the map immediately
		   as this screws up the deleteConnection function...
		*/
		for ( itC = qmapHyperConnections.begin(); itC != qmapHyperConnections.end(); ++itC ) {
		    string strConnectionID =  itC.data()->getID();
		    string strSourceID = itC.data()->getSourceGroupID();
		    string strTargetID = itC.data()->getTargetGroupID();
		    if(!strNodeID.compare(strSourceID) || !strNodeID.compare(strTargetID)){
			lstConnections.push_back(strConnectionID);
		    }
		}

		lstConnections.unique();
		/* delete all the connections in the list */
		list<string>::iterator itList;
		for(itList = lstConnections.begin(); itList != lstConnections.end(); ++itList){
//		    cout << "to delete: " << *itList << endl;
		    deleteConnection(*itList);
		}

		deletePhantomGroups(strNodeID);
	    }
	}

	qmapNodes.erase(qmapNodes.find(strProcessID));
	clsDiagItemProcess->Hide(); //ZZZ we should delete the object...

	/* remove the Canvas and the Tab... */
	clsQDiagramView00->removeTab(strProcessID );
	clsQDiagramView01->removeTab(strProcessID );

	if(qmapCanvases.find(strProcessID.c_str())!=qmapCanvases.end()){
//	    delete qmapCanvases.find(strProcessID.c_str()).data(); /* calls the QCanvas destructor, which also removes all items on it.. */
	    qmapCanvases.erase(qmapCanvases.find(strProcessID.c_str()));
	}
    }
}

void ClsBlockDiagram::deletePhantomGroups(string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::deletePhantomGroups(string strID)" << endl;
#endif
    QMap<string, ClsDiagItemPhantom*>::Iterator it;
    for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	if(!strID.compare(it.data()->getID())){
	    /* this can lead to segfault...	*/    
	    delete it.data();
	    qmapPhantomNodes.erase(it);
//	    it.data()->Hide();
	}
    }
}

void ClsBlockDiagram::deleteGroup(string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::deleteGroup(string strID): " << strID << endl;
#endif

    if(qmapNodes.find(strID) != qmapNodes.end()){
//	ClsDiagItem *clsDiagItem = qmapNodes.find(strID).data();
	delete qmapNodes.find(strID).data();
	qmapNodes.erase(qmapNodes.find(strID));
    }

    deletePhantomGroups(strID);

   
    QMap<string, ClsDiagHyperConnection*>::Iterator itC;
    for ( itC = qmapHyperConnections.begin(); itC != qmapHyperConnections.end(); ++itC ) {
/*
  cout << "itC.data()->getID(): "  << itC.data()->getID() << endl;
  cout << "itC.data()->getSourceID(): "  << itC.data()->getSourceGroupID() << endl;
  cout << "itC.data()->getTargetID(): " <<  itC.data()->getTargetGroupID() << endl;;
  cout << "--------------" << endl;
*/
	if(!strID.compare(itC.data()->getSourceGroupID())){
	    itC.data()->setSourceGroupID("");
	}

	if(!strID.compare(itC.data()->getTargetGroupID())){
	    itC.data()->setTargetGroupID("");
	}
    }


}

void ClsBlockDiagram::deleteConnection(string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "\n\tClsBlockDiagram::deleteConnection(string strID)::strID:" << strID << endl;
#endif

/* design comment:
   the removal of all the concerned AP references is pretty lenghty. This might be a call
   for a signals/slots implementation. At the moment, though, the ClsDiagItem class is
   not derived from QObject...
*/

    if(qmapHyperConnections.find(strID) != qmapHyperConnections.end()){
	delete qmapHyperConnections.find(strID).data();
	qmapHyperConnections.remove(strID);
    }
}

void ClsBlockDiagram::slotItemDeleted (int iType, string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemDeleted (int iType, string strID)" << endl;
#endif
    if (iType== ClsFESystemManager::ITEM_PROCESS){
	deleteProcess(strID);
    }
    else if (iType== ClsFESystemManager::ITEM_GROUP){
	deleteGroup(strID);
    }
    else if (iType== ClsFESystemManager::ITEM_CONNECTION){
	deleteConnection(strID);
    }
}


void ClsBlockDiagram::slotItemUnDeleted(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemUnDeleted(int iType, string strID )" << endl;
#endif

    iType = 0;
    strID = "";
/*
  DON'T FORGET TO REBUILD THE PHANTOM GROUPS!
*/
}

string ClsBlockDiagram::getNewConnectionProcessID() {
    if(clsDiagItemAPSource!=NULL){
	return dynamic_cast<ClsScene*>(clsDiagItemAPSource->scene())->getID();
    }
    else {
	return "";
    }
};


void ClsBlockDiagram::setRoot(string strID, string strName) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setRoot(string strID, string strName)" << endl;
#endif

    strName = "";
    ClsScene *clsSceneTemp = new ClsScene(this, strID, ClsScene::CANVAS_SYSTEM);
    clsSceneTemp->setSceneRect(0, 0, CANVAS_DEFAULT_X, CANVAS_DEFAULT_Y);

    qmapCanvases[strID.c_str()] = clsSceneTemp;
};


void ClsBlockDiagram::addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon)" << endl;
#endif

    if(qmapNodes.find(strPrcID) == qmapNodes.end()){
	clsQDiagramView00->addTab(ClsFESystemManager::ITEM_PROCESS, strPrcName, strPrcID );
	clsQDiagramView01->addTab(ClsFESystemManager::ITEM_PROCESS, strPrcName, strPrcID );

	ClsScene *clsSceneTemp = new ClsScene(this, strPrcID, ClsScene::CANVAS_PROCESS);
	clsSceneTemp->setName(strPrcID.c_str());
	clsSceneTemp->setSceneRect(0, 0, CANVAS_DEFAULT_X, CANVAS_DEFAULT_Y);


	qmapCanvases[strPrcID.c_str()] = clsSceneTemp;

/* draw the prc on the first canvas in the map (i.e. the systemview canvas) */
	string strSystemID = ClsFESystemManager::Instance()->getSystemID();
	ClsScene * clsSceneTemp2 = qmapCanvases[strSystemID.c_str()];

	ClsDiagItem *i = new ClsDiagItemProcess( ClsFESystemManager::ITEM_PROCESS,
						 clsInfoDiagramIcon.getX(), clsInfoDiagramIcon.getY(),
						 clsInfoDiagramIcon.getHeight(), clsInfoDiagramIcon.getWidth(),
						 NULL,
						 strPrcName, strPrcID);

	if(ClsFESystemManager::Instance()->getFEProcess(strPrcID)!=NULL){
	    i->setColor(ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getColor());
	}


	clsSceneTemp2->addItem(i);
	i->show();

	if(ClsFESystemManager::Instance()->getFEProcess(strPrcID)->hasModule()){
	    moduleIcon mi = ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getModuleIcon();
	    bool bModuleEnabled = ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getEnableModules();
	    dynamic_cast<ClsDiagItemProcess*>(i)->setModuleIcon(mi);
	    dynamic_cast<ClsDiagItemProcess*>(i)->setModuleEnabled(bModuleEnabled);
	}

	if(ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getPath().length()>0){
	    dynamic_cast<ClsDiagItemProcess*>(i)->setIsExternal(true);
	} else {
	    dynamic_cast<ClsDiagItemProcess*>(i)->setIsExternal(false);
	}

	i->setZValue(0);
	i->show();
	qmapNodes[strPrcID] = i;
    }

};


void  ClsBlockDiagram::addGroup(string strPrcID, string strGroupID, string strGroupName, ClsInfoDiagramIcon clsInfoDiagramIcon) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::addGroup(string strPrcID, string strGroupID, string strGroupName,....)" << endl;
#endif

    if(qmapNodes.find(strGroupID) == qmapNodes.end()){

	ClsScene *clsSceneTemp = (qmapCanvases.find(strPrcID.c_str())).data();
//    cout << "clsSceneTemp->name():" << clsSceneTemp->name() << endl;

	ClsDiagItem *i = new ClsDiagItemGroup( ClsFESystemManager::ITEM_GROUP,
					       clsInfoDiagramIcon.getX(), clsInfoDiagramIcon.getY(),
					       clsInfoDiagramIcon.getHeight(), clsInfoDiagramIcon.getWidth(),
					       NULL,
					       strGroupName, strGroupID);
	if(ClsFESystemManager::Instance()->getFEGroup(strGroupID)!=NULL){
	    i->setColor(ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getColor());
	}


	clsSceneTemp->addItem(i);
	i->setZValue(0);
	i->show();
	qmapNodes[strGroupID] = i;
    }
};



void  ClsBlockDiagram::createProcess(string, int iX, int iY) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::createProcess(string strID, int iX, int iY)"<< endl;
#endif
    string strPrcID = ClsFESystemManager::Instance()->createProcess(); //####
    /* 
       the creation happend actually from the SystemManager back to
       this via addProcess...
    */

    if(qmapNodes.find(strPrcID) != qmapNodes.end()){
	qmapNodes.find(strPrcID).data()->moveBy(iX, iY );

    }
}


void  ClsBlockDiagram::createGroup(string strPrcID, int iX, int iY) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::createGroup(string strPrcID, int iX, int iY)" << endl;
#endif

    string strGroupID = ClsFESystemManager::Instance()->createGroup(strPrcID);

    if(qmapNodes.find(strGroupID) != qmapNodes.end()){
	qmapNodes.find(strGroupID).data()->moveBy(iX, iY );

    }

};

void ClsBlockDiagram::addConnection(string strConnectionID, ClsInfoDiagramLine clsInfoDiagramLine){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "++ClsBlockDiagram::addConnection(string strConnectionID, string strConnectionName, " <<
	"string strSourceID, string strTargetID, int iConnectionType, ClsInfoDiagramLine clsInfoDiagramLine)" << endl;
    cout << "ID: " << strConnectionID << endl;
#endif

    if(qmapHyperConnections.find(strConnectionID) == qmapHyperConnections.end()){
	ClsDiagHyperConnection* hyperConn = new ClsDiagHyperConnection( strConnectionID,  clsInfoDiagramLine);
	qmapHyperConnections[strConnectionID] = hyperConn;

    }
    
};


void ClsBlockDiagram::init(){
    clear();
};

ClsBlockDiagram::~ClsBlockDiagram() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::~ClsBlockDiagram()" << endl;
#endif
    cleanup();
}

void ClsBlockDiagram::cleanup() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::cleanup()" << endl;
#endif


    QMap<string, ClsDiagHyperConnection*>::Iterator itqmapHyperConnections;
    for (itqmapHyperConnections = qmapHyperConnections.begin(); itqmapHyperConnections != qmapHyperConnections.end(); ++itqmapHyperConnections){
	delete itqmapHyperConnections.data();
    }
    qmapHyperConnections.clear();


    QMap<string, ClsDiagItem*>::Iterator itqmapNodes;
    for (itqmapNodes = qmapNodes.begin(); itqmapNodes != qmapNodes.end(); ++itqmapNodes){
	delete itqmapNodes.data();
    }
    qmapNodes.clear();

    QMap<string, ClsDiagItemPhantom*>::Iterator itqmapPhantomNodes;
    for (itqmapPhantomNodes = qmapPhantomNodes.begin(); itqmapPhantomNodes != qmapPhantomNodes.end(); ++itqmapPhantomNodes){
	delete itqmapPhantomNodes.data();
    }
    qmapPhantomNodes.clear();

    QMap<QString, ClsScene*>::Iterator itqmapCanvases;
    for (itqmapCanvases = qmapCanvases.begin(); itqmapCanvases != qmapCanvases.end(); ++itqmapCanvases){
	delete itqmapCanvases.data();
    }
    qmapCanvases.clear();

    delete printer;
    printer = NULL;

    delete qboxlayoutMain;
    qboxlayoutMain = NULL;

    delete qframeToolbar;
    qframeToolbar = NULL;

    delete qsplitter;
    qsplitter = NULL;


}


void ClsBlockDiagram::clear(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::clear()" << endl;
#endif
    clsQDiagramView00->clear();
    clsQDiagramView01->clear();
    resetAddState();
}



void ClsBlockDiagram::print(){
    if ( !printer ) {
	printer = new QPrinter;
    }

    if ( printer->setup(this) ) {
	if(clsQDiagramViewActive!=NULL){
	    QGraphicsScene * clsSceneTemp = clsQDiagramViewActive->scene();

	    QPainter pp(printer);
	    QRectF qrect2save = clsSceneTemp->itemsBoundingRect ();
	    clsSceneTemp->render( &pp, QRectF(0, 0, 0, 0), qrect2save );
	}

    }
}

void ClsBlockDiagram::save(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::save()" << endl;
#endif

    QString filename = QFileDialog::getSaveFileName ( this, 
						      "save file dialog Choose a filename to save under", 
						      "", 
						      "Images (*.svg *.png)" );

    if(filename.length()<=0){
	return;
    }
    if(clsQDiagramViewActive!=NULL){
	QGraphicsScene * clsSceneTemp = clsQDiagramViewActive->scene();
	QRectF qrect2save=clsSceneTemp->itemsBoundingRect ();
/* check the format */
	if(filename.contains( ".svg", FALSE )){
//	cout << "saving as svg" << endl;
	    QSvgGenerator qsvggenerator;
	    qsvggenerator.setFileName ( filename );
	    qsvggenerator.setSize(qrect2save.size().toSize());
	    qsvggenerator.setViewBox(QRectF(0, 0, qrect2save.width(), qrect2save.height()));
	    QPainter paint(&qsvggenerator);
	    clsSceneTemp->render(&paint, QRectF(0, 0, 0, 0), qrect2save);
	    paint.end();
	} else {
//	cout << "saving as png" << endl;
	    QImage* image = new QImage(qrect2save.width(), qrect2save.height(), QImage::Format_ARGB32_Premultiplied);
	    QPainter paint( image );
	    clsSceneTemp->render(&paint, QRectF(0, 0, 0, 0), qrect2save);
	    paint.end();
	    image->save(filename, "PNG");
	}
    }
}



void ClsBlockDiagram::zoomIn() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::zoomIn()" << endl;
#endif
    resetAddState();
    if(clsQDiagramViewActive!=NULL){
	clsQDiagramViewActive->zoomIn();
    }
};

void ClsBlockDiagram::zoomOut() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::zoomOut()" << endl;
#endif
    resetAddState();
    if(clsQDiagramViewActive!=NULL){
	clsQDiagramViewActive->zoomOut();
    }
};

bool ClsBlockDiagram::infoMode(){
    return qpbtnItemInfo->isOn();
}



void ClsBlockDiagram::slotSplitHorizontal(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotSplitHorizontal()" << endl;
#endif
    qsplitter->setOrientation(Qt::Vertical );

    if(clsQDiagramViewActive == clsQDiagramView00){
	clsQDiagramView01->show();
    } else {
	clsQDiagramView00->show();
    }

};

void ClsBlockDiagram::slotSplitVertical() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotSplitVertical()" << endl;
#endif

    qsplitter->setOrientation(Qt::Horizontal );

    if(clsQDiagramViewActive == clsQDiagramView00){
	clsQDiagramView01->show();
    } else {
	clsQDiagramView00->show();
    }

   
};

void ClsBlockDiagram::slotNoSplit(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotNoSplit()" << endl;
#endif
    if(clsQDiagramViewActive == clsQDiagramView00){
	clsQDiagramView01->hide();
    } else {
	clsQDiagramView00->hide();
    }
};



ClsDiagItem* ClsBlockDiagram::getCanvasNode( string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getCanvasNode( string strID)" << endl;
#endif
    if(qmapNodes.find(strID) != qmapNodes.end()){
	return qmapNodes[strID];
    }
    return 0;
};

void ClsBlockDiagram::slotAddNode(bool){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddNode()" << endl;
#endif
    iAddState = ClsBlockDiagram::ADD_NODE;
    clsQDiagramView00->setAddState(iAddState);
    clsQDiagramView01->setAddState(iAddState);
};

void ClsBlockDiagram::slotAddConnectionExcitatory(bool){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionExcitatory()" << endl;
#endif
    iAddState = ClsBlockDiagram::ADD_CONNECTION_EXC;
    clsQDiagramView00->setAddState(iAddState);
    clsQDiagramView01->setAddState(iAddState);
};
void ClsBlockDiagram::slotAddConnectionInhibitory(bool){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionInhibitory()" << endl;
#endif
    iAddState = ClsBlockDiagram::ADD_CONNECTION_INH;
    clsQDiagramView00->setAddState(iAddState);
    clsQDiagramView01->setAddState(iAddState);
};
void ClsBlockDiagram::slotAddConnectionModulatory(bool){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionModulatory()" << endl;
#endif
    iAddState = ClsBlockDiagram::ADD_CONNECTION_MOD;
    clsQDiagramView00->setAddState(iAddState);
    clsQDiagramView01->setAddState(iAddState);

};



/* serialization related */
ClsInfoDiagramIcon ClsBlockDiagram::getDiagramIcon(string strID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getDiagramIcon(string strID): " << strID << endl;
#endif
//    QMap<string, ClsDiagItem*> qmapNodes;
    ClsInfoDiagramIcon clsInfoDiagramIcon;
    if(qmapNodes.find(strID.c_str())!=qmapNodes.end()){
	ClsDiagItem *n = qmapNodes[strID.c_str()];
	QPointF p = n->getPosition ();
	QSizeF s = n->getSize ();
	clsInfoDiagramIcon.setX(p.x());
	clsInfoDiagramIcon.setY(p.y());
	clsInfoDiagramIcon.setWidth(s.width());
	clsInfoDiagramIcon.setHeight(s.height());
    }
    return clsInfoDiagramIcon;
};

ClsInfoDiagramLine ClsBlockDiagram::getDiagramLine(string strID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getDiagramLine(string strID)::strID::" << strID << endl;
#endif
    ClsInfoDiagramLine clsInfoDiagramLine;

    if(qmapHyperConnections.find(strID.c_str())!=qmapHyperConnections.end()){
	ClsDiagHyperConnection *n = qmapHyperConnections[strID.c_str()];
	vector<vector<int> > v = n->getPoints();
	for(unsigned int ii = 0; ii< v.size(); ii++){
//	    cout << "adding point" << endl;
	    int iX = v[ii][0];
	    int iY = v[ii][1];
	    clsInfoDiagramLine.addPoint( iX, iY);
	}

	int iAPSource = qmapHyperConnections.find(strID).data()->getStartAPNumber();
	int iAPTarget = qmapHyperConnections.find(strID).data()->getEndAPNumber();

	clsInfoDiagramLine.setAPSource(iAPSource);
	clsInfoDiagramLine.setAPTarget(iAPTarget);
//	cout << "\t\tAPSource:: " << iAPSource << endl;
//	cout << "\t\tAPTarget:: " << iAPTarget << endl;
    }
    return clsInfoDiagramLine;
};


void ClsBlockDiagram::showProcessTab(string strProcessID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::showProcessTab(string strProcessID)" << endl;
    cout << "strProcessID: " << strProcessID << endl;
#endif

    if(clsQDiagramViewActive!=NULL){
	clsQDiagramViewActive->showProcessTab(strProcessID);
    }
}


void ClsBlockDiagram::slotDiagViewGotFocus(int iID, int iCanvasType, string strID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagViewGotFocus(int iID, int iCanvasType): " << iID << ", " << iCanvasType << ", " << strID << endl;
#endif
    
    if(iID == DIAG_00){
	clsQDiagramViewActive = clsQDiagramView00;
	clsQDiagramView01->mark(false);
    } else if(iID == DIAG_01){
	clsQDiagramViewActive = clsQDiagramView01;
	clsQDiagramView00->mark(false);
    }
    
    int iType = -99;
    if(iCanvasType==ClsScene::CANVAS_SYSTEM){
	iType = ClsFESystemManager::ITEM_SYSTEM;
    }
    else if(iCanvasType==ClsScene::CANVAS_PROCESS){
	iType = ClsFESystemManager::ITEM_PROCESS;
    }

//ALT    emit sigDiagViewActivated(iType, strID);
}

void ClsBlockDiagram::slotDiagViewActivated(int iActiveItemType, string) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemActivated(int iActiveItemType,string): " << iActiveItemType << endl;
#endif
    
    if(bSimulationRunning){
	qpbtnNewProcess->setEnabled(false);
	qpbtnNewGroup->setEnabled(false);
	qpbtnNewConnectionExcitatory->setEnabled(false);
	qpbtnNewConnectionModulatory->setEnabled(false);
	qpbtnNewConnectionInhibitory->setEnabled(false);
    } else {
	if(iActiveItemType == ClsFESystemManager::ITEM_SYSTEM){
//	    cout << "SYS" << endl;
	    qpbtnNewProcess->setEnabled(true);
	    qpbtnNewGroup->setEnabled(false);
	    qpbtnNewConnectionExcitatory->setEnabled(false);
	    qpbtnNewConnectionModulatory->setEnabled(false);
	    qpbtnNewConnectionInhibitory->setEnabled(false);
	    
	}
	else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){
//	    cout << "PRC" << endl;
	    qpbtnNewProcess->setEnabled(false);
	    qpbtnNewGroup->setEnabled(true);
	    qpbtnNewConnectionExcitatory->setEnabled(true);
	    qpbtnNewConnectionModulatory->setEnabled(true);
	    qpbtnNewConnectionInhibitory->setEnabled(true);
	}
    }
    iCurrentActivItemType = iActiveItemType;
}


ClsDiagItemPhantom* ClsBlockDiagram::getDiagItemPhantom(QGraphicsScene* clsSceneTemp, string strID, ClsDiagItem* clsDiagItemNeighbour){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getDiagItemPhantom(QGraphicsScene* clsSceneTemp, string strID, ClsDiagItem* clsDiagItemNeighbour)" << endl;
    cout << "strID: " << strID << endl;
#endif


    ClsDiagItemPhantom* clsDiagItemPhantom = NULL;

    /* first we check if the scene already has a phantom group */
    QList<QGraphicsItem *> qlstItems = clsSceneTemp->items ();
    QList<QGraphicsItem *>::const_iterator itItems;
    for(itItems = qlstItems.begin(); itItems != qlstItems.end(); itItems++){
	if(dynamic_cast<ClsDiagItemPhantom*>(*itItems)){
	    string strIDTemp = dynamic_cast<ClsDiagItemPhantom*>(*itItems)->getID();
	    if(!strIDTemp.compare(strID)){
		clsDiagItemPhantom = dynamic_cast<ClsDiagItemPhantom*>(*itItems);
	    }
	}
    }


    if(clsDiagItemPhantom==NULL) {
	string strGroupName = ClsFESystemManager::Instance()->getGroupName(strID);
	
	
	int iXNearest = clsDiagItemNeighbour->getCenter().x();
	clsDiagItemPhantom = new ClsDiagItemPhantom( ClsFESystemManager::ITEM_GROUP,
						     iXNearest - PHANTOMGROUP_WIDTH/2, -3, PHANTOMGROUP_WIDTH, 24,
						     NULL,
						     strGroupName, strID);
	
	if(ClsFESystemManager::Instance()->getFEGroup(strID)!=NULL){
	    clsDiagItemPhantom->setColor(ClsFESystemManager::Instance()->getFEGroup(strID)->getColor());
	}
	
	
	clsSceneTemp->addItem(clsDiagItemPhantom);
	
/* this still might not quite work, but it's the best I can come up at the moment...*/
	QMap<string, ClsDiagItemPhantom*>::Iterator it;
	for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	    if(it.data()->scene() == clsSceneTemp){
		while(clsDiagItemPhantom->collidesWithItem(it.data())) {
		    clsDiagItemPhantom->moveBy(10,0);
		}
	    }
	}
	qmapPhantomNodes.insert(strID, clsDiagItemPhantom);
    }

    clsDiagItemPhantom->setZValue(0);
    clsDiagItemPhantom->show();
    return clsDiagItemPhantom;
}



void ClsBlockDiagram::prunePhantomGroups(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::prunePhantomGroups()" << endl;
#endif
    QMap<string, ClsDiagItemPhantom*>::Iterator it;

    for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	if(!it.data()->hasActiveAPs()){
//	    cout << "Free phantom group: " << it.key() << endl;
	    deletePhantomGroups(it.data()->getID());
	}
    }

#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "Exit ClsBlockDiagram::prunePhantomGroups()" << endl;
#endif

}

ClsDiagHyperConnection* ClsBlockDiagram::getCanvasHyperConnection(string strID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getCanvasHyperConnection(string strID)" << endl;
#endif
    if(qmapHyperConnections.find(strID) != qmapHyperConnections.end()){
	return qmapHyperConnections[strID];
    }
    return NULL;
}





void ClsBlockDiagram::setGroupModuleIOIcons(string strProcessID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setGroupModuleIOIcons(string strProcessID)" << endl;
#endif
    list<string>::iterator it;
    list<string> lstIFM = ClsFESystemManager::Instance()->getFEProcess(strProcessID)->getListGroupsInputFromModule();
    for(it=lstIFM.begin(); it!=lstIFM.end(); ++it){
	setClsDiagItemGroupIcon ((*it), ClsDiagItemIcon::ICON_GROUP_MODULE_IN, true);
//		    cout << "lstIFM: " << (*it) << endl;
    }
    
    list<string> lstOTM = ClsFESystemManager::Instance()->getFEProcess(strProcessID)->getListGroupsOutputToModule();
    for(it=lstOTM.begin(); it!=lstOTM.end(); it++){
//		    cout << "lstOTM: " << (*it) << endl;
	setClsDiagItemGroupIcon ((*it), ClsDiagItemIcon::ICON_GROUP_MODULE_OUT, true);
    }
}


void ClsBlockDiagram::resetGroupModuleIOIcons(string strProcessID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::resetGroupModuleIOIcons(string strProcessID)" << endl;
#endif

    map<string, ClsFEGroup*> mapFEGroups;
    mapFEGroups = ClsFESystemManager::Instance()->getMapFEGroups();

    for ( map<string, ClsFEGroup*>::iterator it = mapFEGroups.begin(); it != mapFEGroups.end(); ++it ){
	if(!strProcessID.compare((*it).second->getProcessID())){
	    string strGroupID = (*it).first;
	    setClsDiagItemGroupIcon (strGroupID, ClsDiagItemIcon::ICON_GROUP_MODULE_IN, false);
	    setClsDiagItemGroupIcon (strGroupID, ClsDiagItemIcon::ICON_GROUP_MODULE_OUT, false);
	}
    }
}

void ClsBlockDiagram::resetGroupSynapseInputIcons(string /* strProcessID */){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::resetGroupSynapseInputIcons(string /* strProcessID */)" << endl;
#endif

}

void ClsBlockDiagram::setClsDiagItemGroupIcon (string strGroupID, int iIconType, bool b){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setClsDiagItemGroupIcon (string strGroupID, int iIconType, bool b)" << endl;
#endif
    if(qmapNodes.find(strGroupID) != qmapNodes.end()){
	if( dynamic_cast<ClsDiagItemGroup*>(qmapNodes.find(strGroupID).data()) ){
	    if(iIconType == ClsDiagItemIcon::ICON_GROUP_MODULE_IN){
		dynamic_cast<ClsDiagItemGroup*>(qmapNodes.find(strGroupID).data())->setHasInputFromModule(b);
	    } else if (iIconType == ClsDiagItemIcon::ICON_GROUP_MODULE_OUT){
		dynamic_cast<ClsDiagItemGroup*>(qmapNodes.find(strGroupID).data())->setHasOutputToModule(b);
	    } else if (iIconType == ClsDiagItemIcon::ICON_GROUP_SYNAPSE_IN){
		dynamic_cast<ClsDiagItemGroup*>(qmapNodes.find(strGroupID).data())->setHasInputFromSynapse(b);
	    }
	}
    }
}




const map<string, int> ClsBlockDiagram::getListSelectedItems(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getListSelectedItems()" << endl;
#endif
    map<string, int> mapSelections;
    if(clsQDiagramViewActive!=NULL){
	mapSelections = clsQDiagramViewActive->getListSelectedItems();
    }    
    return mapSelections;
}

void ClsBlockDiagram::markItem(int iType, string strID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::markItem(int iType, string strID)" << endl;
#endif

    if(iType == ClsFESystemManager::ITEM_PROCESS || iType == ClsFESystemManager::ITEM_GROUP){
	if(qmapNodes.find(strID) != qmapNodes.end()){
	    qmapNodes[strID]->mark(true);
	}
    }
    else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	if(qmapHyperConnections.find(strID) != qmapHyperConnections.end()){
	    qmapHyperConnections[strID]->mark(true);
	}
    } 

}


void ClsBlockDiagram::disableToolbarButtons(bool _b){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::disableToolbarButtons(bool b)" << endl;
#endif

    bSimulationRunning = _b;
    if(bSimulationRunning){
	qpbtnNewProcess->setEnabled(false);
	qpbtnNewGroup->setEnabled(false);
	qpbtnNewConnectionExcitatory->setEnabled(false);
	qpbtnNewConnectionModulatory->setEnabled(false);
	qpbtnNewConnectionInhibitory->setEnabled(false);
    } else {
	if(iCurrentActivItemType == ClsFESystemManager::ITEM_SYSTEM){
	    qpbtnNewProcess->setEnabled(true);
	    qpbtnNewGroup->setEnabled(false);
	    qpbtnNewConnectionExcitatory->setEnabled(false);
	    qpbtnNewConnectionModulatory->setEnabled(false);
	    qpbtnNewConnectionInhibitory->setEnabled(false);
	    
	}
	else if(iCurrentActivItemType == ClsFESystemManager::ITEM_PROCESS){
	    qpbtnNewProcess->setEnabled(false);
	    qpbtnNewGroup->setEnabled(true);
	    qpbtnNewConnectionExcitatory->setEnabled(true);
	    qpbtnNewConnectionModulatory->setEnabled(true);
	    qpbtnNewConnectionInhibitory->setEnabled(true);
	}
    }
}



void ClsBlockDiagram::mouseLeftClickAP(ClsDiagItemAP* ap){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::mouseLeftClickAP(ClsDiagItemAP* ap)" << endl;
#endif

    if(iAddState==ClsBlockDiagram::ADD_CONNECTION_EXC ||
       iAddState==ClsBlockDiagram::ADD_CONNECTION_INH ||
       iAddState==ClsBlockDiagram::ADD_CONNECTION_MOD ){

	if(ap->parentItem()->type() == nodegroupRTTI){
	    if(clsDiagItemAPSource == NULL){
		clsDiagItemAPSource = ap; 
	    } else {
		
		ClsDiagItemAP *clsDiagItemNewConnectionAPTaget = ap;
		string strSourceID = clsDiagItemAPSource->getParentID();
		string strTargetID = clsDiagItemNewConnectionAPTaget->getParentID();
		
		int iConnectionType = -99;
		if(iAddState==ClsBlockDiagram::ADD_CONNECTION_EXC){
		    iConnectionType = ClsInfoConnection::CONN_EXCITATORY;
		}
		else if(iAddState==ClsBlockDiagram::ADD_CONNECTION_INH){
		    iConnectionType = ClsInfoConnection::CONN_INHIBITORY;
		}
		else if(iAddState==ClsBlockDiagram::ADD_CONNECTION_MOD){
		    iConnectionType = ClsInfoConnection::CONN_MODULATORY;				
		}
		
		
		string strConnectionID = ClsFESystemManager::Instance()->createConnection(strSourceID, strTargetID, iConnectionType);
		
		
		string strProcessIDSource = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
		string strProcessIDTarget = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
		
		if(strProcessIDSource.compare(strProcessIDTarget)){
//		    cout << "CREATE IPC" << endl;
		    ClsInfoDiagramLine clsInfoDiagramLine;
		    clsInfoDiagramLine.setAPSource(clsDiagItemAPSource->getNumber());
		    clsInfoDiagramLine.setAPTarget(clsDiagItemNewConnectionAPTaget->getNumber());
		    addConnection( strConnectionID, clsInfoDiagramLine);
		} else {
//		    cout << "CREATE LOCAL CONNECTION" << endl;
		    vector<vector<int> > vectorNewConnectionPoints;
		    
		    /* first and last point are coming from clicking the APs...*/ 
		    queueConnectionPoints.pop_front();
		    queueConnectionPoints.pop_back();
		    
		    while( !queueConnectionPoints.empty() ) {
			pair<string, QPointF> pairTemp = queueConnectionPoints.front();
			if(!strProcessIDSource.compare(pairTemp.first)){
			    vector<int> vectorPoint;
			    vectorPoint.push_back(pairTemp.second.toPoint().x()); 
			    vectorPoint.push_back(pairTemp.second.toPoint().y());
			    vectorNewConnectionPoints.push_back(vectorPoint);
			}
			queueConnectionPoints.pop_front();
		    }   
		    ClsInfoDiagramLine clsInfoDiagramLine;
		    clsInfoDiagramLine.setPoints(vectorNewConnectionPoints);
		    clsInfoDiagramLine.setAPSource(clsDiagItemAPSource->getNumber());
		    clsInfoDiagramLine.setAPTarget(clsDiagItemNewConnectionAPTaget->getNumber());
		    
		    addConnection(strConnectionID, clsInfoDiagramLine);
		}
		clsDiagItemAPSource = NULL;
		
		while( !queueConnMarkers.empty() ) {
		    delete queueConnMarkers.front();
		    queueConnMarkers.pop_front();
		}   
		
		resetAddState();
	    }
	    
	    
	}
    }
    
/* get the vector of handles from both canvases... */
    
    
}					       


void ClsBlockDiagram::mouseLeftClickPane(string strSceneID, QPointF p){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::mouseLeftClickPane(string strSceneID, QPointF p): " << p.x() << ", " << p.y() << endl;
    cout << "iAddState: " << iAddState << endl;
#endif

    if( iAddState == ClsBlockDiagram::ADD_NODE ){
	ClsScene *scene = getCanvas(strSceneID.c_str());
	if(scene != NULL){
	    int iCanvasType = scene->getCanvasType();
	    if(iCanvasType == ClsScene::CANVAS_SYSTEM){
		createProcess("", p.x(), p.y());
	    }
	    else {
		createGroup(strSceneID, p.x(), p.y());
	    }
	}
	resetAddState();
    } else if(iAddState == ClsBlockDiagram::ADD_CONNECTION_EXC || 
	      iAddState == ClsBlockDiagram::ADD_CONNECTION_INH || 
	      iAddState == ClsBlockDiagram::ADD_CONNECTION_MOD){

/* collect points */
	pair<string, QPointF> pairTemp(strSceneID, p);
	queueConnectionPoints.push_back(pairTemp);

#define MARKER_DIAM 16
	QGraphicsEllipseItem* marker = new QGraphicsEllipseItem(p.x()-MARKER_DIAM/2, p.y()-MARKER_DIAM/2, MARKER_DIAM, MARKER_DIAM);

	if(iAddState == ClsBlockDiagram::ADD_CONNECTION_EXC){
	    marker->setBrush(QBrush ( QColor(255, 0, 0), Qt::SolidPattern ));
	} else if(iAddState == ClsBlockDiagram::ADD_CONNECTION_INH){
	    marker->setBrush(QBrush ( QColor(0, 0, 255), Qt::SolidPattern ));
	} else if(iAddState == ClsBlockDiagram::ADD_CONNECTION_MOD){
	    marker->setBrush(QBrush ( QColor(0, 255, 0), Qt::SolidPattern ));
	}

	marker->setPen(QPen(Qt::NoPen));
	marker->setOpacity ( .4 );
	marker->setZValue(20);
	ClsScene *scene = getCanvas(strSceneID.c_str());
	scene->addItem(marker);
	queueConnMarkers.push_back(marker);


    } else {
	resetAddState();


    }
}

void ClsBlockDiagram::mouseRightClickPane(string strSceneID, QPointF p){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::mouseRightClickPane(string strSceneID, QPointF p)" << endl;
#endif
    if(iAddState>0){  
	clsDiagItemAPSource = NULL;
	resetAddState();
	queueConnectionPoints.clear();
	while( !queueConnMarkers.empty() ) {
	    delete queueConnMarkers.front();
	    queueConnMarkers.pop_front();
	}  
    }
}




void ClsBlockDiagram::resetAddState(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::resetAddState()" << endl;
#endif
    iAddState = -99;
    unsetCursor();
    clsQDiagramView00->setAddState(iAddState);
    clsQDiagramView01->setAddState(iAddState);
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


