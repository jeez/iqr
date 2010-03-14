#ifndef CLSDIAGBASESCONNECTION_H
#define CLSDIAGBASESCONNECTION_H

#include <algorithm> 
#include <iostream> 
#include <string> 
#include <vector> 
#include <map> 


#include <QGraphicsScene> 
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QMap>

//#define DEBUG_CLSDIAGBASESCONNECTION
using namespace std;

class ClsDiagConnectionHandle;
class ClsDiagConnectionHandleStart;
class ClsDiagConnectionHandleEnd;
class ClsDiagHyperConnection;

static const int connectionRTTI = 984377;

class ClsDiagBaseConnection :  public QGraphicsLineItem {
public:

    enum {
	LOCAL,
	IP,
	PHANTOM_START,
	PHANTOM_END
    };


    enum ORIENTATION {
	UNKNOWN,
	WEST_EAST,
	EAST_WEST,
	NORTH_SOUTH,
	SOUTH_NORTH
    };

    enum SEGMENT_POSITION {
	SEG_START,
	SEG_MIDDLE,
	SEG_END,
	SEG_SINGLE
    };


    enum CONNECTION_ST {
	CONNECTION_SOURCE,
	CONNECTION_TARGET
    };


    ClsDiagBaseConnection ( ClsDiagHyperConnection* _hyperParent, int _iConnectionType,int _iCanvasConnectionType );
    ~ClsDiagBaseConnection();

//--    string getID(){ return strID;};    
    string getConnectionID();
    
    int type () const;
    
    virtual string getTargetID( )  = 0;
    virtual void setTargetID(string _strTargetID) = 0;

    virtual string getSourceID( ) = 0;
    virtual void setSourceID(string _strSourceID) = 0;

    int getConnectionType(){ return iConnectionType; };
    virtual void setConnectionType(int _iConnectionType);



    virtual void addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientatio) = 0;
    virtual QGraphicsLineItem* getFirstSegment() = 0;
    virtual QGraphicsLineItem* getLastSegment() = 0;

    int getCanvasConnectionType(){return iCanvasConnectionType;};

    virtual void mark(bool b);

    ClsDiagConnectionHandleStart* getStartHandle();
    ClsDiagConnectionHandleEnd* getEndHandle();

    string getStartHandleID();
    string getEndHandleID();
    
    int getStartHandleAPNumber();
    int getEndHandleAPNumber();


	    

    static const char* connTypeLocal() {
	return "local";
    }

    static const char* connTypeIP() {
	return "IP";
    }
    static const char* connTypePhantomStart() {
	return "PhantomStart";
    }
    static const char* connTypePhantomEnd() {
	return "PhantomEnd";
    }

    
    string connTypeAsString(){
	if(mapConnTypes.find(iCanvasConnectionType)!=mapConnTypes.end()){
	    return mapConnTypes[iCanvasConnectionType];
	}
	return "";
    }

    virtual void setConnected(bool b);



protected:

    ClsDiagHyperConnection *hyperParent;

    map<int, string> mapConnTypes;
    

    int iConnectionType;
    QBrush qbrush; 
    QPen qpen; 
    int iCanvasConnectionType;

    ClsDiagConnectionHandleStart* clsCanvasHandleStart;
    ClsDiagConnectionHandleEnd* clsCanvasHandleEnd;

    

};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


