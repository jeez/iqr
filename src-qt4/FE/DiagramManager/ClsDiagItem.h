#ifndef CLSDIAGITEM_H
#define CLSDIAGITEM_H

#include <math.h>
#include <string> 
#include <vector> 

#include <QFont>
#include <QColor>
#include <QFontMetrics> 
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>

#include "ClsDiagConnection.h"
#include "ClsDiagItemAP.h"

using namespace std;

static const int nodeRTTI = 984374;


class ClsDiagItem :  public QGraphicsRectItem {

public:
    
    ClsDiagItem ( int x, int y, int width, int height, QGraphicsItem * _parent);
    
    
    ClsDiagItem ( int _iType, int _x, int _y, int _width , int _height, QGraphicsItem * _parent, string _strName, string _strID);
    
    virtual ~ClsDiagItem();

    virtual int type() const { return nodeRTTI; }


    string getName(){ return strName;};
    virtual void setName( string str ) {
#ifdef DEBUG_CLSDIAGITEM 
	cout << "ClsDiagItem::setName (string str)" << endl;
#endif                     
	strName = str;
	drawLabel();
    };
    
    
    string getID(){ return strID;};    
    int getType() {return iType;};




    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event);
    
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

    virtual void drawShadow();
    virtual void drawLabel();
    virtual void drawAPS();
    void doFont();
    
    virtual void mark(bool b );
    virtual void Hide() {
	QGraphicsRectItem::hide();
	qgtiLabel->hide();
	for(unsigned int ii=0; ii<vectorAPs.size(); ii++){
	    vectorAPs[ii]->hide();
	};
    };
    
    
    virtual QPointF getPosition ();
    virtual QSizeF getSize ();
    QPointF getCenter();


    virtual ClsDiagItemAP* getAPbyID(unsigned int ii);

    virtual ClsDiagItemAP* getNearestAP(QPointF qp);

//--    virtual int getAPHandleNumberByConnID(QString qstrID);
    int getAPHandleNumberByAddress(ClsDiagItemAP* ap);

    virtual ClsDiagItemAP* getNextFreeAP();


    virtual bool hasActiveAPs();
    
    void setColor(string strColor){
	setBrush(QBrush ( QColor(strColor.c_str()), Qt::SolidPattern ));


    }


signals:

    
protected:
    string strName;
    string strID;
    QGraphicsTextItem* qgtiLabel;

    int iType;

    vector<ClsDiagItemAP *> vectorAPs;

    QPen qpenSelected; 
    QPen qpenNotSelected; 

    QGraphicsPixmapItem* qgpiShadow;
};

#endif



//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

