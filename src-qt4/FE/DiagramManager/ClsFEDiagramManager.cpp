#include <QMessageBox> 

#include "ClsFEDiagramManager.h"
#include "ClsQGroupStateManip.h"

#include "diagramTypes.h"

#include <ClsFESystemManager.h>


//#define DEBUG_CLSFEDIAGRAMMANAGER


ClsFEDiagramManager*  ClsFEDiagramManager::_instanceDiagramManager = NULL;

void ClsFEDiagramManager::initializeDiagramManager(QWidget* _toplevel){
    _instanceDiagramManager = new ClsFEDiagramManager(_toplevel);
}


ClsFEDiagramManager* ClsFEDiagramManager::Instance(){
    return _instanceDiagramManager;
}



ClsFEDiagramManager::ClsFEDiagramManager(QWidget* _toplevel) : toplevel(_toplevel) {

};

/**
 * Slot that will show a certain diagram
 *
 * @param iClientType
 * @param strID
 */
void ClsFEDiagramManager::DiagramShow(int iClientType , string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::DiagramShow(int iClientType , string strID)" << endl;
#endif
    if(iClientType== diagramTypes::DIAGRAM_BLOCK){
	ClsBlockDiagram::Instance()->showProcessTab(strID);
    }

};

 

void ClsFEDiagramManager::slotBlockDiagramShow(string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotBlockDiagramShow(string strID)" << endl;
#endif
    ClsBlockDiagram::Instance()->showProcessTab(strID);

};


void ClsFEDiagramManager::slotSystemChanged() {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotSystemChanged()" << endl;
#endif

    ClsBlockDiagram::Instance()->slotSystemChanged();
}


void ClsFEDiagramManager::closeSystem() {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::closeSystem()" << endl;
#endif
    ClsBlockDiagram::Instance()->cleanup();
}


void ClsFEDiagramManager::slotItemChanged(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemChanged(int iType, string strID )::strID: " << strID << endl;
    cout << "iType: " << iType << endl;
#endif
    ClsBlockDiagram::Instance()->slotItemChanged(iType, strID);

    if(iType == ClsFESystemManager::ITEM_GROUP){
	QMap<QString, QWidget*>::iterator it;
	for(it=qmapDiagrams.begin(); it!=qmapDiagrams.end();it++){
	    if(dynamic_cast<ClsQGroupStateManip*>(it.data())) {
		(dynamic_cast<ClsQGroupStateManip*>(it.data()))->slotGroupChanged();
	    }
// now in DataManager 	    else if(dynamic_cast<ClsFEConnectionDiagram*>(it.data())) {
// now in DataManager 		    (dynamic_cast<ClsFEConnectionDiagram*>(it.data()))->groupChanged(strID);
// now in DataManager 	    }
	}
    } else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	QMap<QString, QWidget*>::iterator it;
	for(it=qmapDiagrams.begin(); it!=qmapDiagrams.end();it++){
// now in DataManager 	    if(dynamic_cast<ClsFEConnectionDiagram*>(it.data())) {
// now in DataManager 		(dynamic_cast<ClsFEConnectionDiagram*>(it.data()))->connectionChanged(strID);
// now in DataManager 	    }
	}
    } 
//    connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(iType, strID)), clsQGroupStateManip, SLOT(slotItemChanged(int,string)));

}

void ClsFEDiagramManager::slotItemDeleted(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemDeleted(int iType, string strID )" << endl;
#endif


	ClsBlockDiagram::Instance()->slotItemDeleted(iType, strID);

    if(iType == ClsFESystemManager::ITEM_GROUP){
	if(qmapDiagrams.find(QString(strID.c_str())) != qmapDiagrams.end()){
	    qmapDiagrams.find(QString(strID.c_str())).data()->close();
	} 
	
	/* find the connection diagrams that use the group */
	QMap<QString, QWidget*>::iterator it;
	for(it=qmapDiagrams.begin(); it!=qmapDiagrams.end();it++){

// now in DataManager	    if(dynamic_cast<ClsFEConnectionDiagram*>(it.data())) {
// now in DataManager		string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(it.key().latin1());
// now in DataManager		string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(it.key().latin1());
// now in DataManager		if(!strID.compare(strSourceID) || !strID.compare(strTargetID)){
// now in DataManager		    it.data()->close();
// now in DataManager		}
// now in DataManager	    }
	}

    } else if(iType == ClsFESystemManager::ITEM_CONNECTION){
	if(qmapDiagrams.find(QString(strID.c_str())) != qmapDiagrams.end()){
	    qmapDiagrams.find(QString(strID.c_str())).data()->close();
	} 
    }




}

void ClsFEDiagramManager::slotItemUnDeleted(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemUnDeleted(int iType, string strID )" << endl;
#endif
    ClsBlockDiagram::Instance()->slotItemUnDeleted(iType, strID);

}



/**
 * Create a new diagram of type iClientType with no IDs used
 *
 * @param iClientType
 */
void ClsFEDiagramManager::DiagramCreate(int iClientType){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::DiagramCreate(int iClientType)" << endl;
#endif

    if( ClsBlockDiagram::Instance()!=NULL){
	delete ClsBlockDiagram::Instance();
    }

    ClsBlockDiagram::initializeBlockDiagram(toplevel);
    ClsBlockDiagram::Instance()->show();
    
    connect(ClsBlockDiagram::Instance(), SIGNAL(sigDiagItemActivated(int,string)), this, SIGNAL(sigDiagItemActivated(int, string)));
    connect(ClsBlockDiagram::Instance(), SIGNAL(sigDiagViewActivated(int,string)), this, SIGNAL(sigDiagViewActivated(int, string)));
    
};


void ClsFEDiagramManager::DiagramCreate(int iClientType , string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::DiagramCreate(int iClientType , string strID)" << endl;
#endif

    if(iClientType== diagramTypes::DIAGRAM_CONNECTION){
	if(qmapDiagrams.find(QString(strID.c_str())) != qmapDiagrams.end()){
//	    cout << "Group manip for " << strID << " already opened" << endl;
	    qmapDiagrams.find(QString(strID.c_str())).data()->raise();
	} else {
	    string strSourceID = ClsFESystemManager::Instance()->getConnectionSourceID(strID);
	    string strTargetID = ClsFESystemManager::Instance()->getConnectionTargetID(strID);
	    
	    if(strSourceID.length()<=0 && strTargetID.length()<=0){
		QMessageBox::information( 0, "IQR", "Cannot create diagram.\n"
					  "Source and Target Groups do not exist." );
	    }
	    else if(strSourceID.length()<=0){
		QMessageBox::information( 0, "IQR", "Cannot create diagram.\n"
					  "Source Group does not exist." );
	    } else if(strTargetID.length()<=0){
		QMessageBox::information( 0, "IQR", "Cannot create diagram.\n"
					  "Target Group does not exist." );
	    } 
	}
    } else if(iClientType== diagramTypes::DIAGRAM_GROUP_MANIP){
	if(qmapDiagrams.find(QString(strID.c_str())) != qmapDiagrams.end()){
//	    cout << "Group manip for " << strID << " already opened" << endl;
	    qmapDiagrams.find(QString(strID.c_str())).data()->raise();
	} else {
	    ClsQGroupStateManip* clsQGroupStateManip = new ClsQGroupStateManip("", strID);
	    clsQGroupStateManip->show();
	    QString qstr(strID.c_str());
	    qmapDiagrams[qstr] = (QWidget*)clsQGroupStateManip;
	    connect(clsQGroupStateManip, SIGNAL(sigDiagramClosed(string)), this, SLOT( slotDiagramClosed(string)));
	}
    }
};

/**
 * unsused for now. Will be needed to save the arrangement of diagrams
 *
 */
void ClsFEDiagramManager::saveConfig(){};

/**
 * unsused for now. Will be needed to load an arrangement of diagrams
 *
 */
void ClsFEDiagramManager::applyConfig(){};

/**
 * Function used for serialization: passes back the information about
 * process and group icons in the block diagram
 *
 * @param strID
 *
 * @return
 */
ClsInfoDiagramIcon ClsFEDiagramManager::getDiagramIcon(string strID) {
    return ClsBlockDiagram::Instance()->getDiagramIcon( strID);
}


/**
 * Function used for serialization: passes back the information about
 * connection point in the block diagram
 *
 * @param strID
 *
 * @return
 */
ClsInfoDiagramLine ClsFEDiagramManager::getDiagramLine(string strID) {
    return ClsBlockDiagram::Instance()->getDiagramLine(strID);
};




void ClsFEDiagramManager::printBlockDiagram(){
	    ClsBlockDiagram::Instance()->print();
};


void ClsFEDiagramManager::saveBlockDiagram(){
	    ClsBlockDiagram::Instance()->save();

};


void ClsFEDiagramManager::slotItemAdded(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemAdded(int iType, string strID )" << endl;
#endif
    ClsBlockDiagram::Instance()->slotItemAdded(iType, strID); //###
}

void ClsFEDiagramManager::slotItemDuplicated(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemDuplicated(int iType, string strID )" << endl;
#endif
    ClsBlockDiagram::Instance()->slotItemDuplicated(iType, strID); //###
}

void ClsFEDiagramManager::slotDiagramClosed(string strID) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotDiagramClosed(string)" << endl;
#endif

    if(qmapDiagrams.find(strID.c_str()) != qmapDiagrams.end()){
	qmapDiagrams.remove(qmapDiagrams.find(strID.c_str()));
    }
}


void ClsFEDiagramManager::slotSimulationRunning(bool b) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotSimulationRunning(bool)" << endl;
#endif

    ClsBlockDiagram::Instance()->disableToolbarButtons(b);

}



void ClsFEDiagramManager::markItem(int iType, string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::markItem(int iType, string strID)" << endl;
#endif
    ClsBlockDiagram::Instance()->markItem(iType, strID);
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
