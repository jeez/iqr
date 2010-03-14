#ifndef CLSCANVASNODEAP_H
#define CLSCANVASNODEAP_H

#include <string> 
#include <list> 

#include <qcanvas.h> 
#include <qmap.h> 
#include <qpainter.h> 
#include <qpixmap.h> 



using namespace std;

class ClsCanvasConnection;
class ClsCanvasHandleEnd;
class ClsCanvasHandleStart;

static const int nodeAPRTTI = 984381;
static const int nodeAPPhantomRTTI = 984384;


class ClsCanvasNodeAP :  public QCanvasRectangle {
public:

    ClsCanvasNodeAP ( QCanvas * canvas, string _strParentID, int _iOrientation, bool _bPhantom = false );
    int getOrientation(){ return iOrientation; };
    int rtti () const { 
	if(bPhantom){
	    return nodeAPPhantomRTTI;
	}
	return nodeAPRTTI; 
    }
    void removeHandleStart( QString qstID);
    void removeHandleEnd( QString qstID);
    void removeHandle( QString qstID);

    bool hasAPHandle(QString qstrID);
    
    

    void drawShape( QPainter &p );
    void addHandleStart(ClsCanvasHandleStart *_hs);
    void addHandleEnd(ClsCanvasHandleEnd *_he);
    void moveBy ( double dx, double dy );
    string getParentID(){ return strParentID;};    
    
    bool isFree() {
	if(mapHE.size() > 0 || mapHS.size() > 0){
	    return false;
	} else {
	    return true;
	}
	return true;
    };
    

    list<string> getActiveAPHandleIDs(){
	list<string> lst;
	QMap<QString, ClsCanvasHandleEnd*>::Iterator itHE;
	for ( itHE = mapHE.begin(); itHE != mapHE.end(); ++itHE ) {
	    lst.push_back(itHE.key());
	}

	QMap<QString, ClsCanvasHandleStart*>::Iterator itHS;
	for ( itHS = mapHS.begin(); itHS != mapHS.end(); ++itHS ) {
	    lst.push_back(itHS.key());
	}
	return lst;
    }


    list<string> getActiveStartHandleIDs(){
	list<string> lst;
	QMap<QString, ClsCanvasHandleStart*>::Iterator itHS;
	for ( itHS = mapHS.begin(); itHS != mapHS.end(); ++itHS ) {
	    lst.push_back(itHS.key());
	}
	return lst;
    }

    list<string> getActiveEndHandleIDs(){
	list<string> lst;
	QMap<QString, ClsCanvasHandleEnd*>::Iterator itHE;
	for ( itHE = mapHE.begin(); itHE != mapHE.end(); ++itHE ) {
	    lst.push_back(itHE.key());
	}
	return lst;
    }



    
    QMap <QString, ClsCanvasHandleEnd*> mapHE;
    QMap <QString, ClsCanvasHandleStart*> mapHS;

	
    
    
private:
    string strParentID;
    int iOrientation;
    bool bPhantom;

    const char **caAPIcon;

};

#endif

//// Local Variables: 
//// mode: c++
//// End: 
