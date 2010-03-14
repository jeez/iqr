#include <stdlib.h>
#include <iostream>
#include <map>

#include <qapplication.h>
#include <qfiledialog.h>
#include <qimage.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpicture.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qprinter.h>
#include <qvaluelist.h>
#include <qregion.h>
#include <qinputdialog.h>
#include <qtooltip.h>


#include <ClsFESystemManager.h>
#include "ClsFEDiagramManager.h"
#include <ClsSysFileParserException.h>

#include "ClsBlockDiagram.h"
#include "ClsCanvasNodeProcess.h"
#include "ClsCanvasNodeGroup.h"
#include "ClsCanvasNodePhantom.h"
#include "ClsCanvasNodeAP.h"
#include "ClsCanvasConnection.h"
#include "ClsCanvasMetaConnection.h"
#include "ClsCanvasHandle.h"

#include "ClsFEProcess.h"
#include "ClsFEGroup.h"
#include "ClsFEConnection.h"
#include "tagLibrary.hpp"
#include "parameter.hpp"


#include <fileopen_16x14.xpm>
#include <zoomInIcon_16x13.xpm>
#include <zoomOutIcon_16x13.xpm>
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

#define CANVAS_DEFAULT_X 1600
#define CANVAS_DEFAULT_Y 1200

#define PHANTOMGROUP_WIDTH 72
pair<QPoint, int> findNearestPoint(QPoint qp0, QRect qrect){

//      --- D ---
//      |       |
//      A       C
//      |       |
//      --- B ---

    QPoint qpA(qrect.left(), qrect.center().y());
    QPoint qpB(qrect.center().x(), qrect.bottom());
    QPoint qpC(qrect.right(), qrect.center().y());
    QPoint qpD(qrect.center().x(), qrect.top());

    double distA = sqrt((double)(pow((double)(qpA.x() - qp0.x()), 2) + pow((double)(qpA.y() - qp0.y()), 2)));
    double distB = sqrt((double)(pow((double)(qpB.x() - qp0.x()), 2) + pow((double)(qpB.y() - qp0.y()), 2)));
    double distC = sqrt((double)(pow((double)(qpC.x() - qp0.x()), 2) + pow((double)(qpC.y() - qp0.y()), 2)));
    double distD = sqrt((double)(pow((double)(qpD.x() - qp0.x()), 2) + pow((double)(qpD.y() - qp0.y()), 2)));


    double dMin = std::min(min(distA, distB), min(distC, distD));
    QPoint qpMin = (dMin == distA ? qpA : (dMin == distB ? qpB : (dMin == distC ? qpC : qpD) ));

    int iOrientation = (dMin == distA ? ClsCanvasConnection::WEST_EAST :
			(dMin == distB ? ClsCanvasConnection::SOUTH_NORTH :
			 (dMin == distC ? ClsCanvasConnection::EAST_WEST :
			  ClsCanvasConnection::NORTH_SOUTH) ));


    pair<QPoint, int> pairTemp(qpMin, iOrientation);
    return pairTemp;
}


ClsBlockDiagram::ClsBlockDiagram( QWidget* _parent) : QFrame(_parent, 0 ,0) {
    clsCanvasNodeNewConnectionAPSource = NULL;

    qframeToolbar = new QFrame(this);
    QHBoxLayout *qboxlayoutTB= new QHBoxLayout( qframeToolbar );

//    qmenuBar = new QMenuBar(this);

    qsplitter = new QSplitter(this); /* splitter for multiple diagram views */
    clsQDiagramView = new ClsQDiagramView( qsplitter, this, "", 0);
    clsQDiagramView0 = new ClsQDiagramView( qsplitter, this, "", 0);
    clsQDiagramViewActive = clsQDiagramView;

    connect(clsQDiagramView,SIGNAL(sigGotFocus(int,string)), this, SLOT(slotDiagramViewGotFocus_1(int,string)));
    connect(clsQDiagramView0,SIGNAL(sigGotFocus(int,string)), this, SLOT(slotDiagramViewGotFocus_2(int,string)));

    connect(clsQDiagramView,SIGNAL(canvasChanged(int,string)), this, SLOT(slotDiagramViewGotFocus_1(int,string)));
    connect(clsQDiagramView0,SIGNAL(canvasChanged(int,string)), this, SLOT(slotDiagramViewGotFocus_2(int,string)));

    connect(this,SIGNAL(sigItemActivated(int,string)), this, SLOT(slotItemActivated(int,string)));

    qpbtnZoomIn = new QPushButton ( QIconSet(QPixmap(zoomInIcon)), "", qframeToolbar, "ZoomIn" );
    connect(qpbtnZoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));

    qpbtnZoomOut = new QPushButton ( QIconSet(QPixmap(zoomOutIcon)), "", qframeToolbar, "ZoomOut" );
    connect(qpbtnZoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));

    qpbtnNewProcess = new QPushButton( QIconSet(QPixmap(newProcessIcon)), "", qframeToolbar, "NewProcess" );
    connect(qpbtnNewProcess, SIGNAL(clicked()),this, SLOT(slotAddNode()));

    qpbtnNewGroup = new QPushButton( QIconSet(QPixmap(newGroupIcon)), "", qframeToolbar, "NewGroup" );
    connect(qpbtnNewGroup, SIGNAL(clicked()),this, SLOT(slotAddNode()));

    qpbtnNewConnectionExcitatory = new QPushButton( QIconSet(QPixmap(connectionExcitatory)), "", qframeToolbar, "NewConnExc" );
    connect(qpbtnNewConnectionExcitatory, SIGNAL(clicked()),this, SLOT(slotAddConnectionExcitatory()));

    qpbtnNewConnectionModulatory = new QPushButton( QIconSet(QPixmap(connectionModulatory)), "", qframeToolbar, "NewConnMod" );
    connect(qpbtnNewConnectionModulatory, SIGNAL(clicked()),this, SLOT(slotAddConnectionModulatory()));

    qpbtnNewConnectionInhibitory = new QPushButton( QIconSet(QPixmap(connectionInhibitory)), "", qframeToolbar, "NewConnInh" );
    connect(qpbtnNewConnectionInhibitory, SIGNAL(clicked()),this, SLOT(slotAddConnectionInhibitory()));





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


    qboxlayoutTB->addWidget(qpbtnZoomIn);
    qboxlayoutTB->addWidget(qpbtnZoomOut);
    qboxlayoutTB->addWidget(qpbtnNewProcess);
    qboxlayoutTB->addWidget(qpbtnNewGroup);
    qboxlayoutTB->addWidget(qpbtnNewConnectionExcitatory);
    qboxlayoutTB->addWidget(qpbtnNewConnectionModulatory);
    qboxlayoutTB->addWidget(qpbtnNewConnectionInhibitory);


//    qlbl = new QLabel(qframeToolbar);
//    qlbl->show();
//    qboxlayoutTB->addWidget(qlbl, 1);
    qboxlayoutTB->insertStretch (7, 1 ) ;


    qpbtnItemInfo = new QPushButton( QIconSet(QPixmap(infoIcon)), "", qframeToolbar, "ItemInfo" );
    connect(qpbtnItemInfo, SIGNAL(clicked()),this, SLOT(slotItemInfoMode()));
    qpbtnItemInfo->setToggleButton(true);

    qpbtnSplitVertical = new QPushButton( QIconSet(QPixmap(iconSplitVertical)), "", qframeToolbar, "SplitVer" );
    connect(qpbtnSplitVertical, SIGNAL(clicked()),this, SLOT(slotSplitVertical()));

    qpbtnSplitHorizontal = new QPushButton( QIconSet(QPixmap(iconSplitHorizontal)), "", qframeToolbar, "SplitHor" );
    connect(qpbtnSplitHorizontal, SIGNAL(clicked()),this, SLOT(slotSplitHorizontal()));

    qpbtnNoSplit = new QPushButton( QIconSet(QPixmap(iconNoSplit)), "", qframeToolbar, "NoSplit" );
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
//    qboxlayoutMain->addWidget ( qmenuBar, 0,0 );
    qboxlayoutMain->addWidget ( qframeToolbar, 0,0 );
    qboxlayoutMain->addWidget ( qsplitter, 10,0 );
/* ---------------- */

    printer = 0;

    clsQDiagramView->setFocus(true);
    clsQDiagramView0->setFocus(false);

    clsQDiagramView->show();
    clsQDiagramView0->hide();

    bSimulationRunning = false;
    slotItemActivated(ClsFESystemManager::ITEM_SYSTEM, "");
}




ClsQCanvas* ClsBlockDiagram::getCanvas(QString qstr) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "*ClsBlockDiagram::getCanvas(QString qstr)" << endl;
#endif

//    cout << "ClsBlockDiagram::getCanvas(QString qstr)" << endl;
//    cout << "qstr: " << qstr << endl;
//    cout << "qmapCanvases.size(): " << qmapCanvases.size() << endl;
    if(qmapCanvases.find(qstr)!=qmapCanvases.end()){
	ClsQCanvas *qcanvasTemp = (qmapCanvases.find(qstr)).data();
	return qcanvasTemp;
    } else {
	cout << "CANVAS NOT FOUND" << endl;
	return 0;
    }
    return 0;
}


void ClsBlockDiagram::slotDiagramViewLeftClicked(int i,string s,  QPoint p) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewLeftClicked(int i,string s,  QPoint p)" << endl;
#endif
    emit blockdiagramLeftClick(i, s, p);
}


void ClsBlockDiagram::slotDiagramViewRightClicked(int i,string s,  QPoint p) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewRightClicked(int i,string s,  QPoint p)" << endl;
#endif
    emit blockdiagramRightClick(i, s, p);
}

void ClsBlockDiagram::slotDiagramViewRightClicked(int i,list<string> l,  QPoint p) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewRightClicked(int i,list<string> l,  QPoint p)" << endl;
#endif
    emit blockdiagramRightClick(i, l, p);
}

void ClsBlockDiagram::slotDiagramViewDoubleClicked(int i,string s,  QPoint p) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewDoubleClicked(int i,string s,  QPoint p)" << endl;
#endif
    emit blockdiagramDoubleClick(i, s, p);
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

    clsQDiagramView->addTab(ClsFESystemManager::ITEM_SYSTEM, strSystemName, strSystemID );
    clsQDiagramView0->addTab(ClsFESystemManager::ITEM_SYSTEM, strSystemName, strSystemID );

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
//	cout << __FILE__ <<  ":strConnectionID:" << strConnectionID << endl;
	string strConnectionName = mapIteratorConnections->second->getConnectionName();
//	cout << __FILE__ << ":strConnectionName:" << strConnectionName << endl;
	string strConnectionSourceID = mapIteratorConnections->second->getConnectionSourceID( );
//	cout << __FILE__ << ":strConnectionSourceID:" << strConnectionSourceID << endl;
	string strConnectionTargetID = mapIteratorConnections->second->getConnectionTargetID( );
//	cout << __FILE__ << ":strConnectionTargetID:" << strConnectionTargetID << endl;

	ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strConnectionID);
	int iConnectionType= mapIteratorConnections->second->getConnectionTypeAsInt();
	addConnection(strConnectionID, strConnectionName, strConnectionSourceID, strConnectionTargetID, iConnectionType, clsInfoDiagramLine);
    }

   
    ClsQCanvas *qcanvasTemp = (qmapCanvases.find(strSystemID.c_str())).data();
    if(qcanvasTemp!=NULL){
	clsQDiagramView->setCanvas(qcanvasTemp);
	clsQDiagramView0->setCanvas(qcanvasTemp);
    }

};


