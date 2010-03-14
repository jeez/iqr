#ifndef CLSCANVASCONNECTIONSEGMENT_H
#define CLSCANVASCONNECTIONSEGMENT_H

#include <string> 

#include <qcanvas.h> 

static const int connectionsegmentRTTI = 984382;
using namespace std;

class ClsCanvasConnectionSegment : public QCanvasLine {

public:

    ClsCanvasConnectionSegment (QCanvas * _canvas, ClsCanvasConnection *_parent): QCanvasLine(_canvas), parent(_parent) {
    };
    
    string getSourceID( ) { return parent->getSourceID(); };
    string getTargetID( ) { return parent->getTargetID(); };
    string getParentID( ) { return parent->getID(); };
    string getConnectionID( ) { return parent->getConnectionID(); };

    void split(QPoint qpPos){
//	cout  << "ClsCanvasConnectionSegment::split()" << endl;
	parent->split(this, qpPos);
	
    }


    void setSelected(bool b) {
//	cout << "ClsCanvasConnectionSegment::setSelected(bool b)" << endl;
	
	markSegment(b);
	parent->setSelected(b); 
    }

    void setPen(QPen pen){
	qpenNormal = pen;
	qpenSelected = pen;
	qpenSelected.setWidth(pen.width()*1.8);
	QCanvasPolygonalItem::setPen(pen);
    }

    void markSegment(bool b){
//	cout << "ClsCanvasConnectionSegment::markSegment(bool b)" << endl;
	QPen qpen;
	qpen = pen();
	if(b){
	    QCanvasPolygonalItem::setPen(qpenSelected);
	} else {
	    QCanvasPolygonalItem::setPen(qpenNormal);
	}
	QCanvasItem::setSelected(b); 
    }

    
    int rtti () const { return connectionsegmentRTTI; }

private:
    ClsCanvasConnection *parent;
    QPen qpenNormal;
    QPen qpenSelected;

};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
