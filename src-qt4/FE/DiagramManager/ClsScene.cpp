#include "ClsScene.h" 
#include "ClsDiagItemProcess.h"
#include <ClsFESystemManager.h>
#include <ClsBlockDiagram.h>

ClsScene::ClsScene( QObject * parent = 0, string _strID = 0, int _iCanvasType = 0) : 
    QGraphicsScene ( parent ), strID(_strID), iCanvasType(_iCanvasType) { 
};


ClsScene::~ClsScene() {
//    cout << "ClsScene::~ClsScene()" << endl;

    /* let's remove all the left overs... */
    list<QGraphicsItem *> lstItemToDelete;
    QList<QGraphicsItem*> qCanvasItemList = items();


    QList<QGraphicsItem*>::iterator itq;
    for(itq=qCanvasItemList.begin(); itq!=qCanvasItemList.end();itq++){
	if( (*itq)->type() == nodeprocessRTTI){
	    lstItemToDelete.push_back((*itq));
	}
    }
    
    list<QGraphicsItem *>::iterator itDelete;
    for(itDelete=lstItemToDelete.begin();itDelete!=lstItemToDelete.end();itDelete++){
	delete (*itDelete);
    }
};

    
int ClsScene::getCanvasType() { return iCanvasType; };
string ClsScene::getID() { return strID; };


void ClsScene::mousePressEvent ( QGraphicsSceneMouseEvent * event){
//	cout << "ClsScene::mousePressEvent ( QGraphicsSceneMouseEvent * event)" << endl;
	if(event->button() == Qt::RightButton){
	    ClsBlockDiagram::Instance()->mouseRightClickPane(strID, event->scenePos());
	} else if(event->button() == Qt::LeftButton){
	    ClsBlockDiagram::Instance()->mouseLeftClickPane(strID, event->scenePos());
	}
	QGraphicsScene::mousePressEvent (event);
};








//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
