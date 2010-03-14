#include <qcursor.h>
#include <ClsQCanvasPanner.h>
#include <ClsQPanner.h>
#include "ClsQDiagramCanvasView.h"

//#define DEBUG_CLSQCANVASPANNER

/** 
 * Constructor for the ClsQCanvasPanner.
 * 
 * @param _parent This _must_ be a valid *ClsQDiagramCanvasView
 * @param name unused
 * @param f frame style, unused
 */
ClsQCanvasPanner::ClsQCanvasPanner ( QWidget * _parent, ClsQDiagramCanvasView* _clsQDiagramCanvasView, const char * name, WFlags f ) : 
    QFrame(_parent, name, f), clsQDiagramCanvasView(_clsQDiagramCanvasView), parent(_parent) {
    resize(100, 75);
    setBackgroundMode(Qt::NoBackground);

    qcanvasview = new QCanvasView(this);
    qcanvasview->resize(this->size());
    qcanvasview->setHScrollBarMode(QScrollView::AlwaysOff);
    qcanvasview->setVScrollBarMode(QScrollView::AlwaysOff);
    qcanvasview->setFrameStyle( QFrame::Panel | QFrame::Plain );
    qcanvasview->setLineWidth( 1 );

    qfmPanner = new ClsQPanner(this, "", 0);
    qfmPanner->resize(10, 7);
    qfmPanner->move(lineWidth(), lineWidth());

    connect(qfmPanner,SIGNAL(sigScroll(int, int)),this,SLOT(scrollParent (int, int )));
    
    qfmPanner->show();
};
    
void ClsQCanvasPanner::setCanvas(QCanvas *qcanvas) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsQCanvasPanner::setCanvas(QCanvas *qcanvas)" << endl;
#endif
    
    qcanvasview->setCanvas(qcanvas);

//    QWMatrix m = ((ClsQDiagramCanvasView*)parent)->worldMatrix(); 
    QWMatrix m = clsQDiagramCanvasView->worldMatrix(); 


    double dFactorX = (double)this->contentsRect().width() / 
	( qcanvasview->canvas()->width() > 0 ? (double)(qcanvasview->canvas()->width()) : .1 );
    double dFactorY = (double)this->contentsRect().height() / 
	( qcanvasview->canvas()->height() > 0 ? (double)(qcanvasview->canvas()->height()) : .1 );

//    cout << "dFactorX: " << dFactorX << endl;
//    cout << "dFactorY: " << dFactorY << endl;

    m.scale( dFactorX, dFactorY );
    qcanvasview->setWorldMatrix( m );

//    dFactorX = (parent->width() * (double)this->contentsRect().width() )/ 
    dFactorX = (clsQDiagramCanvasView->width() * (double)this->contentsRect().width() )/ 

	( qcanvasview->canvas()->width() > 0 ? (double)(qcanvasview->canvas()->width()) : .1 );
//    dFactorY = (parent->height() * (double)this->contentsRect().height() )/ 
    dFactorY = (clsQDiagramCanvasView->height() * (double)this->contentsRect().height() )/ 
	( qcanvasview->canvas()->height() > 0 ? (double)(qcanvasview->canvas()->height()) : .1 );

    qfmPanner->resize((int)dFactorX, (int)dFactorY);
};


/** 
 * Function the resize the ClsQPanner
 * 
 * @param iW 
 * @param iH 
 */
void ClsQCanvasPanner::canvasViewResize ( int iW, int iH ) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsQCanvasPanner::canvasViewResize ( int iW, int iH )" << endl;
#endif
    if(qcanvasview->canvas()!=NULL){
	double dFactorX = ((double)(iW) * (double)(this->contentsRect().width()))/ 
	    ( qcanvasview->canvas()->width() > 0 ? (double)(qcanvasview->canvas()->width()) : .1 );
	double dFactorY = ((double)(iH)* (double)(this->contentsRect().height()))/ 
	    ( qcanvasview->canvas()->height() > 0 ? (double)(qcanvasview->canvas()->height()) : .1 );
	qfmPanner->resize((int)dFactorX, (int)dFactorY);
    }
}

/** 
 * Slot triggered when ever the canvas is resized
 * 
 */
void ClsQCanvasPanner::canvasResized ( ) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsQCanvasPanner::canvasResized ( )" << endl;
#endif
    
    if(qcanvasview->canvas()!=NULL){
//	QWMatrix m = ((ClsQDiagramCanvasView*)parent)->worldMatrix(); /* this should be the parents matrix */
	QWMatrix m = clsQDiagramCanvasView->worldMatrix(); /* this should be the parents matrix */
	double dFactorX = (double)(this->contentsRect().width())/ 
	    ( qcanvasview->canvas()->width() > 0 ? (double)(qcanvasview->canvas()->width()) : .1 );
	double dFactorY = (double)(this->contentsRect().height()) / 
	    ( qcanvasview->canvas()->height() > 0 ? (double)(qcanvasview->canvas()->height()) : .1 );
//	cout << "dFactorX: " << dFactorX << endl;
//	cout << "dFactorY: " << dFactorY << endl;
	
	m.scale( dFactorX, dFactorY );
	qcanvasview->setWorldMatrix( m );

//	canvasViewResize(parent->width() , parent->height());
	canvasViewResize(clsQDiagramCanvasView->width() , clsQDiagramCanvasView->height());
    }
}

/** 
 * This slot forwards the signal from the ClsQPanner to the parent (a QCanvasView)
 * 
 * @param iX 
 * @param iY 
 */
void ClsQCanvasPanner::scrollParent (int iX, int iY) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsQCanvasPanner::scrollParent (int iX, int iY)" << endl;
#endif
    if(qcanvasview->canvas()!=NULL){
	double dFactorX = (double)(this->contentsRect().width()) /  ( iX > 0 ? (double)(iX) : .1 );
	double dFactorY = (double)(this->contentsRect().height()) / ( iY > 0 ? (double)(iY) : .1 );
	
	iX = (int)((double)(qcanvasview->canvas()->width())  / ( dFactorX > 0 ? dFactorX : 0.1 ));
	iY = (int)((double)(qcanvasview->canvas()->height()) / ( dFactorY > 0 ? dFactorY : 0.1 ));
	
	emit sigScroll(iX, iY);
    }
}


/** 
 * Slot that is acivated when the parent (a QCanvasView) scrolls
 * 
 * @param iX 
 * @param iY 
 */
void ClsQCanvasPanner::parentScrolled (int iX, int iY) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsQCanvasPanner::parentScrolled (int, int )" << endl;
#endif

//    cout << "iX: " << iX << endl;
//    cout << "iY: " << iY << endl;
    
    if(qcanvasview->canvas()!=NULL){
	double dX = (double)(this->contentsRect().width()) * (double)iX/ 
	    ( qcanvasview->canvas()->width() > 0 ? (double)(qcanvasview->canvas()->width()) : .1 );
	double dY = (double)(this->contentsRect().height()) * (double)iY / 
	    ( qcanvasview->canvas()->height() > 0 ? (double)(qcanvasview->canvas()->height()) : .1 );
	qfmPanner->move((int)dX, (int)dY);	
    }
    
};


