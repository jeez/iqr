/**
 * @file   ClsQPanner.cpp
 * @author Ulysses Bernardet
 * @date   Wed Feb 19 01:11:16 2003
 * 
 * @brief  This is a helper class for the ClsQCanvasPanner. It should never be seen/called outside of that context
 * 
 * 
 */
#include <qcursor.h>
#include <qimage.h>
#include <qpixmap.h>

#include <ClsQPanner.h>
#include "ClsQDiagramCanvasView.h"

//#define DEBUG_CLSQPANNER

ClsQPanner::ClsQPanner ( QFrame * _parent = 0, const char * name = 0, WFlags f = 0 ) : QFrame(_parent, name, f), parent(_parent) {
    setCursor (QCursor(Qt::SizeAllCursor) );

    QWidget::setAutoMask(true);

//    setBackgroundMode( NoBackground );

    setFrameStyle( QFrame::Panel | QFrame::Plain );
    setLineWidth( 1 );
    

//    image.load( "/home/ulysses/qt/canvas/alpha.png" );
//    pixmap.convertFromImage(image, OrderedAlphaDither);



//      QImage img( width(), height(), 32 );
//      img.setAlphaBuffer( TRUE );
//      for ( int x = 0; x < width(); x++ )
//  	for ( int y = 0; y < height(); y++ )
//  	    img.setPixel( x, y, qRgba( 128, 128, 128, y ) );
    
//    alpha.load( "/home/ulysses/Code/iqr421_Redesign/FE/DataMgr/alpha1.png" );  
//    ia.setAlphaBuffer(true);
//    ia.load( "/home/ulysses/Code/iqr421_Redesign/FE/DataMgr/alpha1.png" );  
//     alpha.convertFromImage ( img);
    
//    setBackgroundPixmap ( alpha);
//     setPaletteBackgroundPixmap ( img );

//    setBackgroundPixmap (pixmap);
    

    
};


void ClsQPanner::updateMask() {
    QBitmap bm( size() );
    bm.fill( color1 );			//transparent
    
    QPainter paint;
    paint.begin( &bm, this );
    paint.setBrush( color1 );		// use non-transparent color
    paint.setPen( color1 );
    paint.fillRect (  contentsRect(), QBrush(color1, Qt::Dense3Pattern ) );
    paint.end();
    setMask( bm );
}


// + void ClsQPanner::updateMask() {
// +     QBitmap bm( size() );
// +     bm.fill( color0 );			//transparent
// +     
// +     QPainter paint;
// +     paint.begin( &bm, this );
// +     paint.setBrush( color1 );		// use non-transparent color
// +     paint.setPen( color1 );
// +     paint.fillRect (  contentsRect(), QBrush(color1, Qt::Dense3Pattern ) );
// +     
// +     paint.end();
// +     setMask( bm );
// + }

// void ClsQPanner::updateMask() {
//      QBitmap bm( size() );
//      bm.fill( color0 );			//transparent
//      //bm = alpha.createHeuristicMask();
    
//      QPainter paint;
//      paint.begin( &bm, this );
// //     paint.setBrush( color1 );		// use non-transparent color
// //     paint.setPen( color1 );
// //     paint.fillRect (  contentsRect(), QBrush(color1, Qt::Dense3Pattern ) );
//      paint.drawPixmap ( 0, 0, alpha);
     
//      paint.end();
//      setMask( bm );
//  }

// void ClsQPanner::updateMask() {
// //void ClsQPanner::paintEvent( QPaintEvent * ) {
//     QPainter p( this );
//     QImage img( width(), height(), 32 );
//     img.setAlphaBuffer( TRUE );
//     for ( int x = 0; x < width(); x++ )
// 	for ( int y = 0; y < height(); y++ )
// 	    img.setPixel( x, y, qRgba( 128, 128, 128, y ) );
// //    p.drawPixmap( 0, 0, alpha ); 
//     p.drawImage( 0, 0, ia ); 

//     cout << "ALPHA: " << ia.hasAlphaBuffer () << endl;
//     cout << "ALPHA: " << img.hasAlphaBuffer () << endl;
//     exit(0);
    
// }

// void ClsQPanner::paintEvent ( QPaintEvent * ){
//       QPainter p( this );
//       p.drawPixmap( int(x()), int(y()), pixmap );  
// };



// void ClsQPanner::drawContentsMask ( QPainter *p ){
//       p->drawPixmap( int(x()), int(y()), pixmap );  
// };



// void ClsQPanner::drawContents ( QPainter *p ){
//     cout << "ClsQPanner::drawContents ( QPainter * )" << endl;
//       p->drawPixmap( int(x()), int(y()), pixmap );  
// // contentsRect()
// };



 
void ClsQPanner::mousePressEvent ( QMouseEvent * e ) {
#ifdef DEBUG_CLSQPANNER
    cout << "ClsQPanner::mousePressEvent ( QMouseEvent * e )" << endl;
#endif
    qpointMoveStart = e->pos();
};



void ClsQPanner::mouseMoveEvent ( QMouseEvent * e ) {
#ifdef DEBUG_CLSQPANNER
    cout << "ClsQPanner::mouseMoveEvent ( QMouseEvent * e )" << endl;
#endif
    /* check for boundaries */
    
    QPoint qpRel = mapToParent(e->pos());

    int iXOrig = qpRel.x() - qpointMoveStart.x();
    int iYOrig = qpRel.y() - qpointMoveStart.y();
    

    move( iXOrig, iYOrig );

    
    if(mapToParent(this->frameRect().topLeft()).x() < parent->lineWidth()){
	move(  parent->lineWidth(), iYOrig );
    } 
    if(mapToParent(this->frameRect().topLeft()).y() < parent->lineWidth()){
	move( iXOrig,  parent->lineWidth() );
    }
    
    if(mapToParent(this->frameRect().topLeft()).x() < parent->lineWidth() || mapToParent(this->frameRect().topLeft()).y() < parent->lineWidth()){
	move(  parent->lineWidth(),  parent->lineWidth() );
    }
    
    if(mapToParent(this->frameRect().bottomRight()).x() > parent->width() +  parent->lineWidth()){
	move( parent->width() + parent->lineWidth() - width(), iYOrig );
    }
    if(mapToParent(this->frameRect().bottomRight()).y() > parent->height() +  parent->lineWidth()){
	move( iXOrig,  parent->height() +  parent->lineWidth() - height());
    }
    
    if(mapToParent(this->frameRect().bottomRight()).x() > parent->width() +  parent->lineWidth() || 
       mapToParent(this->frameRect().bottomRight()).y() > parent->height() +  parent->lineWidth()){
	move( parent->width() +  parent->lineWidth() - width(), parent->height() +  parent->lineWidth() - height());
    }
};


/** 
 * When the mouse is released, the sigScroll is emitted. 
 * 
 * @param e 
 */
void ClsQPanner::mouseReleaseEvent ( QMouseEvent * e ) {
    QPoint qpRel = mapToParent(e->pos());
    emit sigScroll(qpRel.x() - e->pos().x() + width()/2 , qpRel.y() - e->pos().y() + height()/2  );
}


    

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
