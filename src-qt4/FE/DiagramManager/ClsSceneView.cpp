#include <q3popupmenu.h>

#include <qdrawutil.h> 
#include <QPixmap>
#include <QMouseEvent>
#include <QLinkedList>

#include "ClsSceneView.h"
#include "ClsInfoConnection.h"
#include "ClsScene.h"
#include "ClsDiagItemProcess.h"
#include "ClsDiagItemGroup.h"
#include "ClsDiagConnection.h"
#include "ClsDiagConnectionSegment.h"
#include "ClsDiagConnectionHandle.h"
#include "ClsDiagItemAP.h"
#include "ClsDiagItemPhantom.h"
#include "ClsDiagConnectionHandleEnd.h"
#include "ClsDiagConnectionHandleStart.h"
#include "ClsFESystemManager.h"
#include "ClsSubDiagram.h"


//#define DEBUG_CLSQDIAGRAMCANVASVIEW

#define GRID_SPACING 8


ClsSceneView::ClsSceneView(QWidget* _parent) : QGraphicsView(_parent) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsSceneView::ClsSceneView(QWidget* parent,const char* name)" << endl;
#endif
    setLineWidth( 1 );
    setDragMode (QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);


}


void ClsSceneView::mousePressEvent ( QMouseEvent * event){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsSceneView::mousePressEvent ( QMouseEvent * event)" << endl;
#endif
    if(scene()!=NULL){
	int iType = dynamic_cast<ClsScene*>(scene())->getCanvasType();
	string strID = dynamic_cast<ClsScene*>(scene())->getID();
	emit sigGotFocus(iType, strID);
    }
    QGraphicsView::mousePressEvent (event);
}

void ClsSceneView::clear() { /* DO WE NEED THIS?????*/
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsSceneView::clear()" << endl;
#endif
    QList<QGraphicsItem*> list = scene()->items();
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
	if ( *it )
	    delete *it;
    }
}

const map<string, int> ClsSceneView::getListSelectedItems(){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsSceneView::getListSelectedItems()" << endl;
#endif
    map<string, int> mapSelections;
    if(scene ()!=NULL){
	QList<QGraphicsItem *> lst = scene ()->selectedItems ();
	QList<QGraphicsItem *>::iterator itSel;
	for(itSel=lst.begin(); itSel!=lst.end(); itSel++){
	    if(dynamic_cast<ClsDiagItemGroup*>((*itSel))){
//	    cout << "ClsDiagItemGroup" << endl;
		string strID = dynamic_cast<ClsDiagItem*>((*itSel))->getID();
		pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_GROUP);
//	    cout << pairTemp.first << ":" << pairTemp.second << endl;
		mapSelections.insert(pairTemp);
	    }
	    else if(dynamic_cast<ClsDiagItemProcess*>((*itSel))){
//	    cout << "ClsDiagItemProcess" << endl;
		string strID = dynamic_cast<ClsDiagItem*>((*itSel))->getID();
		pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_PROCESS);
		mapSelections.insert(pairTemp);
	    } 
	    
	}
	
    }
    return mapSelections;
};




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

