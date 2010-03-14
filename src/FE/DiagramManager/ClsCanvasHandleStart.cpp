#include <string> 
#include <iostream> 
#include <qcanvas.h> 
#include <qmap.h>
#include <qpointarray.h> 

using namespace std;

#include "ClsCanvasHandleStart.h"
#include "ClsCanvasConnection.h"

#define DIAMETER 7

ClsCanvasHandleStart::ClsCanvasHandleStart ( ClsCanvasConnection *_parent, int x, int y, QCanvas * canvas, string _strID): 
    QCanvasEllipse(canvas), parent(_parent), strID(_strID) {


    setSize(DIAMETER, DIAMETER);
    move(x,y);
    show();

};

int  ClsCanvasHandleStart::rtti () const { return handlestartRTTI; }

string ClsCanvasHandleStart::getConnectionID(){
    if(parent!=NULL){
	return parent->getConnectionID();
    } 
    return NULL;
};
    

int ClsCanvasHandleStart::getCanvasConnectionType(){
    if(parent!=NULL){
	return parent->getCanvasConnectionType();
    } 
    return -1;
};


// void  ClsCanvasHandleStart::setSelected(bool b ) {
//     bSelected = b;

//     if(b){
// 	setBrush(qbrushSelected  );
// 	setPen(qpenSelected );
//     } else {
// 	setBrush(qbrushNotSelected  );
// 	setPen(qpenNotSelected );
//     }
// };

//bool  ClsCanvasHandleStart::isSelected() { return bSelected; };
    
void  ClsCanvasHandleStart::moveBy ( double dx, double dy ){

//     if( parent->isConnected()){ !!! this would also prevent reconnecting !!!
	

    QCanvasEllipse::moveBy( dx, dy );

	 
	QCanvasLine *qlinePost = parent->getFirstSegment();

//	QPointArray qpa = this->areaPoints();
//	QPoint qpHere = qpa[0]; 
	QPoint qpHere =  boundingRect().center();
	if(qlinePost!=NULL){
	    qlinePost->setPoints(qpHere.x(), qpHere.y(), qlinePost->endPoint().x(), qlinePost->endPoint().y());
	}
	
//    }
    
};

void  ClsCanvasHandleStart::refresh ( ){
	QCanvasLine *qlinePost = parent->getFirstSegment();
	QPoint qpHere =  boundingRect().center();
	if(qlinePost!=NULL){
	    qlinePost->setPoints(qpHere.x(), qpHere.y(), qlinePost->endPoint().x(), qlinePost->endPoint().y());
	}
};
    
// void ClsCanvasHandleStart::setConnected(bool b){
// //    cout << "ClsCanvasHandleStart::setConnected(bool b)" << endl;
//     parent->setConnected(b);
// };

void ClsCanvasHandleStart::setSourceID(string _strSourceID) {
    strSourceID = _strSourceID;
    parent->setSourceID(_strSourceID);
}
