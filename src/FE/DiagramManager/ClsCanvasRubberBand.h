#ifndef CLSCANVASRUBBERBAND_H
#define CLSCANVASRUBBERBAND_H

#include <iostream>
#include <qcanvas.h>

#include "ClsCanvasNodeProcess.h"
#include "ClsCanvasNodeGroup.h"
#include "ClsCanvasHandle.h"
#include "ClsCanvasConnectionSegment.h"
using namespace std;

static const int rubberbandRTTI = 984386;
class ClsCanvasRubberBand :  public QCanvasRectangle {

public:
    ClsCanvasRubberBand (QCanvas * _canvas): QCanvasRectangle(_canvas) {
	setBrush( NoBrush);
	setPen( DotLine);
	setSize(0,0);
	hide();
    }

    virtual int rtti () const { return rubberbandRTTI; }

    QValueList<QCanvasItem* > getListItemsWithin(){
    QCanvasItemList qcListInRect;
    QCanvasItemList qcListColls = this->collisions( true );
    QRect r = this->rect();
    for (QCanvasItemList::Iterator it=qcListColls.begin(); it!=qcListColls.end(); ++it) {
//	cout << "RTTI: " << (*it)->rtti() << endl;
	if((*it)->rtti() !=  rubberbandRTTI){
	    if((*it)->rtti() == nodeprocessRTTI || 
	       (*it)->rtti() == nodegroupRTTI ||
	       (*it)->rtti() == handleRTTI ||
	       (*it)->rtti() == connectionsegmentRTTI){
		
		QRect r = (*it)->boundingRect();
		QCanvasItemList l2;
		bool bCovered = true;
		
		l2 = canvas()->collisions (r.topLeft() );
		if(l2.find(this)==l2.end()){
		    bCovered = false;
		}
		
		l2 = canvas()->collisions ( r.bottomLeft() );
		if(l2.find(this)==l2.end()){
		    bCovered = false;
		}
		
		l2 = canvas()->collisions ( r.topRight() );
		if(l2.find(this)==l2.end()){
		    bCovered = false;
		}
		
		l2 = canvas()->collisions ( r.bottomRight() );
		if(l2.find(this)==l2.end()){
		    bCovered = false;
		}
		
		if(bCovered){
		    qcListInRect.append(*it);
		}
	    }
	}
    }
    return qcListInRect;
    
    }

};



#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
