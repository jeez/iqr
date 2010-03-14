#ifndef CLSCANVASCONNECTION_H
#define CLSCANVASCONNECTION_H

#include <algorithm> //MARK
#include <iostream> 
#include <string> 
#include <vector> 
#include <qcanvas.h> 
#include <qmap.h>

//#define DEBUG_CLSCANVASCONNECTION
using namespace std;

class ClsCanvasHandle;
class ClsCanvasHandleStart;
class ClsCanvasHandleEnd;
class ClsCanvasConnectionSegment;
class ClsCanvasMetaConnection;

static const int connectionRTTI = 984377;

class ClsCanvasConnection :  public QCanvasLine {
public:
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

    enum CANVASCONNTECTION_TYPES{
	LOCAL,
	IP,
	PHANTOM_START,
	PHANTOM_END
    };

    ClsCanvasConnection ( QCanvas * canvas, string _strID, string _strSourceID, string _strTargetID, int _iConnectionType,int _iCanvasConnectionType );
    ~ClsCanvasConnection();

    string getID(){ return strID;};    
    string getConnectionID();
    
    int rtti () const;
    void addSegment(QPoint qp0, QPoint qp1, int iPosition, int iOrientatio);
    
    string getTargetID( ) { return strTargetID; };
    string getOldTargetID( ) { return strOldTargetID; };
    void setTargetID(string _strTargetID);

    string getSourceID( ) { return strSourceID; };
    string getOldSourceID( ) { return strOldSourceID; };
    void setSourceID(string _strSourceID);

    void setConnectionType(int _iConnectionType);
    int getConnectionType(){ return iConnectionType; };

    void setMetaConnectionParent(ClsCanvasMetaConnection* p);

    int getCanvasConnectionType(){return iCanvasConnectionType;};

    QCanvasLine *getLine(QString qstrID);

    QCanvasLine* getFirstSegment(){
	if(vectorSegments.size()>0){
	    return vectorSegments[0];
	}
	else {
	    return NULL;
	}
    };

    QCanvasLine* getLastSegment(){
	if(vectorSegments.size()>0){
	    return vectorSegments[vectorSegments.size()-1];
	}
	else {
	    return NULL;
	}
    };
    
    QCanvasLine* getPrevSegment(ClsCanvasHandle* h){
	unsigned int iPos = findHandlePos(h);
	if(iPos<vectorSegments.size()){
	    return vectorSegments[iPos];
	}
	else {
	    return NULL;
	}
	
    };

    QCanvasLine* getNextSegment(ClsCanvasHandle* h){
	unsigned int iPos = findHandlePos(h) + 1;
	if(iPos<vectorSegments.size()){
	    return vectorSegments[iPos];
	}
	else {
	    return NULL;
	}
    };


    void setSelected(bool b);





    ClsCanvasHandleStart* getStartHandle();
    ClsCanvasHandleEnd* getEndHandle();
    
    string getStartHandleID();
    string getEndHandleID();

    int findHandlePos(ClsCanvasHandle* h){
	vector<ClsCanvasHandle*>::iterator pos;
	pos = find(vectorHandles.begin(), vectorHandles.end(), h);
	int iPos = distance(vectorHandles.begin(), pos);
	return iPos;
    }

    int findSegmentPos(QCanvasLine* h){
	vector<QCanvasLine*>::iterator pos;
	pos = find(vectorSegments.begin(), vectorSegments.end(), h);
	int iPos = distance(vectorSegments.begin(), pos);
	return iPos;
    }
    
    void split(QCanvasLine* s, QPoint qpPos);
    void removeHandle(ClsCanvasHandle* h);
    
    vector<ClsCanvasHandle*> getHandles(){
	return vectorHandles;
    }


    vector<vector<int> > getPoints();

    bool isConnected() {
	if(strSourceID.size()>0 && strSourceID.size()>0){
	    return true;
	}
	return false;
    }
    
	    
	    

private:
    void setConnected(bool b); /* should be private */

    QCanvas *qcanvasParent;
    ClsCanvasMetaConnection* clsCanvasMetaConnectionParent;

    vector<QCanvasLine*> vectorSegments;


    vector<ClsCanvasHandle*> vectorHandles;
    
    string strID;
    string strSourceID, strTargetID;
    string strOldSourceID, strOldTargetID;

    int iConnectionType;
    QBrush qbrush; 
    QPen qpen; 
    int iCanvasConnectionType;

    ClsCanvasHandleStart* clsCanvasHandleStart;
    ClsCanvasHandleEnd* clsCanvasHandleEnd;
};


#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
