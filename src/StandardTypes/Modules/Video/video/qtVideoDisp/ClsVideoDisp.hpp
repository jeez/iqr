#ifndef VIDEODISP_H
#define VIDEODISP_H

#include <ctype.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <valarray>

#include <qapplication.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwidget.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>

#include <qfiledialog.h> 


using namespace std;

class QCEventData : public QCustomEvent {
    public:
        QCEventData( uchar* _frame ) : QCustomEvent( 65433 ), frame(_frame) {
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
            void customEvent( QCustomEvent * e );

            void paintEvent( QPaintEvent* );

            int iImgWidth, iImgHeight;
            //TEMP    QPainter *paintBuffer;
            QPixmap *_buffer;
            QImage *qimage;
};

#endif
