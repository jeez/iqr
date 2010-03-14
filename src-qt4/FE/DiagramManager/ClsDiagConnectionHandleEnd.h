#ifndef CLSCANVASHANDLEEND_H
#define CLSCANVASHANDLEEND_H

#include <string> 

#include <QGraphicsScene> 
#include <QGraphicsPolygonItem>
#include <QPolygonF>

using namespace std;

static const int handleendRTTI = 984379;



class ClsDiagConnection;
class ClsDiagBaseConnection;

class ClsDiagConnectionHandleEnd :  public QGraphicsPixmapItem {
public:

    
    ClsDiagConnectionHandleEnd ( ClsDiagBaseConnection *_parentConnection, string _strID, int iOrientation);

    int type () const;
    
    void setOrientation(int iOrientation);
    int getOrientation() { return iOrientation;}
    

    void setTargetID(string _strTargetID);
    string getTargetID( ){ return strTargetID;};

    string getID() { return strID; }
    
    string getConnectionID();
    int getCanvasConnectionType();

    void setConnectionType(int _iConnectionType);

//--    QPointF getReferencePoint(){ 
//--	return qrectMyBR.center();
//--    }

    void refresh();

    string getAPID();

    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

    void mousePressEvent ( QGraphicsSceneMouseEvent * event);
    void moveChildren(  );

    int getAPNumber();
private:
    QPixmap qpmExc, qpmInh, qpmMod;


    string strID;
    int iOrientation;
    string strTargetID;
    QRectF qrectParentBR;
    QRectF qrectMyBR;

    ClsDiagBaseConnection* parentConnection;
    

};




#endif




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


