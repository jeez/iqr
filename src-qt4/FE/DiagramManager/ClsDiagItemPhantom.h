#ifndef CLSDIAGITEMPHANTOM_H
#define CLSDIAGITEMPHANTOM_H

#include <string> 
#include <vector> 

#include <QGraphicsScene> 
#include <QFontMetrics> 
#include <QMatrix> 

#include "ClsDiagConnection.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsDiagItem.h"

using namespace std;

static const int nodephantomRTTI = 984383;

#define APSIZE 8
#define PHANTOM_APCOUNT 6
#define PHANTOM_TEXTOFFSET 3.0

class ClsDiagItemPhantom :  public ClsDiagItem {

public:

    
    ClsDiagItemPhantom ( int _iType, int _x, int _y, int iWidth, int iHeight, QGraphicsItem * _parent, string _strName, string _strID  ): 
	ClsDiagItem ( _iType, _x, _y, iWidth, iHeight, _parent, _strName, _strID) {

	qpenNotSelected.setColor(QColor(100,100,100));
	qpenNotSelected.setWidth(1);	
	qpenSelected.setColor(QColor(255,90,90));
	qpenSelected.setWidth(2);

	setBrush( QBrush(QColor(230,230,230), Qt::SolidPattern));
 	setPen( qpenNotSelected );

 	drawLabel();
	
/* the attachemnt points for the connections 

|                       |
-00-01-02-03-04-05-06-07-
*/    

	int iX = this->rect().left();
	int iY = this->rect().top();

	double dAPdistX = rect().width() / (double)(PHANTOM_APCOUNT + 1);

	/* point 00 - 07 */
	for(int ii = 1; ii<= PHANTOM_APCOUNT; ii++){
	    ClsDiagItemAP *qcr = new ClsDiagItemAP(this, strID, ClsDiagConnection::SOUTH_NORTH, /*bPhantom*/ true);
	    double fSize = qcr->boundingRect().width(); 
	    qcr->setPos(iX + dAPdistX * ii - fSize / 2., iY + rect().height() - fSize / 2);
	    qcr->setZValue(1);
	    qcr->show();
	    vectorAPs.push_back(qcr);
	}
    };

//    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
//	cout << "ClsDiagItemPhantom::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )" << endl;
//NEED IMPLEMENTING	    string strID = ((ClsDiagItem*)(*it))->getID();
//NEED IMPLEMENTING	    string strProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strID);
//NEED IMPLEMENTING	    string strProcessName = ClsFESystemManager::Instance()->getProcessName(strProcessID);
//NEED IMPLEMENTING	    Q3PopupMenu* qpopupMenu = new Q3PopupMenu( this );
//NEED IMPLEMENTING	    string str = "Parent process: " + strProcessName;
//NEED IMPLEMENTING	    qpopupMenu->insertItem( str.c_str());
//NEED IMPLEMENTING	    qpopupMenu->exec( e->globalPos());
//    }
    

    int type() const { return nodephantomRTTI; };
    

    ClsDiagItemAP* getNearestAP(QPointF qp) {
	/* procedure:
	   - define the closest edge
	   - loop through the AP on that edge and find shortest distance
	*/
	
	ClsDiagItemAP* clsDiagitemAP = NULL;
	double dDistMin = 1000000.0;
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    QRectF qrect = vectorAPs[ii]->boundingRect();
	    double dX = vectorAPs[ii]->pos().x() + (double)qrect.center().x();
	    double dY = vectorAPs[ii]->pos().y() + (double)qrect.center().y();
	    double dDistLocal = sqrt((double)(pow((double)(qp.x() - dX), 2) + pow((double)(qp.y() - dY), 2)));
	    if(dDistLocal < dDistMin && vectorAPs[ii]->isFree() ){
		clsDiagitemAP = vectorAPs[ii];
		dDistMin = dDistLocal;
	    }
	}
	
	if(clsDiagitemAP==NULL){
	    clsDiagitemAP = vectorAPs[0];
	}
	
	return clsDiagitemAP;
	
    };


    
    void drawLabel() {    
	qgtiLabel->setTextWidth(this->rect().width());

	int iX = this->rect().left();
	int iY = this->rect().top();

  	/* this could also be done in the drawShape functio with just using plain text... */
  	qgtiLabel->setPlainText(strName.c_str());
  	qgtiLabel->setPos(iX+PHANTOM_TEXTOFFSET,iY+PHANTOM_TEXTOFFSET);
  	qgtiLabel->setZValue(1);
  	qgtiLabel->show();
    };

    QVariant itemChange ( GraphicsItemChange change, const QVariant & value ){
//	cout << "ClsDiagItemPhantom::itemChange ( GraphicsItemChange change, const QVariant & value )" << endl;
	if ((change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemPositionHasChanged) && scene()) { 
//	    cout << "QGraphicsItem::ItemPositionChange" << endl;

	    QPointF newPos = value.toPointF();
	    newPos.setX(newPos.x()); 
	    newPos.setY(0); /* phantom nodes can only be moved horizontally... */
	    
	    for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
		vectorAPs[ii]->moveChildren();
	    }
	    return newPos;
	}
	return QGraphicsItem::itemChange(change,value);
    }
    

private:


};

#endif




//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

