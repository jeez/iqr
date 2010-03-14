#ifndef CLSCANVASHANDLESTART_H
#define CLSCANVASHANDLESTART_H

#include <string> 

#include <QGraphicsScene> 
#include <QGraphicsEllipseItem>
#include <QPixmap>

using namespace std;

static const int handlestartRTTI = 984380;



class ClsDiagConnection;
class ClsDiagBaseConnection;

class ClsDiagConnectionHandleStart :  public QGraphicsPixmapItem {
public:

    
    ClsDiagConnectionHandleStart ( ClsDiagBaseConnection *_parentConnection, string _strID, int iOrientation);

    int type () const;

    void setSourceID(string _strSourceID);
    string getID(){ return strID;};    
    
    string getConnectionID();
    int getCanvasConnectionType();

    void setConnectionType(int _iConnectionType);
    void setOrientation(int _iOrientation);

    string getAPID();
    int getAPNumber();

    void refresh();

//--    QPointF getReferencePoint(){ 
//--	return pos() + boundingRect().center();
//--    }

    QVariant itemChange( GraphicsItemChange change, const QVariant & value );

    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void moveChildren( );

private:

    QPixmap qpmExc, qpmInh, qpmMod;

    string strID;
    int iOrientation;
    QRectF qrectParentBR;
    QRectF qrectMyBR;

    string strSourceID;
    ClsDiagBaseConnection* parentConnection;
};




#endif




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:




