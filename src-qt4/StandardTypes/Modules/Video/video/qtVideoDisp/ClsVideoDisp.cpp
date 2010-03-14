#include "ClsVideoDisp.hpp"


ClsVideoDisp::ClsVideoDisp(const char *caption, int w, int h) {
    cout << "ClsVideoDisp::ClsVideoDisp( const char *caption)" << endl;

    setWindowTitle(caption);
    setGeometry(0, 0, w, h);
    setMinimumSize(w,h);
    setMaximumSize(w,h);

    _buffer = new QPixmap(width(), height());
    _buffer->fill(Qt::black);

    qimage = new QImage(width(), height(), QImage::Format_RGB32);
    qimage->setNumColors(32);

}

void ClsVideoDisp::paintEvent( QPaintEvent* /* event */){
//    cout << "ClsVideoDisp::paintEvent( QPaintEvent* event)" << endl;
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, *_buffer);
    p.end();

};

void ClsVideoDisp::customEvent( QEvent* e ) {
//    cout << "ClsVideoDisp::customEvent( QCustomEvent* e )" << endl;
    if (dynamic_cast<QCEventData*>(e)){
        uchar* frame = dynamic_cast<QCEventData*>(e)->getData();
        uchar* bits = qimage->bits();
        int step = 0;

        // Beware: May fail on big-endian machines
        for(int loop=0; loop < qimage->numBytes(); loop+=4) {
            bits[loop]   = frame[step];   // Blue
            bits[loop+1] = frame[step+1]; // Green
            bits[loop+2] = frame[step+2]; // Red
            bits[loop+3] = 255;           // Alpha
            step+=3;
        }
//        _buffer->convertFromImage(*qimage);
        *_buffer = QPixmap::fromImage(*qimage);

        repaint();
    }

}
