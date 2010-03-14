/**
 * @file   ClsQPanner.h
 * @author Ulysses Bernardet
 * @date   Wed Feb 19 01:13:51 2003
 * 
 * @brief  This is a helper class for the ClsQCanvasPanner. It should never be seen/called outside of that context
 * 
 * 
 */
#ifndef CLSQPANNER_H
#define CLSQPANNER_H

#include <iostream> 

#include <qcanvas.h>
#include <qframe.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qpixmap.h> 

using namespace std;


class ClsQPanner  : public QFrame {
    Q_OBJECT

public:
    ClsQPanner ( QFrame * _parent, const char * name, WFlags f );

signals:
    void sigScroll(int, int);    

private:
    void mousePressEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void updateMask();
//    void paintEvent( QPaintEvent * );
  
//    void drawContentsMask ( QPainter * p ); 
//    void drawContents ( QPainter * );

//    void drawContentsMask ( QPainter * p ){};
//    void drawContents ( QPainter * ){};

  
    QFrame * parent;
    QPoint qpointMoveStart;

//    QPixmap alpha;
//    QImage ia;

    QImage image;
    QPixmap pixmap;

    
};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
