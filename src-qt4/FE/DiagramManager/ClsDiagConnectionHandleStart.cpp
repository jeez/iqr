#include <string> 
#include <iostream> 

#include <QGraphicsScene> 
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QCursor>
using namespace std;


#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagConnection.h"
#include "ClsDiagBaseConnection.h"
#include "ClsInfoConnection.h"
#include "ClsScene.h"
#include "ClsDiagItemAP.h"

#include <ConnStartRed.xpm>
#include <ConnStartGreen.xpm>
#include <ConnStartBlue.xpm>


ClsDiagConnectionHandleStart::ClsDiagConnectionHandleStart ( ClsDiagBaseConnection *_parentConnection, string _strID, int _iOrientation): 
    parentConnection(_parentConnection), strID(_strID), iOrientation(_iOrientation) {

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIgnoresParentOpacity);

    qpmExc = QPixmap(ConnStartRed);
    qpmMod = QPixmap(ConnStartGreen); 
    qpmInh = QPixmap(ConnStartBlue);

    setPixmap( qpmExc);

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

void ClsDiagConnectionHandleStart::setOrientation(int _iOrientation) {
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



int  ClsDiagConnectionHandleStart::type () const { return handlestartRTTI; }

string ClsDiagConnectionHandleStart::getConnectionID(){
    if(parentConnection){
	return parentConnection->getConnectionID();
    } 
    return NULL;
};
    

int ClsDiagConnectionHandleStart::getCanvasConnectionType(){
    if(parentConnection){
	return parentConnection->getCanvasConnectionType();
    } 
    return -1;
};

void ClsDiagConnectionHandleStart::setConnectionType(int iConnectionType){

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

string ClsDiagConnectionHandleStart::getAPID() {
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	return dynamic_cast<ClsDiagItemAP*>(parentItem())->getParentID();
    }
    return "";
}

int ClsDiagConnectionHandleStart::getAPNumber() {
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	return dynamic_cast<ClsDiagItemAP*>(parentItem())->getNumber();
    }
    return -1;
}


void ClsDiagConnectionHandleStart::refresh(){
    qrectParentBR.setSize(parentItem()->boundingRect().size());

    qrectParentBR.setX(0);
    qrectParentBR.setY(0);

    setPos(qrectParentBR.center());
    if(dynamic_cast<ClsDiagItemAP*>(parentItem())){
	iOrientation = dynamic_cast<ClsDiagItemAP*>(parentItem())->getOrientation();
	setOrientation(iOrientation);
    }
    QPointF qpHere = parentItem()->scenePos() + qrectParentBR.center();	    
    
    QGraphicsLineItem *qlinePost = parentConnection->getFirstSegment();
    if(qlinePost!=NULL){
	if(iOrientation == ClsDiagConnection::WEST_EAST){
	    qlinePost->setLine(qpHere.x() - qrectMyBR.height(), qpHere.y() - 1, qlinePost->line().p2().x(), qlinePost->line().p2().y()); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::EAST_WEST){
	    qlinePost->setLine(qpHere.x() + qrectMyBR.height(), qpHere.y(), qlinePost->line().p2().x(), qlinePost->line().p2().y()); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::NORTH_SOUTH){
	    qlinePost->setLine(qpHere.x(), qpHere.y() - qrectMyBR.height(), qlinePost->line().p2().x(), qlinePost->line().p2().y());
	} else if(iOrientation == ClsDiagConnection::SOUTH_NORTH){
	    qlinePost->setLine(qpHere.x() - 1, qpHere.y() + qrectMyBR.height(), qlinePost->line().p2().x(), qlinePost->line().p2().y());
	}
    }
    qlinePost->setZValue(0);	    
    setZValue(10);	    
}


QVariant ClsDiagConnectionHandleStart::itemChange ( GraphicsItemChange change, const QVariant & value ){




    if(change == QGraphicsItem::ItemParentHasChanged  && scene()){



	if(parentItem()!=NULL){
	    refresh();
	} else {
	    setSourceID("");
	    QGraphicsLineItem *qlinePost = parentConnection->getFirstSegment();
	    if(qlinePost!=NULL){
		QPointF p = qlinePost->line().p1();
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

void ClsDiagConnectionHandleStart::setSourceID(string _strSourceID) {
    strSourceID = _strSourceID;
    parentConnection->setSourceID(_strSourceID);
}



void ClsDiagConnectionHandleStart::mousePressEvent ( QGraphicsSceneMouseEvent * event){
//    cout << "ClsDiagConnectionHandleStart::mousePressEvent ( QGraphicsSceneMouseEvent * event)" << endl;

    if(dynamic_cast<ClsScene*>(scene())->getCanvasType()==ClsScene::CANVAS_PROCESS){
	QMimeData *data = new QMimeData;
	QString qstr = QString("HandleStart ") + strID.c_str() + QString(" ") + parentConnection->connTypeAsString().c_str();
	data->setText(qstr);
	
	QDrag *drag = new QDrag(event->widget());
	drag->setMimeData(data);
	drag->exec();
    } else {
	event->ignore();
    }


}


void ClsDiagConnectionHandleStart::moveChildren (){
//    cout << "ClsDiagConnectionHandleStart::moveChildrenBy ( double dx, double dy)" << endl;
    
    QPointF qpHere = parentItem()->scenePos() + qrectParentBR.center();

    QGraphicsLineItem *qlinePost = parentConnection->getFirstSegment();
    if(qlinePost!=NULL){
	if(iOrientation == ClsDiagConnection::WEST_EAST){
	    qlinePost->setLine(qpHere.x() - qrectMyBR.height(), qpHere.y() -1 , qlinePost->line().p2().x(), qlinePost->line().p2().y()); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::EAST_WEST){
	    qlinePost->setLine(qpHere.x() + qrectMyBR.height(), qpHere.y(), qlinePost->line().p2().x(), qlinePost->line().p2().y()); /* height because we are rotated! */
	} else if(iOrientation == ClsDiagConnection::NORTH_SOUTH){
	    qlinePost->setLine(qpHere.x(), qpHere.y() - qrectMyBR.height(), qlinePost->line().p2().x(), qlinePost->line().p2().y());
	} else if(iOrientation == ClsDiagConnection::SOUTH_NORTH){
	    qlinePost->setLine(qpHere.x() - 1, qpHere.y() + qrectMyBR.height(), qlinePost->line().p2().x(), qlinePost->line().p2().y());
	}
    }



}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:



