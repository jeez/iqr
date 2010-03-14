#ifndef CLSCANVASHANDLE_H
#define CLSCANVASHANDLE_H

#include <string> 
#include <qcanvas.h> 
#include <qmap.h>
#include <qpainter.h> 

using namespace std;
using namespace iqrcommon;

static const int handleRTTI = 984378;


#define HANDLESIZE 8

class ClsCanvasConnection;

class ClsCanvasHandle :  public QCanvasRectangle {
public:

    ClsCanvasHandle ( ClsCanvasConnection *_parent, int x, int y, QCanvas * canvas);

    int rtti () const;
    
    void setSelected(bool b );

    void drawShape( QPainter &p );
    void refresh();
    void remove();
    
    void moveBy ( double dx, double dy );
    

    void setConnectionType(int _iConnectionType);
    
private:
    ClsCanvasConnection *parent;
//    QBrush qbrushSelected; 
//    QPen qpenSelected; 

//    QBrush qbrushNotSelected; 
//    QPen qpenNotSelected; 
    QPixmap qpmCurrent;
    QPixmap qpmExc, qpmInh, qpmMod, qpmCurrentNS;
    QPixmap qpmExcSelected, qpmInhSelected, qpmModSelected, qpmCurrentSelected;

};




#endif




//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
