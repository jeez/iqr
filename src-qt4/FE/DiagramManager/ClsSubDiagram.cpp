#include <QResizeEvent>
#include <QFrame>
#include <QPixmap>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPalette>
#include <QColor>

#include "ClsSubDiagram.h"
#include "ClsDiagItemProcess.h"
#include "ClsDiagItemGroup.h"
#include "ClsDiagConnection.h"

#include "ClsBlockDiagram.h"
#include "ClsFESystemManager.h"

#include "LetterP_2_8x12.xpm"
#include "LetterS_2_8x12.xpm"

#include "cursors/new_group_cursor.xpm"
#include "cursors/new_connection_cursor.xpm"
#include "cursors/new_process_cursor.xpm"

//#define DEBUG_CLSSUBDIAGRAM


ClsSubDiagram::ClsSubDiagram(QFrame *parent, ClsBlockDiagram* _clsBlockDiagramParent, int _iID, int _iSpacerHeight) : 
    QFrame(parent, Qt::WDestructiveClose), clsBlockDiagramParent(_clsBlockDiagramParent), iID(_iID), iSpacerHeight(_iSpacerHeight)  {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::ClsSubDiagram(ClsBlockDiagram* clsBlockDiagramParent,const char* name)" << endl;
#endif



    setFrameStyle(  QFrame::WinPanel | QFrame::Raised );
//    setLineWidth (10);
    
    bFocus = false;
    iAddState = -99;
    createCursors();


    clsSceneView = new ClsSceneView(this);
    clsSceneView->setCursor(Qt::ArrowCursor);
    clsSceneView->setTransformationAnchor( QGraphicsView::NoAnchor );
    clsSceneView->setResizeAnchor( QGraphicsView::NoAnchor );


    qtabbar = new QTabBar( this );
    connect(qtabbar, SIGNAL(/*selected(int)*/currentChanged(int) ), SLOT(slotTabChanged(int) ));


    clsclsScenepanner = new ClsPanner(this, clsSceneView);


    qboxlayout= new QVBoxLayout( this );
    qboxlayout->setContentsMargins ( 0, 0, 0, 0 );
    qboxlayout->setSpacing ( 0);

    
    /* it's getting better and better... not the qwgtSpacer needs to exist to ensure
       the tabbar doesn't dissapear, but it needs not have any minimum height specified... */
    qwgtSpacer = new QWidget(this);
    qwgtSpacer->show();
    qwgtSpacer->setMinimumHeight( iSpacerHeight);
    
    qboxlayout->addWidget ( qwgtSpacer, 0,0 );

    qboxlayout->addWidget ( qtabbar, 0,0 );
    qboxlayout->addWidget ( clsSceneView, 10,0 );


    connect(clsSceneView, SIGNAL(sigGotFocus(int,string)), this, SLOT(slotGotFocus(int,string)));


    connect(clsclsScenepanner, SIGNAL(sigScroll(int, int)), this, SLOT(pannerScroll(int, int)));

    qtabbar->show();
    clsSceneView->show();
    clsclsScenepanner->show();
    this->show();


}

void ClsSubDiagram::paintEvent ( QPaintEvent * event ){
    QPainter painter(this);
    QColor qcolorFrame;
    if(bFocus){
	qcolorFrame = Qt::red;
    } else {
	qcolorFrame = paletteBackgroundColor ();
    }
    qDrawPlainRect( &painter, frameRect(), qcolorFrame, lineWidth());
}


void ClsSubDiagram::pannerScroll(int iX, int iY){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::pannerScroll(int iX, int iX): " << iX << ", " << iY << endl;
#endif
    clsSceneView->centerOn(iX, iY);
};



string ClsSubDiagram::getNewConnectionProcessID(){
    return clsBlockDiagramParent->getNewConnectionProcessID();
};




void ClsSubDiagram::slotTabChanged(int ii){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::slotTabChanged(): " << ii << endl;
#endif
    

    if(qtabbar->currentIndex()!= ii){
	qtabbar->setCurrentTab (ii);
    }
    
    QString qstr = qtabbar->tabData(ii).toString();
    
    ClsScene *clsSceneTemp = clsBlockDiagramParent->getCanvas(qstr);
    if(clsSceneTemp!=NULL){
	clsSceneView->setScene(clsSceneTemp);
	clsclsScenepanner->setScene(clsSceneTemp);
	
	int iType = clsSceneTemp->getCanvasType();
	string strID = clsSceneTemp->getID();
	setCursor(iAddState, iType);
	slotGotFocus(iType, strID);
    } else {
//	cout << "++SCENE NULL" << endl;
    }


}


void ClsSubDiagram::resizeEvent ( QResizeEvent * ) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::resizeEvent ( QResizeEvent * )" << endl;
#endif

    int iOffSet = 16;

    QScrollBar *qsV = clsSceneView->verticalScrollBar();
    if(qsV!=NULL){
	iOffSet = qsV->width() + frameWidth ();
    }
    clsclsScenepanner->move(width() - clsclsScenepanner->width() - iOffSet, height() - clsclsScenepanner->height() -iOffSet);

/*
  clsclsScenepanner->move(clsSceneView->viewport()->width() - clsclsScenepanner->width(), 
  clsSceneView->viewport()->height() - clsclsScenepanner->height());
*/


}

void ClsSubDiagram::addTab(int iType, string strName, string strID) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::addTab(int iType, string strName, string strID)" << endl;
#endif

    int iT;
    if(iType == ClsFESystemManager::ITEM_SYSTEM){
	iT = qtabbar->addTab(QIcon(QPixmap(LetterS_2)), strName.c_str());
    }
    else if(iType == ClsFESystemManager::ITEM_PROCESS){
	iT = qtabbar->addTab(QIcon(QPixmap(LetterP_2)), strName.c_str());
	
    }
    
//    cout << "ClsSubDiagram::iT" <<  iT << endl;
//    cout << "ClsSubDiagram::strID" <<  strID << endl;

    qtabbar->setTabData ( iT, QVariant(strID.c_str()));
    qtabbar->repaint();

};

void ClsSubDiagram::removeTab( string strID) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::removeTab( string strID): " << strID << endl;
#endif

    for(int ii = 0; ii<qtabbar->count(); ii++){
	QString qstrTab = qtabbar->tabData(ii).toString();
	if(!strID.compare(qstrTab.toStdString())) {
	    qtabbar->removeTab(ii);
	}
    }
    qtabbar->repaint();
};


void ClsSubDiagram::renameTab( string strID, string strName) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::renameTab( string strID, string strName)" << endl;
#endif

    for(int ii = 0; ii<qtabbar->count(); ii++){
	QString qstrTab = qtabbar->tabData(ii).toString();
	if(!strID.compare(qstrTab.toStdString())) {
	    qtabbar->setTabText(ii, strName.c_str());
	}
    }
    qtabbar->repaint();
};



void ClsSubDiagram::useSpacer(bool b) {
    if(b){ qwgtSpacer->setMinimumHeight( iSpacerHeight); }
    else { qwgtSpacer->setMinimumHeight( 0); }
};

QGraphicsScene *ClsSubDiagram::scene(){
    return clsSceneView->scene();
};

void ClsSubDiagram::setScene(ClsScene *q) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::setScene(ClsScene *q)" << endl;
#endif

    clsSceneView->setScene(q);
    clsclsScenepanner->setScene(q);

//    CHECK CURSOR???

};

void ClsSubDiagram::zoomIn(){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::zoomIn()" << endl;
#endif
    QMatrix m = clsSceneView->matrix();
    m.scale( 1.1111, 1.1111 );
    clsSceneView->setMatrix( m );
    clsclsScenepanner->canvasResized();
};

void ClsSubDiagram::zoomOut() {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::zoomOut()" << endl;
#endif
    QMatrix m = clsSceneView->matrix();
    m.scale( 0.9, 0.9 );
    clsSceneView->setMatrix( m );
    clsclsScenepanner->canvasResized();
};



void ClsSubDiagram::showProcessTab(string strProcessID) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::showProcessTab(string strProcessID)" << endl;
#endif

    QString qstr = strProcessID.c_str();

    ClsScene *clsSceneTemp = clsBlockDiagramParent->getCanvas(qstr);
    if(clsSceneTemp!=NULL){
	clsSceneView->setScene(clsSceneTemp);

	clsclsScenepanner->setScene(clsSceneTemp);
    }
    else{
//	cout << "CANVAS NOT FOUND" << endl;
    }

    for(int ii = 0; ii<qtabbar->count(); ii++){
	QString qstrTab = qtabbar->tabData(ii).toString();
	if(!strProcessID.compare(qstrTab.toStdString())) {
	    if(qtabbar->currentIndex()!= ii){
		qtabbar->setCurrentTab(ii);
	    }
	}
    }


    int iType = clsSceneTemp->getCanvasType();
    string strID = clsSceneTemp->getID();
    emit canvasChanged(iType, strID);


}



void ClsSubDiagram::slotGotFocus(int iType, string strID) {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::slotDiagramCanvasViewGotFocus()" << endl;
#endif

    ClsScene* q = dynamic_cast<ClsScene*>(clsSceneView->scene());

    if(q!=NULL){
	int iType = q->getCanvasType();

//	    if(!bFocus){
	mark(true);
	emit sigGotFocus(iID, iType, q->getID());
//	    }

	if(iType == ClsScene::CANVAS_SYSTEM){
	    emit sigDiagItemActivated(ClsFESystemManager::ITEM_SYSTEM, q->getID());
	} else if(iType == ClsScene::CANVAS_PROCESS){
	    emit sigDiagItemActivated(ClsFESystemManager::ITEM_PROCESS, q->getID());
	}


    }
    bFocus = true;
}


void ClsSubDiagram::mark(bool b){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "void ClsSubDiagram::slotLostFocus()" << endl;
#endif
    bFocus = b;
    repaint();
}

void ClsSubDiagram::createCursors(){
    QPixmap qpmNG( new_group_cursor );
    QPixmap qpmNP( new_process_cursor );
    QPixmap qpmNC( new_connection_cursor );

    qcursorNewGroup = new QCursor( qpmNG, 0, 0); 
    qcursorNewProcess = new QCursor( qpmNP, 0 ,0); 
    qcursorNewConnection = new QCursor( qpmNC, 0 ,0); 
}


void ClsSubDiagram::setCursor(int _iAddState, int _iCanvasType){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::setCursor(int _iAddState, int _iCanvasType): " << _iAddState << ", " << _iCanvasType << endl;
#endif

    if(clsSceneView!=NULL){
	if(_iAddState > 0){
	    if(_iCanvasType == ClsScene::CANVAS_SYSTEM){
		if(_iAddState == ClsBlockDiagram::ADD_NODE){
		    clsSceneView->setCursor(*qcursorNewProcess);
		    clsSceneView->viewport()->setCursor(*qcursorNewProcess);
		} else {
		    clsSceneView->setCursor(Qt::ArrowCursor);
		    clsSceneView->viewport()->setCursor(Qt::ArrowCursor);
		}
	    } else if(_iCanvasType == ClsScene::CANVAS_PROCESS){
		if(_iAddState == ClsBlockDiagram::ADD_NODE){
		    clsSceneView->setCursor(*qcursorNewGroup);
		    clsSceneView->viewport()->setCursor(*qcursorNewGroup);
		} else {
		    clsSceneView->setCursor(*qcursorNewConnection);
		    clsSceneView->viewport()->setCursor(*qcursorNewConnection);
		}
	    } 
	} else {
	    clsSceneView->setCursor(Qt::ArrowCursor);
	    clsSceneView->viewport()->setCursor(Qt::ArrowCursor);
	}
    }

}

void ClsSubDiagram::setAddState(int iState){
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::setAddState(): " << iState << "ID: " << iID << endl;
#endif
    iAddState = iState;
    
    if(clsSceneView->scene()!=NULL){
	int iCanvasType = dynamic_cast<ClsScene*>(clsSceneView->scene())->getCanvasType();
	setCursor(iAddState, iCanvasType);
    }

}

const map<string, int> ClsSubDiagram::getListSelectedItems(){
    return clsSceneView->getListSelectedItems();
//keep   map<string, int> mapSelections;
//keep    if(scene ()!=NULL){
//keep	QList<QGraphicsItem *> lst = clsSceneView->scene ()->selectedItems ();
//keep	QList<QGraphicsItem *>::iterator itSel;
//keep	for(itSel=lst.begin(); itSel!=lst.end(); itSel++){
//keep	    if(dynamic_cast<ClsDiagItemGroup*>((*itSel))){
//keep	    cout << "ClsDiagItemGroup" << endl;
//keep		string strID = dynamic_cast<ClsDiagItem*>((*itSel))->getID();
//keep		pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_GROUP);
//keep	    cout << pairTemp.first << ":" << pairTemp.second << endl;
//keep		mapSelections.insert(pairTemp);
//keep	    }
//keep	    else if(dynamic_cast<ClsDiagItemProcess*>((*itSel))){
//keep	    cout << "ClsDiagItemProcess" << endl;
//keep		string strID = dynamic_cast<ClsDiagItem*>((*itSel))->getID();
//keep		pair<string, int> pairTemp(strID, ClsFESystemManager::ITEM_PROCESS);
//keep		mapSelections.insert(pairTemp);
//keep	    } 
//keep	}
//keep    }
//keep    return mapSelections;
};


void ClsSubDiagram::clear() {
#ifdef DEBUG_CLSSUBDIAGRAM
    cout << "ClsSubDiagram::clear()" << endl;
#endif
    clsSceneView->clear();

//keep    QList<QGraphicsItem*> list = clsSceneView->scene()->items();
//keep    QList<QGraphicsItem*>::Iterator it = list.begin();
//keep    for (; it != list.end(); ++it) {
//keep	if ( *it )
//keep	    delete *it;
//keep    }

};





//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:




