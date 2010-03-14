#ifndef CLSQCANVAS_H
#define CLSQCANVAS_H

#include <string>
#include <iostream>
#include <list>

#include <QGraphicsScene> 

using namespace std;

class ClsFESystemManager;

class ClsScene :  public QGraphicsScene {
    Q_OBJECT

public:
    enum {
	CANVAS_SYSTEM,
	CANVAS_PROCESS
    };
    
    ClsScene( QObject * parent, string _strID, int _iCanvasType);
    ~ClsScene();
    
    int getCanvasType();
    string getID();


    void mousePressEvent ( QGraphicsSceneMouseEvent * event);



signals:

private:
    string strID;
    int iCanvasType;
};


#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
