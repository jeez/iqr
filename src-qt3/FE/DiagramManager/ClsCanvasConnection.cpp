#include <iostream>
#include <string> 
#include <qcanvas.h> 
#include <qmap.h>


#include "ClsFEConnection.h"

#include "ClsCanvasConnection.h"
#include "ClsCanvasConnectionSegment.h"
#include "ClsCanvasHandle.h"
#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasHandleStart.h"
#include "ClsCanvasMetaConnection.h"


#include <ClsFESystemManager.h>

using namespace std;
using namespace iqrcommon;

//#define DEBUG_CLSCANVASCONNECTION

ClsCanvasConnection::ClsCanvasConnection ( QCanvas * canvas, string _strID, string _strSourceID, string _strTargetID, int _iConnectionType, int _iCanvasConnectionType ): 
    QCanvasLine(canvas), qcanvasParent(canvas), strID(_strID), 
    strSourceID(_strSourceID), 
    strTargetID(_strTargetID), iConnectionType(_iConnectionType),
    iCanvasConnectionType(_iCanvasConnectionType) {

    clsCanvasMetaConnectionParent = NULL;
    clsCanvasHandleStart = NULL;
    clsCanvasHandleEnd = NULL;

    if(iConnectionType == ClsFEConnection::CONN_EXCITATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_EXCITATORY" << endl;
	qbrush.setColor(QColor(255,0,0));
	qpen.setColor(QColor(255,0,0));
    }
    else if(iConnectionType == ClsFEConnection::CONN_INHIBITORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_INHIBITORY" << endl;
	qbrush.setColor(QColor(0,0, 255));
	qpen.setColor(QColor(0,0,255));
    }
    else if(iConnectionType == ClsFEConnection::CONN_MODULATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_MODULATORY" << endl;
	qbrush.setColor(QColor(0,255,0));
	qpen.setColor(QColor(0,255,0));
    }


    qbrush.setStyle ( SolidPattern ) ;
    qpen.setWidth(3);

};

ClsCanvasConnection::~ClsCanvasConnection() {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection()::~ClsCanvasConnection()::strID:" << strID << endl;
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

string ClsCanvasConnection::getConnectionID() {
    if(clsCanvasMetaConnectionParent==NULL){
	return strID;
    } else {
	return clsCanvasMetaConnectionParent->getID();
    }
};



void ClsCanvasConnection::setMetaConnectionParent(ClsCanvasMetaConnection* p) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "meta:ClsCanvasConnection::setMetaConnectionParent(ClsCanvasMetaConnection* p)" << endl;
#endif
    clsCanvasMetaConnectionParent = p;
};

void ClsCanvasConnection::setSourceID(string _strSourceID) { 
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::setSourceID(string _strSourceID): " << _strSourceID << endl;
#endif

    if(strSourceID.compare(_strSourceID)){
	ClsFESystemManager::Instance()->getFEConnection(getConnectionID())->setConnectionSourceID(_strSourceID);

	if(strSourceID.size()>0){
	    strOldSourceID = strSourceID;
	}

//    }

    strSourceID = _strSourceID; 

    if((strSourceID.length() * strTargetID.length()) > 0) {
	setConnected(true);
    }
    else {
	setConnected(false);
    }

    if(clsCanvasMetaConnectionParent!=NULL){
	clsCanvasMetaConnectionParent->setSourceID(strSourceID);
    }
    }

};


void ClsCanvasConnection::setTargetID(string _strTargetID) { 
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::setTargetID(string _strTargetID): " << _strTargetID << endl;
#endif



    if(strTargetID.compare(_strTargetID)){
	ClsFESystemManager::Instance()->getFEConnection(getConnectionID())->setConnectionTargetID(_strTargetID);
	if(strTargetID.size()>0){
	    strOldTargetID = strTargetID;
	}
    }

    strTargetID = _strTargetID; 
    if((strSourceID.length() * strTargetID.length()) > 0) {
	setConnected(true);
    }
    else {
	setConnected(false);
    }
    
    if(clsCanvasMetaConnectionParent!=NULL){
	clsCanvasMetaConnectionParent->setTargetID(strTargetID); /* this is going to come back, but stop because of the above check... */
    }

};


void ClsCanvasConnection::setConnected(bool b) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::setConnected(bool b)" << endl;
#endif
    
    if(!b){     /* set pen to dashed */
	qpen.setStyle(Qt::DotLine);
    }
    else {
	qpen.setStyle(Qt::SolidLine);
    }
    
    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	vectorSegments[ii]->setPen(qpen);
    }
};

int ClsCanvasConnection::rtti () const { return connectionRTTI; }

void ClsCanvasConnection::setConnectionType(int _iConnectionType){
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::setConnectionType(int _iConnectionType)::iConnectionType:" << _iConnectionType << endl;
#endif

    iConnectionType = _iConnectionType;
    if(iConnectionType == ClsFEConnection::CONN_EXCITATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_EXCITATORY" << endl;
	qbrush.setColor(QColor(255,0,0));
	qpen.setColor(QColor(255,0,0));
    }
    else if(iConnectionType == ClsFEConnection::CONN_INHIBITORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_INHIBITORY" << endl;
	qbrush.setColor(QColor(0,0, 255));
	qpen.setColor(QColor(0,0,255));
    }
    else if(iConnectionType == ClsFEConnection::CONN_MODULATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_MODULATORY" << endl;
	qbrush.setColor(QColor(0,255,0));
	qpen.setColor(QColor(0,255,0));
    }

    qbrush.setStyle ( SolidPattern ) ;
    qpen.setWidth(3);

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->setConnectionType(iConnectionType);
    }

    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	vectorSegments[ii]->setPen(qpen);
    }

    clsCanvasHandleStart->setPen(qpen);
    clsCanvasHandleStart->setBrush(qbrush);
    
    clsCanvasHandleEnd->setPen(qpen);
    clsCanvasHandleEnd->setBrush(qbrush);


    if(clsCanvasMetaConnectionParent!=NULL){
	clsCanvasMetaConnectionParent->setConnectionType(iConnectionType);
    }
}

void ClsCanvasConnection::split(QCanvasLine* s, QPoint qpPos) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::split(QCanvasLine* s)" << endl;
#endif
    
    int iPos = findSegmentPos(s);
//    cout << "iPos: " <<iPos << endl;
    
    ClsCanvasConnectionSegment *clsCanvasConnectionSegment = new ClsCanvasConnectionSegment( qcanvasParent, this);
    vector<QCanvasLine*>::iterator pos0;
    pos0 = vectorSegments.begin() + iPos;
    vectorSegments.insert(pos0 + 1, clsCanvasConnectionSegment);
    clsCanvasConnectionSegment->setPen( qpen );
    clsCanvasConnectionSegment->setZ(2);
    clsCanvasConnectionSegment->show();
    
    vector<ClsCanvasHandle*>::iterator pos1;
    ClsCanvasHandle *clsCanvasHandle = new ClsCanvasHandle(this, qpPos.x(), qpPos.y(), qcanvasParent);
    pos1 = vectorHandles.begin() + iPos;
    vectorHandles.insert(pos1, clsCanvasHandle);
    clsCanvasHandle->setConnectionType(iConnectionType);
    clsCanvasHandle->setZ(5);
    clsCanvasHandle->show();

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->refresh();
    }

    clsCanvasHandleEnd->refresh();

    
};

string ClsCanvasConnection::getStartHandleID(){
    if(clsCanvasHandleStart!=NULL){
	return clsCanvasHandleStart->getID();
    }
    return "";
}

string ClsCanvasConnection::getEndHandleID(){
    if(clsCanvasHandleEnd!=NULL){
	return clsCanvasHandleEnd->getID();
    }
    return "";
}

void ClsCanvasConnection::removeHandle(ClsCanvasHandle* h) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::removeHandle(ClsCanvasHandle* h)" << endl;
#endif
    int iPos = findHandlePos(h);
//    cout << "iPos: " << iPos << endl;

    vector<ClsCanvasHandle*>::iterator pos1;
    pos1 = vectorHandles.begin() + iPos;
    delete vectorHandles[iPos];
    vectorHandles.erase(pos1);
    

    vector<QCanvasLine*>::iterator pos0;
    pos0 = vectorSegments.begin() + iPos + 1;
    delete vectorSegments[iPos+1];
    vectorSegments.erase(pos0);

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	vectorHandles[ii]->refresh();
    }
    clsCanvasHandleEnd->refresh();
}

void ClsCanvasConnection::addSegment(QPoint qp0, QPoint qp1, int iPosition, int iOrientation) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::addSegment(QPoint qp0, QPoint qp1, int iPosition, int iOrientation)" << endl;
#endif
    QString qstrNum;
    ClsCanvasConnectionSegment *i = new ClsCanvasConnectionSegment( qcanvasParent, this);
    vectorSegments.push_back(i);
    
    i->setPoints( qp0.x(), qp0.y(), qp1.x(), qp1.y());
    i->setPen( qpen );
    i->setZ(2);
    i->show();

    if(iPosition == ClsCanvasConnection::SEG_START){ /* we'll need two handles in this case! */
	clsCanvasHandleStart = new ClsCanvasHandleStart(this, qp0.x(), qp0.y(), qcanvasParent, strID);
 	clsCanvasHandleStart->setBrush( qbrush );
	clsCanvasHandleStart->setPen( qpen );
	clsCanvasHandleStart->setZ(5);
	clsCanvasHandleStart->show();

	ClsCanvasHandle *clsCanvasHandle = new ClsCanvasHandle(this, qp1.x(), qp1.y(), qcanvasParent);
	vectorHandles.push_back(clsCanvasHandle);
	clsCanvasHandle->setConnectionType(iConnectionType);
	clsCanvasHandle->setZ(10);
	clsCanvasHandle->show();
    }
    else if(iPosition == ClsCanvasConnection::SEG_MIDDLE){
	ClsCanvasHandle *clsCanvasHandle = new ClsCanvasHandle(this, qp1.x(), qp1.y(), qcanvasParent);
	vectorHandles.push_back(clsCanvasHandle);
	clsCanvasHandle->setConnectionType(iConnectionType);
	clsCanvasHandle->setZ(10);
	clsCanvasHandle->show();
    } 
    else if(iPosition == ClsCanvasConnection::SEG_END){
	clsCanvasHandleEnd = new ClsCanvasHandleEnd(this, qp1.x(), qp1.y(), qcanvasParent, strID, iOrientation);
 	clsCanvasHandleEnd->setBrush( qbrush );
	clsCanvasHandleEnd->setPen( qpen );
	clsCanvasHandleEnd->setZ(5);
	clsCanvasHandleEnd->show();
    }
    else if(iPosition == ClsCanvasConnection::SEG_SINGLE){
	clsCanvasHandleStart = new ClsCanvasHandleStart(this, qp0.x(), qp0.y(), qcanvasParent, strID);
 	clsCanvasHandleStart->setBrush( qbrush );
	clsCanvasHandleStart->setPen( qpen );
	clsCanvasHandleStart->setZ(5);
	clsCanvasHandleStart->show();
	
	clsCanvasHandleEnd = new ClsCanvasHandleEnd(this, qp1.x(), qp1.y(), qcanvasParent, strID, iOrientation);
 	clsCanvasHandleEnd->setBrush( qbrush );
	clsCanvasHandleEnd->setPen( qpen );
	clsCanvasHandleEnd->setZ(5);
	clsCanvasHandleEnd->show();
    }
    else {
//	cout << "HandleID UNKNOWN" << endl;
    }
}
    

ClsCanvasHandleStart* ClsCanvasConnection::getStartHandle() {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::getStartHandle()" << endl;
#endif
    return clsCanvasHandleStart;
};

ClsCanvasHandleEnd* ClsCanvasConnection::getEndHandle() {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::getEndHandle()" << endl;
#endif
    return clsCanvasHandleEnd;
};


vector<vector<int> > ClsCanvasConnection::getPoints() {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "ClsCanvasConnection::getPoints()" << endl;
#endif

    vector<vector<int> > v;

    for(unsigned int ii=0; ii<vectorHandles.size(); ii++){
	int iX = vectorHandles[ii]->boundingRect().center().x();
	int iY = vectorHandles[ii]->boundingRect().center().y();
	vector<int> vectorPoint;
	vectorPoint.push_back(iX); vectorPoint.push_back(iY);
	v.push_back(vectorPoint);
    }
    return v;
}

void ClsCanvasConnection::setSelected(bool b) {
#ifdef DEBUG_CLSCANVASCONNECTION
    cout << "**ClsCanvasConnection::setSelected(bool b)" << endl;
#endif
    for(unsigned int ii=0; ii<vectorSegments.size(); ii++){
	if( vectorSegments[ii]->isSelected() != b){
	    if(vectorSegments[ii]->rtti()==connectionsegmentRTTI){
		((ClsCanvasConnectionSegment*)vectorSegments[ii])->markSegment(b);
	    }
	}
    }

    QCanvasItem::setSelected(b); 
    if(clsCanvasMetaConnectionParent!=NULL){
	clsCanvasMetaConnectionParent->setSelected(b);
    }

}



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:




