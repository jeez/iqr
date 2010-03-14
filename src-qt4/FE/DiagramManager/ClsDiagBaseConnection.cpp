#include <iostream>
#include <string> 

#include <QGraphicsScene> 
#include <QMap>
#include <QGraphicsLineItem>

#include "ClsFEConnection.h"

#include "ClsDiagBaseConnection.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagHyperConnection.h"

#include <ClsFESystemManager.h>

using namespace std;
using namespace iqrcommon;

//#define DEBUG_CLSDIAGBASECONNECTION

ClsDiagBaseConnection::ClsDiagBaseConnection (ClsDiagHyperConnection* _hyperParent, int _iConnectionType, int _iCanvasConnectionType ): 
    QGraphicsLineItem(NULL), 
    hyperParent(_hyperParent),
    iConnectionType(_iConnectionType),
    iCanvasConnectionType(_iCanvasConnectionType) {

    clsCanvasHandleStart = NULL;
    clsCanvasHandleEnd = NULL;

    mapConnTypes[ClsDiagBaseConnection::LOCAL] = connTypeLocal();
    mapConnTypes[ClsDiagBaseConnection::IP] = connTypeIP();
    mapConnTypes[ClsDiagBaseConnection::PHANTOM_START] = connTypePhantomStart(); 
    mapConnTypes[ClsDiagBaseConnection::PHANTOM_END] = connTypePhantomEnd(); 
    

    if(iConnectionType == ClsFEConnection::CONN_EXCITATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_EXCITATORY" << endl;
	qbrush.setColor(QColor(240,0,0));
	qpen.setColor(QColor(240,0,0));
    }
    else if(iConnectionType == ClsFEConnection::CONN_INHIBITORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_INHIBITORY" << endl;
	qbrush.setColor(QColor(0,0, 240));
	qpen.setColor(QColor(0,0,240));
    }
    else if(iConnectionType == ClsFEConnection::CONN_MODULATORY){
//	cout << "iConnectionType == ClsFEConnection::CONN_MODULATORY" << endl;
	qbrush.setColor(QColor(0,240,0));
	qpen.setColor(QColor(0,240,0));
    }

    qbrush.setStyle ( Qt::SolidPattern ) ;
    qpen.setWidth(3);
};

ClsDiagBaseConnection::~ClsDiagBaseConnection() {
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "ClsDiagBaseConnection()::~ClsDiagBaseConnection()" << endl;
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

//--bool ClsDiagBaseConnection::isConnected() {
//--    if(hyperParent==NULL){
//--	return hyperParent->isConnected();
//--    }
//--    return false;
//--}


string ClsDiagBaseConnection::getConnectionID() {
//    cout << "ClsDiagBaseConnection::getConnectionID()" << endl;
    if(hyperParent!=NULL){
	return hyperParent->getID();
    }
};

void ClsDiagBaseConnection::setConnected(bool b) {
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "ClsDiagBaseConnection::setConnected(bool b)" << endl;
#endif
    if(!b){     /* set pen to dashed */
	qpen.setStyle(Qt::DotLine);
    }
    else {
	qpen.setStyle(Qt::SolidLine);
    }
};

int ClsDiagBaseConnection::type () const { return connectionRTTI; }

void ClsDiagBaseConnection::setConnectionType(int _iConnectionType){
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "ClsDiagBaseConnection::setConnectionType(int _iConnectionType)::iConnectionType:" << _iConnectionType << endl;
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

    qbrush.setStyle ( Qt::SolidPattern ) ;
    qpen.setWidth(3);

    clsCanvasHandleStart->setConnectionType(iConnectionType);
    clsCanvasHandleEnd->setConnectionType(iConnectionType);
}


string ClsDiagBaseConnection::getStartHandleID(){
    if(clsCanvasHandleStart!=NULL){
	return clsCanvasHandleStart->getID();
    }
    return "";
}

string ClsDiagBaseConnection::getEndHandleID(){
    if(clsCanvasHandleEnd!=NULL){
	return clsCanvasHandleEnd->getID();
    }
    return "";
}

int ClsDiagBaseConnection::getStartHandleAPNumber(){
    if(clsCanvasHandleStart!=NULL){
	return clsCanvasHandleStart->getAPNumber();
    }
    return -1;
}

int ClsDiagBaseConnection::getEndHandleAPNumber(){
    if(clsCanvasHandleEnd!=NULL){
	return clsCanvasHandleEnd->getAPNumber();
    }
    return -1;
}


    

ClsDiagConnectionHandleStart* ClsDiagBaseConnection::getStartHandle() {
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "ClsDiagBaseConnection::getStartHandle()" << endl;
#endif
    return clsCanvasHandleStart;
};

ClsDiagConnectionHandleEnd* ClsDiagBaseConnection::getEndHandle() {
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "ClsDiagBaseConnection::getEndHandle()" << endl;
#endif
    return clsCanvasHandleEnd;
};



void ClsDiagBaseConnection::mark(bool b) {
#ifdef DEBUG_CLSDIAGBASECONNECTION
    cout << "**ClsDiagBaseConnection::setSelected(bool b)" << endl;
#endif
    if(hyperParent!=NULL){
	hyperParent->mark(b);
    }
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:




