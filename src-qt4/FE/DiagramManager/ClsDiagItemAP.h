#ifndef CLSCANVASNODEAP_H
#define CLSCANVASNODEAP_H

#include <string> 
#include <list> 
#include <iostream> 

//--#include <QMap> 
#include <QGraphicsScene> 
#include <QGraphicsRectItem>
#include <QPainter> 
#include <QPixmap> 
#include <QGraphicsPixmapItem>
#include <QVariant>

using namespace std;

class ClsDiagConnection;
class ClsDiagConnectionHandleEnd;
class ClsDiagConnectionHandleStart;

static const int nodeAPRTTI = 984381;
static const int nodeAPPhantomRTTI = 984384;



class ClsDiagItemAP :  public QGraphicsPixmapItem {
public:

    ClsDiagItemAP ( QGraphicsItem * parent, string _strParentID, int _iOrientation, bool _bPhantom = false );

    ~ClsDiagItemAP ( ){
//	cout << "ClsDiagItemAP::~ClsDiagItemAP()" << childItems().size() << endl;
	QList<QGraphicsItem *> lst = childItems ();
	QList<QGraphicsItem *>::iterator it;
	for(it = lst.begin();it!=lst.end(); it++){
	    (*it)->setParentItem(NULL);
	}
	if(scene()){
	    scene()->removeItem ( this );
	}
    }	


    int getOrientation(){ return iOrientation; };
    int type() const { 
	if(bPhantom){
	    return nodeAPPhantomRTTI;
	}
	return nodeAPRTTI; 
    }

    int getNumber();

    bool hasAPHandle(QString qstrID);
    
    void moveChildren( );

    string getParentID(){ return strParentID;};    
   
    bool isFree();
    
    void mousePressEvent ( QGraphicsSceneMouseEvent * );

    void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
    void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
    void dropEvent ( QGraphicsSceneDragDropEvent * event );

    QPointF getCenter(){ return scenePos() + boundingRect().center(); };
    
private:
    string strParentID;
    int iOrientation;
    bool bPhantom;
/*
    int iParentType;
    int iID;
*/
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

