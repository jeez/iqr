#include "ClsVideoDisp.hpp"

ClsVideoDisp::ClsVideoDisp(const char *caption, int w, int h) {
    cout << "ClsVideoDisp::ClsVideoDisp( const char *caption)" << endl;

    setCaption(caption);
    setGeometry(0, 0, w, h);
    setMinimumSize(w,h);
    setMaximumSize(w,h);

    _buffer = new QPixmap(width(), height());
    _buffer->fill(black);

    qimage = new QImage(width(), height(), 32);
}

void ClsVideoDisp::paintEvent( QPaintEvent* /* event */){
//    cout << "ClsVideoDisp::paintEvent( QPaintEvent* event)" << endl;
    bitBlt (this, 0, 0, _buffer);
};

void ClsVideoDisp::customEvent( QCustomEvent* e ) {
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
        _buffer->convertFromImage(*qimage);
        repaint();
    }
}
