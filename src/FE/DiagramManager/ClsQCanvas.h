#ifndef CLSQCANVAS_H
#define CLSQCANVAS_H

#include <string>
#include <iostream>
#include <list>
#include <qcanvas.h> 

using namespace std;

class ClsCanvasRubberBand;

class ClsQCanvas :  public QCanvas {

public:
    enum {
	CANVAS_SYSTEM,
	CANVAS_PROCESS
    };
    
    ClsQCanvas( QObject * parent = 0, const char * name = 0, string _strID = 0, int _iCanvasType = 0);
    ClsQCanvas( QObject * parent, string _strID, int _iCanvasType);
    ~ClsQCanvas();
    
    int getCanvasType();
    string getID();

    ClsCanvasRubberBand* getRubberBand();

    
private:
    string strID;
    int iCanvasType;
    ClsCanvasRubberBand *clsCanvasRubberBand;
    
};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
