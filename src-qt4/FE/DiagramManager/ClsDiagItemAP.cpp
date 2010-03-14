#include <sstream>
#include <QPixmap>

#include <QGraphicsSceneMouseEvent>

#include "ClsFESystemManager.h"

#include "ClsDiagItemAP.h"

#include "ClsDiagItem.h"
#include "ClsDiagConnection.h"
#include "ClsDiagHyperConnection.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"

#include <APIcon_9x9.xpm>

#include "ClsFEDiagramManager.h"

//#define DEBUG_CLSCANVASNODEAP


ClsDiagItemAP::ClsDiagItemAP ( QGraphicsItem * parent, string _strParentID, int _iOrientation, bool _bPhantom ): 
    QGraphicsPixmapItem(parent), 

    strParentID(_strParentID), 
    iOrientation(_iOrientation), bPhantom(_bPhantom) {
    setPixmap ( QPixmap(APIcon));
    setAcceptDrops(true);
//    setCursor(Qt::CrossCursor);
    setOpacity ( 0.3 );

};



bool ClsDiagItemAP::hasAPHandle(QString _qstrID) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::hasAPHandle(QString _qstrID)" << endl;
#endif

    QList<QGraphicsItem *> lst = childItems ();
    QList<QGraphicsItem *>::iterator it;
    for(it = lst.begin();it!=lst.end(); it++){
	string strID = "";
	if((*it)->type()==handlestartRTTI){
	    strID = dynamic_cast<ClsDiagConnectionHandleStart*>(*it)->getID();
	} else if((*it)->type()==handleendRTTI){
	    strID = dynamic_cast<ClsDiagConnectionHandleEnd*>(*it)->getID();
	}
	if(!strID.compare(_qstrID.toStdString())){
	    return true;
	}
    }
    return false;
};


int ClsDiagItemAP::getNumber(){
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::getNumber()" << endl;
#endif
    if(dynamic_cast<ClsDiagItem*>(parentItem())){
	return dynamic_cast<ClsDiagItem*>(parentItem())->getAPHandleNumberByAddress(this);
    }
    return -1;
}


void ClsDiagItemAP::moveChildren(  ){
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::moveChildrenBy ( double dx, double dy)" << endl;
#endif

    QList<QGraphicsItem *> lst = childItems ();
    QList<QGraphicsItem *>::iterator it;

    for(it = lst.begin();it!=lst.end(); it++){
	if((*it)->type()==handlestartRTTI){
	    dynamic_cast<ClsDiagConnectionHandleStart*>(*it)->moveChildren();
	} else if((*it)->type()==handleendRTTI){
	    dynamic_cast<ClsDiagConnectionHandleEnd*>(*it)->moveChildren();
	}

    }
};
    

void ClsDiagItemAP::mousePressEvent ( QGraphicsSceneMouseEvent * ){
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::mousePressEvent ( QGraphicsSceneMouseEvent * )" << endl;
#endif
    ClsBlockDiagram::Instance()->mouseLeftClickAP(this);
}


void ClsDiagItemAP::dragEnterEvent ( QGraphicsSceneDragDropEvent * event ){
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )" << endl;
#endif
    setOpacity (1 );
    if (event->mimeData()->hasFormat("text/plain")){
	event->acceptProposedAction();
    }

}

void ClsDiagItemAP::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event ){
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsDiagItemAP::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )" << endl;
#endif
    setOpacity ( 0.3 );
}

void ClsDiagItemAP::dropEvent ( QGraphicsSceneDragDropEvent * event ){
#ifdef DEBUG_CLSCANVASNODEAP
  cout << "ClsDiagItemAP::dropEvent ( QGraphicsSceneDragDropEvent * event )" << endl;
#endif


    QString qstr = event->mimeData()->text();

    stringstream ss;
    ss.str(qstr.toStdString());
    
    string strType, strID, strConnType;
    ss >> strType;
    ss >> strID;
    ss >> strConnType;
//    cout << "TYPE: " << strType << ", ID: " << strID << ", CONN TYPE: " << strConnType << endl ;


    ClsDiagHyperConnection* hyperConn = ClsBlockDiagram::Instance()->getCanvasHyperConnection(strID);

    if(hyperConn!=NULL){

	bool isIP = hyperConn->isIPConnection();
	bool willBeIP;
	
	if(!strType.compare("HandleStart")){
	    willBeIP = hyperConn->checkIfIP(strParentID, hyperConn->getTargetGroupID());
	} else if(!strType.compare("HandleEnd")){
	    willBeIP = hyperConn->checkIfIP(hyperConn->getSourceGroupID(), strParentID);
	}
	
	if(isIP != willBeIP){
//	    cout << "CHANGE IN IP STATE" << endl;
	} else {
//	    cout << "NO CHANGE IN IP STATE" << endl;	
	    
	    ClsDiagBaseConnection* conn = NULL;
	    
	    /* this is all we care about... */
	    
	    if(bPhantom){
		if(!strConnType.compare(ClsDiagBaseConnection::connTypeLocal())){
//		    /* does this case exist??? */
		    conn = hyperConn->getGrpGrpConnection();
//		    cout << "*********" << __LINE__ << endl;
		} else if(!strConnType.compare(ClsDiagBaseConnection::connTypePhantomStart())){
		    conn = hyperConn->getPhantomStart();
//		    cout << "*********" << __LINE__ << endl; //TARGET CHANGED
		} else if(!strConnType.compare(ClsDiagBaseConnection::connTypePhantomEnd())){
		    conn = hyperConn->getPhantomEnd();
//		    cout << "*********" << __LINE__ << endl;
		}
	    }
	    
	    if(!bPhantom){
		if(!strConnType.compare(ClsDiagBaseConnection::connTypeLocal())){
		    conn = hyperConn->getGrpGrpConnection();
//		    cout << "*********" << __LINE__ << endl; //OK
		} else if(!strConnType.compare(ClsDiagBaseConnection::connTypePhantomStart())){
		    /* does this case exist??? */
		    conn = hyperConn->getPhantomStart();
//		    cout << "*********" << __LINE__ << endl; //SOURCE CHANGED
		} else if(!strConnType.compare(ClsDiagBaseConnection::connTypePhantomEnd())){
		    /* does this case exist??? */
		    conn = hyperConn->getPhantomEnd();
//		    cout << "*********" << __LINE__ << endl; //TARGET CHANGED
		}
	    }
	    
	    if(conn!=NULL){
		if(!strType.compare("HandleStart")){
//		cout << "HANDLE START" << endl;
		    ClsDiagConnectionHandleStart* h = conn->getStartHandle();
		    if(h!=NULL){
			h->setParentItem(this);
		    }
		} else if(!strType.compare("HandleEnd")){
//		cout << "HANDLE END" << endl;
		    ClsDiagConnectionHandleEnd* h = conn->getEndHandle();
		    if(h!=NULL){
			h->setParentItem(this);
		    } 
		}
	    }

	}
	
	if(!strType.compare("HandleStart")){
	    hyperConn->setSourceGroupID(strParentID, getNumber());
	} else if(!strType.compare("HandleEnd")){
	    hyperConn->setTargetGroupID(strParentID, getNumber());
	}
   }    
    setOpacity ( 0.3 );
}


bool ClsDiagItemAP::isFree() {
    if(childItems ().size()>0){
	return false;
    } else {
	return true;
    }
};

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
