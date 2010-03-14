#include <qmessagebox.h> 

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
    clsBlockDiagram = NULL;
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
	if(clsBlockDiagram!=NULL){
//	    int ii;
//	    memcpy(&ii, strID.c_str(), sizeof(int));
//	    clsBlockDiagram->slotTabChanged(ii);
	    clsBlockDiagram->showProcessTab(strID);
	}
    }

};

void ClsFEDiagramManager::slotBlockDiagramShow(string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotBlockDiagramShow(string strID)" << endl;
#endif
    if(clsBlockDiagram!=NULL){
	clsBlockDiagram->showProcessTab(strID);
    }

};


void ClsFEDiagramManager::slotSystemChanged() {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotSystemChanged()" << endl;
#endif

    if(clsBlockDiagram!=NULL){

	clsBlockDiagram->slotSystemChanged();
    } else {
//	cout << "clsBlockDiagram==NULL" << endl;

    }
}


void ClsFEDiagramManager::closeSystem() {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::closeSystem()" << endl;
#endif
    if(clsBlockDiagram!=NULL){
	delete clsBlockDiagram;
	clsBlockDiagram = NULL;
    }
}


void ClsFEDiagramManager::slotItemChanged(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemChanged(int iType, string strID )::strID: " << strID << endl;
    cout << "iType: " << iType << endl;
#endif
    if(clsBlockDiagram!=NULL){
	clsBlockDiagram->slotItemChanged(iType, strID);
    }

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


    if(clsBlockDiagram!=NULL){
	clsBlockDiagram->slotItemDeleted(iType, strID);
    }

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
    if(clsBlockDiagram!=NULL){
	clsBlockDiagram->slotItemUnDeleted(iType, strID);
    }

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
    if(iClientType== diagramTypes::DIAGRAM_BLOCK){
	if(clsBlockDiagram==NULL){
	    clsBlockDiagram = new ClsBlockDiagram(toplevel);
	    clsBlockDiagram->show();

// 	    connect(clsBlockDiagram, SIGNAL(blockdiagramRightClick(int ,string,  QPoint)), clsFEDialogManager, SLOT(createPopup(int , string, QPoint)));
// 	    connect(clsBlockDiagram, SIGNAL(blockdiagramRightClick(int ,list<string>,  QPoint)), clsFEDialogManager, SLOT(createPopup(int , list<string>, QPoint)));


	    connect(clsBlockDiagram, SIGNAL(blockdiagramLeftClick(int ,string,  QPoint)), this, SLOT(slotBlockDiagramLeftClicked(int , string, QPoint)));
	    connect(clsBlockDiagram, SIGNAL(blockdiagramRightClick(int ,string,  QPoint)), this, SLOT(slotBlockDiagramRightClicked(int , string, QPoint)));
	    connect(clsBlockDiagram, SIGNAL(blockdiagramRightClick(int ,list<string>,  QPoint)), this, SLOT(slotBlockDiagramRightClicked(int , list<string>, QPoint)));
	    connect(clsBlockDiagram, SIGNAL(blockdiagramDoubleClick(int ,string,  QPoint)), this, SLOT(slotBlockDiagramDoubleClicked(int , string, QPoint)));
	    connect(clsBlockDiagram, SIGNAL(sigItemActivated(int,string)), this, SLOT(slotItemActivated(int, string)));


//	connect(clsBlockDiagram, SIGNAL(cloneMe(int) ), this, SLOT(DiagramCreate(int) ));
//	    connect(clsFESystemManager, SIGNAL(sigSystemChanged() ), clsBlockDiagram, SLOT(slotSystemChanged() )); //done at toplevel, cos didn't work for suse here!





	}
    } else if(iClientType== diagramTypes::DIAGRAM_CONNECTION){


    }
};


// void ClsFEDiagramManager::DiagramCreate(int iClientType , list<string> listIDs){
//     list<string>::iterator it;
//     it = listIDs.begin();
//     string strID1 = *(it);
//     it++;
//     string strID2 = *(it);
//     cout << "strID1:" << strID1 << endl;
//     cout << "strID2:" << strID2 << endl;



// #ifdef DEBUG_CLSFEDIAGRAMMANAGER
//     cout << "ClsFEDiagramManager::DiagramCreate(int iClientType , list<string> listIDs)" << endl;
// #endif
//     if(iClientType== diagramTypes::DIAGRAM_CONNECTION){

// 	QString qstr("ID");

// 	ClsFEConnectionDiagram *clsFEConnectionDiagram = new ClsFEConnectionDiagram(0, "", strID1);
// 	clsFEConnectionDiagram->setSystemManagerPointer(ClsFESystemManager::Instance());
// 	clsFEConnectionDiagram->createDiagram();
// 	clsFEConnectionDiagram->show();

// 	qmapDiagrams[qstr] = (QWidget*)clsFEConnectionDiagram;
//     }
// };

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
// now in DataManager	    else {
// now in DataManager		ClsFEConnectionDiagram *clsFEConnectionDiagram = new ClsFEConnectionDiagram("", strID);
// now in DataManager		clsFEConnectionDiagram->createDiagram();
// now in DataManager		clsFEConnectionDiagram->show();
// now in DataManager		QString qstr(strID.c_str());
// now in DataManager		qmapDiagrams[qstr] = (QWidget*)clsFEConnectionDiagram;
// now in DataManager		connect(clsFEConnectionDiagram, SIGNAL(sigDiagramClosed(string)), this, SLOT( slotDiagramClosed(string)));
// now in DataManager	    }
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
    return clsBlockDiagram->getDiagramIcon( strID);
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
    return clsBlockDiagram->getDiagramLine(strID);
};


void ClsFEDiagramManager::slotBlockDiagramLeftClicked(int i,string s,  QPoint p) {
    ClsFEDialogManager::Instance()->createInfoTip(i, s, p);
};


void ClsFEDiagramManager::slotBlockDiagramRightClicked(int i,string s,  QPoint p) {
    ClsFEDialogManager::Instance()->createPopup(i, s, p);
};


void ClsFEDiagramManager::slotBlockDiagramRightClicked(int i,list<string> l,  QPoint p) {
    ClsFEDialogManager::Instance()->createPopup(i, l, p);
};

void ClsFEDiagramManager::slotBlockDiagramDoubleClicked(int i,string s,  QPoint p) {
    ClsFEDialogManager::Instance()->createPropertyDialog(i, s, p);
}


void ClsFEDiagramManager::printBlockDiagram(){
	if(clsBlockDiagram!=NULL){
	    clsBlockDiagram->print();
	}

};


void ClsFEDiagramManager::saveBlockDiagram(){
	if(clsBlockDiagram!=NULL){
	    clsBlockDiagram->save();
	}

};

void ClsFEDiagramManager::slotItemActivated(int iType, string strID) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemActivated(int, string)" << endl;
#endif
    emit sigItemActivated(iType, strID);
}

void ClsFEDiagramManager::slotItemAdded(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemAdded(int iType, string strID )" << endl;
#endif
    clsBlockDiagram->slotItemAdded(iType, strID); //###
}

void ClsFEDiagramManager::slotItemDuplicated(int iType, string strID ) {
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::slotItemDuplicated(int iType, string strID )" << endl;
#endif
    clsBlockDiagram->slotItemDuplicated(iType, strID); //###
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

    if(clsBlockDiagram!=NULL){
	clsBlockDiagram->disableToolbarButtons(b);
    }

}



void ClsFEDiagramManager::markItem(int iType, string strID){
#ifdef DEBUG_CLSFEDIAGRAMMANAGER
    cout << "ClsFEDiagramManager::markItem(int iType, string strID)" << endl;
#endif
	if(clsBlockDiagram!=NULL){
 	    clsBlockDiagram->markItem(iType, strID);
	}
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
