#ifndef CLSQCANVASPANNER_H
#define CLSQCANVASPANNER_H

#include <iostream> 

#include <QGraphicsScene>
#include <QFrame>

#include "ClsPannerSceneView.h"

using namespace std;

class ClsPannerItem;
class ClsSceneView;

class ClsPanner  : public QFrame {
    Q_OBJECT

public:
    ClsPanner ( QWidget * _parent, ClsSceneView* _clsQDiagramCanvasView);
    void setScene(QGraphicsScene *clsScene);

signals:
    void sigScroll(int, int);
    

public slots:
    void canvasResized ( );

private slots:
    void scrollParent (int, int );
    

private:

    ClsPannerSceneView *qgraphicsview;
    ClsSceneView* clsQDiagramCanvasView;
};



#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
