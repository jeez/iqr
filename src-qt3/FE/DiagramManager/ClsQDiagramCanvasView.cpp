#include <qpopupmenu.h>
#include <qdrawutil.h> 


#include "ClsQDiagramCanvasView.h"
#include "ClsInfoConnection.h"
#include "ClsQCanvas.h"
#include "ClsCanvasNodeProcess.h"
#include "ClsCanvasNodeGroup.h"
#include "ClsCanvasConnection.h"
#include "ClsCanvasMetaConnection.h"
#include "ClsCanvasConnectionSegment.h"
#include "ClsCanvasHandle.h"
#include "ClsCanvasNodeAP.h"
#include "ClsCanvasNodePhantom.h"
#include "ClsCanvasHandleEnd.h"
#include "ClsCanvasHandleStart.h"
#include "ClsCanvasRubberBand.h" 
#include "ClsFESystemManager.h"
#include "ClsQDiagramView.h"

#include "cursors/new_group_cursor.xpm"
#include "cursors/new_connection_cursor.xpm"
#include "cursors/new_process_cursor.xpm"

//#define DEBUG_CLSQDIAGRAMCANVASVIEW

#define GRID_SPACING 8

//ClsQDiagramCanvasView::ClsQDiagramCanvasView(ClsQCanvas& c, QWidget* parent,const char* name, WFlags f) : QCanvasView(&c,parent,name,f) {
ClsQDiagramCanvasView::ClsQDiagramCanvasView(ClsQCanvas* c, QWidget* _parent,const char* name, WFlags f) : QCanvasView(c,_parent,name,f) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::ClsQDiagramCanvasView(ClsQCanvas* c, QWidget* parent,const char* name, WFlags f)" << endl;
#endif
    iSelected = 0;
    iAddState = 0;
    bInfoMode = false;
    clsCanvasNodeAPSource = 0;
    moving = NULL;

/* RubberBand business  */
    clsCanvasRubberBand=NULL;

    qcolorFrame = paletteBackgroundColor ();
    setLineWidth( 1 );
//    setMouseTracking(true);
    createCursors();

}

ClsQDiagramCanvasView::ClsQDiagramCanvasView(QWidget* _parent,const char* name) : QCanvasView(_parent,name) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::ClsQDiagramCanvasView(QWidget* parent,const char* name)" << endl;
#endif
    iSelected = 0;
    iAddState = 0;
    bInfoMode = false;
    clsCanvasNodeAPSource = 0;
    moving = NULL;
/* RubberBand business */
    clsCanvasRubberBand=NULL;


    qcolorFrame = paletteBackgroundColor ();
    setLineWidth( 1 );
//    setMouseTracking(true);
    createCursors();
}


void ClsQDiagramCanvasView::createCursors(){
    QPixmap qpmNG( new_group_cursor );
    QPixmap qpmNP( new_process_cursor );
    QPixmap qpmNC( new_connection_cursor );

    qcursorNewGroup = new QCursor( qpmNG, 0, 0); 
    qcursorNewProcess = new QCursor( qpmNP, 0 ,0); 
    qcursorNewConnection = new QCursor( qpmNC, 0 ,0); 
}



void ClsQDiagramCanvasView::setAddState(int _iAddState) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::setAddState(int _iAddState)" << endl;
#endif
    
//    cout << __FILE__ << ":" << __LINE__ << "((ClsQCanvas*)canvas())->getCanvasType(): " << ((ClsQCanvas*)canvas())->getCanvasType() << endl;

    if(iCanvasType== ClsQCanvas::CANVAS_SYSTEM){
	/* we don't allow new connections at the system level */
	if( _iAddState < ClsQDiagramCanvasView::ADD_NODE ){
	    iAddState = 0;
	    QCanvasView::setCursor(QCursor(ArrowCursor));
	} else {
	    iAddState = _iAddState;
//	    QCanvasView::setCursor(QCursor(CrossCursor));
	    QCanvasView::setCursor(*qcursorNewProcess);
	}
    } 
    /* if we're not at the system level, all adding is permitted */
    else {
	iAddState = _iAddState;
	QCanvasView::setCursor(QCursor(CrossCursor));

	if( _iAddState < ClsQDiagramCanvasView::ADD_NODE){ /* i.e. add connection */
	    QCanvasView::setCursor(*qcursorNewConnection);
	} else { /* i.e. group */
	    QCanvasView::setCursor(*qcursorNewGroup);
	}
    }
};

// void ClsQDiagramCanvasView::setCanvasType(int i) { 
//     cout << "ClsQDiagramCanvasView::setCanvasType(int i)" << endl;
//     cout << "iCanvasType: " << iCanvasType << endl;
//     cout << "iAddState: " << iAddState << endl;
    
//     iCanvasType = i; 
//     if(iCanvasType == ClsQCanvas::CANVAS_SYSTEM && 
//        iAddState < ClsQDiagramCanvasView::ADD_NODE ){ 
// 	iAddState = 0;
//     }
// };

void ClsQDiagramCanvasView::setItemInfoMode(bool b){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::setItemInfoMode(bool b)" << endl;
#endif

    bInfoMode = b;
    iAddState = 0;

    if(bInfoMode){
	QCanvasView::setCursor(QCursor(WhatsThisCursor));
    } else {
	QCanvasView::setCursor(QCursor(ArrowCursor));
    }
    
}


void ClsQDiagramCanvasView::contentsMouseReleaseEvent ( QMouseEvent * ){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::contentsMouseReleaseEvent ( QMouseEvent * )" << endl;
#endif
    /* would be nice to add the resizing of the canvas here ... 
       I guess it's more effective than in the MouseMove slot */

//    cout << (moving==NULL ? "moving==NULL" : "moving!=NULL") << endl;
//    cout << "lstSelections.size(): " << lstSelections.size() << endl;


    if(moving!=NULL){
	int iRight = moving->boundingRect().right();
	int iBottom = moving->boundingRect().bottom();
	if(iRight > canvas()->width() && iRight < MAX_X_CANVAS ){
	    canvas()->resize(iRight + 20, canvas()->height() );
	    emit sigCanvasEnlarged();
	}

	if(iBottom > canvas()->height() && iBottom < MAX_Y_CANVAS ){
	    canvas()->resize(canvas()->width(), iBottom + 20 );
	    emit sigCanvasEnlarged();
	}
    }

    
    if ( moving ) {
	if(iCanvasType!= ClsQCanvas::CANVAS_SYSTEM){
	    bool bAPContact = false;
	    if(moving->rtti()==handleendRTTI || moving->rtti()==handlestartRTTI){
		ClsCanvasNodeAP *clsCanvasNodeAP = NULL;
		QCanvasItemList l=moving->collisions(true );
		for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
//		    if((*it)->rtti() == nodeAPRTTI){
		    if((*it)->rtti() == nodeAPRTTI || (*it)->rtti() == nodeAPPhantomRTTI){
			clsCanvasNodeAP = (ClsCanvasNodeAP*)(*it); 
			bAPContact = true;
			continue;
		    }
		}
		if(moving->rtti()==handlestartRTTI){
		    if(clsCanvasNodeAP!=NULL && bAPContact){
			clsCanvasNodeAP->addHandleStart((ClsCanvasHandleStart*)moving);
//			cout << "AP Parent: " << clsCanvasNodeAP->getParentID() << endl;
			emit sigConnectionChanged(((ClsCanvasHandleStart*)moving)->getConnectionID(), 
						  clsCanvasNodeAP->getParentID(), 
						  ClsCanvasConnection::CONNECTION_SOURCE,
						  ((ClsCanvasHandleStart*)moving)->getCanvasConnectionType());
		    } else {
			emit sigConnectionChanged(((ClsCanvasHandleStart*)moving)->getConnectionID(), "", 
						  ClsCanvasConnection::CONNECTION_SOURCE, 
						  ((ClsCanvasHandleStart*)moving)->getCanvasConnectionType());
		    }
		} else if(moving->rtti()==handleendRTTI){
		    if(clsCanvasNodeAP!=NULL && bAPContact){
			clsCanvasNodeAP->addHandleEnd((ClsCanvasHandleEnd*)moving);
			emit sigConnectionChanged(((ClsCanvasHandleEnd*)moving)->getConnectionID(), 
						  clsCanvasNodeAP->getParentID(), 
						  ClsCanvasConnection::CONNECTION_TARGET,
						  ((ClsCanvasHandleEnd*)moving)->getCanvasConnectionType());
		    } else {
			emit sigConnectionChanged(((ClsCanvasHandleEnd*)moving)->getConnectionID(), 
						  "", 
						  ClsCanvasConnection::CONNECTION_TARGET,
						  ((ClsCanvasHandleEnd*)moving)->getCanvasConnectionType());
		    }
		} 
	    }
	}

	/* first go at implementing a grid */

//	cout << "lstSelections.size(): " << lstSelections.size() << endl;

	QCanvasItemList qcLst = canvas()->allItems();
 	for (QCanvasItemList::Iterator itSel=lstSelections.begin(); itSel!=lstSelections.end(); ++itSel) {
	    if((*itSel)->rtti() == nodeprocessRTTI || 
 	       (*itSel)->rtti() == nodegroupRTTI ||
	       (*itSel)->rtti() == handleRTTI ){
//		if(dynamic_cast<ClsCanvasNodeProcess*>(*itSel) || dynamic_cast<ClsCanvasNode*>(*itSel)){

 		int iXCenter = (*itSel)->boundingRect().left() + ((*itSel)->boundingRect().right() - (*itSel)->boundingRect().left())/2;
 		int iYCenter = (*itSel)->boundingRect().top() +  ((*itSel)->boundingRect().bottom() - (*itSel)->boundingRect().top())/2;
 //		cout << "iYCenter: " << iYCenter << endl;
 //		cout << "iYCenter: " << iYCenter%GRID_SPACING << endl;
		if(qcLst.contains(*itSel)>0){
		    (*itSel)->moveBy(-iXCenter%GRID_SPACING, -iYCenter%GRID_SPACING);
		}
 	    }
 	}
 



//if(dynamic_cast<ClsCanvasNodeProcess*>(*itSel) || dynamic_cast<ClsCanvasNode*>(*itSel)){
	
    } else {
/* RubberBand business */
	if(clsCanvasRubberBand!=NULL){
	    cancelSelections();
	    QValueList<QCanvasItem* > l = clsCanvasRubberBand->getListItemsWithin();
	    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
		(*it)->setSelected(true);

		/* selecting connections makes a mess when moving... */
		if((*it)->rtti() != connectionsegmentRTTI){
		    lstSelections.append((*it));
		    iSelected++;
		}
	    }
	    clsCanvasRubberBand->hide();
	    clsCanvasRubberBand = NULL;
	}
	
    }


}

