#include <string> 
#include <iostream> 
#include <qcanvas.h> 
#include <qmap.h>
#include <qpointarray.h> 

using namespace std;

#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasConnection.h"

#define EDGELENGTH 12

QPointArray ClsCanvasHandleEnd::constructArrowBrickHead(int x, int y, int _iOrientation, int _iEdgeLength) {
    QPointArray qpaShape(9);
    iReferencePointIndex = 2;
     switch (_iOrientation) {
     case ClsCanvasConnection::WEST_EAST:
	 qpaShape[0] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 qpaShape[1] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[3] = QPoint(x, y+_iEdgeLength);
	 qpaShape[4] = QPoint(x+_iEdgeLength, y+_iEdgeLength);
	 qpaShape[5] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 qpaShape[6] = QPoint(x, y-_iEdgeLength);
	 qpaShape[7] = QPoint(x, y);
	 qpaShape[8] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 break;
     case ClsCanvasConnection::EAST_WEST:
	 qpaShape[0] = QPoint(x+_iEdgeLength, y+_iEdgeLength); 
	 qpaShape[1] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[3] = QPoint(x, y-_iEdgeLength);
	 qpaShape[4] = QPoint(x-_iEdgeLength, y-_iEdgeLength);
	 qpaShape[5] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[6] = QPoint(x, y+_iEdgeLength);
	 qpaShape[7] = QPoint(x, y);
	 qpaShape[8] = QPoint(x+_iEdgeLength, y+_iEdgeLength); 
 	break;
     case ClsCanvasConnection::NORTH_SOUTH:
	 qpaShape[0] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 qpaShape[1] = QPoint(x-_iEdgeLength, y-_iEdgeLength);
	 qpaShape[2] = QPoint(x,              y);
	 qpaShape[3] = QPoint(x-_iEdgeLength, y);
	 qpaShape[4] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[5] = QPoint(x+_iEdgeLength, y+_iEdgeLength);
	 qpaShape[6] = QPoint(x+_iEdgeLength, y);
	 qpaShape[7] = QPoint(x,              y);
	 qpaShape[8] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 break;
     case ClsCanvasConnection::SOUTH_NORTH:
	 qpaShape[0] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[1] = QPoint(x+_iEdgeLength, y+_iEdgeLength);
	 qpaShape[2] = QPoint(x,              y);
	 qpaShape[3] = QPoint(x+_iEdgeLength, y);
	 qpaShape[4] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 qpaShape[5] = QPoint(x-_iEdgeLength, y-_iEdgeLength);
	 qpaShape[6] = QPoint(x-_iEdgeLength, y);
	 qpaShape[7] = QPoint(x,              y);
	 qpaShape[8] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
 	break;
     default:
	 qpaShape[0] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 qpaShape[1] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[3] = QPoint(x, y+_iEdgeLength);
	 qpaShape[4] = QPoint(x+_iEdgeLength, y+_iEdgeLength);
	 qpaShape[5] = QPoint(x+_iEdgeLength, y-_iEdgeLength);
	 qpaShape[6] = QPoint(x, y-_iEdgeLength);
	 qpaShape[7] = QPoint(x, y);
	 qpaShape[8] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 break;
     };


     return qpaShape;
}

QPointArray ClsCanvasHandleEnd::constructArrowHead(int x, int y, int _iOrientation, int _iEdgeLength) {
    QPointArray qpaShape(4);
    iReferencePointIndex = 2;
     switch (_iOrientation) {
     case ClsCanvasConnection::WEST_EAST:
	 qpaShape[0] = QPoint(x-_iEdgeLength, (int)(y-(double)_iEdgeLength * 0.5)); 
	 qpaShape[1] = QPoint(x-_iEdgeLength, (int)(y+(double)_iEdgeLength * 0.5));
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[3] = QPoint(x-_iEdgeLength, (int)(y-(double)_iEdgeLength * 0.5)); 
	 break;
     case ClsCanvasConnection::EAST_WEST:
	 qpaShape[0] = QPoint(x+_iEdgeLength, (int)(y+(double)_iEdgeLength * 0.5)); 
	 qpaShape[1] = QPoint(x+_iEdgeLength, (int)(y-(double)_iEdgeLength * 0.5));
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[3] = QPoint(x+_iEdgeLength, (int)(y+(double)_iEdgeLength * 0.5)); 
 	break;
     case ClsCanvasConnection::NORTH_SOUTH:
	 qpaShape[0] = QPoint((int)(x+(double)_iEdgeLength * 0.5), y-_iEdgeLength);
	 qpaShape[1] = QPoint((int)(x-(double)_iEdgeLength * 0.5), y-_iEdgeLength);
	 qpaShape[2] = QPoint(x,              y);
	 qpaShape[3] = QPoint((int)(x+(double)_iEdgeLength * 0.5), y-_iEdgeLength);
	 break;
     case ClsCanvasConnection::SOUTH_NORTH:
	 qpaShape[0] = QPoint((int)(x-(double)_iEdgeLength * 0.5), y+_iEdgeLength);
	 qpaShape[1] = QPoint((int)(x+(double)_iEdgeLength * 0.5), y+_iEdgeLength);
	 qpaShape[2] = QPoint(x,              y);
	 qpaShape[3] = QPoint((int)(x-(double)_iEdgeLength * 0.5), y+_iEdgeLength);
 	break;
     default:
	 qpaShape[0] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 qpaShape[1] = QPoint(x-_iEdgeLength, y+_iEdgeLength);
	 qpaShape[2] = QPoint(x, y);
	 qpaShape[4] = QPoint(x-_iEdgeLength, y-_iEdgeLength); 
	 break;
     };


     return qpaShape;
}


