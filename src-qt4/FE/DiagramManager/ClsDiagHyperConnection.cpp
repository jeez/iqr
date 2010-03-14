#include "ClsDiagHyperConnection.h"

#include <string>

#include <QGraphicsScene>

#include <ClsFESystemManager.h>
#include "ClsBlockDiagram.h"

#include "ClsInfoDiagramLine.h"

#include <ClsFEConnection.h>


#include "ClsDiagBaseConnection.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagItemPhantom.h"


//#define DEBUG_CLSDIAGHYPERCONNECTION


ClsDiagHyperConnection::ClsDiagHyperConnection( string _strID, ClsInfoDiagramLine clsInfoDiagramLine) : strID(_strID) {
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::ClsDiagHyperConnection( string _strID, ClsInfoDiagramLine clsInfoDiagramLine) : strID(_strID)" << endl;
#endif
    clsDiagConnectionPrcPrc = NULL;
    clsDiagConnectionGrpGrp = NULL;
    clsDiagPhantomConnectionStart = NULL;
    clsDiagPhantomConnectionEnd = NULL;

    strSourceGroupID = ClsFESystemManager::Instance()->getConnectionSourceID(strID );
    strTargetGroupID = ClsFESystemManager::Instance()->getConnectionTargetID(strID );
    strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceGroupID);
    strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetGroupID);
    iConnectionType= ClsFESystemManager::Instance()->getConnectionTypeAsInt(strID);

    if(!strSourceProcessID.compare(strTargetProcessID)){
	bInterprocessConnection = false;
    } else {
	bInterprocessConnection = true;
    }

    if(!bInterprocessConnection){
	createConnectionGrpGrp(strSourceGroupID, strTargetGroupID, clsInfoDiagramLine);
    } else {
	createConnectionPrcPrc(strSourceProcessID, strTargetProcessID);
	createPhantomConnection(ClsDiagBaseConnection::PHANTOM_START, strSourceProcessID, strTargetProcessID, strSourceGroupID, strTargetGroupID, clsInfoDiagramLine);
	createPhantomConnection(ClsDiagBaseConnection::PHANTOM_END, strSourceProcessID, strTargetProcessID, strSourceGroupID, strTargetGroupID, clsInfoDiagramLine);
    }
};

ClsDiagHyperConnection::~ClsDiagHyperConnection() {
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::~ClsDiagHyperConnection()" << endl;
#endif
    
    if(clsDiagConnectionPrcPrc!=NULL){
	delete clsDiagConnectionPrcPrc;	      
    }					      
    if(clsDiagConnectionGrpGrp!=NULL){
	delete clsDiagConnectionGrpGrp;	      
    }					      
    if(clsDiagPhantomConnectionStart!=NULL){
	delete clsDiagPhantomConnectionStart; 
    }					      
    if(clsDiagPhantomConnectionEnd!=NULL){   
	delete clsDiagPhantomConnectionEnd;   
    }                                       
}


bool ClsDiagHyperConnection::checkIfIP(string _strSourceGroupID, string _strTargetGroupID){
    string _strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(_strSourceGroupID);
    string _strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(_strTargetGroupID);

    if(!_strSourceProcessID.compare(_strTargetProcessID)){
	return false;
    } else {
	return true;
    }

    return true;
}


void ClsDiagHyperConnection::setSourceGroupID(string _strSourceGroupID, int iAP/* = -1*/){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setSourceGroupID(string _strSourceGroupID)" << endl;
#endif
    if(strSourceGroupID.compare(_strSourceGroupID)){
	strSourceGroupID = _strSourceGroupID;

	ClsFESystemManager::Instance()->getFEConnection(getID())->setConnectionSourceID(_strSourceGroupID);

	if((strTargetGroupID.length() * strSourceGroupID.length()) > 0) {
	    bool bNewInterprocessConnection = checkIfIP(_strSourceGroupID, strTargetGroupID);
	    if(bInterprocessConnection && bNewInterprocessConnection){
//		cout << "was ip, is ip" << endl;
		ClsInfoDiagramLine clsInfoDiagramLine;
		clsInfoDiagramLine.setAPTarget(clsDiagPhantomConnectionEnd->getEndHandleAPNumber());
		delete clsDiagPhantomConnectionEnd;
		clsDiagPhantomConnectionEnd = NULL;
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_END, strSourceProcessID, strTargetProcessID, strSourceGroupID, strTargetGroupID, clsInfoDiagramLine);
	    } else if(bInterprocessConnection && !bNewInterprocessConnection){
//		cout << "was ip, is local" << endl; 
		strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceGroupID);
		strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetGroupID);
		ClsInfoDiagramLine clsInfoDiagramLine; 
		clsInfoDiagramLine.setAPSource(iAP);
		clsInfoDiagramLine.setAPTarget(clsDiagPhantomConnectionEnd->getEndHandleAPNumber());
		delete clsDiagPhantomConnectionStart; 
		delete clsDiagPhantomConnectionEnd; 
		delete clsDiagConnectionPrcPrc; 
		clsDiagPhantomConnectionStart = NULL; 
		clsDiagPhantomConnectionEnd = NULL; 
		clsDiagConnectionPrcPrc = NULL; 
		createConnectionGrpGrp(strSourceGroupID, strTargetGroupID, clsInfoDiagramLine); 
		ClsBlockDiagram::Instance()->prunePhantomGroups();
	    } else if(!bInterprocessConnection && bNewInterprocessConnection){
//		cout << "was local, is ip" << endl;
		strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceGroupID);
		strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetGroupID);
		ClsInfoDiagramLine clsInfoDiagramLineSource; 
		ClsInfoDiagramLine clsInfoDiagramLineTarget; 
		clsInfoDiagramLineTarget.setAPSource(iAP); 
		clsInfoDiagramLineTarget.setAPTarget(clsDiagConnectionGrpGrp->getEndHandleAPNumber());
		delete clsDiagConnectionGrpGrp;
		clsDiagConnectionGrpGrp = NULL;
		createConnectionPrcPrc(strSourceProcessID, strTargetProcessID); 
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_START, strSourceProcessID, strTargetProcessID,  
					strSourceGroupID, strTargetGroupID, clsInfoDiagramLineSource);
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_END, strSourceProcessID, strTargetProcessID, 
					strSourceGroupID, strTargetGroupID, clsInfoDiagramLineTarget);
	    }
	    bInterprocessConnection = bNewInterprocessConnection;
	    setConnected(true);
	}
	else {
	    setConnected(false);
	}
    }
}
void ClsDiagHyperConnection::setTargetGroupID(string _strTargetGroupID, int iAP){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setTargetGroupID(string _strTargetGroupID):" << _strTargetGroupID << endl;
#endif
    if(strTargetGroupID.compare(_strTargetGroupID)){
	strTargetGroupID = _strTargetGroupID;

	ClsFESystemManager::Instance()->getFEConnection(getID())->setConnectionTargetID(_strTargetGroupID);


	if((strSourceGroupID.length() * strTargetGroupID.length()) > 0) {
	    bool bNewInterprocessConnection = checkIfIP(strSourceGroupID, _strTargetGroupID);


	    if(bInterprocessConnection && bNewInterprocessConnection){
//		cout << "was ip, is ip" << endl;
		ClsInfoDiagramLine clsInfoDiagramLine;
		clsInfoDiagramLine.setAPSource(clsDiagPhantomConnectionStart->getStartHandleAPNumber());
		delete clsDiagPhantomConnectionStart;
		clsDiagPhantomConnectionStart = NULL;
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_START, strSourceProcessID, strTargetProcessID, strSourceGroupID, strTargetGroupID, clsInfoDiagramLine);
	    } else if(bInterprocessConnection && !bNewInterprocessConnection){
//		cout << "was ip, is local" << endl; 
		strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceGroupID);
		strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetGroupID);
		ClsInfoDiagramLine clsInfoDiagramLine; 
		clsInfoDiagramLine.setAPSource(clsDiagPhantomConnectionStart->getStartHandleAPNumber());
		clsInfoDiagramLine.setAPTarget(iAP);
		delete clsDiagPhantomConnectionStart; 
		delete clsDiagPhantomConnectionEnd; 
		delete clsDiagConnectionPrcPrc; 
		clsDiagPhantomConnectionStart = NULL; 
		clsDiagPhantomConnectionEnd = NULL; 
		clsDiagConnectionPrcPrc = NULL; 

		createConnectionGrpGrp(strSourceGroupID, strTargetGroupID, clsInfoDiagramLine); 
		ClsBlockDiagram::Instance()->prunePhantomGroups();
	    }  else if(!bInterprocessConnection && bNewInterprocessConnection){
//		cout << "was local, is ip" << endl;
		strSourceProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strSourceGroupID);
		strTargetProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strTargetGroupID);
		ClsInfoDiagramLine clsInfoDiagramLineSource; 
		ClsInfoDiagramLine clsInfoDiagramLineTarget; 
		clsInfoDiagramLineSource.setAPSource(clsDiagConnectionGrpGrp->getStartHandleAPNumber());
		clsInfoDiagramLineSource.setAPTarget(iAP); //OK

		delete clsDiagConnectionGrpGrp;
		clsDiagConnectionGrpGrp = NULL;
		createConnectionPrcPrc(strSourceProcessID, strTargetProcessID); 
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_START, strSourceProcessID, strTargetProcessID,
					strSourceGroupID, strTargetGroupID, clsInfoDiagramLineSource);
		createPhantomConnection(ClsDiagBaseConnection::PHANTOM_END, strSourceProcessID, strTargetProcessID,
					strSourceGroupID, strTargetGroupID, clsInfoDiagramLineTarget);
	    }
	    bInterprocessConnection = bNewInterprocessConnection;
	    setConnected(true);
	}
	else {
	    setConnected(false);
	}


    }

}
    
void ClsDiagHyperConnection::setSourceProcessID(string _strSourceProcessID){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setSourceProcessID(string _strSourceProcessID)" << endl;
#endif
    strSourceProcessID = _strSourceProcessID;
}
void ClsDiagHyperConnection::setTargetProcessID(string _strTargetProcessID){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setTargetProcessID(string _strTargetProcessID)" << endl;
#endif
    strTargetProcessID = _strTargetProcessID;
}
void ClsDiagHyperConnection::createConnectionGrpGrp( string strSourceGroupID, 
						     string strTargetGroupID,
						     ClsInfoDiagramLine clsInfoDiagramLine){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::createConnectionGrpGrp(...)" << endl;
    cout << "strSourceGroupID: " << strSourceGroupID << endl;
    cout << "strTargetGroupID: " << strTargetGroupID << endl;
#endif

    ClsScene* clsScene = ClsBlockDiagram::Instance()->getCanvas(strSourceProcessID.c_str());
    clsDiagConnectionGrpGrp = new ClsDiagConnection(this, iConnectionType, ClsDiagBaseConnection::LOCAL);
    clsScene->addItem(clsDiagConnectionGrpGrp);
    

    ClsDiagItem* clsDiagItemGroupStart = ClsBlockDiagram::Instance()->getCanvasNode( strSourceGroupID);
    ClsDiagItem* clsDiagItemGroupEnd = ClsBlockDiagram::Instance()->getCanvasNode( strTargetGroupID);

    
    ClsDiagItemAP* clsDiagItemAPSource = NULL;
    ClsDiagItemAP* clsDiagItemAPTarget = NULL;

    vector<vector<int> > vectorPoints = clsInfoDiagramLine.getPoints();
    
    QPointF qpStart, qpEnd;
    if(vectorPoints.size() > 0){
	qpEnd.setX(vectorPoints[0][0]);
	qpEnd.setY(vectorPoints[0][1]);
	
	qpStart.setX(vectorPoints.back()[0]);
	qpStart.setY(vectorPoints.back()[1]);
    }
    else { /* vector size = 0 */
	qpStart = clsDiagItemGroupStart->getCenter();
	qpEnd = clsDiagItemGroupEnd->getCenter();
    }
    
    
    int iAPSource = clsInfoDiagramLine.getAPSource();
    int iAPTarget = clsInfoDiagramLine.getAPTarget();
    
    if(iAPSource<0){
	clsDiagItemAPSource = clsDiagItemGroupStart->getNearestAP(qpEnd);
    } else {
	clsDiagItemAPSource = clsDiagItemGroupStart->getAPbyID(iAPSource);
    }
    
    if(iAPTarget<0){
	clsDiagItemAPTarget = clsDiagItemGroupEnd->getNearestAP(qpStart);
    } else {
	clsDiagItemAPTarget = clsDiagItemGroupEnd->getAPbyID(iAPTarget);
    }
    
    
    QPointF qpSourceAP, qpTargetAP;
    if(vectorPoints.size() > 0){
	QPointF qpLinePointFirst, qpLinePointLast;

	qpLinePointFirst.setX(vectorPoints[0][0]);
	qpLinePointFirst.setY(vectorPoints[0][1]);

	qpLinePointLast.setX(vectorPoints.back()[0]);
	qpLinePointLast.setY(vectorPoints.back()[1]);

/* do the first segments */
	qpSourceAP = clsDiagItemAPSource->getCenter();
	clsDiagConnectionGrpGrp->addSegment(qpSourceAP, qpLinePointFirst, ClsDiagConnection::SEG_START, 0);
/* ---------------------- */

/* do the middle segments (this is skipped, if we only have one point in the vector) */
	QPointF qp0, qp1;
	qp0 = qp1 = qpLinePointFirst; /* necessary in case we only have 1 point! */

	for(unsigned int ii=1; ii<vectorPoints.size();ii++){
	    qp0.setX(vectorPoints[ii][0]);
	    qp0.setY(vectorPoints[ii][1]);
	    clsDiagConnectionGrpGrp->addSegment(qp1, qp0, ClsDiagConnection::SEG_MIDDLE, ClsDiagConnection::UNKNOWN);
	    qp1 = qp0;
	}
/* ---------------------- */

	qpLinePointLast = qp0;

/* do the end segments */
	qpTargetAP = clsDiagItemAPTarget->getCenter();
	int iOrientation = clsDiagItemAPTarget->getOrientation();
	clsDiagConnectionGrpGrp->addSegment(qpLinePointLast, qpTargetAP, ClsDiagConnection::SEG_END, iOrientation);
    }
    else { /* vector size = 0 */
	qpSourceAP = clsDiagItemAPSource->getCenter();
	qpTargetAP = clsDiagItemAPTarget->getCenter();
	int iOrientation = clsDiagItemAPTarget->getOrientation();
	clsDiagConnectionGrpGrp->addSegment(qpSourceAP, qpTargetAP, ClsDiagConnection::SEG_SINGLE, iOrientation);
    } /* end check on vector.size() */


    ClsDiagConnectionHandleStart* clsDiagConnectionHandleStart = clsDiagConnectionGrpGrp->getStartHandle();
    ClsDiagConnectionHandleEnd* clsDiagConnectionHandleEnd = clsDiagConnectionGrpGrp->getEndHandle();
    
    if(clsDiagConnectionHandleStart!=NULL){
	clsDiagConnectionHandleStart->setParentItem(clsDiagItemAPSource);
    }
    if(clsDiagConnectionHandleEnd!=NULL){
	clsDiagConnectionHandleEnd->setParentItem(clsDiagItemAPTarget);
    }
}


void ClsDiagHyperConnection::createConnectionPrcPrc(string strSourceProcessID, string strTargetProcessID){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::createConnectionPrcPrc(..." << endl;
#endif
    clsDiagConnectionPrcPrc = new ClsDiagConnection(this, iConnectionType, ClsDiagBaseConnection::IP);
    ClsScene* clsSceneSystem  = ClsBlockDiagram::Instance()->getSystemCanvas();
    clsSceneSystem->addItem(clsDiagConnectionPrcPrc);
    ClsDiagItem* clsDiagItemProcessStart = ClsBlockDiagram::Instance()->getCanvasNode( strSourceProcessID);
    ClsDiagItem* clsDiagItemProcessEnd = ClsBlockDiagram::Instance()->getCanvasNode( strTargetProcessID);

    QPointF qpStart = clsDiagItemProcessStart->getCenter();
    QPointF qpEnd = clsDiagItemProcessEnd->getCenter();

    ClsDiagItemAP* clsDiagItemAPSource = clsDiagItemProcessStart->getNearestAP(qpEnd);
    ClsDiagItemAP* clsDiagItemAPTarget = clsDiagItemProcessEnd->getNearestAP(qpStart);

    QPointF qpSourceAP = clsDiagItemAPSource->getCenter();
    QPointF qpTargetAP = clsDiagItemAPTarget->getCenter();
    int iOrientation = clsDiagItemAPTarget->getOrientation();
    clsDiagConnectionPrcPrc->addSegment(qpSourceAP, qpTargetAP, ClsDiagConnection::SEG_SINGLE, iOrientation);
    
    
    ClsDiagConnectionHandleStart* clsDiagConnectionHandleStart = clsDiagConnectionPrcPrc->getStartHandle();
    ClsDiagConnectionHandleEnd* clsDiagConnectionHandleEnd = clsDiagConnectionPrcPrc->getEndHandle();
    if(clsDiagConnectionHandleStart!=NULL){
	clsDiagConnectionHandleStart->setParentItem(clsDiagItemAPSource);
    }

    if(clsDiagConnectionHandleEnd!=NULL){
	clsDiagConnectionHandleEnd->setParentItem(clsDiagItemAPTarget);
    }

}





void ClsDiagHyperConnection::createPhantomConnection(int iCanvasConnectionType, string strSourceProcessID, string strTargetProcessID, 
						     string strSourceGroupID, string strTargetGroupID, ClsInfoDiagramLine clsInfoDiagramLine){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::createPhantomConnection(...)" << endl;
#endif
    ClsDiagItem* clsDiagItemReal;
    ClsDiagItemPhantom *clsDiagItemPhantom;    
    
    if(iCanvasConnectionType == ClsDiagBaseConnection::PHANTOM_START){
	QGraphicsScene* clsSceneTemp = ClsBlockDiagram::Instance()->getCanvas(strSourceProcessID.c_str());
	clsDiagItemReal = ClsBlockDiagram::Instance()->getCanvasNode( strSourceGroupID);
	clsDiagItemPhantom =  ClsBlockDiagram::Instance()->getDiagItemPhantom(clsSceneTemp, strTargetGroupID, clsDiagItemReal);
	clsDiagPhantomConnectionStart = new ClsDiagPhantomConnection(this, iConnectionType, iCanvasConnectionType);
	clsSceneTemp->addItem(clsDiagPhantomConnectionStart);

	int iAPSource = clsInfoDiagramLine.getAPSource();
	int iAPTarget = clsInfoDiagramLine.getAPTarget();

	ClsDiagItemAP *clsDiagItemAPSource = clsDiagItemReal->getAPbyID(iAPSource);

	ClsDiagItemAP *clsDiagItemAPTarget = NULL;
	if(iAPTarget>=0){
	    clsDiagItemAPTarget = clsDiagItemPhantom->getAPbyID(iAPTarget);
	} else {
	    clsDiagItemAPTarget = clsDiagItemPhantom->getNearestAP(clsDiagItemReal->getCenter());
	}

	drawConnectionPhantom(clsDiagPhantomConnectionStart, clsDiagItemAPSource, clsDiagItemAPTarget);
    } else {
	QGraphicsScene* clsSceneTemp = ClsBlockDiagram::Instance()->getCanvas(strTargetProcessID.c_str());
	clsDiagItemReal = ClsBlockDiagram::Instance()->getCanvasNode( strTargetGroupID);
	clsDiagItemPhantom = ClsBlockDiagram::Instance()->getDiagItemPhantom(clsSceneTemp, strSourceGroupID, clsDiagItemReal);
	clsDiagPhantomConnectionEnd = new ClsDiagPhantomConnection(this, iConnectionType, iCanvasConnectionType);
	clsSceneTemp->addItem(clsDiagPhantomConnectionEnd);

	int iAPSource = clsInfoDiagramLine.getAPSource();
	int iAPTarget = clsInfoDiagramLine.getAPTarget();

	ClsDiagItemAP *clsDiagItemAPSource = NULL;
	if(iAPSource>=0){
	    clsDiagItemAPSource = clsDiagItemPhantom->getAPbyID(iAPSource);
	} else {
	    clsDiagItemAPSource = clsDiagItemPhantom->getNearestAP(clsDiagItemReal->getCenter());
	}

	ClsDiagItemAP *clsDiagItemAPTarget = clsDiagItemReal->getAPbyID(iAPTarget);
	drawConnectionPhantom(clsDiagPhantomConnectionEnd, clsDiagItemAPSource, clsDiagItemAPTarget);
    }
}

void ClsDiagHyperConnection::drawConnectionPhantom(ClsDiagPhantomConnection *clsDiagConnection, ClsDiagItemAP* clsDiagItemAPSource, ClsDiagItemAP* clsDiagItemAPTarget){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::drawConnectionPhantom(ClsDiagPhantomConnection *clsDiagConnection, ClsDiagItemAP* clsDiagItemAPSource, ClsDiagItemAP* clsDiagItemAPTarget)" << endl;
#endif

    if( clsDiagItemAPSource!=NULL && clsDiagItemAPTarget!=NULL){
	QPointF qpSourceAP = clsDiagItemAPSource->getCenter();
	QPointF qpTargetAP = clsDiagItemAPTarget->getCenter();

	int iOrientation = clsDiagItemAPTarget->getOrientation();
	clsDiagConnection->addSegment(qpSourceAP, qpTargetAP, ClsDiagConnection::SEG_SINGLE, iOrientation);
	
/* tell the AP of the node that it is connected */

	ClsDiagConnectionHandleStart *clsDiagConnectionHandleStart = clsDiagConnection->getStartHandle();
	ClsDiagConnectionHandleEnd *clsDiagConnectionHandleEnd = clsDiagConnection->getEndHandle();
	
	if(clsDiagConnectionHandleStart!=NULL){
	    clsDiagConnectionHandleStart->setParentItem(clsDiagItemAPSource);
	}
	if(clsDiagConnectionHandleEnd!=NULL){
	    clsDiagConnectionHandleEnd->setParentItem(clsDiagItemAPTarget);
	}
    }
}


void ClsDiagHyperConnection::setConnectionType(int i){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setConnectionType(int i)" << endl;
#endif
    iConnectionType = i; 
    if(clsDiagConnectionPrcPrc!=NULL){
	clsDiagConnectionPrcPrc->setConnectionType(iConnectionType);	      
    }					      
    if(clsDiagConnectionGrpGrp!=NULL){
	clsDiagConnectionGrpGrp->setConnectionType(iConnectionType);	      
    }					      
    if(clsDiagPhantomConnectionStart!=NULL){
	clsDiagPhantomConnectionStart->setConnectionType(iConnectionType); 
    }					      
    if(clsDiagPhantomConnectionEnd!=NULL){   
	clsDiagPhantomConnectionEnd->setConnectionType(iConnectionType);   
    }                                       
}


void ClsDiagHyperConnection::setConnected(bool b){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::setConnected(bool b)" << endl;
#endif

    if(clsDiagConnectionPrcPrc!=NULL){
	clsDiagConnectionPrcPrc->setConnected(b);	      
    }					      
    if(clsDiagConnectionGrpGrp!=NULL){
	clsDiagConnectionGrpGrp->setConnected(b);	      
    }					      
    if(clsDiagPhantomConnectionStart!=NULL){
	clsDiagPhantomConnectionStart->setConnected(b); 
    }					      
    if(clsDiagPhantomConnectionEnd!=NULL){   
	clsDiagPhantomConnectionEnd->setConnected(b);   
    }                                       

};







void ClsDiagHyperConnection::mark(bool b){
#ifdef DEBUG_CLSDIAGHYPERCONNECTION
    cout << "ClsDiagHyperConnection::mark(bool b)" << endl;
#endif
    
    if(clsDiagConnectionPrcPrc!=NULL){
	clsDiagConnectionPrcPrc->mark(b);	      
    }					      
    if(clsDiagConnectionGrpGrp!=NULL){
	clsDiagConnectionGrpGrp->mark(b);	      
    }					      
    if(clsDiagPhantomConnectionStart!=NULL){
	clsDiagPhantomConnectionStart->mark(b); 
    }					      
    if(clsDiagPhantomConnectionEnd!=NULL){   
	clsDiagPhantomConnectionEnd->mark(b);   
    }   

};


int ClsDiagHyperConnection::getStartAPNumber(){
    if(bInterprocessConnection){
	if(clsDiagPhantomConnectionStart!=NULL){
	    return clsDiagPhantomConnectionStart->getStartHandleAPNumber();
	}
    } else {
	if(clsDiagConnectionGrpGrp!=NULL){
	    return clsDiagConnectionGrpGrp->getStartHandleAPNumber();
	}
    }
    return -1;    
}

int ClsDiagHyperConnection::getEndAPNumber(){
    if(bInterprocessConnection){
	if(clsDiagPhantomConnectionEnd!=NULL){
	    return clsDiagPhantomConnectionEnd->getEndHandleAPNumber();
	}
    } else {
	if(clsDiagConnectionGrpGrp!=NULL){
	    return clsDiagConnectionGrpGrp->getEndHandleAPNumber();
	}
    }
    return -1;
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
