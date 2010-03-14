/****************************************************************************
 ** $Filename: ClsFEDialogManager.cpp
 ** $Id: ClsFEDialogManager.cpp,v 1.5 2005/10/29 20:18:17 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Wed Feb 20 14:50:21 2002
 ** $Date: 2005/10/29 20:18:17 $
 **
 ** $Log: ClsFEDialogManager.cpp,v $
 ** Revision 1.5  2005/10/29 20:18:17  ulysses
 ** *** empty log message ***
 **
 ** Revision 1.4  2003/04/29 11:59:44  ulysses
 ** before using the mediator
 **
 ** Revision 1.3  2003/03/27 14:50:39  ulysses
 ** before adding Mark's autoWidgets
 **
 ** Revision 1.2  2003/03/11 09:57:38  ulysses
 ** before implementing property dialogs
 **
 ** Revision 1.1  2002/02/20 13:51:41  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/
#include <qmessagebox.h>

#include <iqrMainWindow.h>
#include <ClsFEDialogManager.h>
#include <ClsFEDataClient.h>
#include <ClsFEDiagramManager.h>

#include <ClsFESystemManager.h>
#include "ClsFESystem.h"
#include "ClsFEProcess.h"
#include "ClsFEConnection.h"
#include "ClsFEGroup.h"
#include "ClsBaseArborization.h"
#include "idGenerator.hpp"
#include "ClsQInfoTip.h"

/* autoDialog related */
#include "autoDialog.hpp"
//#include "groupAutoDialog.hpp"
#include "subtypedAutoDialog.hpp"
#include "HelpDlgs/iqrAbout.h"
#include "HelpDlgs/iqrReleaseNotes.h"

#include "ClsFESettings.h"
#include "diagramTypes.h"

using namespace iqrcommon;
using namespace iqrfe;

//#define DEBUG_CLSFEDIALOGMANAGER

ClsFEDialogManager*  ClsFEDialogManager::_instanceDialogManager = NULL;


void ClsFEDialogManager::initializeDialogManager(iqrMainWindow* _parent){
    _instanceDialogManager = new ClsFEDialogManager(_parent);
}


ClsFEDialogManager* ClsFEDialogManager::Instance(){
    return _instanceDialogManager;
}



ClsFEDialogManager::ClsFEDialogManager(iqrMainWindow* _parent) : parent(_parent) {
    bSubTypesChangable = true;
};


void ClsFEDialogManager::createPopup(int iType, string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createPopup(int iType, string strID, QPoint qpLocaltion)" << endl;
#endif
//    cout << "iType:" << iType << endl;
//    cout << "ID: " << strID << endl;

/* instead of passing the type, it could also be queried from the systemManager... */
    switch (iType){
    case ClsFESystemManager::ITEM_SYSTEM:
	createSystemPopup( strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_PROCESS:
	createProcessPopup( strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_GROUP:
	createGroupPopup( strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_CONNECTION:
	createConnectionPopup( strID, qpLocation);
	break;
    default:
	break;
    }


};

void ClsFEDialogManager::createPopup(int iType, list<string> listIDs, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createPopup((int iType, list<string> listIDs, QPoint qpLocation)" << endl;
    cout << "iType:" << iType << endl;
#endif
/* instead of passing the type, it could also be queried from the systemManager... */
    switch (iType){
    case ClsFESystemManager::ITEM_GROUP:
	createMultiGroupPopup( listIDs, qpLocation);
	break;
//       case ClsFESystemManager::ITEM_PROCESS:
//	       createProcessPopup( strID, qpLocation);
//	  break;
//    case ClsFESystemManager::ITEM_CONNECTION:
//	createConnectionPopup( listIDs, qpLocation);
//	break;
    default:
	break;
    }
};


int ClsFEDialogManager::createPropertyDialog(int iType, string strID, QPoint qpLocation) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createPropertyDialog(int iType, string strID, QPoint qpLocation)" << endl;
#endif

    switch (iType){
    case ClsFESystemManager::ITEM_SYSTEM:
	createSystemPropertyDialog(strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_PROCESS:
	createProcessPropertyDialog(strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_GROUP:
	createGroupPropertyDialog(strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_CONNECTION:
	createConnectionPropertyDialog(strID, qpLocation);
	break;
    default:
	break;
    }

    return 0;

};



void ClsFEDialogManager::createSystemPopup(string strID, QPoint qpLocation){
    strID = "";
//    qpLocation.setX(0);

    QPopupMenu* qpopupMenu = new QPopupMenu( this );
    int iPropertyID = 990; /* not very elegant, but it should work... */
    qpopupMenu->insertItem( "Properties", iPropertyID, -1 );
    int iClientType = qpopupMenu->exec( qpLocation );

    if(iClientType==iPropertyID){
	createSystemPropertyDialog(strID, qpLocation);
    }


};

void ClsFEDialogManager::createProcessPopup(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createProcessPopup(string strID, QPoint qpLocation)" << endl;
#endif
    QPopupMenu* qpopupMenu = new QPopupMenu( this );
    int iPropertyID = 990; /* not very elegant, but it should work... */
    int iDeleteID = 1000;
    int iCopyID = 1100;
    int iExportID = 1200;


    qpopupMenu->insertItem( "Properties", iPropertyID, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Show block diagram", diagramTypes::DIAGRAM_BLOCK, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Copy Process", iCopyID, -1 );
    qpopupMenu->insertItem( "Export Process", iExportID, -1 );
    if(bSubTypesChangable){
	qpopupMenu->insertItem( "Delete Process", iDeleteID, -1 );
    }

    int iClientType = qpopupMenu->exec( qpLocation );


    if(iClientType==iPropertyID){
	createProcessPropertyDialog(strID, qpLocation);
    }
    else if(iClientType==iDeleteID){
	createItemDeleteDialog(ClsFESystemManager::ITEM_PROCESS, strID);
    }
    else if(iClientType==iCopyID){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_PROCESS , lst);
	}
    }
    else if(iClientType==iExportID){
	if(parent!=NULL){
	    parent->exportProcess(strID);
	}
    }
    else if(iClientType==diagramTypes::DIAGRAM_BLOCK){
	if(ClsFEDiagramManager::Instance()!=NULL){
	    ClsFEDiagramManager::Instance()->DiagramShow(iClientType , strID);
	}
    }
};

void ClsFEDialogManager::createGroupPopup(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "createGroupPopup(string strID, QPoint qpLocation)" << endl;
#endif

    QPopupMenu* qpopupMenu = new QPopupMenu( this );
    int iPropertyID = 990; /* not very elegant, but it should work... */
    int iDeleteID = 1000;
    int iCopyID = 1100;
    int iStateManipID = 1200;

    qpopupMenu->insertItem( "Properties", iPropertyID, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Space plot", ClsFEDataClient::CLIENT_SPACEPLOT, -1 );
    qpopupMenu->insertItem( "Time Plot", ClsFEDataClient::CLIENT_TIMEPLOT, -1 );
//    qpopupMenu->insertItem( "Correlation Plot", ClsFEDataClient::CLIENT_CORRPLOT, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "State Manipulation Panel", iStateManipID, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Copy Group", iCopyID, -1 );
    if(bSubTypesChangable){
	qpopupMenu->insertItem( "Delete Group", iDeleteID, -1 );
    }

//     connect( qpopupMenu, SIGNAL(activated(int)), SLOT(slotSendMessage(int)) );
//     qpopupMenu->popup( qpLocation );

    int iClientType = qpopupMenu->exec( qpLocation );
//    cout << "iClientType: "  << iClientType << endl;
    if(iClientType==iPropertyID){
	createGroupPropertyDialog(strID, qpLocation);
    }
    else if(iClientType==iDeleteID){
	createItemDeleteDialog(ClsFESystemManager::ITEM_GROUP, strID);
    }
    else if(iClientType==iCopyID){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , lst);
	}
    }
    else if(iClientType==ClsFEDataClient::CLIENT_SPACEPLOT ||
	    iClientType==ClsFEDataClient::CLIENT_TIMEPLOT ||
	    iClientType==ClsFEDataClient::CLIENT_CORRPLOT ){
	if(ClsFEDataManager::Instance()!=NULL){
	    ClsFEDataManager::Instance()->DataClientCreate(iClientType , strID, "", "");
	}

    } else if(iClientType==iStateManipID){
	if(ClsFEDiagramManager::Instance()!=NULL){
	    ClsFEDiagramManager::Instance()->DiagramCreate(diagramTypes::DIAGRAM_GROUP_MANIP , strID);
	}

    }


};


//void ClsFEDialogManager::createConnectionPopup(list<string> listIDs, QPoint qpLocation) {
void ClsFEDialogManager::createConnectionPopup(string strID, QPoint qpLocation) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createConnectionPopup(string strID, QPoint qpLocation)" << endl;
#endif
    QPopupMenu* qpopupMenu = new QPopupMenu( this );
    int iPropertyID = 990; /* not very elegant, but it should work... */
    int iDeleteID = 1000;
    int iCopyID = 1100;
/*    int iArbID = 1010; */


    qpopupMenu->insertItem( "Properties", iPropertyID, -1 );
/*    qpopupMenu->insertItem( "Arborization", iArbID, -1 ); */
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Connection plot", diagramTypes::DIAGRAM_CONNECTION, -1 );
    qpopupMenu->insertSeparator();


    /* don't even try to copy a disconnected connection */
    if(ClsFESystemManager::Instance()->getConnectionSourceID(strID).size()>0 && 
       ClsFESystemManager::Instance()->getConnectionTargetID(strID).size()>0){
	qpopupMenu->insertItem( "Copy Connection", iCopyID, -1 );
    }

    if(bSubTypesChangable){
	qpopupMenu->insertItem( "Delete Connection", iDeleteID, -1 );
    }

    int iClientType = qpopupMenu->exec( qpLocation );
//    cout << "iClientType: "  << iClientType << endl;
    if(iClientType==iPropertyID){
	createConnectionPropertyDialog(strID, qpLocation);
    }
    else if(iClientType==iDeleteID){
	createItemDeleteDialog(ClsFESystemManager::ITEM_CONNECTION, strID);
    }
    else if(iClientType==iCopyID){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_CONNECTION , lst);
	}
    }
    else if(iClientType==diagramTypes::DIAGRAM_CONNECTION){
	if(ClsFEDataManager::Instance()!=NULL){
	    ClsFEDataManager::Instance()->DataClientCreate(iClientType , strID, "", "");
	}
    }

};

void ClsFEDialogManager::createMultiGroupPopup(list<string> listIDs, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createMultiGroupPopup(list<string> listIDs, QPoint qpLocation)" << endl;
#endif

    int iCopyID = 1000;
    int iMultiCopyID = 1100;
    int iDeleteID = 1200;
    int iMultiDeleteID = 1300;
    QPopupMenu* qpopupMenu = new QPopupMenu( this );
    qpopupMenu->insertItem( "Space plots", ClsFEDataClient::CLIENT_SPACEPLOT, -1 );
    qpopupMenu->insertItem( "Time Plots", ClsFEDataClient::CLIENT_TIMEPLOT, -1 );
//    qpopupMenu->insertItem( "Correlation Plots", ClsFEDataClient::CLIENT_CORRPLOT, -1 );
    qpopupMenu->insertSeparator();
    qpopupMenu->insertItem( "Copy Groups", iCopyID, -1 );
    qpopupMenu->insertItem( "Copy Groups and Connections", iMultiCopyID, -1 );

    if(bSubTypesChangable){
	qpopupMenu->insertItem( "Delete Groups", iDeleteID, -1 );
	qpopupMenu->insertItem( "Delete Groups and Connections", iMultiDeleteID, -1 );
    }

    int iClientType = qpopupMenu->exec( qpLocation );

//    cout << "iClientType: "  << iClientType << endl;
    if(iClientType==ClsFEDataClient::CLIENT_SPACEPLOT ||
       iClientType==ClsFEDataClient::CLIENT_TIMEPLOT ||
       iClientType==ClsFEDataClient::CLIENT_CORRPLOT ){
	list<string>::iterator it;
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    if(ClsFEDataManager::Instance()!=NULL){
		ClsFEDataManager::Instance()->DataClientCreate(iClientType , *it, "", "");
	    }

	}
    }
    else if(iClientType==iCopyID){
	if(parent!=NULL){
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , listIDs );
	}
    } 
    else if(iClientType==iMultiCopyID){
	if(parent!=NULL){
	    list<string> lstAffectedConnections = ClsFESystemManager::Instance()->getConnectionsByInvolvedGroups(listIDs);
	    if(lstAffectedConnections.size()>0){
		parent->copyItemsToClipboard(listIDs , lstAffectedConnections );
	    } else {
		parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , listIDs );
	    }
	}
    }
    else if(iClientType==iDeleteID){
	list<string>::iterator it;
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    createItemDeleteDialog(ClsFESystemManager::ITEM_GROUP, *it);
	}
    }
    else if(iClientType==iMultiDeleteID){
	list<string>::iterator it;
	list<string> lstAffectedConnections = 
	ClsFESystemManager::Instance()->getConnectionsByInvolvedGroups(listIDs);
	for (it = lstAffectedConnections.begin(); it != lstAffectedConnections.end(); ++it){
	    createItemDeleteDialog(ClsFESystemManager::ITEM_CONNECTION, *it);
	}
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    createItemDeleteDialog(ClsFESystemManager::ITEM_GROUP, *it);
	}
    }
};





int ClsFEDialogManager::createSystemPropertyDialog(string strID, QPoint qpLocation) {
    strID = "";
    qpLocation.setX(0);
    /* this function is responsible for bringing up property dialogs.
       The information is retreived and stored in the ClsFESystemManager.
       If the string is "", this means, that the object has to be created.
       IDs are created here */
    ClsFESystem *clsFESystem = ClsFESystemManager::Instance()->getFESystem ();
    strID = clsFESystem->getSystemID();


    if(clsFESystem!=NULL){
	string strCaption = "Properties for System: ";
	strCaption.append(ClsFESystemManager::Instance()->getSystemName());
	ClsAutoDialog *systemDialog = new ClsAutoDialog(*clsFESystem,
							ClsFESystemManager::ITEM_SYSTEM, strID,
							true,
							NULL,
							strCaption.c_str());

	systemDialog->exec();

	ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_SYSTEM, strID);

    }


    return 0;
}

//--int ClsFEDialogManager::createProcessPropertyDialog_OLD(string strID, QPoint qpLocation) {
//--    qpLocation.setX(0);
//--    /* this function is responsible for bringing up property dialogs.
//--       The information is retreived and stored in the ClsFESystemManager.
//--       If the string is "", this means, that the object has to be created.
//--       IDs are created here */
//--
//--    ClsFEProcess *clsFEProcess = NULL;
//--    if(strID.length()>0){
//--	clsFEProcess = ClsFESystemManager::Instance()->getFEProcess( strID);
//--    }
//--
//--
//--    if(clsFEProcess!=NULL){
//--	string strCaption = "Properties for Process: ";
//--	strCaption.append(ClsFESystemManager::Instance()->getProcessName(strID));
//--	ClsAutoDialog *processDialog = new ClsAutoDialog(*clsFEProcess,
//--							  ClsFESystemManager::ITEM_PROCESS, strID,
//--							  false,
//--							  NULL,
//--							  strCaption.c_str());
//--
//--	processDialog->exec();
//--
//--	ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_PROCESS, strID);
//--
//--    }
//--    return 0;
//--}


int ClsFEDialogManager::createProcessPropertyDialog(string strID, QPoint /*qpLocation*/) {
    if(qmapAutoDialogs.find(strID) != qmapAutoDialogs.end()){
	cout << "groupDialog for " << strID << " already opened" << endl;
	qmapAutoDialogs.find(strID).data()->raise();
    } else {
	ClsFEProcess *clsFEProcess = NULL;
	string strCaption = "Properties for Process: ";
	if(strID.length()>0){
	    clsFEProcess = ClsFESystemManager::Instance()->getFEProcess( strID);
		strCaption.append(ClsFESystemManager::Instance()->getProcessName(strID));
	}

	if(clsFEProcess!=NULL){
	    ClsSubtypedAutoDialog *processDialog = new ClsSubtypedAutoDialog ( *clsFEProcess,
									       ClsFESystemManager::ITEM_PROCESS, 
									       strID,
									       false, // true, /* _bModal */
									       NULL, 
									       strCaption.c_str());
	    
	    
	    connect(this, SIGNAL(sigSubTypesChangable(bool)), processDialog, SLOT(slotSubTypesChangable(bool)));
	    processDialog->slotSubTypesChangable(bSubTypesChangable);
	    connect(processDialog, SIGNAL(sigApplied(int, string)), this, SLOT(slotItemChanged(int, string)));
	    connect(processDialog, SIGNAL(sigDialogClosed(string)), this, SLOT( slotDialogClosed(string)));

	    qmapAutoDialogs[strID] = processDialog;
	    processDialog->show();
	}
    }
    return 0;
}


int ClsFEDialogManager::createSettingsDialog() {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createSettingsDialog()" << endl;
#endif

    ClsFESettings& settings = ClsFESettings::instance();

    ClsAutoDialog *settingsDialog = new ClsAutoDialog(settings,
				 -99, "Settings",
				 false,
				 NULL,
				 "settings properties");


    settingsDialog->exec();
    return 0;

}


int ClsFEDialogManager::createGroupPropertyDialog(string strID, QPoint) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createGroupPropertyDialog(string strID, QPoint qpLocation)" << endl;
#endif
    /* this function is responsible for bringing up property dialogs.
       The information is retreived and stored in the ClsFESystemManager.
       If the string is "", this means, that the object has to be created.
       IDs are created here */


    if(qmapAutoDialogs.find(strID) != qmapAutoDialogs.end()){
	cout << "groupDialog for " << strID << " already opened" << endl;

	qmapAutoDialogs.find(strID).data()->raise();
    } else {
	ClsFEGroup *clsFEGroup = NULL;
	string strCaption = "Properties for Group: ";
	if(strID.length()>0){
	    clsFEGroup = ClsFESystemManager::Instance()->getFEGroup( strID);
	    strCaption.append(ClsFESystemManager::Instance()->getGroupName(strID));
	}

	if(clsFEGroup!=NULL){

	    ClsSubtypedAutoDialog *groupDialog = new ClsSubtypedAutoDialog( *clsFEGroup,
									   ClsFESystemManager::ITEM_GROUP,
									   strID,
									   false,
									   NULL,
									   strCaption.c_str());


	    connect(this, SIGNAL(sigSubTypesChangable(bool)), groupDialog, SLOT(slotSubTypesChangable(bool)));
	    groupDialog->slotSubTypesChangable(bSubTypesChangable);
	    connect(groupDialog, SIGNAL(sigApplied(int, string)), this, SLOT(slotItemChanged(int, string)));
	    connect(groupDialog, SIGNAL(sigDialogClosed(string)), this, SLOT( slotDialogClosed(string)));


	    qmapAutoDialogs[strID] = groupDialog;
	    groupDialog->show();

//	    ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_GROUP, strID);

	}
    }

    return 0;

}


