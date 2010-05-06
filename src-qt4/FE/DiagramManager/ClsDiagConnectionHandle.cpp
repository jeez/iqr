#include <string> 

#include <QGraphicsScene> 
#include <QMap>
#include <QPixmap>


#include "ClsInfoConnection.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagConnection.h"


#include <square8blue_8x8.xpm>
#include <square8green_8x8.xpm>
#include <square8red_8x8.xpm>

#include <square8blueSelected_8x8.xpm>
#include <square8greenSelected_8x8.xpm>
#include <square8redSelected_8x8.xpm>

#include <ConnHandleRed.xpm>
#include <ConnHandleGreen.xpm>
#include <ConnHandleBlue.xpm>

using namespace std;
using namespace iqrcommon;


ClsDiagConnectionHandle::ClsDiagConnectionHandle ( ClsDiagConnection *_parent, int x, int y): 
    QGraphicsPixmapItem(_parent) {

    qpmExc = QPixmap(ConnHandleRed);
    qpmMod = QPixmap(ConnHandleGreen); 
    qpmInh = QPixmap(ConnHandleBlue);

    qpmExcSelected = QPixmap(square8redSelected);
    qpmModSelected = QPixmap(square8greenSelected); 
    qpmInhSelected = QPixmap(square8blueSelected);


    qpmCurrentNS = qpmExc;
    qpmCurrentSelected = qpmExcSelected;
    qpmCurrent = qpmCurrentNS;

    setPixmap ( qpmCurrent);
    setPos(x - boundingRect().width() / 2, y - boundingRect().height() / 2);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);



};

int  ClsDiagConnectionHandle::type () const { return handleRTTI; }
    
void ClsDiagConnectionHandle::setConnectionType(int iConnectionType){
    
    if(iConnectionType == ClsInfoConnection::CONN_EXCITATORY){
	qpmCurrent = qpmExc;
	qpmCurrentNS = qpmExc;
	qpmCurrentSelected = qpmExcSelected;
    }
    else if(iConnectionType == ClsInfoConnection::CONN_INHIBITORY){
	qpmCurrent = qpmInh;
	qpmCurrentNS = qpmInh;
	qpmCurrentSelected = qpmInhSelected;
    }
    else if(iConnectionType == ClsInfoConnection::CONN_MODULATORY){
	qpmCurrent = qpmMod;
	qpmCurrentNS = qpmMod;
	qpmCurrentSelected = qpmModSelected;
    }
    setPixmap ( qpmCurrent);
};


void  ClsDiagConnectionHandle::setSelected(bool b ) {
//    cout << "ClsDiagConnectionHandle::setSelected(bool b )" << endl;
    if(b){
	qpmCurrent = qpmCurrentSelected;
// 	setBrush(qbrushSelected  );
// 	setPen(qpenSelected );
    } else {
	qpmCurrent = qpmCurrentNS;
// 	setBrush(qbrushNotSelected  );
// 	setPen(qpenNotSelected );
    }

    setPixmap ( qpmCurrent);
    QGraphicsPixmapItem::setSelected(b);
    update();
};


void ClsDiagConnectionHandle::refresh ( ){
    QGraphicsLineItem *qlinePre = dynamic_cast<ClsDiagConnection*>(parentItem())->getPrevSegment(this);
    if(qlinePre!=NULL){
	qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), (int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2));
    }
    
    QGraphicsLineItem *qlinePost = dynamic_cast<ClsDiagConnection*>(parentItem())->getNextSegment(this);
    if(qlinePost!=NULL){
	qlinePost->setLine((int)(this->x()+HANDLESIZE/2), (int)(this->y()+HANDLESIZE/2), qlinePost->line().p2().x(), qlinePost->line().p2().y());
    }
};


QVariant ClsDiagConnectionHandle::itemChange ( GraphicsItemChange change, const QVariant & value ){
//    cout << "ClsDiagConnectionHandle::itemChange ( GraphicsItemChange change, const QVariant & value)" << endl;
    
    if (change == ItemPositionChange && scene()) {
	unsetCursor ();
	QPointF newPos = value.toPointF();

	QGraphicsLineItem *qlinePre = dynamic_cast<ClsDiagConnection*>(parentItem())->getPrevSegment(this);
	if(qlinePre!=NULL){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), newPos.x()+HANDLESIZE/2, newPos.y()+HANDLESIZE/2);
	}
	
	QGraphicsLineItem *qlinePost = dynamic_cast<ClsDiagConnection*>(parentItem())->getNextSegment(this);
	if(qlinePost!=NULL){
	    qlinePost->setLine(newPos.x()+HANDLESIZE/2, newPos.y()+HANDLESIZE/2, qlinePost->line().p2().x(), qlinePost->line().p2().y());
	}
    }
    return QGraphicsItem::itemChange(change,value);
}

/* this is strange; so far It seemed not necessary to implement this function.... ulysses:20100504*/
void ClsDiagConnectionHandle:: mouseMoveEvent ( QGraphicsSceneMouseEvent * event ){
	QPointF newPos = pos();

	QGraphicsLineItem *qlinePre = dynamic_cast<ClsDiagConnection*>(parentItem())->getPrevSegment(this);
	if(qlinePre!=NULL){
	    qlinePre->setLine(qlinePre->line().p1().x(), qlinePre->line().p1().y(), newPos.x()+HANDLESIZE/2, newPos.y()+HANDLESIZE/2);
	}
	
	QGraphicsLineItem *qlinePost = dynamic_cast<ClsDiagConnection*>(parentItem())->getNextSegment(this);
	if(qlinePost!=NULL){
	    qlinePost->setLine(newPos.x()+HANDLESIZE/2, newPos.y()+HANDLESIZE/2, qlinePost->line().p2().x(), qlinePost->line().p2().y());
	}
	QGraphicsItem::mouseMoveEvent ( event ) ;
}





void ClsDiagConnectionHandle::remove(  ) {
    dynamic_cast<ClsDiagConnection*>(parentItem())->removeHandle(this);
};


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


