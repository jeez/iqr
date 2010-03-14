#ifndef CLSDIAGCONNECTION_H
#define CLSDIAGCONNECTION_H

#include <algorithm> //MARK
#include <iostream> 
#include <string> 
#include <vector> 

#include "ClsDiagBaseConnection.h"

using namespace std;


class ClsDiagConnectionSegment;

//static const int connectionRTTI = 984377;

class ClsDiagConnection :  public ClsDiagBaseConnection {
public:


    ClsDiagConnection (ClsDiagHyperConnection* _hyperParent, int _iConnectionType,int _iCanvasConnectionType );
    ~ClsDiagConnection();

    void setSourceID(string _strSourceID); 
    string getSourceID( ); 
    void setTargetID(string _strTargetID); 
    string getTargetID( ); 

    void setConnected(bool b);
    void setConnectionType(int _iConnectionType);
    void mark(bool b);



    void addSegment(QPointF qp0, QPointF qp1, int iPosition, int iOrientatio);

    QGraphicsLineItem *getLine(QString qstrID);

    QGraphicsLineItem* getFirstSegment(){
	if(vectorSegments.size()>0){
	    return vectorSegments[0];
	}
	else {
	    return NULL;
	}
    };

    QGraphicsLineItem* getLastSegment(){
	if(vectorSegments.size()>0){
	    return vectorSegments[vectorSegments.size()-1];
	}
	else {
	    return NULL;
	}
    };
    
    QGraphicsLineItem* getPrevSegment(ClsDiagConnectionHandle* h){
	unsigned int iPos = findHandlePos(h);
	if(iPos<vectorSegments.size()){
	    return vectorSegments[iPos];
	}
	else {
	    return NULL;
	}
	
    };

    QGraphicsLineItem* getNextSegment(ClsDiagConnectionHandle* h){
	unsigned int iPos = findHandlePos(h) + 1;
	if(iPos<vectorSegments.size()){
	    return vectorSegments[iPos];
	}
	else {
	    return NULL;
	}
    };


    int findSegmentPos(QGraphicsLineItem* h){
	vector<QGraphicsLineItem*>::iterator pos;
	pos = find(vectorSegments.begin(), vectorSegments.end(), h);
	int iPos = distance(vectorSegments.begin(), pos);
	return iPos;
    }
    
    int findHandlePos(ClsDiagConnectionHandle* h){
	vector<ClsDiagConnectionHandle*>::iterator pos;
	pos = find(vectorHandles.begin(), vectorHandles.end(), h);
	int iPos = distance(vectorHandles.begin(), pos);
	return iPos;
    }


    void split(QGraphicsLineItem* s, QPointF qpPos);
    void removeHandle(ClsDiagConnectionHandle* h);
    
    vector<ClsDiagConnectionHandle*> getHandles(){
	return vectorHandles;
    }

    vector<vector<int> > getPoints();

private:
    vector<QGraphicsLineItem*> vectorSegments;
   vector<ClsDiagConnectionHandle*> vectorHandles;


};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