ClsCanvasHandleEnd::ClsCanvasHandleEnd ( ClsCanvasConnection *_parent, int x, int y, QCanvas * canvas, string _strID, int _iOrientation): 
    QCanvasPolygon(canvas), parent(_parent), strID(_strID), iOrientation(_iOrientation) {

    QCanvasLine *qlinePre = parent->getLastSegment();

    QPointArray qpaShape = constructArrowHead(x, y, iOrientation, EDGELENGTH);

    switch (iOrientation) {
    case ClsCanvasConnection::WEST_EAST:
	if(qlinePre!=NULL){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qlinePre->endPoint().x()-EDGELENGTH, qlinePre->endPoint().y());
	}
	break;
    case ClsCanvasConnection::EAST_WEST:
	if(qlinePre!=NULL){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qlinePre->endPoint().x()+EDGELENGTH, qlinePre->endPoint().y());
	}
	break;
    case ClsCanvasConnection::NORTH_SOUTH:
	if(qlinePre!=NULL){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qlinePre->endPoint().x(), qlinePre->endPoint().y()-EDGELENGTH);
	}
	break;
    case ClsCanvasConnection::SOUTH_NORTH:
	if(qlinePre!=NULL){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qlinePre->endPoint().x(), qlinePre->endPoint().y()+EDGELENGTH);
	}
 	break;
    default:
	break;
    };
  

    this->setPoints(qpaShape);

    show();

};

void ClsCanvasHandleEnd::setOrientation(int _iOrientation) {
    iOrientation = _iOrientation;
    QPointArray qpa = this->areaPoints();
    int x = qpa[iReferencePointIndex].x();
    int y = qpa[iReferencePointIndex].y();
    QPointArray qpaShape = constructArrowHead(x, y, iOrientation, EDGELENGTH);

    QCanvasLine *qlinePre = parent->getLastSegment();
    
    if(qlinePre!=NULL){
	
	if(iOrientation == ClsCanvasConnection::WEST_EAST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), x-EDGELENGTH, y);
	} else if(iOrientation == ClsCanvasConnection::EAST_WEST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), x+EDGELENGTH, y);
	} else if(iOrientation == ClsCanvasConnection::NORTH_SOUTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), x,            y-EDGELENGTH);
	} else if(iOrientation == ClsCanvasConnection::SOUTH_NORTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), x,            y+EDGELENGTH);
	}
    }
    

    this->setPoints(qpaShape);

}

int  ClsCanvasHandleEnd::rtti () const { return handleendRTTI; }
    
string ClsCanvasHandleEnd::getConnectionID(){
    if(parent!=NULL){
	return parent->getConnectionID();
    } 
    return NULL;
};

int  ClsCanvasHandleEnd::getCanvasConnectionType(){
    if(parent!=NULL){
	return parent->getCanvasConnectionType();
    } 
    return -1;
};

    
// void ClsCanvasHandleEnd::setConnected(bool b){
// #ifdef DEBUG_CLSCANVASHANDLEEND
//     cout << "ClsCanvasHandleEnd::setConnected(bool b)" << endl;
// #endif
//     parent->setConnected(b);
// };

void ClsCanvasHandleEnd::setTargetID(string _strTargetID) {
    strTargetID = _strTargetID;
    parent->setTargetID(_strTargetID);
}


void  ClsCanvasHandleEnd::moveBy ( double dx, double dy ){
#ifdef DEBUG_CLSCANVASHANDLEEND
    cout << "ClsCanvasHandleEnd::moveBy ( double dx, double dy )" << endl;
#endif

    QCanvasPolygon::moveBy( dx, dy );

	 
	QCanvasLine *qlinePre = parent->getLastSegment();

	QPointArray qpa = this->areaPoints();
	QPoint qpHere = qpa[iReferencePointIndex]; 

	if(qlinePre!=NULL){
	if(iOrientation == ClsCanvasConnection::WEST_EAST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x()-EDGELENGTH, qpHere.y());
	} else if(iOrientation == ClsCanvasConnection::EAST_WEST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x()+EDGELENGTH, qpHere.y());
	} else if(iOrientation == ClsCanvasConnection::NORTH_SOUTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x(), qpHere.y()-EDGELENGTH);
	} else if(iOrientation == ClsCanvasConnection::SOUTH_NORTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x(), qpHere.y()+EDGELENGTH);
	}
    }

};

void  ClsCanvasHandleEnd::refresh ( ){
#ifdef DEBUG_CLSCANVASHANDLEEND
    cout << "ClsCanvasHandleEnd::refresh ( )" << endl;
#endif
    
    QCanvasLine *qlinePre = parent->getLastSegment();
    
    QPointArray qpa = this->areaPoints();
    QPoint qpHere = qpa[iReferencePointIndex]; 
    
    if(qlinePre!=NULL){
	if(iOrientation == ClsCanvasConnection::WEST_EAST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x()-EDGELENGTH, qpHere.y());
	} else if(iOrientation == ClsCanvasConnection::EAST_WEST){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x()+EDGELENGTH, qpHere.y());
	} else if(iOrientation == ClsCanvasConnection::NORTH_SOUTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x(), qpHere.y()-EDGELENGTH);
	} else if(iOrientation == ClsCanvasConnection::SOUTH_NORTH){
	    qlinePre->setPoints(qlinePre->startPoint().x(), qlinePre->startPoint().y(), qpHere.x(), qpHere.y()+EDGELENGTH);
	}
    }
    
};
    