int ClsFEDialogManager::createConnectionPropertyDialog(string strID, QPoint /*qpLocation*/){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createConnectionPropertyDialog(string strID, QPoint qpLocation)::strID:" << strID << endl;
#endif

    /* this function is responsible for bringing up property dialogs.
       The information is retreived and stored in the ClsFESystemManager.
       If the string is "", this means, that the object has to be created.
       IDs are created here */

    if(qmapAutoDialogs.find(strID) != qmapAutoDialogs.end()){
	cout << "groupDialog for " << strID << " already opened" << endl;
	qmapAutoDialogs.find(strID).data()->raise();
    } else {
	ClsFEConnection *clsFEConnection = NULL;
	string strCaption = "Properties for Connection: ";
	if(strID.length()>0){
	    clsFEConnection = ClsFESystemManager::Instance()->getFEConnection( strID);
		strCaption.append(ClsFESystemManager::Instance()->getConnectionName(strID));
	}

	if(clsFEConnection!=NULL){
	    ClsSubtypedAutoDialog *connectionDialog = new ClsSubtypedAutoDialog ( *clsFEConnection,
										  ClsFESystemManager::ITEM_CONNECTION, strID,
										  false,
										  NULL,
										 strCaption.c_str());


	    connect(this, SIGNAL(sigSubTypesChangable(bool)), connectionDialog, SLOT(slotSubTypesChangable(bool)));
	    connectionDialog->slotSubTypesChangable(bSubTypesChangable);
	    connect(connectionDialog, SIGNAL(sigApplied(int, string)), this, SLOT(slotItemChanged(int, string)));
	    connect(connectionDialog, SIGNAL(sigDialogClosed(string)), this, SLOT( slotDialogClosed(string)));

	    qmapAutoDialogs[strID] = connectionDialog;
	    connectionDialog->show();

	}
    }

    return 0;
}

int ClsFEDialogManager::createConnectionArborizationPropertyDialog(string strID, QPoint /*qpLocation*/){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createConnectionArborizationPropertyDialog(string strID, QPoint qpLocation)::strID:" << strID << endl;
#endif


     ClsFEConnection *clsFEConnection = NULL;

     if(strID.length()>0){
	clsFEConnection = ClsFESystemManager::Instance()->getFEConnection( strID);
     }


     if(clsFEConnection!=NULL){
	 ClsBaseArborization* clsBaseArborization = clsFEConnection->getArborization();
	 if(clsBaseArborization!=NULL){
	     ClsAutoDialog *connectionDialog = new ClsAutoDialog (*clsBaseArborization,
								 ClsFESystemManager::ITEM_CONNECTION, strID,
								 true,
								 NULL,
								 "connection arborization properties");
	     connectionDialog->exec();
	     ClsFESystemManager::Instance()->itemChanged(ClsFESystemManager::ITEM_CONNECTION, strID); /* is this necessary? */
	 }

     }
    return 0;
}


void ClsFEDialogManager::createAboutDialog(){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createAboutDialog()" << endl;
#endif
     dlgAboutIqr * aboutIqr = new dlgAboutIqr ( this, "aboutIqr", TRUE);
     aboutIqr->show();
}

void ClsFEDialogManager::createReleaseNotesDialog(){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createReleaseNotesDialog()" << endl;
#endif
     dlgReleaseNotes * releaseNotesIqr = new dlgReleaseNotes ( this, "releaseNotesIqr", TRUE);
     releaseNotesIqr->show();
}


string ClsFEDialogManager::createSaveDialog(string strFileTypeDescription, string strExtension){
    QString qstrFileName = "";
    bool bValidFile = false;
    while(!bValidFile){
	qstrFileName = QFileDialog::getSaveFileName(
	    /*QDir::homeDirPath ()*/ "", 
	    strFileTypeDescription.c_str(),
	    this,   
	    "Save File"
	    "Choose a filename to save under" );

	/* append default extension here */    
	QString qstrExtension( strExtension.c_str() );
	QString qstrCurrentTail = qstrFileName.right(qstrExtension.length());
	if(qstrCurrentTail.compare(qstrExtension)){
	    qstrFileName.append(qstrExtension);
	} 
	/* ------------------------------ */ 


	QFileInfo qFileInfo(qstrFileName );          
	QString qstrPath = qFileInfo.dirPath(true);
	qFileInfo.setFile (qstrPath);
//	cout << "qFileInfo.isWritable(): " << qFileInfo.isWritable() << endl;
	
	if ( !qFileInfo.isWritable() ) {
	    int iReturn = QMessageBox::critical( this, "iqr",
						 "No permission to in selected directory\n",
						 "Retry", "Cancel", 0, 0, 1 );
	    if(iReturn == 1){
		return "";
	    }
	} else {
	    bValidFile = true;
	}

    }

    /* this check should not be necessay, because the dialog should always return a name... */
    if(qstrFileName.length()<=0){
	return "";
    }


    if ( QFile::exists(qstrFileName) ) {
/*      warning on overwrite */
	int iReturn = QMessageBox::warning( this, "iqr",
					    "A file with the seleteced name exists\n",
					    "Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
#ifdef DEBUG_CLSFEDIALOGMANAGER
	    cout << "saving aborted" << endl;
#endif
	    return "";
	}
    }

    return qstrFileName.latin1();
}


void  ClsFEDialogManager::createItemDeleteDialog( int iType, string strID) {

    string strName = "";
    string strType = "";
    
    
    switch (iType){
    case ClsFESystemManager::ITEM_PROCESS:
	strName = " \"" + ClsFESystemManager::Instance()->getProcessName(strID) + "\"";
	strType = "Process";
//	cout << "PROCESS" << endl;
	break;
    case ClsFESystemManager::ITEM_GROUP:
	strName = " \"" + ClsFESystemManager::Instance()->getGroupName(strID) + "\"";
	strType = "Group";
//	cout << "GROUP" << endl;
	break;
    case ClsFESystemManager::ITEM_CONNECTION:
	strName = ClsFESystemManager::Instance()->getConnectionLongName(strID);
//	strMessage = "Connection " + strSourceGroupName + "->" + strTargetGroupName + strMessage;
	strType = "Connection";
//	cout << "CONNECTION" << endl;
	break;
    default:
	break;
    }


    string strTitle = "Delete " + strType;
    string strMessage  = "Are you sure you want to delete ";
    strMessage = strMessage  + "\n" + strType + strName;


    int iReturn = QMessageBox::warning( this, strTitle.c_str(),
					strMessage.c_str(),
					"no",
					"yes", 0, 0, 1 );

    if(iReturn == 1){
	ClsFESystemManager::Instance()->deleteItem(iType, strID);
    }
}


void ClsFEDialogManager::createInfoTip(int iType, string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createInfoTip(int iType, string strID, QPoint qpLocation)" << endl;
#endif
    switch (iType){
    case ClsFESystemManager::ITEM_PROCESS:
	createProcessInfoTip( strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_GROUP:
	createGroupInfoTip( strID, qpLocation);
	break;
    case ClsFESystemManager::ITEM_CONNECTION:
	createConnectionInfoTip( strID, qpLocation);
	break;
    default:
	break;
    }
}



void ClsFEDialogManager::createProcessInfoTip(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createProcessInfoTip(string strID,  QPoint qpLocation)" << endl;
#endif

    ClsFEProcess *clsFEProcess = NULL;
    if(strID.length()>0){
	clsFEProcess = ClsFESystemManager::Instance()->getFEProcess( strID);
    }
    
    if(clsFEProcess!=NULL){
	string strInfo;
	strInfo = "Process";
	strInfo = strInfo + "\nID:\t" + clsFEProcess->getProcessID();
	strInfo = strInfo + "\nName:           " + clsFEProcess->getProcessName();
	strInfo = strInfo + "\nModule enabled: " + clsFEProcess->getEnableModulesAsString();
	strInfo = strInfo + "\nModule label:   " + clsFEProcess->getProcessModuleLabel();   
	strInfo = strInfo + "\n--Notes--\n" + clsFEProcess->getNotes();
    	ClsQInfoTip *clsQInfoTip = new ClsQInfoTip(qpLocation, strInfo);
    }
}


void ClsFEDialogManager::createGroupInfoTip(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createGroupInfoTip(string strID, QPoint qpLocation)" << endl;
#endif
    
    ClsFEGroup *clsFEGroup = NULL;
    if(strID.length()>0){
	clsFEGroup = ClsFESystemManager::Instance()->getFEGroup( strID);
    }
    
    if(clsFEGroup!=NULL){
	string strInfo;
	strInfo = "Group";
	strInfo = strInfo + "\nID:" + clsFEGroup->getGroupID();
	strInfo = strInfo + "\nName:        " + clsFEGroup->getGroupName();
	strInfo = strInfo + "\nNeuron:      " + clsFEGroup->getGroupNeuronLabel();
	strInfo = strInfo + "\nTopology:    " + clsFEGroup->TopologyType();
	strInfo = strInfo + "\nGroup width: " + iqrUtils::int2string(clsFEGroup->getNrCellsHorizontal());
	strInfo = strInfo + "\nGroup height:" + iqrUtils::int2string(clsFEGroup->getNrCellsVertical());
	strInfo = strInfo + "\n--Notes--\n" + clsFEGroup->getNotes();
	ClsQInfoTip *clsQInfoTip = new ClsQInfoTip(qpLocation, strInfo);
    }


}

void ClsFEDialogManager::createConnectionInfoTip(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createConnectionInfoTip(string strID, QPoint qpLocation)" << endl;
#endif
    
    ClsFEConnection *clsFEConnection = NULL;
    if(strID.length()>0){
	clsFEConnection = ClsFESystemManager::Instance()->getFEConnection( strID);
    }
    
    if(clsFEConnection!=NULL){
	string strInfo;
	strInfo = "Connection";
	strInfo = strInfo + "\nID:" + clsFEConnection->getConnectionID();
	strInfo = strInfo + "\nname:                 " + clsFEConnection->getConnectionName();
	strInfo = strInfo + "\nType:                 " + clsFEConnection->getConnectionTypeAsString();   
	strInfo = strInfo + "\nSynapse:              " + clsFEConnection->getConnectionSynapseLabel();   
	strInfo = strInfo + "\nArborization:         " + clsFEConnection->ArborizationType();	       
	strInfo = strInfo + "\nPattern:              " + clsFEConnection->PatternType();		       
	strInfo = strInfo + "\nAttenuation function: " + clsFEConnection->AttenuationFunctionType();     
	strInfo = strInfo + "\nDelay function:       " + clsFEConnection->DelayFunctionType();           
	strInfo = strInfo + "\n--Notes--\n" + clsFEConnection->getNotes();
    	ClsQInfoTip *clsQInfoTip = new ClsQInfoTip(qpLocation, strInfo);
    }
}



void ClsFEDialogManager::slotSimulationRunning(bool b) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::slotSimulationRunning(bool b)" << endl;
#endif

    bSubTypesChangable = abs(b -1);
    emit sigSubTypesChangable(bSubTypesChangable);
};

void ClsFEDialogManager::slotItemChanged(int iType, string strID) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::slotItemChanged(int iType, strID)" << endl;
#endif
    ClsFESystemManager::Instance()->itemChanged(iType, strID);
}



void ClsFEDialogManager::slotItemDeleted(int /* iType */, string strID ) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::slotItemDeleted(int iType, string strID )" << endl;
#endif
    if(qmapAutoDialogs.find(strID) != qmapAutoDialogs.end()){
	cout << "Dialog for " << strID << " will be closed" << endl;
	qmapAutoDialogs.find(strID).data()->close();
	/* why do I not remove the dialog from the map??? */
    } 
}



void ClsFEDialogManager::slotDialogClosed(string strID) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::slotDialogClosed(string)" << endl;
#endif

    if(qmapAutoDialogs.find(strID) != qmapAutoDialogs.end()){
//--	ClsDialog* d = qmapAutoDialogs.find(strID).data();
	qmapAutoDialogs.remove(qmapAutoDialogs.find(strID));
    }
}


void ClsFEDialogManager::closeAllDialogs(){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::closeAllDialogs()" << endl;
#endif
    QMap<string, ClsDialog*>::Iterator it;
    for(it = qmapAutoDialogs.begin(); it != qmapAutoDialogs.end(); it++){
//	cout << "closing: " << it.key() << endl;
	if(it.data()!=NULL){
	    it.data()->hide();
	    delete it.data();
	    /* this crashes the application: it.data()->close(); */
	}
	qmapAutoDialogs.remove(it);
    } 
}

//	cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


