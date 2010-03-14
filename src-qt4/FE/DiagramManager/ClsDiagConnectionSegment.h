#ifndef CLSCANVASCONNECTIONSEGMENT_H
#define CLSCANVASCONNECTIONSEGMENT_H

#include <string> 

#include <QGraphicsScene> 
#include <QGraphicsLineItem>
#include <QGraphicsSceneContextMenuEvent>

#include <ClsFEDialogManager.h>
#include <ClsFESystemManager.h>
#include <ClsBlockDiagram.h>
#include "ClsDiagBaseConnection.h"
#include "ClsDiagConnection.h"


static const int connectionsegmentRTTI = 984382;
using namespace std;



class ClsDiagConnectionSegment : public QGraphicsLineItem {

public:

    ClsDiagConnectionSegment (ClsDiagBaseConnection *_parent): QGraphicsLineItem(_parent), parent(_parent) {
//	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    };
    
    string getSourceID( ) { return parent->getSourceID(); };
    string getTargetID( ) { return parent->getTargetID(); };
    string getParentID( ) { return parent->getConnectionID(); };
    string getConnectionID( ) { return parent->getConnectionID(); };

    void split(QPointF qpPos){
//	cout  << "ClsDiagConnectionSegment::split()" << endl;
	
	if(dynamic_cast<ClsDiagConnection*>(parent)){
	    dynamic_cast<ClsDiagConnection*>(parent)->split(this, qpPos);
	}
    }


    void setSelected(bool b) {
//	cout << "ClsDiagConnectionSegment::setSelected(bool b)" << endl;
	markSegment(b);
	parent->setSelected(b); 
    }

    void setPen(QPen pen){
	qpenNormal = pen;
	qpenSelected = pen;
	qpenSelected.setWidth(pen.width()*1.8);
	QGraphicsLineItem::setPen(pen);
    }

    void markSegment(bool b){
//	cout << "ClsDiagConnectionSegment::markSegment(bool b)" << endl;
	QPen qpen;
	qpen = pen();
	if(b){
	    setPen(qpenSelected); //zzz
	} else {
	    setPen(qpenNormal); //zz
	}
	QGraphicsItem::setSelected(b); 
    }

    
    int type () const { return connectionsegmentRTTI; }

    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
//	cout << "ClsDiagConnectionSegment::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )" << endl;
    }

    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event){
//	cout << "ClsDiagConnectionSegment::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )" << endl;
//	cout << "PARENT==NULL? " << (parent == NULL ? "YES" : "NO") << endl;
//	cout << "parent->getConnectionID(): " << parent->getConnectionID() << endl;
	ClsFEDialogManager::Instance()->createPropertyDialog(ClsFESystemManager::ITEM_CONNECTION, parent->getConnectionID(), event->screenPos());
    }




    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event){
//	cout << "ClsDiagConnectionSegment::mousePressEvent ( QGraphicsSceneMouseEvent * )" << endl;
	if(event->button() == Qt::LeftButton && ClsBlockDiagram::Instance()->infoMode()){
	    ClsFEDialogManager::Instance()->createInfoTip(ClsFESystemManager::ITEM_CONNECTION, parent->getConnectionID(), event->screenPos());
	}
	else if (event->button() == Qt::RightButton){
	    ClsFEDialogManager::Instance()->createPopup(ClsFESystemManager::ITEM_CONNECTION, parent->getConnectionID(), event->screenPos());

	} else if(event->button() == Qt::LeftButton && event->modifiers()==Qt::ControlModifier){
	    split(event->scenePos());
	}


    }


private:
    ClsDiagBaseConnection *parent;
    QPen qpenNormal;
    QPen qpenSelected;

};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