void ClsQDiagramCanvasView::contentsMouseDoubleClickEvent(QMouseEvent* e) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::contentsMouseDoubleClickEvent(QMouseEvent* e)" << endl;
#endif
    int iType = dynamic_cast<ClsQCanvas*>(canvas())->getCanvasType();
    string strID = dynamic_cast<ClsQCanvas*>(canvas())->getID();
    
    emit sigGotFocus(iType, strID);
    cancelSelections();
    
    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l=canvas()->collisions(p);
    
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
//		cout << "(*it)->rtti(): " << (*it)->rtti() << endl;
	if ( (*it)->rtti() == nodeprocessRTTI || (*it)->rtti() == nodegroupRTTI) {
	    string strID = ((ClsCanvasNode*)(*it))->getID();
	    emit diagramViewDoubleClick(((ClsCanvasNode*)(*it))->getType(), strID, e->globalPos());
	} 
	else if( (*it)->rtti()==connectionsegmentRTTI){
	    /* check if it's a phantom */
	    string strID = ((ClsCanvasConnectionSegment*)(*it))->getConnectionID();
	    emit diagramViewDoubleClick(ClsFESystemManager::ITEM_CONNECTION, strID, e->globalPos());
	} 
	else if( (*it)->rtti()==QCanvasItem::Rtti_Text){
	    continue;
	}
	return;
    }
}




void ClsQDiagramCanvasView::contentsMousePressEvent(QMouseEvent* e) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::contentsMousePressEvent(QMouseEvent* e)" << endl;
#endif

    int iType = dynamic_cast<ClsQCanvas*>(canvas())->getCanvasType();
    string strID = dynamic_cast<ClsQCanvas*>(canvas())->getID();
    
    emit sigGotFocus(iType, strID);
    
    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l=canvas()->collisions(p);
    
    if(l.size()==0){
	cancelSelections();
	moving = 0;
	
	if(iAddState > 0 ){ /* means  that some item should be added */
	    if(e->button() == QMouseEvent::LeftButton){
		
		if(iAddState == ADD_CONNECTION_EXC ||
		   iAddState == ADD_CONNECTION_INH || 
		   iAddState == ADD_CONNECTION_MOD ){
		    if(clsCanvasNodeAPSource!=NULL){
//			cout << "add connection segment" << endl;
			QCanvasLine *l = new QCanvasLine(canvas());
			if(vectorNewConnectionPoints.size()<1){ 
			    l->setPoints ( clsCanvasNodeAPSource->rect().center().x(), clsCanvasNodeAPSource->rect().center().y(), p.x(), p.y() );
			}
			else {
			    QPoint qp;
			    qp.setX(vectorNewConnectionPoints.back()[0]);
			    qp.setY(vectorNewConnectionPoints.back()[1]);
			    
			    l->setPoints ( qp.x(), qp.y(), p.x(), p.y() );
			}
			l->setPen(QPen(black, 1,DotLine));
			l->show();
			listNewLines.push_back(l);
			vector<int> vectorPoint;
			vectorPoint.push_back(p.x()); vectorPoint.push_back(p.y());
			vectorNewConnectionPoints.push_back(vectorPoint);
		    }
		}
		else if( iAddState == ADD_NODE ){
		    if(iCanvasType == ClsQCanvas::CANVAS_SYSTEM){
			string strID = ((ClsQCanvas*)canvas())->getID();
//			cout << "ClsQDiagramCanvasView:sigAddNewProcess.." << endl;
			emit sigAddNewProcess(strID, p.x(), p.y());
		    }
		    else {
			string strPrcID = ((ClsQCanvas*)canvas())->getID();
			emit sigAddNewGroup(strPrcID, p.x(), p.y());
		    }
		    iAddState = 0;
		    QCanvasView::setCursor(QCursor(ArrowCursor));
		    
		} else {
		    iAddState = 0;
		    QCanvasView::setCursor(QCursor(ArrowCursor));
		}
	    }
	    /* this cancels _all_ actions (like drawing a connection */
	    else if(e->button() == QMouseEvent::RightButton){
		slotAbortNewConnection();
	    }
	} else {
	    /* RubberBand business */
//	       clsCanvasRubberBand=(ClsQCanvas*)canvas()->getRubberBand();
	    clsCanvasRubberBand=static_cast<ClsQCanvas*>(canvas())->getRubberBand();
	    clsCanvasRubberBand->setSize(0,0);
	    clsCanvasRubberBand->move(p.x(), p.y());
	    clsCanvasRubberBand->show();
	    

	}
	return;
    } 
    else   /* canvas()->collisions(p) > 0 */ {
	if(e->button() == QMouseEvent::LeftButton){
	    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
		if( (*it)->rtti()==handleRTTI){
		}
		if ( (*it)->rtti() == nodeprocessRTTI || 
		     (*it)->rtti() == nodegroupRTTI ||
		     (*it)->rtti() == handleRTTI /*||
						   (*it)->rtti() == connectionsegmentRTTI*/
		    ) {
//		    cout << "(*it)->isSelected(): " << ((*it)->isSelected() ? "true" : "false" ) << endl;
		    /* if control is pressed, we toggle the selected state */


		    if(bInfoMode){
			string strID = ((ClsCanvasNode*)(*it))->getID();
			emit diagramViewLeftClick(((ClsCanvasNode*)(*it))->getType(), strID, e->globalPos());
			
		    } else {
			if(e->state()==Qt::ControlButton){
			    
			    if((*it)->isSelected()){
				(*it)->setSelected(false);
				lstSelections.remove((*it));
				iSelected--;
			    } 
			    else {
				(*it)->setSelected(true);
				lstSelections.append((*it));
				iSelected++;
			    }
			} else {
			    if(!(*it)->isSelected()){
				cancelSelections();
				(*it)->setSelected(true);
				lstSelections.append((*it));
			    } 
			}
		    }
		    
		} else if( (*it)->rtti()==nodeAPRTTI ){
//		    cout << "(*it)->rtti()==nodeAPRTTI" << endl;
		    if(iAddState > 0){
			string strNewConnectionProcessID = dynamic_cast<ClsQDiagramView*>(parent())->getNewConnectionProcessID();
			string strPrcID = ((ClsQCanvas*)canvas())->getID();
			
			if(strNewConnectionProcessID.compare(strPrcID) /* not the same Process */
			   && strNewConnectionProcessID.length()>0){
			    ClsCanvasNodeAP *clsCanvasNodeAPTarget = (ClsCanvasNodeAP*)(*it);
			    int iConnectionType = -99;
			    if(iAddState==ADD_CONNECTION_EXC){
				iConnectionType = ClsInfoConnection::CONN_EXCITATORY;
			    }
			    else if(iAddState==ADD_CONNECTION_INH){
				iConnectionType = ClsInfoConnection::CONN_INHIBITORY;
			    }
			    else if(iAddState==ADD_CONNECTION_MOD){
				iConnectionType = ClsInfoConnection::CONN_MODULATORY;				
			    }
			    emit sigNewConnectionFinished(clsCanvasNodeAPTarget, iConnectionType);
			    
			} else {
/*			CHECK IF SAME _AND_ LENGTH>0 */
			    if(clsCanvasNodeAPSource==NULL){ /* start a new connection */
				clsCanvasNodeAPSource = (ClsCanvasNodeAP*)(*it);
				emit sigNewConnectionStarted(clsCanvasNodeAPSource);
			    } else { /* finalize new connection */
//				cout << "ClsQDiagramCanvasView::finalize new connection" << endl;
				ClsCanvasNodeAP *clsCanvasNodeAPTarget = (ClsCanvasNodeAP*)(*it);
				int iConnectionType = -99;
				if(iAddState==ADD_CONNECTION_EXC){
				    iConnectionType = ClsInfoConnection::CONN_EXCITATORY;
				}
				else if(iAddState==ADD_CONNECTION_INH){
				    iConnectionType = ClsInfoConnection::CONN_INHIBITORY;
				}
				else if(iAddState==ADD_CONNECTION_MOD){
				    iConnectionType = ClsInfoConnection::CONN_MODULATORY;				
				}
				emit sigNewConnection(clsCanvasNodeAPSource, clsCanvasNodeAPTarget, iConnectionType, vectorNewConnectionPoints);
				iAddState = 0;
				QCanvasView::setCursor(QCursor(ArrowCursor));
				vectorNewConnectionPoints.clear();
				for(list<QCanvasLine*>::iterator it=listNewLines.begin(); it!=listNewLines.end();it++){
				    delete (*it);
				}
				listNewLines.clear();
				clsCanvasNodeAPSource = 0;
			    }
			}
		    }
		    continue;
		} else if( (*it)->rtti()==connectionsegmentRTTI ){
		    if(bInfoMode){
			string strID = ((ClsCanvasConnectionSegment*)(*it))->getConnectionID();
			emit diagramViewLeftClick(ClsFESystemManager::ITEM_CONNECTION, strID, e->globalPos());
		    } else {
			if(e->state()==Qt::ControlButton){
			    // split segment
			    ((ClsCanvasConnectionSegment*)(*it))->split(e->pos());
			} /*
			    else {
			    ((ClsCanvasConnectionSegment*)(*it))->setSelected(true, true);
			    } */
		    }

		    continue;
		}
		else if( (*it)->rtti()==nodephantomRTTI ){
		    cancelSelections();
		    (*it)->setSelected(true);
		    lstSelections.append((*it));
		}
		else if((*it)->rtti()==handlestartRTTI ||
			(*it)->rtti()==handleendRTTI){
		    cancelSelections();
		}
		else if( (*it)->rtti()==QCanvasItem::Rtti_Text || 
			 (*it)->rtti()==QCanvasItem::Rtti_Rectangle ||  
			 (*it)->rtti()==QCanvasItem::Rtti_Line || 
			 (*it)->rtti()==nodegroupIconRTTI ||
			 (*it)->rtti()==nodeAPPhantomRTTI){
		    continue;
		}
		moving = *it;
		moving_start = p;
		return;
	    }
	    moving = 0;

	}
	else if(e->button() == QMouseEvent::RightButton){
	    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
		if ( (*it)->rtti() == nodeprocessRTTI || (*it)->rtti() == nodegroupRTTI) {
		    string strID = ((ClsCanvasNode*)(*it))->getID();
		    list<string> lst;
		    /* tying to find out if some of the selected items are actually Handles */
		    for (QCanvasItemList::Iterator itSel=lstSelections.begin(); itSel!=lstSelections.end(); ++itSel) {
			if(dynamic_cast<ClsCanvasNodeGroup*>((*itSel)) ||
			   dynamic_cast<ClsCanvasNodeProcess*>((*itSel))){
			    lst.push_back(dynamic_cast<ClsCanvasNode*>((*itSel))->getID());
			} 
		    }
		    /* if the current item is not in the list, we clear the list and add the item */
		    if(std::find(lst.begin(), lst.end(), strID)==lst.end()){
			lst.clear();
			lst.push_back(strID);
		    }
		    if(lst.size()==1){
			emit diagramViewRightClick(((ClsCanvasNode*)(*it))->getType(), lst.front(), e->globalPos());
		    } 
		    else if(lst.size()>1){
			emit diagramViewRightClick(((ClsCanvasNode*)(*it))->getType(), lst, e->globalPos());
		    }
		    cancelSelections();
		} 
		else if( (*it)->rtti()==nodephantomRTTI){
		    string strID = ((ClsCanvasNode*)(*it))->getID();
		    string strProcessID = ClsFESystemManager::Instance()->getGroupProcessID(strID);
		    string strProcessName = ClsFESystemManager::Instance()->getProcessName(strProcessID);
		    QPopupMenu* qpopupMenu = new QPopupMenu( this );
		    qpopupMenu->insertItem( "Parent process: " + strProcessName);
		    qpopupMenu->exec( e->globalPos());
		} 

		else if( (*it)->rtti()==connectionsegmentRTTI){
		    /* check if it's a phantom */
		    string strID = ((ClsCanvasConnectionSegment*)(*it))->getConnectionID();
		    emit diagramViewRightClick(ClsFESystemManager::ITEM_CONNECTION, strID, e->globalPos());
		} 
		else if( (*it)->rtti()==handleRTTI){
		    QValueList<QCanvasItem* > lstSelectedHandles;
		    for (QCanvasItemList::Iterator itSel=lstSelections.begin(); itSel!=lstSelections.end(); ++itSel) {
//			if(dynamic_cast<ClsCanvasHandle*>((*itSel))){
			if( (*itSel)->rtti() == handleRTTI){
			    lstSelectedHandles.append((*itSel));
			}
		    }
		    QPopupMenu* qpopupMenu = new QPopupMenu( this );
		    const int iDeleteID = 1000;
		    

		    /* if the current item is not in the list, we clear the list and add the item */
		    if(lstSelectedHandles.find(*it)==lstSelectedHandles.end()){
			lstSelectedHandles.clear();
			lstSelectedHandles.append((*it));
		    }

		    if(lstSelectedHandles.size()==1){
			qpopupMenu->insertItem( "Delete Handle", iDeleteID, -1 );
		    } else {
			qpopupMenu->insertItem( "Delete Handles", iDeleteID, -1 );
		    }
		    
		    switch (qpopupMenu->exec( e->globalPos() )){
		    case iDeleteID:
			for (QCanvasItemList::Iterator itHandles=lstSelectedHandles.begin(); itHandles!=lstSelectedHandles.end(); ++itHandles) {
			    lstSelections.remove(*itHandles);
			    dynamic_cast<ClsCanvasHandle*>(*itHandles)->remove();
			}
			break;
		    default:
			return;
		    }
		}
		else if( (*it)->rtti()==QCanvasItem::Rtti_Text){
		    continue;
		}

		return;
	    }
	    moving = 0;
	}
    }



}

