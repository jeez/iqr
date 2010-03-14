#ifndef VIDEODISP_H
#define VIDEODISP_H

#include <ctype.h>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <vector>
#include <valarray>

#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtCore/QEvent>
#include <QtGui/QPolygon>
#include <QtGui/QPixmap>
#include <QtCore/QEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QImage>

//#include <qpopupmenu.h>

//#include <qfiledialog.h> 


using namespace std;

class QCEventData : public QEvent {
    public:
    QCEventData( uchar* _frame ) : QEvent( (QEvent::Type)65433 ), frame(_frame) {
        }
        uchar* getData(){
            return frame;
        }
    private:
        uchar* frame;
};

class ClsVideoDisp : public QWidget {
    Q_OBJECT
    public:
        ClsVideoDisp(const char *caption, int w, int h);

        public slots:

    private:
            void customEvent( QEvent * e );

            void paintEvent( QPaintEvent* );

            int iImgWidth, iImgHeight;
            QPixmap *_buffer;
            QImage *qimage;
};

#endif
