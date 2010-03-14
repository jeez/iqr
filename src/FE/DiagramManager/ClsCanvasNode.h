#ifndef CLSCANVASGROUP_H
#define CLSCANVASGROUP_H

#include <math.h>
#include <string> 
#include <vector> 
#include <qcanvas.h> 
#include <qfont.h>
#include <qcolor.h>
#include <qfontmetrics.h> 


#include "ClsCanvasConnection.h"
#include "ClsCanvasNodeAP.h"
#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasHandleStart.h"
#include "ClsFESettings.h"

using namespace std;

static const int nodeRTTI = 984374;

#define APSIZE 8
#define APCOUNT 4
#define TEXTOFFSET 5.0



class ClsCanvasNode :  public QCanvasRectangle {
public:

    ClsCanvasNode ( int x, int y, int width, int height, QCanvas * _canvas): 
	QCanvasRectangle(x,y,width,height,_canvas) {
	qcanvasText = new QCanvasText(this->canvas());
	doFont();
    };
    
    
    ClsCanvasNode ( int _iType, int _x, int _y, int _width , int _height, QCanvas * _canvas, string _strName, string _strID): 
	QCanvasRectangle(_x,_y,_width,_height,_canvas), strName(_strName), strID(_strID), iType(_iType) {
	qcanvasText = new QCanvasText(this->canvas());
	doFont();
    }
    
    virtual ~ClsCanvasNode() {
//	cout << "ClsCanvasNode::~ClsCanvasNode()" << endl;
	delete qcanvasText;
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    delete vectorAPs[ii];
	}
     };

    virtual int rtti () const { return nodeRTTI; }

    void doFont(){

	string strFontName = iqrfe::ClsFESettings::instance().getBlockDiagramFontName();
	int iFontSize = iqrfe::ClsFESettings::instance().getBlockDiagramFontSize();

	QFont myFont(strFontName.c_str(), iFontSize, QFont::Normal);
	qcanvasText->setFont(myFont);
    }

    virtual void setSelected(bool b ) {
//	cout << "ClsCanvasNode::setSelected(bool b ):b:"<< (b == true ? "true" : "false") << endl;
	if(b){
// 	    setBrush(qbrushSelected  );
 	    setPen(qpenSelected );
	} else {
// 	    setBrush(qbrushNotSelected  );
 	    setPen(qpenNotSelected );
	}
	QCanvasItem::setSelected(b);
    };

    
    virtual void moveBy ( double dx, double dy ){
	QCanvasRectangle::moveBy( dx, dy );
	qcanvasText->moveBy( dx, dy );

	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    vectorAPs[ii]->moveBy( dx, dy );
	}

    };

    virtual void drawLabel() {
//	cout << "ClsCanvasNode::drawLabel()" << endl;
	
	int iX = this->rect().left();
	int iY = this->rect().top();
	
//	qcanvasText = new QCanvasText(this->canvas());
	
	qcanvasText->setText("");
	QString qstrTemp = "";
	QString qstrLabel = strName.c_str();
	
	QFontMetrics qfontMetrics(qcanvasText->font());
	int iInsertCounter = 0;
 	for(unsigned int ii=0; ii<strName.length(); ii++){
	    qstrTemp.append(strName.at(ii));
	    if(qfontMetrics.width(qstrTemp, -1) >= (this->width() - 2.5 * TEXTOFFSET)){
		qstrLabel.insert(ii+iInsertCounter, "\n");
		qstrTemp = "";
		iInsertCounter++;
	    }
	}
	
	qcanvasText->setText(qstrLabel);
	qcanvasText->move(iX+TEXTOFFSET,iY+TEXTOFFSET);
	qcanvasText->setZ(1);
	qcanvasText->show();
    };
    
    virtual void drawAPS() {
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

	int iX = this->rect().left();
	int iY = this->rect().top();
	
	double dAPdistX = (double)width() / (double)(APCOUNT + 1);
	double dAPdistY = (double)height() / (double)(APCOUNT + 1);

        /* point 0 - 3 */
	for(int ii = 1; ii<= APCOUNT; ii++){
	    ClsCanvasNodeAP *qcr = new ClsCanvasNodeAP(canvas(), strID, ClsCanvasConnection::NORTH_SOUTH);
	    qcr->setSize(APSIZE, APSIZE);
	    qcr->move(iX + dAPdistX * ii - APSIZE / 2, iY - APSIZE / 2);
	    qcr->setBrush(QBrush(QColor(164,195,197), SolidPattern));
	    qcr->setZ(1); qcr->show();
	    vectorAPs.push_back(qcr);
	}

	/* point 4 - 7 */
	for(int ii = 1; ii<= APCOUNT; ii++){
	    ClsCanvasNodeAP *qcr = new ClsCanvasNodeAP(canvas(), strID, ClsCanvasConnection::EAST_WEST);
	    qcr->setSize(APSIZE, APSIZE);
	    qcr->move(iX + width() - APSIZE/2, iY + dAPdistY * ii - APSIZE / 2);
	    qcr->setBrush(QBrush(QColor(164,195,197), SolidPattern));
	    qcr->setZ(1); qcr->show();
	    vectorAPs.push_back(qcr);
	}

	/* point 11 - 8 */
	for(int ii = 1; ii<= APCOUNT; ii++){
	    ClsCanvasNodeAP *qcr = new ClsCanvasNodeAP(canvas(), strID, ClsCanvasConnection::SOUTH_NORTH);
	    qcr->setSize(APSIZE, APSIZE);
	    qcr->move(iX + dAPdistX * ii - APSIZE / 2, iY + height() - APSIZE / 2);
	    qcr->setBrush(QBrush(QColor(164,195,197), SolidPattern));
	    qcr->setZ(1); qcr->show();
	    vectorAPs.push_back(qcr);
	}
	
	/* point 15 - 12 */
	for(int ii = 1; ii<= APCOUNT; ii++){
	    ClsCanvasNodeAP *qcr = new ClsCanvasNodeAP(canvas(), strID, ClsCanvasConnection::WEST_EAST);
	    qcr->setSize(APSIZE, APSIZE);
	    qcr->move(iX - APSIZE/2, iY + dAPdistY * ii - APSIZE / 2);
	    qcr->setBrush(QBrush(QColor(164,195,197), SolidPattern));
	    qcr->setZ(1); qcr->show();
	    vectorAPs.push_back(qcr);
	}

	/* ------------------------ */
    };
    
    string getName(){ return strName;};

    virtual void setName( string str ) {
//	cout << "ClsCanvasNode::setName (string str)" << endl;
	strName = str;
	drawLabel();
    };
    
    
    string getID(){ return strID;};    
    int getType() {return iType;};

    virtual void Hide() {
	QCanvasRectangle::hide();
	qcanvasText->hide();
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    vectorAPs[ii]->hide();
	};
    };
    
    
    virtual QPoint getPosition () {
	return this->rect().topLeft();
    };

    virtual QSize getSize () {
	return this->rect().size();
    };

    virtual void removeAPHandle(QString qstrID) {
//	cout << "ClsCanvasNode::removeAPHandle(QString qstrID):qstrID:" << qstrID << endl;
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    vectorAPs[ii]->removeHandle(qstrID);
	};
    }

    virtual ClsCanvasNodeAP* getAP(unsigned int ii){
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

    virtual int getNumberOfAPS(){
	return vectorAPs.size();
    }

    virtual ClsCanvasNodeAP* getNearestAP(QPoint qp) {
//	cout << "ClsCanvasNodeAP* getNearestAP(QPoint qp)" << endl;
	/* procedure:
	   - define the closest edge
	   - loop through the AP on that edge and find shortest distance
	*/
	
	ClsCanvasNodeAP* clsCanvasNodeAP = NULL;
	double dDistMin = 1000000.0;

//	cout << "qp.x(): " << qp.x()<< endl;
//	cout << "qp.y(): " << qp.y()<< endl;
	

	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    QRect qrect = vectorAPs[ii]->rect();
	    double dX = (double)qrect.center().x();
	    double dY = (double)qrect.center().y();
	    double dDistLocal = sqrt((double)(pow((double)(qp.x() - dX), 2) + pow((double)(qp.y() - dY), 2)));
//	    cout << "ii: " << ii << ", dX : " << dX << ", dY : " << dY  << ", dDistLocal: " << dDistLocal << endl;
	    if(dDistLocal < dDistMin){
		clsCanvasNodeAP = vectorAPs[ii];
//		cout << "winner: " << ii << endl;
		dDistMin = dDistLocal;
	    }
	}
	return clsCanvasNodeAP;
	
    };

    virtual int getAPHandleNumber(QString qstrID) {
//	cout << "@@@@@getAPHandleNumber(QString qstrID):" << qstrID << endl;
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    if(vectorAPs[ii]->hasAPHandle(qstrID)){
//		cout << "@@@found:" << ii << endl;
		return ii;
	    }
	}
	return -1;
    };

    virtual ClsCanvasNodeAP* getNextFreeAP() {
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    if(vectorAPs[ii]->isFree() ){
		return vectorAPs[ii];
	    }
	}
	return vectorAPs[0];
    }


    virtual bool hasActiveAPs() {
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    if(!vectorAPs[ii]->isFree() ){
		return true;
	    }
	}
	return false;
    }


    virtual int countActiveAPs() {
	int ic =0;
	for(unsigned int ii=0;ii<vectorAPs.size();ii++){
	    if(!vectorAPs[ii]->isFree() ){
		ic++;
	    }
	}
	return ic;
    }

    
protected:
    string strName;
    string strID;
    QCanvasText* qcanvasText;

    int iType;

    vector<ClsCanvasNodeAP *> vectorAPs;

    QBrush qbrushSelected; 
    QPen qpenSelected; 
    QBrush qbrushNotSelected; 
    QPen qpenNotSelected; 

};

#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k -j2"
//// End:
