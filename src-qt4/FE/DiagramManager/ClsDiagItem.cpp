#include <math.h>
#include <string> 
#include <vector> 

#include "ClsDiagConnection.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsFESettings.h"

#include <ClsFEDialogManager.h>
#include <ClsBlockDiagram.h>

#include <shadow.xpm>


#define APSIZE 8
#define APCOUNT 4
#define TEXTOFFSET 3

//#define DEBUG_CLSDIAGITEM

#include "ClsDiagItem.h"



ClsDiagItem::ClsDiagItem ( int _x, int _y, int _width, int _height, QGraphicsItem * _parent) : 
    QGraphicsRectItem(_x, _y, _width, _height, _parent) {
    qgtiLabel = new QGraphicsTextItem(this);
    qgtiLabel->setTextWidth(this->rect().width());
    doFont();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
};
    
    
ClsDiagItem::ClsDiagItem ( int _iType, int _x, int _y, int _width , int _height, QGraphicsItem * _parent, string _strName, string _strID): 
    QGraphicsRectItem(0, 0, _width, _height, _parent), 
    strName(_strName), strID(_strID), iType(_iType) {
    setPos(_x, _y);
    qgtiLabel = new QGraphicsTextItem(this);
    qgtiLabel->setTextWidth(this->rect().width());
    doFont();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}
    
ClsDiagItem::~ClsDiagItem() {
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::~ClsDiagItem()" << endl;
#endif                     
    delete qgtiLabel;
    for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	delete vectorAPs[ii];
    }
};


void ClsDiagItem::doFont(){

    string strFontName = iqrfe::ClsFESettings::instance().getBlockDiagramFontName();
    int iFontSize = iqrfe::ClsFESettings::instance().getBlockDiagramFontSize();

    QFont myFont(strFontName.c_str(), iFontSize, QFont::Normal);
    qgtiLabel->setFont(myFont);
}

void ClsDiagItem::mark(bool b ) {
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::setSelected(bool b )" << endl;
#endif                     
    if(b){
	setPen(qpenSelected );
    } else {
	setPen(qpenNotSelected );
    }
};


void ClsDiagItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event){
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )" << endl;
#endif
    ClsFEDialogManager::Instance()->createPropertyDialog(iType, strID, event->screenPos());
}

void ClsDiagItem::mousePressEvent ( QGraphicsSceneMouseEvent * event){
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::mousePressEvent ( QGraphicsSceneMouseEvent * )" << endl;
#endif


    if(event->button() == Qt::LeftButton && ClsBlockDiagram::Instance()->infoMode()){
	ClsFEDialogManager::Instance()->createInfoTip(iType, strID, event->screenPos());
    }
    else if (event->button() == Qt::RightButton){
	ClsFEDialogManager::Instance()->createPopup(iType, strID, event->screenPos());
    } else {
	QGraphicsRectItem:: mousePressEvent ( event);
    }
}

    
QVariant ClsDiagItem::itemChange ( GraphicsItemChange change, const QVariant & value ){
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::itemChange ( GraphicsItemChange change, const QVariant & value ):" << change << endl;
#endif
    if (change == QGraphicsItem::ItemPositionHasChanged && scene()) {
//	cout << "QGraphicsItem::ItemPositionChange" << endl;
	/* this needs to be propagated to the APs, so they can move the handle(End/Start) */
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    vectorAPs[ii]->moveChildren();
	}
    } else if(change == QGraphicsItem::ItemSelectedChange){
//	    cout << "QGraphicsItem::ItemSelectedChange" << endl;
	mark(value.toBool() );
    }
    return QGraphicsItem::itemChange(change,value);
}


void ClsDiagItem::drawShadow() {
    qgpiShadow = new QGraphicsPixmapItem (QPixmap(shadow_xpm), this );
    qgpiShadow->setOpacity(.3);
    int iX = this->rect().left()+8;
    int iY = this->rect().top()+8;
    qgpiShadow->setPos(iX ,iY);
}


void ClsDiagItem::drawLabel() {
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::drawLabel()" << endl;
#endif                     
    int iX = this->rect().left();
    int iY = this->rect().top();
    qgtiLabel->setPlainText(strName.c_str());
    qgtiLabel->setPos(iX ,iY);
    qgtiLabel->setZValue(1);
    qgtiLabel->show();

};
    
void ClsDiagItem::drawAPS() {
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::drawAPS()" << endl;
#endif                     

    /* the attachemnt points for the connections 
       -00-01-02-03-
       |           |    
       15           04
       |           |
       14           05
       |           |
       13           06
       |           |
       12           07
       |           |
       -11-10-09-08-
    */    


    int iAPTotalCount = 0;
    int iX = this->rect().left();
    int iY = this->rect().top();
	
    double dAPdistX = rect().width() / (double)(APCOUNT + 1);
    double dAPdistY = rect().height() / (double)(APCOUNT + 1);

    /* point 0 - 3 */
    for(int ii = 1; ii<= APCOUNT; ii++){
	ClsDiagItemAP *qcr = new ClsDiagItemAP(this, strID, ClsDiagConnection::NORTH_SOUTH);
	double fSize = qcr->boundingRect().width(); 
	qcr->setPos(iX + dAPdistX * ii - fSize / 2., iY - fSize / 2.);
	qcr->setZValue(1); 
	qcr->show();
	vectorAPs.push_back(qcr);
	iAPTotalCount++;
    }

    /* point 4 - 7 */
    for(int ii = 1; ii<= APCOUNT; ii++){
	ClsDiagItemAP *qcr = new ClsDiagItemAP(this, strID, ClsDiagConnection::EAST_WEST);
	double fSize = qcr->boundingRect().width(); 
	qcr->setPos(iX + rect().width() - fSize/2, iY + dAPdistY * ii - fSize / 2);
	qcr->setZValue(1); 
	qcr->show();
	vectorAPs.push_back(qcr);
	iAPTotalCount++;

    }

    /* point 11 - 8 */
    for(int ii = 1; ii<= APCOUNT; ii++){
	ClsDiagItemAP *qcr = new ClsDiagItemAP(this, strID, ClsDiagConnection::SOUTH_NORTH);
	double fSize = qcr->boundingRect().width(); 
	qcr->setPos(iX + dAPdistX * ii - fSize / 2, iY + rect().height() - fSize / 2);
	qcr->setZValue(1); 
	qcr->show();
	vectorAPs.push_back(qcr);
	iAPTotalCount++;

    }
	
    /* point 15 - 12 */
    for(int ii = 1; ii<= APCOUNT; ii++){
	ClsDiagItemAP *qcr = new ClsDiagItemAP(this, strID, ClsDiagConnection::WEST_EAST);
	double fSize = qcr->boundingRect().width(); 
	qcr->setPos(iX - fSize/2, iY + dAPdistY * ii - fSize / 2);
	qcr->setZValue(1); 
	qcr->show();
	vectorAPs.push_back(qcr);
	iAPTotalCount++;

    }

    /* ------------------------ */
};
    
    
    
QPointF ClsDiagItem::getPosition () {
#ifdef DEBUG_CLSDIAGITEM 
    cout << "ClsDiagItem::getPosition ()" << endl;
#endif
    return pos();
};

QSizeF ClsDiagItem::getSize () {
    return this->rect().size();
};

QPointF ClsDiagItem::getCenter(){
    return pos()+rect().center();
}



ClsDiagItemAP* ClsDiagItem::getAPbyID(unsigned int ii){
    if(vectorAPs.size()>0){
	if(ii < vectorAPs.size()){
	    return vectorAPs[ii];
	}
	else {
	    return vectorAPs[0];
	}
    }
    else {
	return NULL;
    }
};


ClsDiagItemAP* ClsDiagItem::getNearestAP(QPointF qp) {
#ifdef DEBUG_CLSDIAGITEM 
	cout << "ClsDiagItemAP* getNearestAP(QPointF qp)" << endl;
	cout << "strName: " << strName << endl;
#endif
    /* procedure:
       - define the closest edge
       - loop through the AP on that edge and find shortest distance
    */
	
    ClsDiagItemAP* clsDiagItemAP = NULL;
    double dDistMin = 1000000.0;

//    cout << "qp.x(): " << qp.x()<< endl;
//    cout << "qp.y(): " << qp.y()<< endl;
    for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	QPointF qpAP = vectorAPs[ii]->getCenter();
	double dX = qpAP.x();
	double dY = qpAP.y();
	double dDistLocal = sqrt((double)(pow((double)(qp.x() - dX), 2) + pow((double)(qp.y() - dY), 2)));
//	cout << "ii: " << ii << ", dX : " << dX << ", dY : " << dY  << ", dDistLocal: " << dDistLocal << endl;
	if(dDistLocal < dDistMin){
	    clsDiagItemAP = vectorAPs[ii];
//		cout << "winner: " << ii << endl;
	    dDistMin = dDistLocal;
	}
    }
    return clsDiagItemAP;
	
};


int ClsDiagItem::getAPHandleNumberByAddress(ClsDiagItemAP* ap) {
#ifdef DEBUG_CLSDIAGITEM 
	cout << "getAPHandleNumberByConnID(QString qstrID):" << qstrID << endl;
#endif
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    if(vectorAPs[ii]==ap){
//		cout << "@@@found:" << ii << endl;
		return ii;
	    }
	}
	return -1;
};




ClsDiagItemAP* ClsDiagItem::getNextFreeAP() {
    for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	if(vectorAPs[ii]->isFree() ){
	    return vectorAPs[ii];
	}
    }
    return vectorAPs[0];
}


bool ClsDiagItem::hasActiveAPs() {
    for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	if(!vectorAPs[ii]->isFree() ){
	    return true;
	}
    }
    return false;
}




//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

