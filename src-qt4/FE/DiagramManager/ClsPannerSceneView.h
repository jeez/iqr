#ifndef CLSPANNERSCENEVIEW_H
#define CLSPANNERSCENEVIEW_H

#include <iostream>
#include <list>
#include <vector>

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QCursor> 
#include <QMatrix> 
#include <QMouseEvent>



using namespace std;

class ClsScene;

class ClsPannerSceneView : public QGraphicsView {
    Q_OBJECT

public:
    ClsPannerSceneView(QWidget* parent, QGraphicsView* _viewReference);

signals:
    void sigScroll(int, int);
  
private:
    void mouseMoveEvent ( QMouseEvent * e );

    void drawForeground ( QPainter * painter, const QRectF & rect );
    QGraphicsView* viewReference;
};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
