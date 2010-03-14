#ifndef CLSQCANVASPANNER_H
#define CLSQCANVASPANNER_H

#include <iostream> 

#include <qcanvas.h>
#include <qframe.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpoint.h>

using namespace std;

class ClsQPanner;
class ClsQDiagramCanvasView;

class ClsQCanvasPanner  : public QFrame {
    Q_OBJECT

public:
    ClsQCanvasPanner ( QWidget * _parent, ClsQDiagramCanvasView* _clsQDiagramCanvasView, const char * name, WFlags f  );
    void setCanvas(QCanvas *qcanvas);

signals:
    void sigScroll(int, int);
    

public slots:
    void canvasViewResize ( int iW, int iH );    
    void canvasResized ( );
    void parentScrolled (int, int );

private slots:
    void scrollParent (int, int );

private:

    QCanvasView *qcanvasview;
    ClsQDiagramCanvasView* clsQDiagramCanvasView;
    
    ClsQPanner *qfmPanner;
    QWidget * parent;
};



#endif


//// Local Variables: 
//// mode: c++
//// End: 
