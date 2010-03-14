#include <QCursor>
#include <QFrame>

#include <ClsPanner.h>
#include "ClsSceneView.h"

//#define DEBUG_CLSQCANVASPANNER

ClsPanner::ClsPanner ( QWidget * _parent, ClsSceneView* _clsQDiagramCanvasView) : 
    QFrame(_parent), clsQDiagramCanvasView(_clsQDiagramCanvasView)
 {
    resize(100, 75);
    setBackgroundMode(Qt::NoBackground);

    qgraphicsview = new ClsPannerSceneView(this, clsQDiagramCanvasView);
    qgraphicsview->resize(this->size());

    connect(qgraphicsview,SIGNAL(sigScroll(int, int)),this,SLOT(scrollParent (int, int )));

};
    
void ClsPanner::setScene(QGraphicsScene *clsScene) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsPanner::setScene(QCanvas *clsScene)" << endl;
#endif
    
    qgraphicsview->setScene(clsScene);

//    QWMatrix m = ((ClsSceneView*)parent)->worldMatrix(); 
    QMatrix m = clsQDiagramCanvasView->matrix(); 


    double dFactorX = (double)this->contentsRect().width() / 
	( qgraphicsview->scene()->width() > 0 ? (double)(qgraphicsview->scene()->width()) : .1 );
    double dFactorY = (double)this->contentsRect().height() / 
	( qgraphicsview->scene()->height() > 0 ? (double)(qgraphicsview->scene()->height()) : .1 );

//    cout << "dFactorX: " << dFactorX << endl;
//    cout << "dFactorY: " << dFactorY << endl;

    m.scale( dFactorX, dFactorY );
    qgraphicsview->setMatrix( m );

};


void ClsPanner::canvasResized ( ) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsPanner::canvasResized ( )" << endl;
#endif
    
    if(qgraphicsview->scene()!=NULL){
//	QWMatrix m = ((ClsSceneView*)parent)->worldMatrix(); /* this should be the parents matrix */
	QMatrix m = clsQDiagramCanvasView->matrix(); /* this should be the parents matrix */
	double dFactorX = (double)(this->contentsRect().width())/ 
	    ( qgraphicsview->scene()->width() > 0 ? (double)(qgraphicsview->scene()->width()) : .1 );
	double dFactorY = (double)(this->contentsRect().height()) / 
	    ( qgraphicsview->scene()->height() > 0 ? (double)(qgraphicsview->scene()->height()) : .1 );
//	cout << "dFactorX: " << dFactorX << endl;
//	cout << "dFactorY: " << dFactorY << endl;
	
	m.scale( dFactorX, dFactorY );
	qgraphicsview->setMatrix( m );

    }
}

void ClsPanner::scrollParent (int iX, int iY) {
#ifdef DEBUG_CLSQCANVASPANNER
    cout << "ClsPanner::scrollParent (int iX, int iY)" << endl;
#endif
    if(qgraphicsview->scene()!=NULL){
	double dFactorX = (double)(this->contentsRect().width()) /  ( iX > 0 ? (double)(iX) : .1 );
	double dFactorY = (double)(this->contentsRect().height()) / ( iY > 0 ? (double)(iY) : .1 );
	
	iX = (int)((double)(qgraphicsview->scene()->width())  / ( dFactorX > 0 ? dFactorX : 0.1 ));
	iY = (int)((double)(qgraphicsview->scene()->height()) / ( dFactorY > 0 ? dFactorY : 0.1 ));
	
	emit sigScroll(iX, iY);
    }
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