void ClsQDiagramCanvasView::clear() {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::clear()" << endl;
#endif
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
	if ( *it )
	    delete *it;
    }
}

void ClsQDiagramCanvasView::contentsMouseMoveEvent(QMouseEvent* e) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::contentsMouseMoveEvent(QMouseEvent* e)" << endl;
#endif

//    cout << (moving==NULL ? "moving==NULL" : "moving!=NULL") << endl;

    QPoint p = inverseWorldMatrix().map(e->pos());  
    if ( moving ) {

	int iXDiff = p.x() - moving_start.x();
	int iYDiff = p.y() - moving_start.y();
	/* to avoid moving objects beyound the edge of the canvas */
	if( moving->boundingRect().left() + iXDiff <= 0 || moving->boundingRect().right() + iXDiff >= MAX_X_CANVAS){
	    iXDiff = 0;
	}
	
	if( moving->boundingRect().top() + iYDiff <= 0 || moving->boundingRect().bottom() + iYDiff >= MAX_Y_CANVAS){
	    iYDiff = 0;
	}

	if((moving->rtti()!=handlestartRTTI && moving->rtti()!=handleendRTTI ) 
	   || iCanvasType!= ClsQCanvas::CANVAS_SYSTEM){  /* no moving of handles at the system level! */

	    if(iXDiff != 0 || iYDiff != 0){
		if(lstSelections.size()>0){

		    QCanvasItemList qcLst = canvas()->allItems();
		    for (QCanvasItemList::Iterator it=lstSelections.begin(); it!=lstSelections.end(); ++it) {
			/* simple NULL pointer check. Might make things slow though... */
			if(qcLst.contains(*it)>0){
			    (*it)->moveBy(iXDiff, iYDiff);
			}
		    }
		} else {
		    moving->moveBy(iXDiff, iYDiff); // this most likely be a Handle then!
		}
	    }
	}
	

	/* no moving of handles at the system level! */
	if(iCanvasType!= ClsQCanvas::CANVAS_SYSTEM){
//	    cout << "@@@@@@@iCanvasType!= ClsQCanvas::CANVAS_PROCESS" << endl;
 	    /* as soon as we start moving a Handle(Start|End), we'll tell the AP that it lost a child.
 	       The AP will get it back on dropping the Handle(Start|End) onto it */
 	    if(moving->rtti()==handlestartRTTI ){
		//	    ((ClsCanvasHandleStart*)moving)->setConnected(false);
 		((ClsCanvasHandleStart*)moving)->setSourceID("");
 		QCanvasItemList l=moving->collisions(true );
 		for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
 		    if((*it)->rtti() == nodeAPRTTI || (*it)->rtti() == nodeAPPhantomRTTI){
 			QString qstrID = ((ClsCanvasHandleStart*)moving)->getID().c_str();
			//			cout << "qstrID: " << qstrID <<endl;
 			((ClsCanvasNodeAP*)(*it))->removeHandleStart(qstrID);
 		    }
 		    
 		}
 	    }
 	    
 	    if(moving->rtti()==handleendRTTI ){
		//	    ((ClsCanvasHandleEnd*)moving)->setConnected(false);
 		((ClsCanvasHandleEnd*)moving)->setTargetID("");
 		QCanvasItemList l=moving->collisions(true );
 		for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
 		    if((*it)->rtti() == nodeAPRTTI  || (*it)->rtti() == nodeAPPhantomRTTI){
 			QString qstrID = ((ClsCanvasHandleEnd*)moving)->getID().c_str();
 			((ClsCanvasNodeAP*)(*it))->removeHandleEnd(qstrID);
 		    }
 		    
 		}
 	    }
	}
	moving_start = p;
	canvas()->update();
    } else {
/* RubberBand business */
	if(clsCanvasRubberBand!=NULL){
	    QRect r = clsCanvasRubberBand->rect();
	    int iXStart = r.left();
	    int iYStart = r.top();
	    clsCanvasRubberBand->setSize(p.x() - iXStart,
					 p.y() - iYStart);
	}


    }  
//    else {
// 	QPoint p = inverseWorldMatrix().map(e->pos());
// 	if(listNewLines.size() > 0 || iAddState>0){
// 	    QCanvasLine* l = listNewLines.back();
// 	    QPoint qpStart = l->startPoint();
// 	    l->setPoints ( qpStart.x(), qpStart.y(), p.x(), p.y() );
// 	}
//     }
    


}

void ClsQDiagramCanvasView::slotAbortNewConnection(){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::slotAbortNewConnection()" << endl;
#endif
    
    iAddState = 0;
    QCanvasView::setCursor(QCursor(ArrowCursor));
    vectorNewConnectionPoints.clear(); 
		
    list<QCanvasLine*>::iterator it;
    for(it=listNewLines.begin(); it!=listNewLines.end();it++){
	delete (*it);
    }
    listNewLines.clear();
    clsCanvasNodeAPSource = 0;
    emit sigNewConnectionAborted();
}



void ClsQDiagramCanvasView::cancelSelections(){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::cancelSelections()" << endl;
#endif

    QCanvasItemList l=canvas()->allItems();
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == nodeprocessRTTI || 
	     (*it)->rtti() == nodegroupRTTI || 
	     (*it)->rtti() == connectionsegmentRTTI) {
	    ClsCanvasNode *g = (ClsCanvasNode*)(*it);
	    if(g->isSelected()){
		g->setSelected(false);
	    }
	} else if( (*it)->rtti()==QCanvasItem::Rtti_Text){
	    continue;
	}
    }

    QCanvasItemList qcLst = canvas()->allItems();
    iSelected = 0;    
    for (QCanvasItemList::Iterator it=lstSelections.begin(); it!=lstSelections.end(); ++it) {
	/* this is a very simple trick to avoid NULL pointers... */
	if(qcLst.contains(*it)>0){
	    (*it)->setSelected(false);
	} 
    }
    lstSelections.clear();


}

list<string> ClsQDiagramCanvasView::findSelections(){
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::findSelections()" << endl;
#endif
    list<string> lst;
    QCanvasItemList l=canvas()->allItems();
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == nodeprocessRTTI || (*it)->rtti() == nodegroupRTTI) {
	    if(((ClsCanvasNode*)(*it))->isSelected()){
		string strID = ((ClsCanvasNode*)(*it))->getID();
//		    cout << "Selected items" << strID << endl;
		lst.push_back(strID);
	    }
	} else if( (*it)->rtti()==QCanvasItem::Rtti_Text){
	    continue;
	}
    }
    return lst;
}

void ClsQDiagramCanvasView::setCanvas(QCanvas *q) {
#ifdef DEBUG_CLSQDIAGRAMCANVASVIEW
    cout << "ClsQDiagramCanvasView::setCanvas(QCanvas *q)" << endl;
#endif
    
    QCanvasView::setCanvas( q );
//    cout << __FILE__ << ":" << __LINE__ << "((ClsQCanvas*)canvas())->getCanvasType(): " << ((ClsQCanvas*)canvas())->getCanvasType() << endl;
    iCanvasType = ((ClsQCanvas*)canvas())->getCanvasType();
//    cout << "iCanvasType: " << iCanvasType << endl;
//    cout << "iAddState: " << iAddState << endl;
    
    if(iCanvasType == ClsQCanvas::CANVAS_SYSTEM && iAddState < ClsQDiagramCanvasView::ADD_NODE ){ 
 	iAddState = 0;
	QCanvasView::setCursor(QCursor(ArrowCursor));
    }
//    cancelSelections();

    if(bInfoMode){
	QCanvasView::setCursor(QCursor(WhatsThisCursor));
    } else {
	QCanvasView::setCursor(QCursor(ArrowCursor));
    }

}

void ClsQDiagramCanvasView::setFocus(bool b){
    if(b){
	qcolorFrame = Qt::red;
    }
    else {
	qcolorFrame = paletteBackgroundColor ();
    }
    
    QPainter paint;
    paint.begin( this );
    drawFrame( &paint );
    paint.end();
}

void ClsQDiagramCanvasView::drawFrame ( QPainter * p ) {
    qDrawPlainRect( p, frameRect(), qcolorFrame, lineWidth());
}


const map<string, int> ClsQDiagramCanvasView::getListSelectedItems(){
//    cout << "ClsQDiagramCanvasView::getListSelectedItems()" << endl;

    map<string, int> mapSelections;

    for (QCanvasItemList::Iterator itSel=lstSelections.begin(); itSel!=lstSelections.end(); ++itSel) {
	if(dynamic_cast<ClsCanvasNodeGroup*>((*itSel))){
//	    cout << "ClsCanvasNodeGroup" << endl;
	    string strID = dynamic_cast<ClsCanvasNode*>((*itSel))->getID();
	    pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_GROUP);
//	    cout << pairTemp.first << ":" << pairTemp.second << endl;
	    mapSelections.insert(pairTemp);
	}
	else if(dynamic_cast<ClsCanvasNodeProcess*>((*itSel))){
//	    cout << "ClsCanvasNodeProcess" << endl;
	    string strID = dynamic_cast<ClsCanvasNode*>((*itSel))->getID();
	    pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_PROCESS);
	    mapSelections.insert(pairTemp);
	} 
    }
//    cout << "mapSelections.size(): " << mapSelections.size() << endl;
    return mapSelections;
};


//--void ClsQDiagramCanvasView::canvasItemDeleted(QCanvasItem* i){
//--//    cout << "ClsQDiagramCanvasView::canvasItemDeleted(QCanvasItem*)" << endl;
//--
//--    QCanvasItemList::Iterator it = lstSelections.find(i);
//--    if(it!=lstSelections.end()){
//--	lstSelections.remove((*it));
//--    }
//--}
//--


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
