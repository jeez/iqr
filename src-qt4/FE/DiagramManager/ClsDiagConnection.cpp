#include <QGraphicsLineItem>


#include "ClsDiagConnection.h"
#include "ClsDiagConnectionSegment.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagHyperConnection.h"

//#define DEBUG_CLSDIAGCONNECTION


ClsDiagConnection::ClsDiagConnection (ClsDiagHyperConnection* _hyperParent, int _iConnectionType,int _iCanvasConnectionType ):
    ClsDiagBaseConnection(_hyperParent, _iConnectionType, _iCanvasConnectionType) {
    
}


ClsDiagConnection::~ClsDiagConnection() {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection()::~ClsDiagConnection()::strID:" <<  endl;
#endif

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	delete vectorHandles[ii];
    }

    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	delete vectorSegments[ii];
    }


    if(clsCanvasHandleStart!=NULL){
	delete clsCanvasHandleStart;
	clsCanvasHandleStart = NULL;
    }
    
    if(clsCanvasHandleEnd!=NULL){
	delete clsCanvasHandleEnd;
	clsCanvasHandleEnd = NULL;
    }
};


void ClsDiagConnection::addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientation) {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientation)" << endl;
#endif
    
    
    string strID = hyperParent->getID();
    
    QString qstrNum;
    ClsDiagConnectionSegment *i = new ClsDiagConnectionSegment( this);
    vectorSegments.push_back(i);
    i->setLine( qp0.x(), qp0.y(), qp1.x(), qp1.y()); 
    i->setPen( qpen );
    i->setZValue(0);
    i->show();
    
    if(iPosition == ClsDiagConnection::SEG_START){ /* we'll need two handles in this case! */
//	cout << "ClsDiagConnection::SEG_START" << endl;
	clsCanvasHandleStart = new ClsDiagConnectionHandleStart(this, strID, iOrientation);
 	clsCanvasHandleStart->setConnectionType(iConnectionType);
	clsCanvasHandleStart->setZValue(10);
	clsCanvasHandleStart->show();
	
	ClsDiagConnectionHandle *clsCanvasHandle = new ClsDiagConnectionHandle(this, qp1.x(), qp1.y());
	vectorHandles.push_back(clsCanvasHandle);
	clsCanvasHandle->setConnectionType(iConnectionType);
	clsCanvasHandle->setZValue(10);
	clsCanvasHandle->show();
    }
    else if(iPosition == ClsDiagConnection::SEG_MIDDLE){
//	cout << "ClsDiagConnection::SEG_MIDDLE" << endl;
	ClsDiagConnectionHandle *clsCanvasHandle = new ClsDiagConnectionHandle(this, qp1.x(), qp1.y());
	vectorHandles.push_back(clsCanvasHandle);
	clsCanvasHandle->setConnectionType(iConnectionType);
	clsCanvasHandle->setZValue(10);
	clsCanvasHandle->show();
    } 
    else if(iPosition == ClsDiagConnection::SEG_END){
//	cout << "ClsDiagConnection::SEG_MIDDLE" << endl;
	clsCanvasHandleEnd = new ClsDiagConnectionHandleEnd(this, strID, iOrientation);
 	clsCanvasHandleEnd->setConnectionType(iConnectionType);
	clsCanvasHandleEnd->setZValue(10);
	clsCanvasHandleEnd->show();
    }
    else if(iPosition == ClsDiagConnection::SEG_SINGLE){
//	cout << "iPosition == ClsDiagConnection::SEG_SINGLE" << endl;
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

void ClsDiagConnection::split(QGraphicsLineItem* s, QPointF qpPos) {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::split(QCanvasLine* s)" << endl;
#endif
    
    int iPos = findSegmentPos(s);
//    cout << "iPos: " <<iPos << endl;
    
//    cout << "qp1.x(), qp1.y()" << qp1.x() << ", " << qp1.y() << endl;


    ClsDiagConnectionSegment *clsCanvasConnectionSegment = new ClsDiagConnectionSegment( this);
    vector<QGraphicsLineItem*>::iterator pos0;
    pos0 = vectorSegments.begin() + iPos;
    vectorSegments.insert(pos0 + 1, clsCanvasConnectionSegment);
    clsCanvasConnectionSegment->setPen( qpen );
    clsCanvasConnectionSegment->setZValue(0);
    clsCanvasConnectionSegment->show();
    
    vector<ClsDiagConnectionHandle*>::iterator pos1;
    ClsDiagConnectionHandle *clsCanvasHandle = new ClsDiagConnectionHandle(this, qpPos.x(), qpPos.y());
    pos1 = vectorHandles.begin() + iPos;
    vectorHandles.insert(pos1, clsCanvasHandle);
    clsCanvasHandle->setConnectionType(iConnectionType);
    clsCanvasHandle->setZValue(10);
    clsCanvasHandle->show();

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->refresh();
    }

    getEndHandle()->refresh();

}

void ClsDiagConnection::removeHandle(ClsDiagConnectionHandle* h) {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::removeHandle(ClsDiagConnectionHandle* h)" << endl;
#endif
    int iPos = findHandlePos(h);
//    cout << "iPos: " << iPos << endl;

    vector<ClsDiagConnectionHandle*>::iterator pos1;
    pos1 = vectorHandles.begin() + iPos;
    delete vectorHandles[iPos];
    vectorHandles.erase(pos1);
    

    vector<QGraphicsLineItem*>::iterator pos0;
    pos0 = vectorSegments.begin() + iPos + 1;
    delete vectorSegments[iPos+1];
    vectorSegments.erase(pos0);

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->refresh();
    }

    if(clsCanvasHandleEnd!=NULL){
	clsCanvasHandleEnd->refresh();
    }
}


vector<vector<int> > ClsDiagConnection::getPoints() {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::getPoints()" << endl;
#endif

    vector<vector<int> > v;

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	int iX = vectorHandles[ii]->pos().x() + vectorHandles[ii]->boundingRect().center().x(); //zzz check this
	int iY = vectorHandles[ii]->pos().y() + vectorHandles[ii]->boundingRect().center().y(); //zzz check this
	vector<int> vectorPoint;
	vectorPoint.push_back(iX); vectorPoint.push_back(iY);
	v.push_back(vectorPoint);
    }
    return v;
}


void ClsDiagConnection::setConnected(bool b) {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::setConnected(bool b)" << endl;
#endif
    ClsDiagBaseConnection::setConnected(b);
    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	vectorSegments[ii]->setPen(qpen);
    }
};

void ClsDiagConnection::setConnectionType(int _iConnectionType){
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "ClsDiagConnection::setConnectionType(int _iConnectionType)::iConnectionType:" << _iConnectionType << endl;
#endif

    ClsDiagBaseConnection::setConnectionType(_iConnectionType);

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->setConnectionType(iConnectionType);
    }

    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	vectorSegments[ii]->setPen(qpen);
    }
}

void ClsDiagConnection::mark(bool b) {
#ifdef DEBUG_CLSDIAGCONNECTION
    cout << "**ClsDiagConnection::setSelected(bool b)" << endl;
#endif
    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	if( vectorSegments[ii]->isSelected() != b){
	    if(vectorSegments[ii]->type()==connectionsegmentRTTI){
		((ClsDiagConnectionSegment*)vectorSegments[ii])->markSegment(b);
	    }
	}
    }
    ClsDiagBaseConnection::setSelected(b);
}
    


void ClsDiagConnection::setSourceID(string _strSourceID) { 
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagConnection::setSourceID(string _strSourceID): " << _strSourceID << endl;
#endif
    if(hyperParent!=NULL){
	if(iCanvasConnectionType == ClsDiagConnection::LOCAL) {
	    hyperParent->setSourceGroupID(_strSourceID);

	} else {
	    hyperParent->setSourceProcessID(_strSourceID);
	}
    }

};

string ClsDiagConnection::getSourceID( ) { 
    if(hyperParent!=NULL){	
	if(iCanvasConnectionType == ClsDiagConnection::LOCAL) {
	    return hyperParent->getSourceGroupID();
	} else {
	    return hyperParent->getSourceProcessID();
	}
    }
    return "";
};


void ClsDiagConnection::setTargetID(string _strTargetID) { 
#ifdef DEBUG_CLSDIAGPHANTOMCONNECTION
    cout << "ClsDiagConnection::setTargetID(string _strTargetID): " << _strTargetID << endl;
#endif
    if(hyperParent!=NULL){
	if(iCanvasConnectionType == ClsDiagConnection::LOCAL) {
	    hyperParent->setTargetGroupID(_strTargetID); 
	} else {
	    hyperParent->setTargetProcessID(_strTargetID); 

	}
    }
};

string ClsDiagConnection::getTargetID( ) { 
    if(hyperParent!=NULL){
	if(iCanvasConnectionType == ClsDiagConnection::LOCAL) {
	    return hyperParent->getTargetGroupID();
	} else {
	    return hyperParent->getTargetProcessID();
	}
    }
    return "";
};









//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
