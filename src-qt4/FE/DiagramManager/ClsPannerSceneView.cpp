#include <QPixmap>
#include <QPen>
#include <QMouseEvent>
#include <QScrollBar>

#include "ClsPannerSceneView.h"

//#define DEBUG_CLSPANNERSCENEVIEW

ClsPannerSceneView::ClsPannerSceneView(QWidget* _parent, QGraphicsView* _viewReference) : QGraphicsView(_parent), viewReference(_viewReference) {
#ifdef DEBUG_CLSPANNERSCENEVIEW
    cout << "ClsPannerSceneView::ClsPannerSceneView(QWidget* parent,const char* name)" << endl;
#endif
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setFrameStyle( QFrame::Panel | QFrame::Plain );
    setLineWidth( 1 );
    setDragMode(QGraphicsView::ScrollHandDrag);
    setStyleSheet("background: transparent");
}

void ClsPannerSceneView::mouseMoveEvent ( QMouseEvent * event){
#ifdef DEBUG_CLSPANNERSCENEVIEW
    cout << "ClsPannerSceneView::mouseMoveEvent ( QMouseEvent * event)" << endl;
#endif
//    cout << "event->buttons(): " << event->buttons() << endl;

    if(event->buttons() == Qt::LeftButton){
	QPointF qpHere = event->pos();
	emit sigScroll(qpHere.x(), qpHere.y());
    }
}

void ClsPannerSceneView::drawForeground ( QPainter * painter, const QRectF & rect ) {
#ifdef DEBUG_CLSPANNERSCENEVIEW
    cout << "ClsPannerSceneView::drawForeground ( QPainter * painter, const QRectF & rect )" << endl;
#endif

    QPoint qp = viewReference->mapFromScene(viewReference->sceneRect().topLeft());
    int iX = qp.x()*-1;
    int iY = qp.y()*-1;

    painter->drawRect ( iX, iY, 
			viewReference->width(), 
			viewReference->height());


}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

