#include <iostream.h>

#include <qframe.h>
#include <qwidget.h>


#include <qrect.h> 
#include <qpainter.h> 
#include <qpixmap.h> 

#ifndef CLSQTHERMO_H
#define CLSQTHERMO_H


class ClsQThermo : public QWidget {
//     Q_OBJECT
     
public:
     ClsQThermo( QWidget * parent=0, const char * name=0) : QWidget ( parent, name, 0){//, true) {
	  qcolorFill = Qt::blue;
	  iOrientation = LEFT2RIGHT;

	  iMinVal = 0;
	  iMaxVal = 1000;

	  dMinVal = 0;
	  dMaxVal = 1.0;
     };
     
  
     void paintEvent(QPaintEvent*) {
  	  QPixmap qpm( this->size());
  	  qpm.fill( backgroundColor());

  	  QPainter qp(&qpm);
  	  qp.setPen( qcolorFill );
  	  qp.setBrush( qcolorFill );

  	  if(iOrientation == TOP2BOTTOM){
  	       qp.setWindow( 0, 0 , 1,  (iMaxVal-iMinVal));
  	       qp.drawRect ( 0, 0, 1, (iValue-iMinVal) );
  	  } 
  	  else if(iOrientation == BOTTOM2TOP){
    	       qp.setWindow( 0, 0 , 1,  (iMaxVal-iMinVal));
    	       qp.translate(0, (iMaxVal-iMinVal));
  	       qp.drawRect ( 0, 0, 1, -(iValue-iMinVal) );
  	  }
  	  else if(iOrientation == LEFT2RIGHT){
  	       qp.setWindow( 0, 0 , (iMaxVal-iMinVal), 1);
  	       qp.drawRect ( 0, 0, (iValue-iMinVal), 1 );
  	  }
  	  else if(iOrientation == RIGHT2LEFT){
  	       qp.setWindow( 0, 0 , (iMaxVal-iMinVal), 1);
    	       qp.translate((iMaxVal-iMinVal), 0);
  	       qp.drawRect ( 0, 0, -(iValue-iMinVal), 1 );
  	  }
	  
  	  bitBlt( this, 0,0, &qpm, 0, 0, -1, -1 );
     };
     
     
     
     void setOrientation(int _iOrientation) { 
	  iOrientation = _iOrientation;
	  repaint();
     };

     void setRange(double _dMinVal, double _dMaxVal){   
	  iMinVal = (int)(_dMinVal*1000.0);
	  iMaxVal = (int)(_dMaxVal*1000.0);
	  dMinVal = _dMinVal;
	  dMaxVal = _dMaxVal;

     };
     
     double maxValue() { return dMaxVal; };
     

     void setFillColor(QColor _qcolorFill) {
	  qcolorFill = _qcolorFill;
     };

     void setValue(double _dValue) { 
	  iValue = (int)(_dValue*1000.0); 
	  repaint();
	  
     };
     

     enum Orientation{
	  LEFT2RIGHT,
	  RIGHT2LEFT,
	  TOP2BOTTOM,
	  BOTTOM2TOP
     };
     
     
private:
     int iValue;
     int iMinVal;
     int iMaxVal;

     double dMinVal;
     double dMaxVal;

     int iOrientation;
     
     QColor qcolorFill;
     
     
     
};


     
#endif


/// Local Variables: 
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
/// End: 
