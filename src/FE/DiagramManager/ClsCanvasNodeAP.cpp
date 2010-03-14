#include "ClsCanvasNodeAP.h"
#include "ClsCanvasConnection.h"
#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasHandleStart.h"
#include <APIcon_8x8.xpm>
#include <APIconGreen_8x8.xpm>
#include <APIconRed_8x8.xpm>


//#define DEBUG_CLSCANVASNODEAP


ClsCanvasNodeAP::ClsCanvasNodeAP ( QCanvas * canvas, string _strParentID, int _iOrientation, bool _bPhantom ): 
    QCanvasRectangle(canvas), strParentID(_strParentID), iOrientation(_iOrientation), bPhantom(_bPhantom) {

    caAPIcon = APIcon;
//    caAPIcon = APIconGreen;
//    caAPIcon = APIconRed;

};

void ClsCanvasNodeAP::removeHandleStart( QString qstrID) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsCanvasNodeAP::removeHandleStart( QString qstrID)" << endl;
#endif
    if(mapHS.find(qstrID)!=mapHS.end()){
#ifdef DEBUG_CLSCANVASNODEAP
	cout << "parent: " << strParentID << ", HandleStart removed: " << qstrID << endl;
#endif
	caAPIcon = APIconGreen;
	mapHS.remove(qstrID);
    }
}

void ClsCanvasNodeAP::removeHandleEnd( QString qstrID) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsCanvasNodeAP::removeHandleEnd( QString qstrID)" << endl;
#endif

    if(mapHE.find(qstrID)!=mapHE.end()){
#ifdef DEBUG_CLSCANVASNODEAP
	cout << "parent: " << strParentID << ", HandleEnd removed: " << qstrID << endl;
#endif
	caAPIcon = APIconGreen;
	mapHE.remove(qstrID);
    }
}

void ClsCanvasNodeAP::removeHandle( QString qstrID) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsCanvasNodeAP::removeHandle( QString qstrID)" << endl;
#endif
    removeHandleStart( qstrID);
    removeHandleEnd( qstrID);
}

bool ClsCanvasNodeAP::hasAPHandle(QString qstrID) {
    if(mapHE.find(qstrID)!=mapHE.end() || mapHS.find(qstrID)!=mapHS.end() ){
	return true;
    } else {
	return false;
    }
};


    
void ClsCanvasNodeAP::drawShape( QPainter &p ) {
#ifdef DEBUG_CLSCANVASNODEAP
//    cout << "ClsCanvasNodeAP::drawShape( QPainter &p )" << endl;
#endif

//    cout << "ClsCanvasNodeAP::drawShape( QPainter &p )" << endl;

//    p.drawPixmap( int(x()), int(y()), QPixmap(APIcon) );
    p.drawPixmap( int(x()), int(y()), QPixmap(caAPIcon) );
}

void ClsCanvasNodeAP::addHandleStart(ClsCanvasHandleStart *_hs) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsCanvasNodeAP::addHandleStart(ClsCanvasHandleStart *_hs)" << endl;
#endif
    caAPIcon = APIconRed;
    QPoint qpThere = _hs->getReferencePoint();
    double dX = x() - qpThere.x() +(double)width() /2.0;
    double dY = y() - qpThere.y() +(double)height() /2.0;
    _hs->moveBy(dX, dY);
    mapHS[_hs->getID().c_str()] = _hs;

}


void ClsCanvasNodeAP::addHandleEnd(ClsCanvasHandleEnd *_he) {
#ifdef DEBUG_CLSCANVASNODEAP
    cout << "ClsCanvasNodeAP::addHandleEnd(ClsCanvasHandleEnd *_he):_he->getID():" << _he->getID() << endl;
#endif
    caAPIcon = APIconRed;
    _he->setOrientation(iOrientation);
    QPoint qpThere = _he->getReferencePoint();
    double dX = x() - qpThere.x() +(double)width() /2.0;
    double dY = y() - qpThere.y() +(double)height() /2.0;
    _he->moveBy(dX, dY);
    mapHE[_he->getID().c_str()] = _he;
//    cout << "\tADD HandleEnd: " << _he->getID() << endl;

}

void ClsCanvasNodeAP::moveBy ( double dx, double dy ){
    QCanvasRectangle::moveBy( dx, dy );


    for (QMap<QString, ClsCanvasHandleStart*>::Iterator it=mapHS.begin(); it!=mapHS.end(); ++it) {
	QPoint qpThere = (it.data())->getReferencePoint();
	double dX = x() - qpThere.x() +(double)width() /2.0;
	double dY = y() - qpThere.y() +(double)height() /2.0;
	(it.data())->moveBy(dX, dY);
    }
	
    for (QMap<QString, ClsCanvasHandleEnd*>::Iterator it=mapHE.begin(); it!=mapHE.end(); ++it) {
	QPoint qpThere = (it.data())->getReferencePoint();
	double dX = x() - qpThere.x() +(double)width() /2.0;
	double dY = y() - qpThere.y() +(double)height() /2.0;
	(it.data())->moveBy(dX, dY);
    }


};
    

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
