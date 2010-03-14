#ifndef CLSDIAGPHANTOMCONNECTION_H
#define CLSDIAGPHANTOMCONNECTION_H

#include "ClsDiagBaseConnection.h"

using namespace std;

//static const int connection-FIX-RTTI = 984377;

class ClsDiagMetaConnection;


class ClsDiagPhantomConnection :  public ClsDiagBaseConnection {
public:

    ClsDiagPhantomConnection (ClsDiagHyperConnection* _hyperParent, int _iConnectionType,int _iCanvasConnectionType );
    ~ClsDiagPhantomConnection();

    void addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientatio);


    QGraphicsLineItem* getFirstSegment(){
	return clsDiagConnectionSegment;
    };

    QGraphicsLineItem* getLastSegment(){
	return clsDiagConnectionSegment;
    };

    void setSourceID(string _strSourceID); 
    string getSourceID( ); 
    void setTargetID(string _strTargetID); 
    string getTargetID( ); 

    void setConnectionType(int _iConnectionType);


private:
    QGraphicsLineItem* clsDiagConnectionSegment;

};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