void ClsBlockDiagram::slotItemAdded(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "**************ClsBlockDiagram::slotItemAdded(int iType, string strID )" << endl;
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
/*
    else if(iType==ClsFESystemManager::ITEM_CONNECTION){

	ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strID);
	string strConnectionID = ClsFESystemManager::Instance()->getConnectionID(strID);
	string strConnectionName = ClsFESystemManager::Instance()->getConnectionName(strID);
	string strConnectionSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strID );
	string strConnectionTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strID );
	int iConnectionType= ClsFESystemManager::Instance()->getConnectionTypeAsInt(strID);
	addConnection(strConnectionID, strConnectionName, strConnectionSourceID, strConnectionTargetID, iConnectionType, clsInfoDiagramLine);

    }
*/
}


void ClsBlockDiagram::slotItemDuplicated(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemDuplicated(int iType, string strID )" << endl;
#endif

    slotItemAdded( iType,  strID );

/*
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
    else 
*/
if(iType==ClsFESystemManager::ITEM_CONNECTION){
	ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strID);
	string strConnectionID = ClsFESystemManager::Instance()->getConnectionID(strID);
	string strConnectionName = ClsFESystemManager::Instance()->getConnectionName(strID);
	string strConnectionSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strID );
	string strConnectionTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strID );
	int iConnectionType= ClsFESystemManager::Instance()->getConnectionTypeAsInt(strID);
	addConnection(strConnectionID, strConnectionName, strConnectionSourceID, strConnectionTargetID, iConnectionType, clsInfoDiagramLine);
    }

}


void ClsBlockDiagram::slotItemChanged(int iType, string strID ) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemChanged(int iType, string strID )" << endl;
#endif
//    cout << "STRID: " << strID << endl;

    string strName = "";
    if(iType==ClsFESystemManager::ITEM_SYSTEM){
	strName = ClsFESystemManager::Instance()->getSystemName( );
	clsQDiagramView->renameTab(strID, strName );
	clsQDiagramView0->renameTab(strID, strName );

//	if(qmapNodes.find(strID) != qmapNodes.end()){
//	    qmapNodes.find(strID).data()->setName(strName);
//	}
    }
    else if(iType==ClsFESystemManager::ITEM_PROCESS){
	strName = ClsFESystemManager::Instance()->getProcessName( strID );
	if(qmapNodes.find(strID) != qmapNodes.end()){
	    qmapNodes.find(strID).data()->setName(strName);

	    resetGroupModuleIOIcons(strID);

	    if(ClsFESystemManager::Instance()->getFEProcess(strID)->hasModule()){
		moduleIcon mi = ClsFESystemManager::Instance()->getFEProcess(strID)->getModuleIcon();
		bool bModuleEnabled = ClsFESystemManager::Instance()->getFEProcess(strID)->getEnableModules();
//		cout << ClsFESystemManager::Instance()->getFEProcess(strID)->getProcessName() << ": " << (bModuleEnabled? "yes" : "no") << endl;
		dynamic_cast<ClsCanvasNodeProcess*>(qmapNodes.find(strID).data())->setModuleIcon(mi);
		dynamic_cast<ClsCanvasNodeProcess*>(qmapNodes.find(strID).data())->setModuleEnabled(bModuleEnabled);
		setGroupModuleIOIcons(strID);
	    } else {
		dynamic_cast<ClsCanvasNodeProcess*>(qmapNodes.find(strID).data())->removeModuleIcon();
	    }


	}
	clsQDiagramView->renameTab(strID, strName );
	clsQDiagramView0->renameTab(strID, strName );

	/*
	  a) determin which groups are affected for I/O
	  b) set module icon
	*/



    }
    else if (iType==ClsFESystemManager::ITEM_GROUP){
	strName = ClsFESystemManager::Instance()->getGroupName( strID );
	if(qmapNodes.find(strID) != qmapNodes.end()){
	    qmapNodes.find(strID).data()->setName(strName);
	}

	QMap<string, ClsCanvasNodePhantom*>::Iterator it;
	for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
//	    cout << "it.data()->getID(): " << it.data()->getID() << endl;
	    if(!strID.compare(it.data()->getID())){
		it.data()->setName(strName);
	    }
	}
    }
    else if (iType==ClsFESystemManager::ITEM_CONNECTION){
	int iConnectionType= ClsFESystemManager::Instance()->getFEConnection( strID )->getConnectionTypeAsInt();
	if(qmapConnections.find(strID) != qmapConnections.end()){
	    qmapConnections.find(strID).data()->setConnectionType(iConnectionType);
	}
    }
/*
  else if(iType==ClsFESystemManager::ITEM_CONNECTION){
  strName = ClsFESystemManager::Instance()->getConnectionName( strID );
  if(qmapConnections.find(strID) != qmapConnections.end()){
  qmapConnections.find(strID).data()->setName(strName);
  }
  if(q mapMetaConnections.find(strID) != qmapMetaConnections.end()){
  qmapMetaConnections.find(strID).data()->setName(strName);
  }

  }
*/
}


void ClsBlockDiagram::slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType, int iCanvasConnectionType){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType)" << endl;
    cout << "strConnectionID: " << strConnectionID << endl;
#endif

    string strOldGroupProcessID = "";
    string strOldGroupID = "";
    string strSourceID = "";
    string strTargetID = "";
    bool bWasIP;

  
    QMap<string, ClsCanvasMetaConnection*>::iterator itmapMetaConnections = qmapMetaConnections.find(strConnectionID);
    if(itmapMetaConnections != qmapMetaConnections.end()){
	bWasIP = true;
    } else {
	bWasIP = false;
    }

//    cout << "@@@bWasIP: " << bWasIP << endl;


    if(qmapConnections.find(strConnectionID) != qmapConnections.end()){
	if(iEndType == ClsCanvasConnection::CONNECTION_SOURCE){
//	    cout << "CONNECTION SOURCE" << endl;
//++	    strOldGroupID = qmapConnections.find(strConnectionID).data()->getSourceID();
	    strOldGroupID = qmapConnections.find(strConnectionID).data()->getOldSourceID();
	    qmapConnections.find(strConnectionID).data()->setSourceID(strGroupID);
 	    strSourceID = strGroupID;
 	    strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
	} else {
//	    cout << "CONNECTION TARGET" << endl;
//++	    strOldGroupID = qmapConnections.find(strConnectionID).data()->getTargetID();
	    strOldGroupID = qmapConnections.find(strConnectionID).data()->getOldTargetID();
	    qmapConnections.find(strConnectionID).data()->setTargetID(strGroupID);
 	    strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
 	    strTargetID = strGroupID;
	}
    }

    if(strOldGroupID.size()>0 && strGroupID.compare(strOldGroupID) /* not the same as before */){
	/* removing the old handles from APs */
	if(qmapNodes.find(strOldGroupID) != qmapNodes.end()){
	    qmapNodes.find(strOldGroupID).data()->removeAPHandle(strConnectionID.c_str());
	}
	if(bWasIP){
	    strOldGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strOldGroupID );

	}
	/* --------------------------------- */
    } 

 
    if(strGroupID.size()>0 && strGroupID.compare(strOldGroupID)){
//   	cout << "strSourceID: " << strSourceID << endl;
//   	cout << "strTargetID: " << strTargetID << endl;
 
   	string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
   	string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
 
//   	cout << "strSourceProcessID: " << strSourceProcessID << endl;
//   	cout << "strTargetProcessID: " << strTargetProcessID << endl;
 
	if(strSourceProcessID.size()>0 && strTargetProcessID.size()>0){
	    bool bIPNow = false;
	    if(strSourceProcessID.compare(strTargetProcessID)) {
		bIPNow = true;
	    } else {
		bIPNow = false;
	    }
	    
	    if(bWasIP && bIPNow){ /* seems to work... */
//		cout << "/// bWasIP && bIPNow ///" << endl;
		if(strSourceID.size()>0 && strTargetID.size()>0){
		    rerouteMetaConnection(strConnectionID, iEndType, iCanvasConnectionType, strOldGroupProcessID);
		    cleanupPhantomGroupAPs();
 		    reconnectIPConnection(strConnectionID, iEndType);
		    /* very dangerous call!: prunePhantomGroups(); */
		}
	    } else if(!bWasIP && bIPNow){ /* cannnot happen at the moment... */
//		cout << "*** !bWasIP && bIPNow **" << endl;
		local2IPConnection(strConnectionID, iEndType); 
	    } else if(bWasIP && !bIPNow){ /* check for unconnected.... */
//		cout << "+++ bWasIP && !bIPNow +++" << endl;
		IP2localConnection(strConnectionID, iEndType, strOldGroupProcessID);
		cleanupPhantomGroupAPs();
		/* very dangerous call!: prunePhantomGroups(); */

	    }
	}
    }
 
    /* for debugging 
    QMap<string, ClsCanvasNode*>::Iterator it2;
    for ( it2 = qmapNodes.begin(); it2 != qmapNodes.end(); ++it2 ) {
 	if(it2.data()->countActiveAPs() >0){
 	    cerr << "active group: " << it2.data()->getName() << "(" << it2.data()->getID() << ") count avtive APS: " << it2.data()->countActiveAPs() << endl;
 	}
    }
 
    QMap<string, ClsCanvasNodePhantom*>::Iterator it3;
    for ( it3 = qmapPhantomNodes.begin(); it3 != qmapPhantomNodes.end(); ++it3 ) {
 	if(it3.data()->countActiveAPs() >0){
 	    cerr << "active phantom group: " << it3.data()->getName() << "(" << it3.data()->getID() << ") count avtive APS: " << it3.data()->countActiveAPs() << endl;
 	}
    }
    */


}


void ClsBlockDiagram::deleteProcess(string strProcessID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::deleteProcess(string strProcessID):" << strProcessID << endl;
#endif

    if(qmapNodes.find(strProcessID) != qmapNodes.end()){
	ClsCanvasNode *clsCanvasNodeProcess = qmapNodes.find(strProcessID).data();

	QCanvas* qCanvasProcess = getCanvas( strProcessID.c_str());

	ClsStorage<ClsCanvasNode*, ClsCanvasNode*> clsStorage;
	clsStorage.addParent(clsCanvasNodeProcess);

	QMap<string, ClsCanvasNode*>::Iterator it;
	for ( it = qmapNodes.begin(); it != qmapNodes.end(); ++it ) {
	    ClsCanvasNode *clsCanvasNode = it.data();

	    /* we have to get the parent of the node via the canvas,
	       because in the systemmanager the item is already deleted... */
	    QCanvas* qCanvasNode = clsCanvasNode->canvas();
	    if(qCanvasNode == qCanvasProcess){
		clsStorage.addChild(clsCanvasNode);
		string strNodeID = clsCanvasNode->getID();
		clsCanvasNode->Hide();


		list<string> lstConnections;
		QMap<string, ClsCanvasConnection*>::Iterator itC;
		/* compile a list of connection that source from or target one of the groups in the process
		   we have to go via creating the list, because we cannot delete the connections from the map immediately
		   as this screws up the deleteConnection function...
		*/
		for ( itC = qmapConnections.begin(); itC != qmapConnections.end(); ++itC ) {
		    string strConnectionID =  itC.data()->getID();
		    string strSourceID = itC.data()->getSourceID();
		    string strTargetID = itC.data()->getTargetID();
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

	clsCanvasNodeProcess->Hide();
	queueDeletedProcesses.push(clsStorage);

	/* remove the Canvas and the Tab... */
	clsQDiagramView->removeTab(strProcessID );
	clsQDiagramView0->removeTab(strProcessID );

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
    QMap<string, ClsCanvasNodePhantom*>::Iterator it;
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
    cout << "ClsBlockDiagram::deleteGroup(string strID)" << endl;
#endif

    if(qmapNodes.find(strID) != qmapNodes.end()){
	ClsCanvasNode *clsCanvasNode = qmapNodes.find(strID).data();
	queueDeletedNodes.push(clsCanvasNode);
	qmapNodes.erase(qmapNodes.find(strID));
	clsCanvasNode->Hide();
    }

    deletePhantomGroups(strID);

    QMap<string, ClsCanvasConnection*>::Iterator itC;
    for ( itC = qmapConnections.begin(); itC != qmapConnections.end(); ++itC ) {
//	cout << "itC.data()->getSourceID(): "  << itC.data()->getSourceID() << endl;
//	cout << "itC.data()->getTargetID(): " <<  itC.data()->getTargetID() << endl;;

	if(!strID.compare(itC.data()->getSourceID())){
	    itC.data()->setSourceID("");
	}

	if(!strID.compare(itC.data()->getTargetID())){
	    itC.data()->setTargetID("");
	}
    }
}

void ClsBlockDiagram::deleteConnection(string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "\n\tClsBlockDiagram::deleteConnection(string strID)::strID:" << strID << endl;
#endif

/* design comment:
   the removal of all the concerned AP references is pretty lenghty. This might be a call
   for a signals/slots implementation. At the moment, though, the ClsCanvasNode class is
   not derived from QObject...
*/

    string strConnectionSourceID;
    string strConnectionTargetID;

    string strSourceGroupProcessID;
    string strTargetGroupProcessID;


    if(qmapConnections.find(strID) != qmapConnections.end()){

	strConnectionSourceID = qmapConnections.find(strID).data()->getSourceID( );
	strConnectionTargetID = qmapConnections.find(strID).data()->getTargetID( );

	strSourceGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strConnectionSourceID );
	strTargetGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strConnectionTargetID );


/* find the source, and the target group, and make sure, that APs of these Groups don't
   point to the deleted Connection (HandleStart, HandleEnd) anymore... */
//	cout << "(qmapNodes.find(strConnectionSourceID)" << endl;
	if(qmapNodes.find(strConnectionSourceID) != qmapNodes.end()){ /* process and system level */
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strConnectionSourceID).data();
	    clsCanvasNode->removeAPHandle(strID.c_str());
	}


//	cout << "(qmapNodes.find(strConnectionTargetID)" << endl;
	if(qmapNodes.find(strConnectionTargetID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strConnectionTargetID).data();
	    clsCanvasNode->removeAPHandle(strID.c_str());
	}
/* ------------- */

/* find the system level process that has the connection attached */

//	cout << "strSourceGroupProcessID: " << strSourceGroupProcessID << endl;
//	cout << "strTargetGroupProcessID: " << strTargetGroupProcessID << endl;

//	cout << "strConnectionSourceID: " << strConnectionSourceID << endl;
//	cout << "strConnectionTargetID: " << strConnectionTargetID << endl;


//	cout << "qmapNodes.find(strSourceGroupProcessID)" << endl;
	if(qmapNodes.find(strSourceGroupProcessID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strSourceGroupProcessID).data();
	    clsCanvasNode->removeAPHandle(strID.c_str());
	}

//	cout << "qmapNodes.find(strTargetGroupProcessID)" << endl;
	if(qmapNodes.find(strTargetGroupProcessID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strTargetGroupProcessID).data();
	    clsCanvasNode->removeAPHandle(strID.c_str());
	}
/* ------------- */


	delete qmapConnections.find(strID).data();
	qmapConnections.remove(strID);
    }

    if(qmapMetaConnections.find(strID) != qmapMetaConnections.end()){
	/* also do phantom groups... */

//	string strSourceKey = strConnectionSourceID + strSourceGroupProcessID; 
//	string strTargetKey = strConnectionTargetID + strTargetGroupProcessID;

//	cout << "strSourceKey: " << strSourceKey << endl;
//	cout << "strTargetKey: " << strTargetKey << endl;


	QMap<string, ClsCanvasNodePhantom*>::Iterator it;
	for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
//	    cout << "it.data()->getID(): " << it.data()->getID() << endl;
//	    cout << "it.key():           " << it.key() << endl;
	    if(!strConnectionSourceID.compare(it.data()->getID())){
//	    if(!strSourceKey.compare(it.key())){
		it.data()->removeAPHandle(strID.c_str());
//		cout << "REMOVED SOURCE AP" << endl;
	    }
	    if(!strConnectionTargetID.compare(it.data()->getID())){
//	    if(!strTargetKey.compare(it.key())){
		it.data()->removeAPHandle(strID.c_str());
//		cout << "REMOVED TARGET AP" << endl;
	    }
	}
        /* ------------- */

	delete qmapMetaConnections.find(strID).data();
	qmapMetaConnections.remove(strID);
    }



/*
  QMap<string, ClsCanvasNode*>::Iterator it2;
  for ( it2 = qmapNodes.begin(); it2 != qmapNodes.end(); ++it2 ) {
  if(it2.data()->countActiveAPs() >0){
  cerr << "active  group: " << it2.data()->getName() << " count avtive APS: " << it2.data()->countActiveAPs() << endl;
  }
  }
*/

}



void ClsBlockDiagram::deleteMetaConnection(string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "\n\tClsBlockDiagram::deleteMetaConnection(string strID)::strID:" << strID << endl;
#endif

    string strConnectionSourceID = qmapConnections.find(strID).data()->getSourceID( );
    string strConnectionTargetID = qmapConnections.find(strID).data()->getTargetID( );


    if(qmapMetaConnections.find(strID) != qmapMetaConnections.end()){
        /* also do phantom groups... */
	QMap<string, ClsCanvasNodePhantom*>::Iterator it;
	for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	    if(!strConnectionSourceID.compare(it.data()->getID())){
		it.data()->removeAPHandle(strID.c_str());
	    }
	    if(!strConnectionTargetID.compare(it.data()->getID())){
		it.data()->removeAPHandle(strID.c_str());
	    }
	}
        /* ------------- */

	delete qmapMetaConnections.find(strID).data();
	qmapMetaConnections.remove(strID);
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
    if(clsCanvasNodeNewConnectionAPSource!=NULL){
	return dynamic_cast<ClsQCanvas*>(clsCanvasNodeNewConnectionAPSource->canvas())->getID();
    }
    else {
	return "";
    }
};

void ClsBlockDiagram::slotNewConnectionStarted(ClsCanvasNodeAP* _clsCanvasNodeAPSource){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotNewConnectionStarted(ClsCanvasNodeAP* clsCanvasNodeAPSource)" << endl;
#endif
//    cout << "ClsBlockDiagram::clsCanvasNodeAPSource->getParentID(): " << _clsCanvasNodeAPSource->getParentID() << endl;
/* determin the Process ID */
    clsCanvasNodeNewConnectionAPSource = _clsCanvasNodeAPSource;
}

void ClsBlockDiagram::slotNewConnectionFinished(ClsCanvasNodeAP* clsCanvasNodeAPTarget, int iConnectionType) {
//    cout << "####################" << endl;
//    cout << "create new IPC" << endl;
//    cout << "####################" << endl;

    emit sigAbortNewConnection();

    string strSourceID = clsCanvasNodeNewConnectionAPSource->getParentID();
    string strTargetID = clsCanvasNodeAPTarget->getParentID();

    string strConnectionID = ClsFESystemManager::Instance()->createConnection(strSourceID, strTargetID, iConnectionType);
    ClsInfoDiagramLine clsInfoDiagramLine;


    addConnection( strConnectionID, "", strSourceID,  strTargetID, iConnectionType, clsInfoDiagramLine);

/* should call
   slotAbortNewConnection();
   int both ClsQDiagramCanvasView..
*/
    clsCanvasNodeNewConnectionAPSource = NULL;
/* still need to add the systemlevel connection.. */

}


void ClsBlockDiagram::slotNewConnectionAborted() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotNewConnectionAborted()" << endl;
#endif
    clsCanvasNodeNewConnectionAPSource = 0;;
}

/*
  works only for "local" connections
*/
void ClsBlockDiagram::slotNewConnection(ClsCanvasNodeAP* clsCanvasNodeAPSource, ClsCanvasNodeAP* clsCanvasNodeAPTarget,
					int iConnectionType, vector<vector<int> > vectorPoints) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotNewConnection(ClsCanvasNodeAP* clsCanvasNodeAPSource, ClsCanvasNodeAP* clsCanvasNodeAPTarget, int iConnectionType, vector<vector<int> > vectorPoints)" << endl;
#endif
    clsCanvasNodeNewConnectionAPSource = 0;

    string strSourceID = clsCanvasNodeAPSource->getParentID();
    string strTargetID = clsCanvasNodeAPTarget->getParentID();

    string strConnectionID = ClsFESystemManager::Instance()->createConnection(strSourceID, strTargetID, iConnectionType);
    string strPrcID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);

    ClsQCanvas *qcanvasTemp = (qmapCanvases.find(strPrcID.c_str())).data();

    ClsCanvasConnection *clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, 
								       strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::LOCAL);
    qmapConnections[strConnectionID] = clsCanvasConnection;

    drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);
};

void ClsBlockDiagram::setRoot(string strID, string strName) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setRoot(string strID, string strName)" << endl;
#endif

    strName = "";
//    ClsQCanvas *qcanvasTemp = new ClsQCanvas(800, 600);
    ClsQCanvas *qcanvasTemp = new ClsQCanvas(this, strID, ClsQCanvas::CANVAS_SYSTEM);
    qcanvasTemp->setAdvancePeriod(30);
    qcanvasTemp->resize(CANVAS_DEFAULT_X,CANVAS_DEFAULT_Y);

    qmapCanvases[strID.c_str()] = qcanvasTemp;
};


void ClsBlockDiagram::addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon)" << endl;
#endif

    if(qmapNodes.find(strPrcID) == qmapNodes.end()){
	clsQDiagramView->addTab(ClsFESystemManager::ITEM_PROCESS, strPrcName, strPrcID );
	clsQDiagramView0->addTab(ClsFESystemManager::ITEM_PROCESS, strPrcName, strPrcID );

	ClsQCanvas *qcanvasTemp = new ClsQCanvas(this, strPrcID, ClsQCanvas::CANVAS_PROCESS);
	qcanvasTemp->setAdvancePeriod(30);
	qcanvasTemp->setName(strPrcID.c_str());
	qcanvasTemp->resize(CANVAS_DEFAULT_X,CANVAS_DEFAULT_Y);
	qmapCanvases[strPrcID.c_str()] = qcanvasTemp;

/* draw the prc on the first canvas in the map (i.e. the systemview canvas) */
	string strSystemID = ClsFESystemManager::Instance()->getSystemID();
	ClsQCanvas * qcanvasTemp2 = qmapCanvases[strSystemID.c_str()];

	ClsCanvasNode *i = new ClsCanvasNodeProcess( ClsFESystemManager::ITEM_PROCESS,
						     clsInfoDiagramIcon.getX(), clsInfoDiagramIcon.getY(),
						     clsInfoDiagramIcon.getHeight(), clsInfoDiagramIcon.getWidth(),
						     qcanvasTemp2,
						     strPrcName, strPrcID);

	if(ClsFESystemManager::Instance()->getFEProcess(strPrcID)->hasModule()){
	    moduleIcon mi = ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getModuleIcon();
	    bool bModuleEnabled = ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getEnableModules();
	    dynamic_cast<ClsCanvasNodeProcess*>(i)->setModuleIcon(mi);
	    dynamic_cast<ClsCanvasNodeProcess*>(i)->setModuleEnabled(bModuleEnabled);
	}

	if(ClsFESystemManager::Instance()->getFEProcess(strPrcID)->getPath().length()>0){
	    dynamic_cast<ClsCanvasNodeProcess*>(i)->setIsExternal(true);
	} else {
	    dynamic_cast<ClsCanvasNodeProcess*>(i)->setIsExternal(false);
	}

	i->setZ(0);
	i->show();
	qmapNodes[strPrcID] = i;
    }

};


void  ClsBlockDiagram::addGroup(string strPrcID, string strGroupID, string strGroupName, ClsInfoDiagramIcon clsInfoDiagramIcon) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::addGroup(string strPrcID, string strGroupID, string strGroupName,....)" << endl;
#endif

    if(qmapNodes.find(strGroupID) == qmapNodes.end()){

	ClsQCanvas *qcanvasTemp = (qmapCanvases.find(strPrcID.c_str())).data();
//    cout << "qcanvasTemp->name():" << qcanvasTemp->name() << endl;
//    cout << __FILE__ << ":" << __LINE__ << "qcanvasTemp->getCanvasType(): " << qcanvasTemp->getCanvasType() << endl;

	ClsCanvasNode *i = new ClsCanvasNodeGroup( ClsFESystemManager::ITEM_GROUP,
						   clsInfoDiagramIcon.getX(), clsInfoDiagramIcon.getY(),
						   clsInfoDiagramIcon.getHeight(), clsInfoDiagramIcon.getWidth(),
						   qcanvasTemp,
						   strGroupName, strGroupID);
	i->setZ(0);
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

/*
  ClsQCanvas *qcanvasTemp = (qmapCanvases.find(strPrcID.c_str())).data();
*/
    string strGroupID = ClsFESystemManager::Instance()->createGroup(strPrcID);

    if(qmapNodes.find(strGroupID) != qmapNodes.end()){
	qmapNodes.find(strGroupID).data()->moveBy(iX, iY );

    }

/*
  string strGroupName = ClsFESystemManager::Instance()->getGroupName(strGroupID);


  ClsCanvasNode *i = new ClsCanvasNodeGroup( ClsFESystemManager::ITEM_GROUP,
  iX, iY,
  DEFAULT_NODE_WIDTH, DEFAULT_NODE_HEIGHT,
  qcanvasTemp,
  strGroupName, strGroupID);
  i->setZ(0);
  i->show();
  qmapNodes[strGroupID] = i;
*/

};

void ClsBlockDiagram::addConnection(string strConnectionID, string /* strConnectionName */,
				    string strSourceID, string strTargetID,
				    int iConnectionType, ClsInfoDiagramLine clsInfoDiagramLine){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "++ClsBlockDiagram::addConnection(string strConnectionID, string strConnectionName, " <<
	"string strSourceID, string strTargetID, int iConnectionType, ClsInfoDiagramLine clsInfoDiagramLine)" << endl;
#endif


    if(qmapConnections.find(strConnectionID) == qmapConnections.end()){
	
	
/*
  cout << "ClsBlockDiagram::addConnection(...):strConnectionID: " << strConnectionID << endl;
  cout << "ClsBlockDiagram::addConnection(...):strSourceID: " << strSourceID << endl;
  cout << "ClsBlockDiagram::addConnection(...):strTargetID: " << strTargetID << endl;
*/
	
	ClsQCanvas *qcanvasTemp = NULL;
	ClsCanvasNode* clsCanvasNodeStart = NULL;
	ClsCanvasNode* clsCanvasNodeEnd = NULL;
	ClsCanvasNodeAP* clsCanvasNodeAPSource = NULL;
	ClsCanvasNodeAP* clsCanvasNodeAPTarget = NULL;
	
	bool bInterprocessConnection = false;
	
	string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
	string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
	
	if(strSourceProcessID.compare(strTargetProcessID)) {
	    bInterprocessConnection = true;
	    qcanvasTemp = qmapCanvases[ClsFESystemManager::Instance()->getSystemID().c_str()];
	} else {
	    bInterprocessConnection = false;
	    qcanvasTemp = (qmapCanvases.find(strSourceProcessID.c_str())).data();
	}
	
	ClsCanvasConnection *clsCanvasConnection;
	if(bInterprocessConnection){
	    clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::IP);
	} else {
	    clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::LOCAL);
	}
	
	
	qmapConnections[strConnectionID] = clsCanvasConnection;
	
	if(!bInterprocessConnection){
	    clsCanvasNodeStart = getCanvasNode( strSourceID);
	} else {
	    clsCanvasNodeStart = getCanvasNode( strSourceProcessID);
	}
	
	if(!bInterprocessConnection){
	    clsCanvasNodeEnd = getCanvasNode( strTargetID);
	} else {
	    clsCanvasNodeEnd = getCanvasNode( strTargetProcessID);
	}
	
	vector<vector<int> > vectorPoints = clsInfoDiagramLine.getPoints();
	QPoint qpStart, qpEnd;
	if(vectorPoints.size() > 0){
	    qpEnd.setX(vectorPoints[0][0]);
	    qpEnd.setY(vectorPoints[0][1]);
	    
	    qpStart.setX(vectorPoints.back()[0]);
	    qpStart.setY(vectorPoints.back()[1]);
	}
	else { /* vector size = 0 */
	    qpStart = clsCanvasNodeStart->rect().center();
	    qpEnd = clsCanvasNodeEnd->rect().center();
	}
	
	int iAPSource = clsInfoDiagramLine.getAPSource();
	int iAPTarget = clsInfoDiagramLine.getAPTarget();
	
	if(bInterprocessConnection){ /* we ignore the APs from the systemfile are the system level */
	    clsCanvasNodeAPSource = clsCanvasNodeStart->getNearestAP(qpEnd);
	    clsCanvasNodeAPTarget = clsCanvasNodeEnd->getNearestAP(qpStart);
	    
	} else {
	    if(iAPSource<0){
		/* better the closest, than the next free.. 
		   clsCanvasNodeAPSource = clsCanvasNodeStart->getNextFreeAP(); */
		clsCanvasNodeAPSource = clsCanvasNodeStart->getNearestAP(qpEnd);
	    }else {
		clsCanvasNodeAPSource = clsCanvasNodeStart->getAP(iAPSource);
	    }
	    
	    if(iAPTarget<0){
		/* better the closest, than the next free.. 
		   clsCanvasNodeAPTarget = clsCanvasNodeEnd->getNextFreeAP(); */
		clsCanvasNodeAPTarget = clsCanvasNodeEnd->getNearestAP(qpStart);
	    }else {
		clsCanvasNodeAPTarget = clsCanvasNodeEnd->getAP(iAPTarget);
	    }
	}
	
	
	
	drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);
	clsCanvasConnection->show();
	
	
	
/* phantom group */     
	if(bInterprocessConnection){
//	cout << "PHANTOM CONNECTION" << endl;
	    vectorPoints.clear();
	    createMetaConnection(clsCanvasConnection, strSourceProcessID, strTargetProcessID, strConnectionID, strSourceID,  strTargetID, iConnectionType);
	}
	qcanvasTemp = NULL;
	delete qcanvasTemp;
    }
    
};

/* initialization:
   - get the closests ap from the diagram node
   - set the closest AP as the target for the connection
   - make sure the AP/Group knows about the HandleEnd/Start connected to it
*/
void ClsBlockDiagram::addConnection(ClsInfoConnection clsInfoConnection){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::addConnection(ClsInfoConnection clsInfoConnection)" << endl;
#endif

    ClsQCanvas *qcanvasTemp;
    ClsCanvasNode* clsCanvasNodeStart;
    ClsCanvasNode* clsCanvasNodeEnd;
    bool bInterprocessConnection = false;

    string strConnectionID = clsInfoConnection.getConnectionID();
    string strSourceID = clsInfoConnection.getConnectionSourceID();
    string strTargetID = clsInfoConnection.getConnectionTargetID();
    int iConnectionType = clsInfoConnection.getConnectionType();

    string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
    string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);

    ClsInfoDiagramLine clsInfoDiagramLine = ClsFESystemManager::Instance()->getDiagramLine(strConnectionID);

    if(strSourceProcessID.compare(strTargetProcessID)) {
	bInterprocessConnection = true;
	qcanvasTemp = qmapCanvases[ClsFESystemManager::Instance()->getSystemID().c_str()];
    } else {
	bInterprocessConnection = false;
	qcanvasTemp = (qmapCanvases.find(strSourceProcessID.c_str())).data();
    }

    ClsCanvasConnection *clsCanvasConnection;
    if(bInterprocessConnection){
	clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::IP);
    } else {
	clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::LOCAL);
    }

    qmapConnections[strConnectionID] = clsCanvasConnection;

    if(!bInterprocessConnection){ 
	clsCanvasNodeStart = getCanvasNode( strSourceID);
    } else { 
	clsCanvasNodeStart = getCanvasNode( strSourceProcessID); 
    }

    if(!bInterprocessConnection){ 
	clsCanvasNodeEnd = getCanvasNode( strTargetID); 
    } else { 
	clsCanvasNodeEnd = getCanvasNode( strTargetProcessID); 
    }

    vector<vector<int> > vectorPoints = clsInfoDiagramLine.getPoints();

    int iAPSource = clsInfoDiagramLine.getAPSource();
    int iAPTarget = clsInfoDiagramLine.getAPTarget();

    ClsCanvasNodeAP* clsCanvasNodeAPSource = clsCanvasNodeStart->getAP(iAPSource);
    ClsCanvasNodeAP* clsCanvasNodeAPTarget = clsCanvasNodeEnd->getAP(iAPTarget);

    drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);

    clsCanvasConnection->show();


/* phantom group */
    if(bInterprocessConnection){
//	cout << "PHANTOM CONNECTION" << endl;
	vectorPoints.clear();
	createMetaConnection(clsCanvasConnection, strSourceProcessID, strTargetProcessID, strConnectionID, strSourceID,  strTargetID, iConnectionType);
    }
}


void ClsBlockDiagram::drawConnection(ClsCanvasConnection *clsCanvasConnection,
				     ClsCanvasNodeAP* clsCanvasNodeAPSource, ClsCanvasNodeAP* clsCanvasNodeAPTarget, vector<vector<int> > vectorPoints){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "++ClsBlockDiagram::drawConnection(ClsCanvasConnection *clsCanvasConnection," <<
	"ClsCanvasNodeAP* clsCanvasNodeAPSource, ClsCanvasNodeAP* clsCanvasNodeAPTarget, vector<vector<int> > vectorPoints)" << endl;
#endif

    if( clsCanvasNodeAPSource!=NULL && clsCanvasNodeAPTarget!=NULL){
	QPoint qpSourceAP, qpTargetAP;
	if(vectorPoints.size() > 0){
	    QPoint qpLinePointFirst, qpLinePointLast;

	    qpLinePointFirst.setX(vectorPoints[0][0]);
	    qpLinePointFirst.setY(vectorPoints[0][1]);

	    qpLinePointLast.setX(vectorPoints.back()[0]);
	    qpLinePointLast.setY(vectorPoints.back()[1]);

/* do the first segments */
	    qpSourceAP = clsCanvasNodeAPSource->rect().center();
	    clsCanvasConnection->addSegment(qpSourceAP, qpLinePointFirst, ClsCanvasConnection::SEG_START, 0);//iOrientation);
/* ---------------------- */

/* do the middle segments (this is skipped, if we only have one point in the vector) */
	    QPoint qp0, qp1;
	    qp0 = qp1 = qpLinePointFirst; /* necessary in case we only have 1 point! */

	    for(unsigned int ii=1; ii<vectorPoints.size();ii++){
		qp0.setX(vectorPoints[ii][0]);
		qp0.setY(vectorPoints[ii][1]);
		clsCanvasConnection->addSegment(qp1, qp0, ClsCanvasConnection::SEG_MIDDLE, ClsCanvasConnection::UNKNOWN);
		qp1 = qp0;
	    }
/* ---------------------- */

	    qpLinePointLast = qp0;

/* do the end segments */
	    qpTargetAP = clsCanvasNodeAPTarget->rect().center();
	    int iOrientation = clsCanvasNodeAPTarget->getOrientation();
	    clsCanvasConnection->addSegment(qpLinePointLast, qpTargetAP, ClsCanvasConnection::SEG_END, iOrientation);
	}
	else { /* vector size = 0 */
	    qpSourceAP = clsCanvasNodeAPSource->rect().center();
	    qpTargetAP = clsCanvasNodeAPTarget->rect().center();
	    int iOrientation = clsCanvasNodeAPTarget->getOrientation();
	    clsCanvasConnection->addSegment(qpSourceAP, qpTargetAP, ClsCanvasConnection::SEG_SINGLE, iOrientation);
	} /* end check on vector.size() */


/* tell the AP of the node that it is connected */
	ClsCanvasHandleStart *clsCanvasHandleStart = clsCanvasConnection->getStartHandle();
	ClsCanvasHandleEnd *clsCanvasHandleEnd = clsCanvasConnection->getEndHandle();

	if(clsCanvasHandleStart!=NULL){
	    clsCanvasNodeAPSource->addHandleStart(clsCanvasHandleStart);
	}
	if(clsCanvasHandleEnd!=NULL){
	    clsCanvasNodeAPTarget->addHandleEnd(clsCanvasHandleEnd);
	}
    } else {
//	cout << "clsCanvasNodeAPSource==NULL || clsCanvasNodeAPTarget==NULL" << endl;
    }
};



void ClsBlockDiagram::drawConnection(ClsCanvasConnection *clsCanvasConnection,
				     ClsCanvasNode* clsCanvasNodeStart, ClsCanvasNode* clsCanvasNodeEnd,
				     vector<vector<int> > vectorPoints) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "--ClsBlockDiagram::drawConnection((ClsCanvasConnection *clsCanvasConnection, ClsCanvasNode* clsCanvasNodeStart, ClsCanvasNode* clsCanvasNodeEnd, vector<vector<int> > vectorPoints)" << endl;
#endif

    if(clsCanvasNodeStart!= NULL && clsCanvasNodeEnd!= NULL){
	QPoint qpLinePointFirst, qpLinePointLast;

	ClsCanvasNodeAP* clsCanvasNodeAPSource = NULL;
	ClsCanvasNodeAP* clsCanvasNodeAPTarget = NULL;

	if(vectorPoints.size() > 0){
	    qpLinePointFirst.setX(vectorPoints[0][0]);
	    qpLinePointFirst.setY(vectorPoints[0][1]);

	    qpLinePointLast.setX(vectorPoints.back()[0]);
	    qpLinePointLast.setY(vectorPoints.back()[1]);

	}
	else { /* vector size = 0 */
	    qpLinePointFirst = clsCanvasNodeEnd->rect().center();
	    qpLinePointLast = clsCanvasNodeStart->rect().center();

	}


	clsCanvasNodeAPSource = clsCanvasNodeStart->getNearestAP(qpLinePointFirst);
	clsCanvasNodeAPTarget = clsCanvasNodeEnd->getNearestAP(qpLinePointLast);
	drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);

//	cout << "@@@vectorPoints.size(): " << vectorPoints.size() << endl;
    } 


};

void ClsBlockDiagram::init(){
    clear();

//    static int r=24;
//    srand(++r);
};


ClsBlockDiagram::~ClsBlockDiagram() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::~ClsBlockDiagram()" << endl;
#endif


    delete printer;
    printer = NULL;

    delete qboxlayoutMain;
    qboxlayoutMain = NULL;

//    delete qmenuBar;
//    qmenuBar = NULL;

    delete qframeToolbar;
    qframeToolbar = NULL;

    delete qsplitter;
    qsplitter = NULL;


// CRASH   delete clsQDiagramView; 
//    clsQDiagramView = NULL;

// CRASH   delete clsQDiagramView0;
//    clsQDiagramView0 = NULL;

    QMap<string, ClsCanvasNode*>::Iterator itqmapNodes;
    for (itqmapNodes = qmapNodes.begin(); itqmapNodes != qmapNodes.end(); ++itqmapNodes){
	delete itqmapNodes.data();
    }
    qmapNodes.clear();

    QMap<string, ClsCanvasNodePhantom*>::Iterator itqmapPhantomNodes;
    for (itqmapPhantomNodes = qmapPhantomNodes.begin(); itqmapPhantomNodes != qmapPhantomNodes.end(); ++itqmapPhantomNodes){
	delete itqmapPhantomNodes.data();
    }
    qmapPhantomNodes.clear();

    QMap<string, ClsCanvasConnection*>::Iterator itqmapConnections;
    for (itqmapConnections = qmapConnections.begin(); itqmapConnections != qmapConnections.end(); ++itqmapConnections){
	delete itqmapConnections.data();
    }
    qmapConnections.clear();

    QMap<string, ClsCanvasMetaConnection*>::Iterator itqmapMetaConnections;
    for (itqmapMetaConnections = qmapMetaConnections.begin(); itqmapMetaConnections != qmapMetaConnections.end(); ++itqmapMetaConnections){
	delete itqmapMetaConnections.data();
    }
    qmapMetaConnections.clear();

    while (queueDeletedNodes.size()) {
	delete queueDeletedNodes.front();
	queueDeletedNodes.pop();
    }

    while (queueDeletedProcesses.size()) {
	queueDeletedProcesses.pop();
    }

    QMap<QString, ClsQCanvas*>::Iterator itqmapCanvases;
    for (itqmapCanvases = qmapCanvases.begin(); itqmapCanvases != qmapCanvases.end(); ++itqmapCanvases){
	delete itqmapCanvases.data();
    }
    qmapCanvases.clear();
}


void ClsBlockDiagram::clear(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::clear()" << endl;
#endif
    clsQDiagramView->clear();
    clsQDiagramView0->clear();
}


// not used at the moment, should still be use full to dynamically resize the canvas (i.e. if an object is
// moved beyond it's boundaries
// void ClsBlockDiagram::enlarge(ClsQCanvas* qcanvasTemp, int iX, int iY) {
//     cout << " ClsBlockDiagram::enlarge(ClsQCanvas* qcanvasTemp, int iX, int iY)" << endl;

// //     ClsQCanvas * qcanvasTemp;
// //     QMap<QString, ClsQCanvas*>::Iterator it;
// //     /* WHY DO I RESIZE ALL THE CANVASES HERE???? */
// //     for ( it = qmapCanvases.begin(); it != qmapCanvases.end(); ++it ) {
// //	qcanvasTemp = it.data();
// // //++	qcanvasTemp->resize(qcanvasTemp->width()*4/3, qcanvasTemp->height()*4/3);
// //	qcanvasTemp->resize(CANVAS_DEFAULT_X,CANVAS_DEFAULT_Y);
// //     }

//     cout << "ix, iy: " << iX << ", " << iY << endl;
//     qcanvasTemp->resize(iX, iY);


// }


/* not used at the moment, should still be use full to dynamically resize the canvas (i.e. if an object is
   moved beyond it's boundaries */
void ClsBlockDiagram::shrink(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::shrink()" << endl;
#endif
    ClsQCanvas * qcanvasTemp;
    QMap<QString, ClsQCanvas*>::Iterator it;
/* WHY DO I RESIZE ALL THE CANVASES HERE???? */
    for ( it = qmapCanvases.begin(); it != qmapCanvases.end(); ++it ) {
	qcanvasTemp = it.data();
//++	qcanvasTemp->resize(qcanvasTemp->width()*3/4, qcanvasTemp->height()*3/4);
	qcanvasTemp->resize(CANVAS_DEFAULT_X,CANVAS_DEFAULT_Y);
    }
}

void ClsBlockDiagram::print(){
    if ( !printer ) printer = new QPrinter;
    if ( printer->setup(this) ) {
	if(clsQDiagramViewActive!=NULL){
	    QCanvas * qcanvasTemp = clsQDiagramViewActive->canvas();
//	QCanvas * qcanvasTemp = clsQDiagramView->canvas();
	    QPainter pp(printer);
	    QRect qrect2save=findRect2save(qcanvasTemp);
	    qcanvasTemp->drawArea( qrect2save, &pp );
	}

    }
}

void ClsBlockDiagram::save(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::save()" << endl;
#endif

/* get the filename  */
    QString filename = QFileDialog::getSaveFileName(
	"",
	"Images (*.svg *.png)",
	this,
	"save file dialog"
	"Choose a filename to save under" );

    if(filename.length()<=0){
	return;
    }
    if ( QFile::exists(filename) ) {
/*      warning on overwrite */
	int iReturn;
	iReturn = QMessageBox::warning( this, "IQR",
					"A file with the seleteced name exists\n",
					"Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
//	    cout << "saving aborted" << endl;
	    return;
	}
    }

    if(clsQDiagramViewActive!=NULL){
	QCanvas * qcanvasTemp = clsQDiagramViewActive->canvas();
//    QCanvas * qcanvasTemp = clsQDiagramView->canvas();
	QRect qrect2save=findRect2save(qcanvasTemp);

/* check the format */
	if(filename.contains( ".svg", FALSE )){
//	cout << "saving as svg" << endl;
	    QPicture pict;
	    QPainter paint(&pict);
	    qcanvasTemp->drawArea( qrect2save, &paint );
	    paint.end();
	    pict.save( filename, "svg" );
	} else {
//	cout << "saving as png" << endl;
	    QPixmap *pict = new QPixmap ( qrect2save.size());
	    QPainter paint( pict );
	    qcanvasTemp->drawArea( qrect2save, &paint );
	    paint.end();
	    pict->save( filename, "PNG" );  //PNG, BMP, XBM, XPM and PNM
	    delete pict;
	}
    }
}

QRect ClsBlockDiagram::findRect2save(QCanvas *qcanvasTemp) {
    QRect qrect2save;
    int iXMin = 10000, iXMax = 0;
    int iYMin = 10000, iYMax = 0;
    QCanvasItemList qcanvasList = qcanvasTemp->allItems ();
    QCanvasItemList::Iterator it;
    for ( it = qcanvasList.begin(); it != qcanvasList.end(); ++it ){
/*
  cout << "left: " << (*it)->boundingRect().left() << endl;
  cout << "top: " << (*it)->boundingRect().top() << endl;
  cout << "rtti: " << (*it)->rtti() << endl;
  cout << "-------------" << endl;
*/

/* looks like a bug: topleft corner has to be at 0,0 ...
   if((*it)->boundingRect().left()<iXMin){
   iXMin = (*it)->boundingRect().left();
   }
*/
	iXMin = 0;
	if((*it)->boundingRect().right()>iXMax){
	    iXMax = (*it)->boundingRect().right();
	}

/* looks like a bug: topleft corner has to be at 0,0 ...
   if((*it)->boundingRect().top()<iYMin ){//&& (*it)->boundingRect().top() > 0){
   iYMin = (*it)->boundingRect().top();
   }
*/
	iYMin = 0;
	if((*it)->boundingRect().bottom()>iYMax){
	    iYMax = (*it)->boundingRect().bottom();
	}
    }
    qrect2save.setLeft (iXMin);
    qrect2save.setRight (iXMax );
    qrect2save.setTop (iYMin);
    qrect2save.setBottom (iYMax );

    return qrect2save;
}



void ClsBlockDiagram::zoomIn() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::zoomIn()" << endl;
#endif
    if(clsQDiagramViewActive!=NULL){
	clsQDiagramViewActive->zoomIn();
    }
};

void ClsBlockDiagram::zoomOut() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::zoomOut()" << endl;
#endif
    if(clsQDiagramViewActive!=NULL){
	clsQDiagramViewActive->zoomOut();
    }
};


void ClsBlockDiagram::slotItemInfoMode(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemInfoMode()" << endl;
#endif
    clsQDiagramView->setItemInfoMode(qpbtnItemInfo->isOn());
    clsQDiagramView0->setItemInfoMode(qpbtnItemInfo->isOn());
}


void ClsBlockDiagram::slotSplitHorizontal(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotSplitHorizontal()" << endl;
#endif
    int iHeight = clsQDiagramView0->height();
//    cout << "iHeight: " << iHeight << endl;
    qsplitter->setOrientation(Qt::Vertical );
    clsQDiagramView0->useSpacer(false);
    clsQDiagramView0->show();

    QValueList<int> qvlist;
    qvlist.append(iHeight / 2);
    qvlist.append(iHeight / 2);
    qsplitter->setSizes(qvlist);

};

void ClsBlockDiagram::slotSplitVertical() {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotSplitVertical()" << endl;
#endif

    int iWidth = this->width();
//    cout << "iWidth: " << iWidth << endl;
    qsplitter->setOrientation(Qt::Horizontal );
    clsQDiagramView0->useSpacer(true);
    clsQDiagramView0->show();
    
    QValueList<int> qvlist;
    qvlist.append(iWidth / 2);
    qvlist.append(iWidth / 2);
    qsplitter->setSizes(qvlist);
};

void ClsBlockDiagram::slotNoSplit(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotNoSplit()" << endl;
#endif
    clsQDiagramView0->hide();
};



ClsCanvasNode* ClsBlockDiagram::getCanvasNode( string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getCanvasNode( string strID)" << endl;
#endif
    if(qmapNodes.find(strID) != qmapNodes.end()){
	return qmapNodes[strID];
    }
    return 0;
};

void ClsBlockDiagram::slotAddNode(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddNode()" << endl;
#endif
    clsQDiagramView->setAddState(ClsQDiagramCanvasView::ADD_NODE);
    clsQDiagramView0->setAddState(ClsQDiagramCanvasView::ADD_NODE);
};

void ClsBlockDiagram::slotAddConnectionExcitatory(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionExcitatory()" << endl;
#endif
    clsQDiagramView->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_EXC);
    clsQDiagramView0->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_EXC);
};
void ClsBlockDiagram::slotAddConnectionInhibitory(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionInhibitory()" << endl;
#endif
    clsQDiagramView->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_INH);
    clsQDiagramView0->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_INH);
};
void ClsBlockDiagram::slotAddConnectionModulatory(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotAddConnectionModulatory()" << endl;
#endif
    clsQDiagramView->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_MOD);
    clsQDiagramView0->setAddState(ClsQDiagramCanvasView::ADD_CONNECTION_MOD);
};



/* serialization related */
ClsInfoDiagramIcon ClsBlockDiagram::getDiagramIcon(string strID){
//    QMap<string, ClsCanvasNode*> qmapNodes;
    ClsInfoDiagramIcon clsInfoDiagramIcon;

    if(qmapNodes.find(strID.c_str())!=qmapNodes.end()){
	ClsCanvasNode *n = qmapNodes[strID.c_str()];
	QPoint p = n->getPosition ();
	QSize s = n->getSize ();
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

    if(qmapConnections.find(strID.c_str())!=qmapConnections.end()){
	ClsCanvasConnection *n = qmapConnections[strID.c_str()];
	vector<vector<int> > v = n->getPoints();
	for(unsigned int ii = 0; ii< v.size(); ii++){
//	    cout << "adding point" << endl;
	    int iX = v[ii][0];
	    int iY = v[ii][1];
	    clsInfoDiagramLine.addPoint( iX, iY);
	}

	int iAPSource = -1, iAPTarget = -1;
	string strConnectionSourceID = qmapConnections.find(strID).data()->getSourceID( );
	if(qmapNodes.find(strConnectionSourceID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strConnectionSourceID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strID.c_str());
	    if(ii>=0){
		iAPSource = ii;
	    }
	}

	string strConnectionTargetID = qmapConnections.find(strID).data()->getTargetID( );
	if(qmapNodes.find(strConnectionTargetID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strConnectionTargetID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strID.c_str());
	    if(ii>=0){
		iAPTarget = ii;
	    }
	}

	string strSourceGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strConnectionSourceID );
	string strTargetGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strConnectionTargetID );

	if(qmapNodes.find(strSourceGroupProcessID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strSourceGroupProcessID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strID.c_str());
	    if(ii>=0){
		iAPSource = ii;
	    }
	}

	if(qmapNodes.find(strTargetGroupProcessID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strTargetGroupProcessID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strID.c_str());
	    if(ii>=0){
		iAPTarget = ii;
	    }
	}

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

void ClsBlockDiagram::slotDiagramViewGotFocus_1(int iType, string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewGotFocus_1(int iType, string strID)" << endl;
#endif
    clsQDiagramViewActive = clsQDiagramView;
    clsQDiagramView->setFocus(true);
    clsQDiagramView0->setFocus(false);

    if(iType==ClsQCanvas::CANVAS_SYSTEM){
	iType = ClsFESystemManager::ITEM_SYSTEM;
    }
    else if(iType==ClsQCanvas::CANVAS_PROCESS){
	iType = ClsFESystemManager::ITEM_PROCESS;
    }

    emit sigItemActivated(iType, strID);
};

void ClsBlockDiagram::slotDiagramViewGotFocus_2(int iType, string strID) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotDiagramViewGotFocus_2(int iType, string strID)" << endl;
#endif

    clsQDiagramViewActive = clsQDiagramView0;
    clsQDiagramView->setFocus(false);
    clsQDiagramView0->setFocus(true);

    if(iType==ClsQCanvas::CANVAS_SYSTEM){
	iType = ClsFESystemManager::ITEM_SYSTEM;
    }
    else if(iType==ClsQCanvas::CANVAS_PROCESS){
	iType = ClsFESystemManager::ITEM_PROCESS;
    }

    emit sigItemActivated(iType, strID);
};

void ClsBlockDiagram::slotItemActivated(int iActiveItemType,string) {
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::slotItemActivated(int iActiveItemType,string)" << endl;
#endif
    
    if(bSimulationRunning){
	qpbtnNewProcess->setEnabled(false);
	qpbtnNewGroup->setEnabled(false);
	qpbtnNewConnectionExcitatory->setEnabled(false);
	qpbtnNewConnectionModulatory->setEnabled(false);
	qpbtnNewConnectionInhibitory->setEnabled(false);
    } else {
	if(iActiveItemType == ClsFESystemManager::ITEM_SYSTEM){
	    qpbtnNewProcess->setEnabled(true);
	    qpbtnNewGroup->setEnabled(false);
	    qpbtnNewConnectionExcitatory->setEnabled(false);
	    qpbtnNewConnectionModulatory->setEnabled(false);
	    qpbtnNewConnectionInhibitory->setEnabled(false);
	    
	}
	else if(iActiveItemType == ClsFESystemManager::ITEM_PROCESS){
	    qpbtnNewProcess->setEnabled(false);
	    qpbtnNewGroup->setEnabled(true);
	    qpbtnNewConnectionExcitatory->setEnabled(true);
	    qpbtnNewConnectionModulatory->setEnabled(true);
	    qpbtnNewConnectionInhibitory->setEnabled(true);
	}
    }
    iCurrentActivItemType = iActiveItemType;
}


ClsCanvasNodePhantom* ClsBlockDiagram::getCanvasNodePhantom(QCanvas* qcanvasTemp, string strPhantomGroupID, string strID, string strGroupName, ClsCanvasNode* clsCanvasNodeNeighbour){
    ClsCanvasNodePhantom *clsCanvasNodePhantom;
    if(qmapPhantomNodes.find(strPhantomGroupID)!= qmapPhantomNodes.end()){
	clsCanvasNodePhantom = (qmapPhantomNodes.find(strPhantomGroupID)).data();
    }
    else {
	int iXNearest = clsCanvasNodeNeighbour->rect().center().x();
	clsCanvasNodePhantom = new ClsCanvasNodePhantom( ClsFESystemManager::ITEM_GROUP,
							 iXNearest - PHANTOMGROUP_WIDTH/2, -3, PHANTOMGROUP_WIDTH, 24,
							 qcanvasTemp,
							 strGroupName, strID);
/* this still might not quite work, but it's the best I can come up at the moment...*/
	QMap<string, ClsCanvasNodePhantom*>::Iterator it;
	for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	    if(it.data()->canvas() == qcanvasTemp){
		while(clsCanvasNodePhantom->collidesWith(it.data())) {
		    clsCanvasNodePhantom->moveBy(10,10);
		}
	    }
	}

	qmapPhantomNodes[strPhantomGroupID] = clsCanvasNodePhantom;
    }

    clsCanvasNodePhantom->setZ(0);
    clsCanvasNodePhantom->show();
    return clsCanvasNodePhantom;
}



ClsCanvasConnection* ClsBlockDiagram::createCanvasConnectionPhantom(int iType, string strProcessID, string strConnectionID, string strSourceID, string strTargetID, int iConnectionType){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::createCanvasConnectionPhantom(int iType, string strProcessID, string strConnectionID, string strSourceID, string strTargetID, int iConnectionType)" << endl;
#endif
    QCanvas* qcanvasTemp = (qmapCanvases.find(strProcessID.c_str())).data();
    
    ClsCanvasNode* clsCanvasNodeReal;
    ClsCanvasNodePhantom *clsCanvasNodePhantom;    
    vector<vector<int> > vectorPoints;
    string strGroupName;
    string strPhantomGroupID;

    if(iType == ClsCanvasConnection::PHANTOM_START){
	clsCanvasNodeReal = getCanvasNode( strSourceID);
	strGroupName = ClsFESystemManager::Instance()->getGroupName(strTargetID);
	strPhantomGroupID = strTargetID;
    } else {
	clsCanvasNodeReal = getCanvasNode( strTargetID);
	strGroupName = ClsFESystemManager::Instance()->getGroupName(strSourceID);
	strPhantomGroupID = strSourceID;
    }
    
    strPhantomGroupID.append(strProcessID);
    string strPhantomConnectionID = strConnectionID; //!!!!!!!!!!!! CAREFUL!!!!! strPhantomConnectionID.append(strProcessID); 
    
    if(iType == ClsCanvasConnection::PHANTOM_START){
	clsCanvasNodePhantom = getCanvasNodePhantom(qcanvasTemp, strPhantomGroupID, strTargetID, strGroupName, clsCanvasNodeReal);
    } else {
	clsCanvasNodePhantom = getCanvasNodePhantom(qcanvasTemp, strPhantomGroupID, strSourceID, strGroupName, clsCanvasNodeReal);
    }

    ClsCanvasConnection *clsCanvasConnectionPhantom = new ClsCanvasConnection(qcanvasTemp, strPhantomConnectionID, strSourceID, strTargetID, iConnectionType, iType);

//    drawConnection(clsCanvasConnectionPhantom, clsCanvasNodeReal, clsCanvasNodePhantom,  vectorPoints);
    if(iType == ClsCanvasConnection::PHANTOM_START){
	drawConnection(clsCanvasConnectionPhantom, clsCanvasNodeReal, clsCanvasNodePhantom, vectorPoints);
    }else {
	drawConnection(clsCanvasConnectionPhantom, clsCanvasNodePhantom, clsCanvasNodeReal, vectorPoints);
    }

    return clsCanvasConnectionPhantom;
}


void ClsBlockDiagram::createMetaConnection(ClsCanvasConnection* clsCanvasConnection, string strSourceProcessID, string strTargetProcessID,
					   string strConnectionID,
					   string strSourceID, string strTargetID,
					   int iConnectionType ){
    
    ClsCanvasConnection *clsCanvasConnectionPhantomStart = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_START, strSourceProcessID, 
											 strConnectionID, strSourceID, strTargetID, iConnectionType);
    ClsCanvasConnection *clsCanvasConnectionPhantomEnd = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_END, strTargetProcessID, 
										       strConnectionID, strSourceID, strTargetID, iConnectionType);
    
/* this is a bit long, but required to keep the phantom connection stuff together */
    ClsCanvasMetaConnection *clsCanvasMetaConnection = new ClsCanvasMetaConnection(strConnectionID);
    clsCanvasConnection->setMetaConnectionParent(clsCanvasMetaConnection);
    clsCanvasConnectionPhantomStart->setMetaConnectionParent(clsCanvasMetaConnection);
    clsCanvasConnectionPhantomEnd->setMetaConnectionParent(clsCanvasMetaConnection);
    
    clsCanvasMetaConnection->addIPConnection(clsCanvasConnection);
    clsCanvasMetaConnection->addPhantomConnectionStart(clsCanvasConnectionPhantomStart);
    clsCanvasMetaConnection->addPhantomConnectionEnd(clsCanvasConnectionPhantomEnd);
    qmapMetaConnections[strConnectionID]=clsCanvasMetaConnection;
}

void ClsBlockDiagram::local2IPConnection(string strConnectionID, int iEndType){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::local2IPConnection(ClsCanvasConnection* clsCanvasConnection)" << endl;
#endif
    
    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
    
    string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
    string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
    
    int iConnectionType= ClsFESystemManager::Instance()->getConnectionTypeAsInt(strConnectionID);
    
/*
  createMetaConnection(clsCanvasConnection, strSourceProcessID, strTargetProcessID,
  strConnectionID,
  strSourceID, strTargetID,
  iConnectionType );
*/


/*
  QMap<string, ClsCanvasNodePhantom*>::Iterator it;
  for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
  string str = it.key();
  cout << "\tkey: " << str << endl;
  }
*/
    
    ClsCanvasConnection *clsCanvasConnectionPhantomStart = NULL;
    ClsCanvasConnection *clsCanvasConnectionPhantomEnd = NULL;

    ClsCanvasNodeAP* clsCanvasNodeAPSource = NULL;
    ClsCanvasNodeAP* clsCanvasNodeAPTarget = NULL;    

    if(iEndType == ClsCanvasConnection::CONNECTION_SOURCE){
	ClsCanvasNodePhantom* clsCanvasNodePhantom = getPhantomNodeByGroupID(strSourceID);
	if(clsCanvasNodePhantom!=NULL){
	    int ii = clsCanvasNodePhantom->getAPHandleNumber(strConnectionID.c_str());
//	    cout << "ii 0: " << ii << endl;
	    if(ii>=0){
		clsCanvasNodeAPTarget =  clsCanvasNodePhantom->getAP(ii);
	    }
	} 

	if(qmapNodes.find(strTargetID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strTargetID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strConnectionID.c_str());
//	    cout << "ii 1: " << ii << endl;
	    if(ii>=0){
		clsCanvasNodeAPTarget =  clsCanvasNode->getAP(ii);
	    }
	}

	if(qmapConnections.find(strConnectionID) != qmapConnections.end()){
	    clsCanvasConnectionPhantomEnd = qmapConnections.find(strConnectionID).data();
	    qmapConnections.erase(qmapConnections.find(strConnectionID));
	}

	clsCanvasConnectionPhantomStart = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_START, strSourceProcessID, 
									strConnectionID, strSourceID, strTargetID, iConnectionType);
    } else {
	if(qmapNodes.find(strSourceID) != qmapNodes.end()){
	    ClsCanvasNode *clsCanvasNode = qmapNodes.find(strSourceID).data();
	    int ii = clsCanvasNode->getAPHandleNumber(strConnectionID.c_str());
//	    cout << "ii 2: " << ii << endl;
	    if(ii>=0){
		clsCanvasNodeAPSource =  clsCanvasNode->getAP(ii);
	    }
	}
	
	ClsCanvasNodePhantom* clsCanvasNodePhantom = getPhantomNodeByGroupID(strTargetID);
	if(clsCanvasNodePhantom!=NULL){
	    int ii = clsCanvasNodePhantom->getAPHandleNumber(strConnectionID.c_str());
//	    cout << "ii 3: " << ii << endl;
	    if(ii>=0){
		clsCanvasNodeAPTarget = clsCanvasNodePhantom->getAP(ii);
	    }
	} 

	if(qmapConnections.find(strConnectionID) != qmapConnections.end()){
	    clsCanvasConnectionPhantomStart = qmapConnections.find(strConnectionID).data();
	    qmapConnections.erase(qmapConnections.find(strConnectionID));
	}

	clsCanvasConnectionPhantomEnd = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_END, strTargetProcessID, 
								      strConnectionID, strSourceID, strTargetID, iConnectionType);
    }

    /* connection at the system level */
    vector<vector<int> > vectorPoints;
    ClsQCanvas *qcanvasTemp = qmapCanvases[ClsFESystemManager::Instance()->getSystemID().c_str()];
    ClsCanvasConnection *clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::IP);
    qmapConnections[strConnectionID] = clsCanvasConnection;
    clsCanvasNodeAPSource = getCanvasNode(strSourceProcessID)->getNextFreeAP();
    clsCanvasNodeAPTarget = getCanvasNode(strTargetProcessID)->getNextFreeAP();
    drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);
    clsCanvasConnection->show();
    /* ------------------- */

    if(clsCanvasConnectionPhantomStart!=NULL && clsCanvasConnectionPhantomEnd!=NULL){
	ClsCanvasMetaConnection *clsCanvasMetaConnection = new ClsCanvasMetaConnection(strConnectionID);
	clsCanvasConnection->setMetaConnectionParent(clsCanvasMetaConnection);
	clsCanvasConnectionPhantomStart->setMetaConnectionParent(clsCanvasMetaConnection);
	clsCanvasConnectionPhantomEnd->setMetaConnectionParent(clsCanvasMetaConnection);
	
	clsCanvasMetaConnection->addIPConnection(clsCanvasConnection);
	clsCanvasMetaConnection->addPhantomConnectionStart(clsCanvasConnectionPhantomStart);
	clsCanvasMetaConnection->addPhantomConnectionEnd(clsCanvasConnectionPhantomEnd);
	qmapMetaConnections[strConnectionID]=clsCanvasMetaConnection;
    } else {
	cerr << "error creating metaconnection" << endl;
    }
}


void ClsBlockDiagram::IP2localConnection(string strConnectionID, int iEndType, string strOldGroupProcessID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::IP2localConnection(ClsCanvasConnection* clsCanvasConnection)" << endl;
#endif
    int iConnectionType= ClsFESystemManager::Instance()->getConnectionTypeAsInt(strConnectionID);
    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);

    string strPrcID = "";
    string strPhantomNodeKey = "";

    if(iEndType == ClsCanvasConnection::CONNECTION_SOURCE){
//	cout << "CONNECTION SOURCE" << endl;
	strPrcID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
	strPhantomNodeKey = strTargetID + strOldGroupProcessID;

    } else {
//	cout << "CONNECTION TARGET" << endl;
	strPrcID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
	strPhantomNodeKey = strSourceID + strOldGroupProcessID;
    }
    
    ClsCanvasNodeAP* clsCanvasNodeAPSource = NULL;
    if(qmapNodes.find(strSourceID) != qmapNodes.end()){
	ClsCanvasNode *clsCanvasNode = qmapNodes.find(strSourceID).data();
	int ii = clsCanvasNode->getAPHandleNumber(strConnectionID.c_str());
//	cout << "ii: " << ii << endl;
	if(ii>=0){
	    clsCanvasNodeAPSource =  clsCanvasNode->getAP(ii);
	}
    }
    
    ClsCanvasNodeAP* clsCanvasNodeAPTarget = NULL;
    if(qmapNodes.find(strTargetID) != qmapNodes.end()){
	ClsCanvasNode *clsCanvasNode = qmapNodes.find(strTargetID).data();
	int ii = clsCanvasNode->getAPHandleNumber(strConnectionID.c_str());
//	cout << "ii: " << ii << endl;
	if(ii>=0){
	    clsCanvasNodeAPTarget =  clsCanvasNode->getAP(ii);
	}
    }
    
    
    if(qmapConnections.find(strConnectionID) != qmapConnections.end()){
//	cout << "\tqmapConnections.find(strConnectionID) != qmapConnections.end()" << endl;
	
	/* remove the AP Handles at the System level */
	if(qmapNodes.find(strPrcID) != qmapNodes.end()){
//		cout << "IP2localConnection: remove AP Handle at System level" << endl;
	    qmapNodes.find(strPrcID).data()->removeAPHandle(strConnectionID.c_str());
	}
	
	
	if(qmapNodes.find(strOldGroupProcessID) != qmapNodes.end()){
//		cout << "IP2localConnection: remove AP Handle at System level" << endl;
	    qmapNodes.find(strOldGroupProcessID).data()->removeAPHandle(strConnectionID.c_str());
	}
	/* ---------- */
	
	/* we delete the connection ONLY in the diagram */
	/* we'll do a local handling of deleting using old id etc
	 */
	delete qmapConnections.find(strConnectionID).data();
	qmapConnections.remove(strConnectionID);
	
	delete qmapMetaConnections.find(strConnectionID).data();
	qmapMetaConnections.remove(strConnectionID);
	
	
	/* we recreate the connection ONLY in the diagram */
	if(clsCanvasNodeAPSource!=NULL && clsCanvasNodeAPTarget!=NULL){
	    vector<vector<int> > vectorPoints;
	    ClsQCanvas *qcanvasTemp = (qmapCanvases.find(strPrcID.c_str())).data();
	    ClsCanvasConnection *clsCanvasConnection = new ClsCanvasConnection(qcanvasTemp, strConnectionID, 
									       strSourceID, strTargetID, iConnectionType, ClsCanvasConnection::LOCAL);
	    qmapConnections[strConnectionID] = clsCanvasConnection;
	    
	    drawConnection(clsCanvasConnection, clsCanvasNodeAPSource, clsCanvasNodeAPTarget, vectorPoints);
	}
    }
}

void ClsBlockDiagram::rerouteMetaConnection(string strConnectionID, int /*iEndType*/, int iCanvasConnectionType, string strOldGroupProcessID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::rerouteMetaConnection(string strConnectionID, int iEndType)" << endl;
#endif
    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
    int iConnectionType =  ClsFESystemManager::Instance()->getConnectionTypeAsInt(strConnectionID);
    
    if(qmapNodes.find(strOldGroupProcessID) != qmapNodes.end()){
//	cout << "rerouteMetaConnection: remove AP Handle at System level" << endl;
	qmapNodes.find(strOldGroupProcessID).data()->removeAPHandle(strConnectionID.c_str());
    }
    
    
    QMap<string, ClsCanvasMetaConnection*>::Iterator itqmapMetaConnections = qmapMetaConnections.find(strConnectionID);
    if(itqmapMetaConnections!= qmapMetaConnections.end()){
	
	if(iCanvasConnectionType == ClsCanvasConnection::PHANTOM_START){
//	    cout << "******** ClsCanvasConnection::PHANTOM_START" << endl;
	    delete itqmapMetaConnections.data()->getPhantomEndConnection();
	    string strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetID);
	    ClsCanvasConnection *clsCanvasConnectionPhantomEnd = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_END, strTargetProcessID, 
											       strConnectionID, strSourceID, strTargetID, iConnectionType);
	    clsCanvasConnectionPhantomEnd->setMetaConnectionParent(itqmapMetaConnections.data());
	    itqmapMetaConnections.data()->addPhantomConnectionEnd(clsCanvasConnectionPhantomEnd);	
	    
	} else if(iCanvasConnectionType == ClsCanvasConnection::PHANTOM_END){
//	    cout << "******** ClsCanvasConnection::PHANTOM_END" << endl;
	    delete itqmapMetaConnections.data()->getPhantomStartConnection(); //ok
	    string strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceID);
	    ClsCanvasConnection *clsCanvasConnectionPhantomStart = createCanvasConnectionPhantom(ClsCanvasConnection::PHANTOM_START, strSourceProcessID, 
												 strConnectionID, strSourceID, strTargetID, iConnectionType);
	    clsCanvasConnectionPhantomStart->setMetaConnectionParent(itqmapMetaConnections.data());
	    itqmapMetaConnections.data()->addPhantomConnectionStart(clsCanvasConnectionPhantomStart);	
	}
    }
}


void ClsBlockDiagram::cleanupPhantomGroupAPs(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::cleanupPhantomGroupAPs()" << endl;
#endif
    QMap<string, ClsCanvasNodePhantom*>::Iterator it;
    /* first pass: 
       find AP of PhantomGroups that are invalid 
    */

    string strConnectionID = "";
    string strPhantomGroupKey = "";
    string strConnectionSourceID = "";
    string strConnectionTargetID = "";
    string strSourceProcessID = "";
    string strTargetProcessID = "";
    list<string>::iterator itLst;
    list<string> lst;

    for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	for(int ii=0; ii<PHANTOM_APCOUNT; ii++){
	    strPhantomGroupKey = it.key();

	    lst = it.data()->getAP(ii)->getActiveStartHandleIDs();
/*	    if(lst.size()>0){
		cout << endl << "@#strPhantomGroupKey:  " << strPhantomGroupKey << endl;
		cout << "it.data()->getName():  " << it.data()->getName() << endl;
		}*/
	    for(itLst=lst.begin(); itLst!=lst.end(); itLst++){
		strConnectionID = (*itLst);
		strConnectionSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
		strConnectionTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
		strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strConnectionTargetID);
 		string strSourceKey = strConnectionSourceID + strTargetProcessID;
// 		cout << "strConnectionSourceID: " << strConnectionSourceID << endl;
// 		cout << "strTargetProcessID:    " << strTargetProcessID << endl;
// 		cout << "sourceKey:             " << strSourceKey << endl;
		if(!strSourceKey.compare(strPhantomGroupKey)){
//		    cout << "VALID AP" << endl;
		} else {
//		    cout << "INVALID AP" << endl;
		    it.data()->removeAPHandle(strConnectionID.c_str());
		}
	    }



	    lst = it.data()->getAP(ii)->getActiveEndHandleIDs();
/*	    if(lst.size()>0){
		cout << endl << "@@strPhantomGroupKey:  " << strPhantomGroupKey << endl;
		cout << "it.data()->getName():  " << it.data()->getName() << endl;
		}*/
	    for(itLst=lst.begin(); itLst!=lst.end(); itLst++){
		strConnectionID = (*itLst);
		strConnectionTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
		strConnectionSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
		strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strConnectionSourceID);
 		string strTargetKey = strConnectionTargetID + strSourceProcessID;
// 		cout << "strConnectionSourceID: " << strConnectionSourceID << endl;
// 		cout << "strSourceProcessID:    " << strTargetProcessID << endl;
// 		cout << "targetKey:             " << strTargetKey << endl;
		if(!strPhantomGroupKey.compare(strTargetKey)){
//		    cout << "VALID AP" << endl;
		} else {
//		    cout << "INVALID AP" << endl;
		    it.data()->removeAPHandle(strConnectionID.c_str());
		}
	    }
	}

    }
}


void ClsBlockDiagram::prunePhantomGroups(){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::prunePhantomGroups()" << endl;
#endif
    QMap<string, ClsCanvasNodePhantom*>::Iterator it;

    for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	if(!it.data()->hasActiveAPs()){
	    cout << "Free phantom group: " << it.key() << endl;
	    deletePhantomGroups(it.data()->getID());
	}
    }

#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "Exit ClsBlockDiagram::prunePhantomGroups()" << endl;
#endif

}

ClsCanvasNodePhantom* ClsBlockDiagram::getPhantomNodeByGroupID(string strGroupID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getPhantomNodeByGroupID(string strGroupID)" << endl;
#endif

    QMap<string, ClsCanvasNodePhantom*>::Iterator it;
    for ( it = qmapPhantomNodes.begin(); it != qmapPhantomNodes.end(); ++it ) {
	if(!strGroupID.compare(it.data()->getID())){
	    return it.data();
	}
    }
    return NULL;
}



ClsCanvasNodePhantom* ClsBlockDiagram::getPhantomNodeByKey(string strKey){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::getPhantomNodeByKey(string strGroupID)" << endl;
#endif

    QMap<string, ClsCanvasNodePhantom*>::Iterator it;
    if(qmapPhantomNodes.find(strKey) != qmapPhantomNodes.end()){
	return qmapPhantomNodes[strKey];
    }
    return NULL;
}






void ClsBlockDiagram::reconnectIPConnection(string strConnectionID, int iEndType){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::reconnectIPConnection(string strConnectionID, int iEndType)" << endl;
#endif

    if(iEndType == ClsCanvasConnection::CONNECTION_SOURCE){
	string strGroupID = ClsFESystemManager::Instance()->getConnectionSourceID(strConnectionID);
	string strGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strGroupID );
	ClsCanvasHandleStart* clsCanvasHandleStart = NULL;
	if(qmapMetaConnections.find(strConnectionID) != qmapMetaConnections.end()){
	    clsCanvasHandleStart = qmapMetaConnections.find(strConnectionID).data()->getIPConnection()->getStartHandle();
	}
	
	if(clsCanvasHandleStart!=NULL){
	    clsCanvasHandleStart->moveBy(20,20);
	    if(qmapNodes.find(strGroupProcessID) != qmapNodes.end()){
		ClsCanvasNode *clsCanvasNode = qmapNodes.find(strGroupProcessID).data();
		ClsCanvasNodeAP* clsCanvasNodeAP = clsCanvasNode->getNextFreeAP();
		clsCanvasNodeAP->addHandleStart(clsCanvasHandleStart);
	    }
	}		
    } else {
	string strGroupID = ClsFESystemManager::Instance()->getConnectionTargetID(strConnectionID);
	string strGroupProcessID = ClsFESystemManager::Instance()->getGroupProcessID( strGroupID );
	ClsCanvasHandleEnd* clsCanvasHandleEnd = NULL;
	if(qmapMetaConnections.find(strConnectionID) != qmapMetaConnections.end()){
	    clsCanvasHandleEnd = qmapMetaConnections.find(strConnectionID).data()->getIPConnection()->getEndHandle();
	}
	
	if(clsCanvasHandleEnd!=NULL){
	    clsCanvasHandleEnd->moveBy(20,20);
	    if(qmapNodes.find(strGroupProcessID) != qmapNodes.end()){
		ClsCanvasNode *clsCanvasNode = qmapNodes.find(strGroupProcessID).data();
		ClsCanvasNodeAP* clsCanvasNodeAP = clsCanvasNode->getNextFreeAP();
		clsCanvasNodeAP->addHandleEnd(clsCanvasHandleEnd);
	    }
	}
    }
    
    
}

void ClsBlockDiagram::setGroupModuleIOIcons(string strProcessID){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setGroupModuleIOIcons(string strProcessID)" << endl;
#endif
    list<string>::iterator it;
    list<string> lstIFM = ClsFESystemManager::Instance()->getFEProcess(strProcessID)->getListGroupsInputFromModule();
    for(it=lstIFM.begin(); it!=lstIFM.end(); ++it){
	setClsCanvasNodeGroupIcon ((*it), ClsCanvasNodeIcon::ICON_GROUP_MODULE_IN, true);
//		    cout << "lstIFM: " << (*it) << endl;
    }
    
    list<string> lstOTM = ClsFESystemManager::Instance()->getFEProcess(strProcessID)->getListGroupsOutputToModule();
    for(it=lstOTM.begin(); it!=lstOTM.end(); it++){
//		    cout << "lstOTM: " << (*it) << endl;
	setClsCanvasNodeGroupIcon ((*it), ClsCanvasNodeIcon::ICON_GROUP_MODULE_OUT, true);
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
//CRASH:
	    setClsCanvasNodeGroupIcon (strGroupID, ClsCanvasNodeIcon::ICON_GROUP_MODULE_IN, false);
//-----
	    setClsCanvasNodeGroupIcon (strGroupID, ClsCanvasNodeIcon::ICON_GROUP_MODULE_OUT, false);
	}
    }
}

void ClsBlockDiagram::resetGroupSynapseInputIcons(string /* strProcessID */){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::resetGroupSynapseInputIcons(string /* strProcessID */)" << endl;
#endif

}

void ClsBlockDiagram::setClsCanvasNodeGroupIcon (string strGroupID, int iIconType, bool b){
#ifdef DEBUG_CLSBLOCKDIAGRAM
    cout << "ClsBlockDiagram::setClsCanvasNodeGroupIcon (string strGroupID, int iIconType, bool b)" << endl;
#endif
    if(qmapNodes.find(strGroupID) != qmapNodes.end()){
	if( dynamic_cast<ClsCanvasNodeGroup*>(qmapNodes.find(strGroupID).data()) ){
	    if(iIconType == ClsCanvasNodeIcon::ICON_GROUP_MODULE_IN){
//CRASH:
		dynamic_cast<ClsCanvasNodeGroup*>(qmapNodes.find(strGroupID).data())->setHasInputFromModule(b);
//-----
	    } else if (iIconType == ClsCanvasNodeIcon::ICON_GROUP_MODULE_OUT){
		dynamic_cast<ClsCanvasNodeGroup*>(qmapNodes.find(strGroupID).data())->setHasOutputToModule(b);
	    } else if (iIconType == ClsCanvasNodeIcon::ICON_GROUP_SYNAPSE_IN){
		dynamic_cast<ClsCanvasNodeGroup*>(qmapNodes.find(strGroupID).data())->setHasInputFromSynapse(b);
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
	    qmapNodes[strID]->setSelected(true);
	}
    }
    else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	if(qmapConnections.find(strID) != qmapConnections.end()){
	    qmapConnections[strID]->setSelected(true);
	}
    } 

}


void ClsBlockDiagram::disableToolbarButtons(bool _b){
//    cout << "ClsBlockDiagram::disableToolbarButtons(bool b)" << endl;

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


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:




