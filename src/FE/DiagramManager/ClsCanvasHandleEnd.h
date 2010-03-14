#ifndef CLSCANVASHANDLEEND_H
#define CLSCANVASHANDLEEND_H

#include <string> 
#include <qcanvas.h> 
#include <qmap.h>

using namespace std;

static const int handleendRTTI = 984379;



class ClsCanvasConnection;

class ClsCanvasHandleEnd :  public QCanvasPolygon {
public:

    
    ClsCanvasHandleEnd ( ClsCanvasConnection *_parent, int x, int y, QCanvas * canvas, string _strID, int iOrientation);

    int rtti () const;
    
//    void setSelected(bool b );

//    bool isSelected();
    
    void moveBy ( double dx, double dy );
    void refresh();
    
    void setOrientation(int iOrientation);
    
    void setTargetID(string _strTargetID);

//    void setConnected(bool b);
    string getID() { return strID; }
    
    string getConnectionID();
    int getCanvasConnectionType();

    QPoint getReferencePoint(){ 
	QPointArray qpa = this->areaPoints();
	QPoint qpHere = qpa[iReferencePointIndex]; 
	return qpHere;
    }
    
    
private:

    QPointArray /*ClsCanvasHandleEnd::*/constructArrowBrickHead(int x, int y, int iOrientation, int iEdgeLength);
    QPointArray /*ClsCanvasHandleEnd::*/constructArrowHead(int x, int y, int iOrientation, int iEdgeLength);
    ClsCanvasConnection *parent;
    string strID;
    int iOrientation;
    int iReferencePointIndex;
    string strTargetID;


};




#endif




