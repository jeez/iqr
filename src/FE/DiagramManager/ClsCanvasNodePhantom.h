#ifndef CLSCANVANODEPHANTOM_H
#define CLSCANVANODEPHANTOM_H

#include <string> 
#include <vector> 
#include <qcanvas.h> 
#include <qfontmetrics.h> 
#include <qwmatrix.h> 

#include "ClsCanvasConnection.h"
#include "ClsCanvasNodeAP.h"
#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasHandleStart.h"
#include "ClsCanvasNode.h"

using namespace std;

static const int nodephantomRTTI = 984383;

#define APSIZE 8
#define PHANTOM_APCOUNT 6
#define PHANTOM_TEXTOFFSET 3.0

class ClsCanvasNodePhantom :  public ClsCanvasNode {

public:

    
    ClsCanvasNodePhantom ( int _iType, int _x, int _y, int iWidth, int iHeight, QCanvas * _canvas, string _strName, string _strID  ): 
//	ClsCanvasNode(_x,_y,iWidth,iHeight,canvas), strName(_strName), iType(_iType) {
	ClsCanvasNode ( _iType, _x, _y, iWidth, iHeight, _canvas, _strName, _strID) {

	qpenNotSelected.setColor(QColor(100,100,100));
	qpenNotSelected.setWidth(1);	
	qpenSelected.setColor(QColor(255,90,90));
	qpenSelected.setWidth(2);

	setBrush( QBrush(QColor(230,230,230), SolidPattern));
 	setPen( qpenNotSelected );



 	drawLabel();
	
/* the attachemnt points for the connections 

|                       |
-00-01-02-03-04-05-06-07-
*/    

	double dAPdistX;
	double dAPdistY;
	
	dAPdistX = (double)iWidth / (double)(PHANTOM_APCOUNT + 1);
	dAPdistY = (double)iHeight / (double)(PHANTOM_APCOUNT + 1);

	/* point 00 - 07 */
	for(int ii = 1; ii<= PHANTOM_APCOUNT; ii++){
	    ClsCanvasNodeAP *qcr = new ClsCanvasNodeAP(_canvas, strID, ClsCanvasConnection::SOUTH_NORTH, /*bPhantom*/ true);
	    qcr->setSize(APSIZE, APSIZE);
	    qcr->move(_x + dAPdistX * ii - APSIZE / 2, _y + iHeight - APSIZE / 2);
	    qcr->setBrush(QBrush(QColor(164,195,197), SolidPattern));
	    qcr->setZ(1);
	    qcr->show();
	    vectorAPs.push_back(qcr);
	}
	
	/* ------------------------ */
    };

    

    int rtti () const { return nodephantomRTTI; };
    


//      void drawShape( QPainter &p ) {
// // 	QWMatrix m, m0;                       // our transformation matrix
// //  	m0 = p.worldMatrix();
// //  	m.rotate( 22.5 );                 // rotated coordinate system
// //  	p.setWorldMatrix( m );            // use rotated coordinate system
//  	p.drawText( 0,0, "detator" );   // draw rotated text at 30,20
// //  	m.rotate( -22.5 );                 // rotated coordinate system
// //  	p.setWorldMatrix( m );            // use rotated coordinate system
//      }
    
    ClsCanvasNodeAP* getNearestAP(QPoint qp) {
	/* procedure:
	   - define the closest edge
	   - loop through the AP on that edge and find shortest distance
	*/
	
	ClsCanvasNodeAP* clsCanvasNodeAP = NULL;
	double dDistMin = 1000000.0;
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    QRect qrect = vectorAPs[ii]->rect();
	    double dX = (double)qrect.center().x();
	    double dY = (double)qrect.center().y();
	    double dDistLocal = sqrt((double)(pow((double)(qp.x() - dX), 2) + pow((double)(qp.y() - dY), 2)));
	    if(dDistLocal < dDistMin && vectorAPs[ii]->isFree() ){
		clsCanvasNodeAP = vectorAPs[ii];
		dDistMin = dDistLocal;
	    }
	}
	
	if(clsCanvasNodeAP==NULL){
	    clsCanvasNodeAP = vectorAPs[0];
	}
	
	return clsCanvasNodeAP;
	
    };
    
    void drawLabel() {    
//	cout << "ClsCanvasNodePhantom::drawLabel()" << endl;
	
 	qcanvasText->setText("");

	int iX = this->rect().left();
	int iY = this->rect().top();

  	QString qstrTemp = "";
  	QString qstrLabel = strName.c_str();
//  	QString qstrLabel = strID.c_str();

  	QFontMetrics qfontMetrics(qcanvasText->font());
  	int iInsertCounter = 0;
   	for(unsigned int ii=0; ii<strName.length(); ii++){
  	    qstrTemp.append(strName.at(ii));
  	    if(qfontMetrics.width(qstrTemp, -1) >= (this->width() - 2.5 * PHANTOM_TEXTOFFSET)){
  		qstrLabel.insert(ii+iInsertCounter, "\n");
  		qstrTemp = "";
  		iInsertCounter++;
  	    }
  	}

  	/* this could also be done in the drawShape functio with just using plain text... */
  	qcanvasText->setText(qstrLabel);
  	qcanvasText->move(iX+PHANTOM_TEXTOFFSET,iY+PHANTOM_TEXTOFFSET);
  	qcanvasText->setColor(QColor(100,100,100));
  	qcanvasText->setZ(1);
  	qcanvasText->show();
    };
    
    
    
    void moveBy ( double dx, double ){
//	cout << "ClsCanvasNodePhantom::moveBy ( double dx, double )" << endl;
	if((rect().left()+dx) >= 0){
	    QCanvasRectangle::moveBy( dx, 0 );
	    qcanvasText->moveBy( dx, 0 );
	    for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
		vectorAPs[ii]->moveBy( dx, 0 );
	    }
	}
    };


private:
//    QPen qpen; 

//    int iWidth, iHeight;

//    vector<ClsCanvasNodeAP *> vectorAPs;

};

#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
