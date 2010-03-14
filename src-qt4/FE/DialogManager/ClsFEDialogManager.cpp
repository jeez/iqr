/****************************************************************************
 ** $Filename: ClsFEDialogManager.cpp
 ** $Id: ClsFEDialogManager.cpp,v 1.5 2005/10/29 20:18:17 ulysses Exp ulysses $
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Wed Feb 20 14:50:21 2002
 ** $Date: 2005/10/29 20:18:17 $
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/
#include <QMessageBox>
#include <QMenu>

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
#include "HelpDlgs/dlgAboutIqr.h"
#include "HelpDlgs/dlgReleaseNotes.h"

#include "ClsFESettings.h"
#include "diagramTypes.h"

using namespace iqrcommon;
using namespace iqrfe;


namespace {
#include <copy.xpm>
#include <properties.xpm>

}
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
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createSystemPopup(string strID, QPoint qpLocation)" << endl;
#endif

    QMenu* qmenu = new QMenu(this);
    QAction *qact = new QAction(QPixmap( properties_xpm ), "Properties", this);
    qmenu->addAction(qact);
    if(qmenu->exec(qpLocation ) == qact){
	createSystemPropertyDialog(strID, qpLocation);
    }		
};

void ClsFEDialogManager::createProcessPopup(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createProcessPopup(string strID, QPoint qpLocation)" << endl;
#endif
    QMenu* qmenu = new QMenu( this );
    QAction *qactProperties = new QAction(QPixmap( properties_xpm ),"Properties", this); qmenu->addAction(qactProperties);
    qmenu->addSeparator();
    QAction *qactShowBlockDiagram = new QAction("Show block diagram", this); qmenu->addAction(qactShowBlockDiagram);
    qmenu->addSeparator();
    QAction *qactCopy = new QAction(QPixmap( copy_xpm ),"Copy Process", this); qmenu->addAction(qactCopy);
    QAction *qactExport = new QAction("Export Process", this); qmenu->addAction(qactExport);

    QAction *qactDelete = new QAction("Delete Process", this); 
    if(bSubTypesChangable){
	qmenu->addAction(qactDelete);
    }

    QAction* qact = qmenu->exec( qpLocation );

    if(qact==qactProperties){
	createProcessPropertyDialog(strID, qpLocation);
    } else if(qact==qactDelete){
	createItemDeleteDialog(ClsFESystemManager::ITEM_PROCESS, strID);
    } else if(qact==qactCopy){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_PROCESS , lst);
	}
    } else if(qact==qactExport){
	if(parent!=NULL){
	    parent->exportProcess(strID);
	}
    } else if(qact==qactShowBlockDiagram){
	if(ClsFEDiagramManager::Instance()!=NULL){
	    ClsFEDiagramManager::Instance()->DiagramShow(diagramTypes::DIAGRAM_BLOCK , strID);
	}
    }
};





void ClsFEDialogManager::createGroupPopup(string strID, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "createGroupPopup(string strID, QPoint qpLocation)" << endl;
#endif

    QMenu* qmenu = new QMenu( this );
    
    QAction *qactProperty = new QAction(QPixmap( properties_xpm ),"Properties", this); qmenu->addAction(qactProperty);
    qmenu->addSeparator();
    QAction *qactSpacePlot = new QAction("Space plot", this); qmenu->addAction(qactSpacePlot);
    QAction *qactTimePlot = new QAction("Time Plot", this); qmenu->addAction(qactTimePlot);
    qmenu->addSeparator();
    QAction *qactStateManipulationPanel = new QAction("State Manipulation Panel", this); qmenu->addAction(qactStateManipulationPanel);
    qmenu->addSeparator();
    QAction *qactCopy = new QAction(QPixmap( copy_xpm ),"Copy Group", this); qmenu->addAction(qactCopy);

    QAction *qactDelete = new QAction("Delete Group", this); 
    if(bSubTypesChangable){
	qmenu->addAction(qactDelete);
    }

    QAction* qact = qmenu->exec( qpLocation );
    
    if(qact==qactProperty){
	createGroupPropertyDialog(strID, qpLocation);
    } else if(qact==qactDelete){
	createItemDeleteDialog(ClsFESystemManager::ITEM_GROUP, strID);
    } else if(qact==qactCopy){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , lst);
	}
    } else if(qact==qactSpacePlot){
	if(ClsFEDataManager::Instance()!=NULL){
	    ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_SPACEPLOT , strID, "", "");
	}
    } else if( qact==qactTimePlot){
	if(ClsFEDataManager::Instance()!=NULL){
	    ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_TIMEPLOT , strID, "", "");
	}
    } else if(qact==qactStateManipulationPanel){
	if(ClsFEDiagramManager::Instance()!=NULL){
	    ClsFEDiagramManager::Instance()->DiagramCreate(diagramTypes::DIAGRAM_GROUP_MANIP , strID);
	}
    }
};


void ClsFEDialogManager::createConnectionPopup(string strID, QPoint qpLocation) {
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createConnectionPopup(string strID, QPoint qpLocation)" << endl;
#endif
    QMenu* qmenu = new QMenu( this );

    QAction *qactProperty = new QAction(QPixmap( properties_xpm ), "Properties", this); qmenu->addAction(qactProperty);
    qmenu->addSeparator();
    QAction *qactConnectionPlot = new QAction("Connection plot", this); qmenu->addAction(qactConnectionPlot);
    qmenu->addSeparator();


    QAction *qactCopy = new QAction(QPixmap( copy_xpm ), "Copy Connection", this); 
    if(ClsFESystemManager::Instance()->getConnectionSourceID(strID).size()>0 && 
       ClsFESystemManager::Instance()->getConnectionTargetID(strID).size()>0){
	qmenu->addAction(qactCopy);
    }

    QAction *qactDelete = new QAction("Delete Connection", this);
    if(bSubTypesChangable){
	qmenu->addAction(qactDelete);
    }
    
    QAction* qact = qmenu->exec( qpLocation );

    if(qact==qactProperty){
	createConnectionPropertyDialog(strID, qpLocation);
    } else if(qact==qactDelete){
	createItemDeleteDialog(ClsFESystemManager::ITEM_CONNECTION, strID);
    } else if(qact==qactCopy){
	if(parent!=NULL){
	    list<string> lst;
	    lst.push_back(strID);
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_CONNECTION , lst);
	}
    } else if(qact==qactConnectionPlot){
	if(ClsFEDataManager::Instance()!=NULL){
	    ClsFEDataManager::Instance()->DataClientCreate(diagramTypes::DIAGRAM_CONNECTION, strID, "", "");
	}
    }

};

void ClsFEDialogManager::createMultiGroupPopup(list<string> listIDs, QPoint qpLocation){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createMultiGroupPopup(list<string> listIDs, QPoint qpLocation)" << endl;
#endif

    QMenu* qmenu = new QMenu( this );
    QAction *qactSpacePlot = new QAction("Space plots", this); qmenu->addAction(qactSpacePlot);
    QAction *qactTimePlot = new QAction("Time Plots", this); qmenu->addAction(qactTimePlot);
    qmenu->addSeparator();
    QAction *qactCopy = new QAction(QPixmap( copy_xpm ), "Copy Groups", this); qmenu->addAction(qactCopy);
    QAction *qactCopyGC = new QAction(QPixmap( copy_xpm ),"Copy Groups and Connections", this); qmenu->addAction(qactCopyGC);
    
    QAction *qactDelete = new QAction("Delete Groups", this);
    QAction *qactMultiDelete = new QAction("Delete Groups and Connections", this);
    if(bSubTypesChangable){
	qmenu->addAction(qactDelete);
	qmenu->addAction(qactMultiDelete);
    }

    QAction* qact = qmenu->exec( qpLocation );

    
    if(qact==qactSpacePlot){
	list<string>::iterator it;
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    if(ClsFEDataManager::Instance()!=NULL){
		ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_SPACEPLOT, *it, "", "");
	    }
	}
    } else if(qact==qactTimePlot){
	list<string>::iterator it;
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    if(ClsFEDataManager::Instance()!=NULL){
		ClsFEDataManager::Instance()->DataClientCreate(ClsFEDataClient::CLIENT_TIMEPLOT, *it, "", "");
	    }
	}
    }
    else if(qact==qactCopy){
	if(parent!=NULL){
	    parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , listIDs );
	}
    } else if(qact==qactCopyGC){
	if(parent!=NULL){
	    list<string> lstAffectedConnections = ClsFESystemManager::Instance()->getConnectionsByInvolvedGroups(listIDs);
	    if(lstAffectedConnections.size()>0){
		parent->copyItemsToClipboard(listIDs , lstAffectedConnections );
	    } else {
		parent->copyItemsToClipboard(ClsFESystemManager::ITEM_GROUP , listIDs );
	    }
	}
    } else if(qact==qactDelete){
	list<string>::iterator it;
	for (it = listIDs.begin(); it != listIDs.end(); ++it){
	    createItemDeleteDialog(ClsFESystemManager::ITEM_GROUP, *it);
	}
    } else if(qact==qactMultiDelete){
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
     dlgAboutIqr * aboutIqr = new dlgAboutIqr ( this);
     aboutIqr->show();
}

void ClsFEDialogManager::createReleaseNotesDialog(){
#ifdef DEBUG_CLSFEDIALOGMANAGER
    cout << "ClsFEDialogManager::createReleaseNotesDialog()" << endl;
#endif
     dlgReleaseNotes * releaseNotesIqr = new dlgReleaseNotes ( this);
     releaseNotesIqr->show();
}


string ClsFEDialogManager::createSaveDialog(string strFileTypeDescription, string strExtension){
    QString qstrFileName = "";
    bool bValidFile = false;
    while(!bValidFile){
	qstrFileName = QFileDialog::getSaveFileName(
	    this,
	    "Save File"
	    "Choose a filename to save under",
	    "",
	    strFileTypeDescription.c_str()
	    );


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


//-- this is now handles by qt     if ( QFile::exists(qstrFileName) ) {
//-- this is now handles by qt /*      warning on overwrite */
//-- this is now handles by qt 	int iReturn = QMessageBox::warning( this, "iqr",
//-- this is now handles by qt 					    "A file with the seleteced name exists\n",
//-- this is now handles by qt 					    "Overwrite", "Cancel", 0, 0, 1 );
//-- this is now handles by qt 	if(iReturn == 1){
//-- this is now handles by qt #ifdef DEBUG_CLSFEDIALOGMANAGER
//-- this is now handles by qt 	    cout << "saving aborted" << endl;
//-- this is now handles by qt #endif
//-- this is now handles by qt 	    return "";
//-- this is now handles by qt 	}
//-- this is now handles by qt     }



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


