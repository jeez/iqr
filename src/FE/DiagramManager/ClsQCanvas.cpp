#include "ClsQCanvas.h" 
#include "ClsCanvasNodeProcess.h"
#include "ClsCanvasRubberBand.h" 

ClsQCanvas::~ClsQCanvas() {
//    cout << "ClsQCanvas::~ClsQCanvas()" << endl;

    /* let's remove all the left overs... */
    list<QCanvasItem *> lstItemToDelete;
    QCanvasItemList qCanvasItemList = allItems();
    QCanvasItemList::iterator itq;
    for(itq=qCanvasItemList.begin(); itq!=qCanvasItemList.end();itq++){
//	    cout << "rtti: " << (*itq)->rtti() << endl;
	if((*itq)->rtti() == nodeprocessRTTI){
	    lstItemToDelete.push_back((*itq));
	}
    }
    
    list<QCanvasItem *>::iterator itDelete;
    for(itDelete=lstItemToDelete.begin();itDelete!=lstItemToDelete.end();itDelete++){
	delete (*itDelete);
    }
};

ClsQCanvas::ClsQCanvas( QObject * parent, const char * name, string _strID, int _iCanvasType) : 
	QCanvas ( parent, name ), strID(_strID), iCanvasType(_iCanvasType) { 
    clsCanvasRubberBand = new ClsCanvasRubberBand(this);
    };
ClsQCanvas::ClsQCanvas( QObject * parent = 0, string _strID = 0, int _iCanvasType = 0) : 
	QCanvas ( parent ), strID(_strID), iCanvasType(_iCanvasType) { 
    clsCanvasRubberBand = new ClsCanvasRubberBand(this);
    };
    
int ClsQCanvas::getCanvasType() { return iCanvasType; };
string ClsQCanvas::getID() { return strID; };



ClsCanvasRubberBand* ClsQCanvas::getRubberBand(){
    return clsCanvasRubberBand;
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
