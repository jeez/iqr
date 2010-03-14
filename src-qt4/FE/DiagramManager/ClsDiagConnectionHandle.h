#ifndef CLSCANVASHANDLE_H
#define CLSCANVASHANDLE_H

#include <string> 
#include <iostream> 

#include <QGraphicsScene> 
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include <QPainter> 
#include <QMap>
#include <QPixmap>

#include <q3popupmenu.h>

using namespace std;
//using namespace iqrcommon;

static const int handleRTTI = 984378;


#define HANDLESIZE 8

class ClsDiagConnection;

class ClsDiagConnectionHandle :  public QGraphicsPixmapItem {
public:

    ClsDiagConnectionHandle ( ClsDiagConnection *_parent, int x, int y);

    int type () const;
    
    void setSelected(bool b );

    void refresh();
    void remove();
    
    void setConnectionType(int _iConnectionType);
    
    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
//	cout << "ClsDiagConnectionHandle::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )" << endl;
	Q3PopupMenu* qpopupMenu = new Q3PopupMenu( );
	const int iDeleteID = 1000;
	
	qpopupMenu->insertItem( "Delete Handle", iDeleteID, -1 );
	
	if(qpopupMenu->exec( event->screenPos()) == iDeleteID){
	    remove();
	}
    }

    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );



private:
    QPixmap qpmCurrent;
    QPixmap qpmExc, qpmInh, qpmMod, qpmCurrentNS;
    QPixmap qpmExcSelected, qpmInhSelected, qpmModSelected, qpmCurrentSelected;

};




#endif




//// Local Variables: 
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End: 

