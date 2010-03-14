#include <QGraphicsLineItem>


#include "ClsDiagPhantomConnection.h"
#include "ClsDiagConnectionSegment.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagHyperConnection.h"

//#define DEBUG_CLSDIAGPHANTOMCONNECTION


ClsDiagPhantomConnection::ClsDiagPhantomConnection ( ClsDiagHyperConnection* _hyperParent, int _iConnectionType,int _iCanvasConnectionType ):
    ClsDiagBaseConnection(_hyperParent, _iConnectionType, _iCanvasConnectionType) {
    clsDiagConnectionSegment = NULL;
}


ClsDiagPhantomConnection::~ClsDiagPhantomConnection() {
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagPhantomConnection()::~ClsDiagPhantomConnection()" << endl;
#endif

    if(clsCanvasHandleStart!=NULL){
	delete clsCanvasHandleStart;
	clsCanvasHandleStart = NULL;
    }
    
    if(clsCanvasHandleEnd!=NULL){
	delete clsCanvasHandleEnd;
	clsCanvasHandleEnd = NULL;
    }
};


void ClsDiagPhantomConnection::addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientation) {
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagPhantomConnection::addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientation)" << endl;
#endif

    string strID = hyperParent->getID();

    QString qstrNum;
    clsDiagConnectionSegment = new ClsDiagConnectionSegment( this);
    clsDiagConnectionSegment->setPen( qpen );
    clsDiagConnectionSegment->setZValue(0);

    if(iPosition == ClsDiagConnection::SEG_SINGLE){
	clsCanvasHandleStart = new ClsDiagConnectionHandleStart(this, strID, iOrientation);
 	clsCanvasHandleStart->setConnectionType(iConnectionType);
	clsCanvasHandleStart->setZValue(10);
	clsCanvasHandleStart->show();
	
	clsCanvasHandleEnd = new ClsDiagConnectionHandleEnd(this, strID, iOrientation);
 	clsCanvasHandleEnd->setConnectionType(iConnectionType);
	clsCanvasHandleEnd->setZValue(10);
	clsCanvasHandleEnd->show();
    }

}


void ClsDiagPhantomConnection::setConnectionType(int _iConnectionType){
    ClsDiagBaseConnection::setConnectionType(_iConnectionType);
    if(clsDiagConnectionSegment!=NULL){
	clsDiagConnectionSegment->setPen( qpen );
    }
}

void ClsDiagPhantomConnection::setSourceID(string _strSourceID) { 
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagPhantomConnection::setSourceID(string _strSourceID): " << _strSourceID << endl;
#endif
    
    if(hyperParent!=NULL){
	hyperParent->setSourceGroupID(_strSourceID);
    }

};

string ClsDiagPhantomConnection::getSourceID( ) { 
    if(hyperParent!=NULL){
	return hyperParent->getSourceGroupID();
    }
    return "";
};


void ClsDiagPhantomConnection::setTargetID(string _strTargetID) { 
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagPhantomConnection::setTargetID(string _strTargetID): " << _strTargetID << endl;
#endif
    
//    cout << "iCanvasConnectionType: " << iCanvasConnectionType << endl;
    
    if(hyperParent!=NULL){
	hyperParent->setTargetGroupID(_strTargetID); /* this is going to come back, but stop because of the above check... */
    }
};

string ClsDiagPhantomConnection::getTargetID( ) { 
    if(hyperParent!=NULL){
	return hyperParent->getTargetGroupID();
    }
    return "";
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
