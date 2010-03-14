#include <qdockwindow.h>
#include <qdockarea.h>

#include "ClsQDiagramView.h"

#include "ClsBlockDiagram.h"
#include "ClsFESystemManager.h"

#include "LetterP_2_8x12.xpm"
#include "LetterS_2_8x12.xpm"

//#define DEBUG_CLSQDIAGRAMVIEW


ClsQDiagramView::ClsQDiagramView(QFrame *parent, ClsBlockDiagram* _clsBlockDiagramParent,const char* name, int _iSpacerHeight) : 
    QFrame(parent,name, WDestructiveClose), clsBlockDiagramParent(_clsBlockDiagramParent), iSpacerHeight(_iSpacerHeight)  {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::ClsQDiagramView(ClsBlockDiagram* clsBlockDiagramParent,const char* name)" << endl;
#endif

    bFocus = false;

    iAddState = 0;
    clsQDiagramCanvasView = new ClsQDiagramCanvasView(this, name);

    qtabbar = new QTabBar( this );
    connect(qtabbar, SIGNAL(selected(int) ), SLOT(slotTabChanged(int) ));

//??????    clsQDiagramCanvasView = new ClsQDiagramCanvasView( this, "");

/* this is just the start of having a panner for the canvas viewer
   qcanvasviewMini = new QCanvasView(clsQDiagramCanvasView);
   qcanvasviewMini->setFixedSize ( QSize(100, 75) );
   qcanvasviewMini->show();
*/

#ifdef USEPANNER
    clsqcanvaspanner = new ClsQCanvasPanner(this, clsQDiagramCanvasView, "", 0 );
//    clsqcanvaspanner = new ClsQCanvasPanner(clsQDiagramCanvasView, clsQDiagramCanvasView, "", 0 );
#endif

//QDockWindow* qdw = new QDockWindow ( clsQDiagramCanvasView);
//    QDockArea* qda = new QDockArea ( Qt::Horizontal, QDockArea::Normal, clsQDiagramCanvasView);



    qboxlayout= new QVBoxLayout( this );

    /* it's getting better and better... not the qwgtSpacer needs to exist to ensure
       the tabbar doesn't dissapear, but it needs not have any minimum height specified... */
     qwgtSpacer = new QWidget(this);
     qwgtSpacer->show();
     qwgtSpacer->setMinimumHeight( iSpacerHeight);

    qboxlayout->addWidget ( qwgtSpacer, 0,0 );

    qboxlayout->addWidget ( qtabbar, 0,0 );
    qboxlayout->addWidget ( clsQDiagramCanvasView, 10,0 );


    connect(clsQDiagramCanvasView, SIGNAL(sigAddNewProcess(string, int, int)),
	    clsBlockDiagramParent, SLOT(createProcess(string, int, int)));

    connect(clsQDiagramCanvasView, SIGNAL(sigAddNewGroup(string, int, int)),
	    clsBlockDiagramParent, SLOT(createGroup(string, int, int)));

    connect(clsQDiagramCanvasView, SIGNAL(sigNewConnection(ClsCanvasNodeAP*, ClsCanvasNodeAP*, int, vector<vector<int> >)),
	    clsBlockDiagramParent, SLOT(slotNewConnection(ClsCanvasNodeAP*, ClsCanvasNodeAP*, int, vector<vector<int> >)));


     connect(clsQDiagramCanvasView, SIGNAL(diagramViewLeftClick(int ,string,  QPoint)),
	     clsBlockDiagramParent, SLOT(slotDiagramViewLeftClicked(int , string, QPoint)));

     connect(clsQDiagramCanvasView, SIGNAL(diagramViewRightClick(int ,string,  QPoint)),
	     clsBlockDiagramParent, SLOT(slotDiagramViewRightClicked(int , string, QPoint)));

     connect(clsQDiagramCanvasView, SIGNAL(diagramViewRightClick(int ,list<string>,  QPoint)),
	     clsBlockDiagramParent, SLOT(slotDiagramViewRightClicked(int , list<string>, QPoint)));

     connect(clsQDiagramCanvasView, SIGNAL(diagramViewDoubleClick(int ,string,  QPoint)),
	     clsBlockDiagramParent, SLOT(slotDiagramViewDoubleClicked(int , string, QPoint)));


     /* stuff needed for inter process connections */
     connect(clsQDiagramCanvasView, SIGNAL(sigNewConnectionStarted(ClsCanvasNodeAP*)),
	     clsBlockDiagramParent, SLOT(slotNewConnectionStarted(ClsCanvasNodeAP*)));

     connect(clsQDiagramCanvasView, SIGNAL(sigNewConnectionFinished(ClsCanvasNodeAP*, int)),
	     clsBlockDiagramParent, SLOT(slotNewConnectionFinished(ClsCanvasNodeAP*, int)));

     connect(clsQDiagramCanvasView, SIGNAL(sigConnectionChanged(string, string, int, int)), clsBlockDiagramParent, SLOT(slotConnectionChanged(string, string, int, int)));

     connect(clsQDiagramCanvasView, SIGNAL(sigGotFocus(int,string)), this, SLOT(slotDiagramCanvasViewGotFocus(int,string)));



//     connect(clsQDiagramCanvasView, SIGNAL(sigAbortNewConnection()),
//	     clsBlockDiagramParent, SLOT(slotAbortNewConnection()));

     connect(clsBlockDiagramParent, SIGNAL(sigAbortNewConnection()),
	     clsQDiagramCanvasView, SLOT(slotAbortNewConnection()));





#ifdef USEPANNER
     connect(clsQDiagramCanvasView, SIGNAL(sigCanvasEnlarged()), clsqcanvaspanner, SLOT(canvasResized()));
     connect(clsQDiagramCanvasView, SIGNAL(contentsMoving( int, int)), clsqcanvaspanner, SLOT(parentScrolled(int, int)));
     connect(clsqcanvaspanner, SIGNAL(sigScroll(int, int)), clsQDiagramCanvasView, SLOT(center(int, int)));
#endif



    qtabbar->show();
    clsQDiagramCanvasView->show();
#ifdef USEPANNER
    clsqcanvaspanner->show();
#endif
    this->show();

//    this->repaint();



}


string ClsQDiagramView::getNewConnectionProcessID(){
    return clsBlockDiagramParent->getNewConnectionProcessID();
};




void ClsQDiagramView::slotTabChanged(int ii){
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::slotTabChanged()" << endl;
#endif

    if(qtabbar->currentTab()!= ii){
	qtabbar->setCurrentTab (ii);
    }

//     char c[sizeof(int)];
//     bzero(c, sizeof(int));


//     memcpy(&c[0], &ii, sizeof(int));
//     cout << "ii:" << ii << endl;
//     cout << "c:" << c << endl;

//     QString qstr = c;


    QString qstr = (qmapTabIdentifyers[ii]).c_str();

    ClsQCanvas *qcanvasTemp = clsBlockDiagramParent->getCanvas(qstr);
    if(qcanvasTemp!=NULL){
	clsQDiagramCanvasView->setCanvas(qcanvasTemp);

#ifdef USEPANNER
	clsqcanvaspanner->setCanvas(qcanvasTemp);
	clsqcanvaspanner->canvasViewResize(clsQDiagramCanvasView->visibleWidth(), clsQDiagramCanvasView->visibleHeight());
#endif
}
    else{
	cout << "CANVAS NOT FOUND" << endl;
    }

    /* if we just switched to the system canvas, we'll not allow to create new connections */

    int iType = qcanvasTemp->getCanvasType();
    string strID = qcanvasTemp->getID();
    emit canvasChanged(iType, strID);


}


void ClsQDiagramView::resizeEvent ( QResizeEvent * ) {
//    cout << "ClsQDiagramView::resizeEvent ( QResizeEvent * )" << endl;

#ifdef USEPANNER
    clsqcanvaspanner->canvasViewResize(clsQDiagramCanvasView->visibleWidth(), clsQDiagramCanvasView->visibleHeight());
//    clsqcanvaspanner->move(clsQDiagramCanvasView->visibleWidth() - clsqcanvaspanner->width(),clsQDiagramCanvasView->visibleHeight() - clsqcanvaspanner->height());
    clsqcanvaspanner->move(width() - clsqcanvaspanner->width() - 16, height() - clsqcanvaspanner->height() -16);

#endif

}

void ClsQDiagramView::setAddState(int _iAddState) {
    iAddState = _iAddState;
    clsQDiagramCanvasView->setAddState(_iAddState);
};


void ClsQDiagramView::setItemInfoMode(bool b) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::setItemInfoMode(bool b)" << endl;
#endif
    iAddState = 0;
    clsQDiagramCanvasView->setItemInfoMode(b);
};


void ClsQDiagramView::addTab(int iType, string strName, string strID) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::addTab(string strName, string strID)" << endl;
#endif


//     int ii;
//     memcpy(&ii, strID.c_str(), sizeof(int));

//     int iT = qtabbar->addTab(new QTab (strName.c_str()));
//     QTab* qq =  qtabbar->tab(iT);
//     qq->setIdentifier(ii);
//     qtabbar->repaint();

//    cout << "strID: " << strID << endl;
//    cout << "strName: " << strName << endl;

    int iT = qtabbar->addTab(new QTab (strName.c_str()));
//    cout << "iT: " << iT << endl;
    qmapTabIdentifyers[iT] = strID;

    QTab* qq =  qtabbar->tab(iT); //tab() or tabAt() ???
//    cout << "qq NULL?: " << (qq==NULL ? "yes" : "no") << endl;

    if(iType == ClsFESystemManager::ITEM_SYSTEM){
	qq->setIconSet(QIconSet(QPixmap(LetterS_2)));
    }
    else if(iType == ClsFESystemManager::ITEM_PROCESS){
	qq->setIconSet(QIconSet(QPixmap(LetterP_2)));
    }

    qq->setIdentifier(iT);
    qtabbar->repaint();

};

void ClsQDiagramView::removeTab( string strID) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::removeTab( string strID)" << endl;
#endif

    int id = 0;

    QMap<int, string>::Iterator it;
    for ( it = qmapTabIdentifyers.begin(); it != qmapTabIdentifyers.end(); ++it ) {
	if(!it.data().compare(strID)) {
	    id = it.key();
	    QTab* qq =  qtabbar->tab(id);
	    qtabbar->removeTab(qq);
	    qmapTabIdentifyers.remove(it);
	    continue;
	}
    }

//    cout << "id: " << id << endl;
//    QTab* qq = qtabbar->tab(id);
//    qtabbar->removeTab(qq);
    qtabbar->repaint();

};


void ClsQDiagramView::renameTab( string strID, string strName) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::renameTab( string strID, string strName)" << endl;
#endif

    int id = 0;

    QMap<int, string>::Iterator it;
    for ( it = qmapTabIdentifyers.begin(); it != qmapTabIdentifyers.end(); ++it ) {
	if(!it.data().compare(strID)) {
	    id = it.key();
	    QTab* qq =  qtabbar->tab(id);
	    qq->setText(strName.c_str());
	    continue;
	}
    }

    qtabbar->repaint();

};



void ClsQDiagramView::useSpacer(bool b) {
    if(b){ qwgtSpacer->setMinimumHeight( iSpacerHeight); }
    else { qwgtSpacer->setMinimumHeight( 0); }
};

QCanvas *ClsQDiagramView::canvas(){
    return clsQDiagramCanvasView->canvas();
};

void ClsQDiagramView::setCanvas(ClsQCanvas *q) {
//    cout << "ClsQDiagramView::setCanvas(ClsQCanvas *q)" << endl;

    clsQDiagramCanvasView->setCanvas(q);
//     clsQDiagramCanvasView->setCanvasType(q->getCanvasType());
//     cout << __FILE__ << ":" << __LINE__ << "q->getCanvasType(): " << q->getCanvasType() << endl;
#ifdef USEPANNER
    clsqcanvaspanner->setCanvas(q);
#endif
};

void ClsQDiagramView::zoomIn(){
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::zoomIn()" << endl;
#endif
QWMatrix m = clsQDiagramCanvasView->worldMatrix();
    m.scale( 1.1111, 1.1111 );
    clsQDiagramCanvasView->setWorldMatrix( m );
#ifdef USEPANNER
    clsqcanvaspanner->canvasResized();
#endif
};

void ClsQDiagramView::zoomOut() {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::zoomOut()" << endl;
#endif
    QWMatrix m = clsQDiagramCanvasView->worldMatrix();
    m.scale( 0.9, 0.9 );
    clsQDiagramCanvasView->setWorldMatrix( m );
#ifdef USEPANNER
    clsqcanvaspanner->canvasResized();
#endif
};

void ClsQDiagramView::clear() {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::clear()" << endl;
#endif
    clsQDiagramCanvasView->clear();
};


void ClsQDiagramView::showProcessTab(string strProcessID) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::showProcessTab(string strProcessID)" << endl;
#endif

    QString qstr = strProcessID.c_str();

    ClsQCanvas *qcanvasTemp = clsBlockDiagramParent->getCanvas(qstr);
    if(qcanvasTemp!=NULL){
	clsQDiagramCanvasView->setCanvas(qcanvasTemp);

#ifdef USEPANNER
	clsqcanvaspanner->setCanvas(qcanvasTemp);
	clsqcanvaspanner->canvasViewResize(clsQDiagramCanvasView->visibleWidth(), clsQDiagramCanvasView->visibleHeight());
#endif
    }
    else{
	cout << "CANVAS NOT FOUND" << endl;
    }


    int iTab = -1;

    QMap<int, string>::Iterator it;
    for ( it = qmapTabIdentifyers.begin(); it != qmapTabIdentifyers.end(); ++it ) {
	if(!it.data().compare(strProcessID)) {
	    iTab = it.key();
	    continue;
	}
    }

    if(iTab>=0){
	if(qtabbar->currentTab()!= iTab){
	    qtabbar->setCurrentTab (iTab);
	}
    }

    /* if we just switched to the system canvas, we'll not allow to create new connections */

//    emit canvasChanged(qstr);
    int iType = qcanvasTemp->getCanvasType();
    string strID = qcanvasTemp->getID();
    emit canvasChanged(iType, strID);


}


void ClsQDiagramView::slotDiagramCanvasViewGotFocus(int iType, string strID) {
#ifdef DEBUG_CLSQDIAGRAMVIEW
        cout << "ClsQDiagramView::slotDiagramCanvasViewGotFocus()" << endl;
#endif

    emit sigGotFocus(iType, strID);
}


void ClsQDiagramView::setFocus(bool b){
#ifdef DEBUG_CLSQDIAGRAMVIEW
    cout << "ClsQDiagramView::setFocus(bool b)" << endl;
#endif
    bFocus = b;
    clsQDiagramCanvasView->setFocus(b);
}



//--void ClsQDiagramView::slotCanvasItemDeleted(QCanvasItem* i) {
//--#ifdef DEBUG_CLSQDIAGRAMVIEW
//--    cout << "ClsQDiagramView::slotCanvasItemDeleted(QCanvasItem*)" << endl;
//--#endif
//--
//--    clsQDiagramCanvasView->canvasItemDeleted(i);
//--
//--}
//--




//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:




