#include <string> 
#include <iostream> 

#include <QGraphicsScene> 
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QCursor>

using namespace std;

#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnection.h"
#include "ClsDiagBaseConnection.h"
#include "ClsDiagItemAP.h"
#include "ClsInfoConnection.h"
#include "ClsScene.h"

#include <ConnEndRed.xpm>
#include <ConnEndGreen.xpm>
#include <ConnEndBlue.xpm>

//#define DEBUG_CLSCANVASHANDLEEND


ClsDiagConnectionHandleEnd::ClsDiagConnectionHandleEnd ( ClsDiagBaseConnection *_parentConnection, string _strID, int _iOrientation): 
    parentConnection(_parentConnection), strID(_strID), iOrientation(_iOrientation) {

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIgnoresParentOpacity);

    qpmExc = QPixmap(ConnEndRed);
    qpmMod = QPixmap(ConnEndGreen); 
    qpmInh = QPixmap(ConnEndBlue);
    setPixmap (qpmExc);

    qrectMyBR = boundingRect();
    setOffset( -0.5 * QPointF( qrectMyBR.width(), 0 ) );

    if(scene()!=NULL){
	if(dynamic_cast<ClsScene*>(scene())->getCanvasType()==ClsScene::CANVAS_PROCESS){
	    setCursor(Qt::OpenHandCursor);
	} else {
	    setCursor(Qt::ArrowCursor);
	}
    }
};

void ClsDiagConnectionHandleEnd::setOrientation(int _iOrientation) {
    iOrientation = _iOrientation;

    resetTransform ();
    switch (iOrientation) {
    case ClsDiagConnection::WEST_EAST:
	rotate(90);
	break;
    case ClsDiagConnection::EAST_WEST:
	rotate(270);
	break;
    case ClsDiagConnection::NORTH_SOUTH:
	rotate(180);
	break;
    case ClsDiagConnection::SOUTH_NORTH:
	rotate(0);
 	break;
    default:
	break;
    };
}
 
int  ClsDiagConnectionHandleEnd::type () const { return handleendRTTI; }
    
string ClsDiagConnectionHandleEnd::getConnectionID(){
    if(dynamic_cast<ClsDiagConnection*>(parentItem())){
	return dynamic_cast<ClsDiagConnection*>(parentItem())->getConnectionID();
    } 
    return NULL;
};

int  ClsDiagConnectionHandleEnd::getCanvasConnectionType(){
    parentConnection->getCanvasConnectionType();
    return -1;
};

    
void ClsDiagConnectionHandleEnd::setTargetID(string _strTargetID) {
    strTargetID = _strTargetID;
    parentConnection->setTargetID(_strTargetID);
}

void ClsDiagConnectionHandleEnd::setConnectionType(int iConnectionType){
    
    if(iConnectionType == iqrcommon::ClsInfoConnection::CONN_EXCITATORY){
      setPixmap ( qpmExc);
      }
    else if(iConnectionType == iqrcommon::ClsInfoConnection::CONN_INHIBITORY){
      setPixmap ( qpmInh);
      }
    else if(iConnectionType == iqrcommon::ClsInfoConnection::CONN_MODULATORY){
      setPixmap ( qpmMod);
      }
};


string ClsDiagConnectionHandleEnd::getAPID() {
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	return dynamic_cast<ClsDiagItemAP*>(parentItem())->getParentID();
    }
    return "";
}

int ClsDiagConnectionHandleEnd::getAPNumber() {
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	return dynamic_cast<ClsDiagItemAP*>(parentItem())->getNumber();
    }
    return -1;
}


void ClsDiagConnectionHandleEnd::refresh(){
    qrectParentBR = parentItem()->boundingRect();
    
    setPos(qrectParentBR.center());
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	iOrientation = dynamic_cast<ClsDiagItemAP*>(parentItem())->getOrientation();
	setOrientation(iOrientation);
    }
    QPointF qpHere = parentItem()->scenePos() + qrectParentBR.center();	    
    
    QGraphicsLineItem *qlinePre = parentConnection->getLastSegment();
    if(qlinePre!=NULL){
	if(iOrientation == ClsDiagConnection::WEST_EAST){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() - qrectMyBR.height(), qpHere.y() -1 ); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::EAST_WEST){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() + qrectMyBR.height(), qpHere.y() -1 ); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::NORTH_SOUTH){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x(), qpHere.y() - qrectMyBR.height());
	} else if(iOrientation == ClsDiagConnection::SOUTH_NORTH){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() -1, qpHere.y() + qrectMyBR.height());
	}
    }
    qlinePre->setZValue(0);	    
    setZValue(10);	    
}

QVariant ClsDiagConnectionHandleEnd::itemChange ( GraphicsItemChange change, const QVariant & value ){
#ifdef DEBUG_CLSCANVASHANDLEEND
    cout << "ClsDiagConnectionHandleEnd::itemChange ( GraphicsItemChange change, const QVariant & value )" << endl;
#endif

    if(change == QGraphicsItem::ItemParentHasChanged  && scene()){
#ifdef DEBUG_CLSCANVASHANDLEEND
	cout << "QGraphicsItem::ItemParentHasChanged" << endl;
#endif
	if(parentItem()!=NULL){
	    refresh();
	} else {	
	    setTargetID("");
	    QGraphicsLineItem *qlinePre = parentConnection->getLastSegment();
	    if(qlinePre!=NULL){
		QPointF p = qlinePre->line().p2();
		setPos(p);
	    }
	}

	if(scene()!=NULL){
	    if(dynamic_cast<ClsScene*>(scene())->getCanvasType()==ClsScene::CANVAS_PROCESS){
		setCursor(Qt::OpenHandCursor);
	    } else {
		setCursor(Qt::ArrowCursor);
	    }
	}
    }
    return QGraphicsItem::itemChange(change,value);
}
    


void ClsDiagConnectionHandleEnd::mousePressEvent ( QGraphicsSceneMouseEvent * event){
#ifdef DEBUG_CLSCANVASHANDLEEND
    cout << "ClsDiagConnectionHandleEnd::mousePressEvent ( QGraphicsSceneMouseEvent * event)" << endl;
#endif
    if(dynamic_cast<ClsScene*>(scene())->getCanvasType()==ClsScene::CANVAS_PROCESS){
	QMimeData *data = new QMimeData;
	QString qstr = QString("HandleEnd ") + strID.c_str() + QString(" ") + parentConnection->connTypeAsString().c_str();
	data->setText(qstr);
	QDrag *drag = new QDrag(event->widget());
	drag->setMimeData(data);
	drag->exec();
    } else {
	event->ignore();
    }
}


void ClsDiagConnectionHandleEnd::moveChildren(){
#ifdef DEBUG_CLSCANVASHANDLEEND
    cout << "ClsDiagConnectionHandleEnd::moveChildrenBy ( qreal dx, qreal dy )" << endl;
#endif
    
    QPointF qpHere = parentItem()->scenePos() + qrectParentBR.center();
   
    QGraphicsLineItem *qlinePre = parentConnection->getLastSegment();
    if(qlinePre!=NULL){
	if(iOrientation == ClsDiagConnection::WEST_EAST){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() - qrectMyBR.height(), qpHere.y() - 1); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::EAST_WEST){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() + qrectMyBR.height(), qpHere.y() - 1); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::NORTH_SOUTH){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x(), qpHere.y() - qrectMyBR.height());
	} else if(iOrientation == ClsDiagConnection::SOUTH_NORTH){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), qpHere.x() -1 , qpHere.y() + qrectMyBR.height());
	}
    }
    
}
    

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

