#ifndef CLSCANVASHANDLESTART_H
#define CLSCANVASHANDLESTART_H

#include <string> 
#include <qcanvas.h> 
#include <qmap.h>

using namespace std;

static const int handlestartRTTI = 984380;



class ClsCanvasConnection;

class ClsCanvasHandleStart :  public QCanvasEllipse {//QCanvasPolygon {
public:

    
    ClsCanvasHandleStart ( ClsCanvasConnection *_parent, int x, int y, QCanvas * canvas, string _strID);

    int rtti () const;
    
    void moveBy ( double dx, double dy );
    void refresh();
    
    
    void setSourceID(string _strSourceID);
//    void setConnected(bool b);
    string getID(){ return strID;};    
    
    string getConnectionID();
    int getCanvasConnectionType();

    QPoint getReferencePoint(){ 
	return boundingRect().center();
    }

private:
    ClsCanvasConnection *parent;
    string strID;
    string strSourceID;


};




#endif




