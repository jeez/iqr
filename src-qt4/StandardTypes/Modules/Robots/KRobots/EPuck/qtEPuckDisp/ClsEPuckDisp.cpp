#include "ClsEPuckDisp.hpp"

#include <QCursor>
#include <QPixmap>
#include <QEvent>
#include <QPaintEvent>

#define MAXPLOT_POINTS  10000
#define PI 3.141592654


ClsEPuckDisp::ClsEPuckDisp( const char *caption) {
    int iFontSize;
     
    setWindowTitle(caption);
     
    aiProximitySensorPos[0] = 180;
    aiProximitySensorPos[1] = 230; 
    aiProximitySensorPos[2] = 270;
    aiProximitySensorPos[3] = 270;
    aiProximitySensorPos[4] = 310;
    aiProximitySensorPos[5] = 0;
    aiProximitySensorPos[6] = 90;   
    aiProximitySensorPos[7] = 90;


// parameters
    iSomaDiameter = 110;
    iAmbientSensorDiameter = 10;

    iWhiskersMaxLength = 50;
    iWheelHeight = 30;
    iWheelWidth = 8; 
    iWheelSpeedbarHeight = 15;
    iWheelSpeedbarWidth = 4;
//////////////////////
    
    iFontSize = 8;
    fontPlot =  new QFont( "helvetica", iFontSize , QFont::Normal);

//ZZZ    setBackgroundMode ( Qt::NoBackground);
    paintWidget = new QPainter();
    _buffer = new QPixmap(width(), height());
    _buffer->fill( Qt::black );
    paintBuffer = new QPainter();
     

    vaAmbient.resize(8);
    vaProximity.resize(8);


    int ii;
    for (ii = 0; ii<8 ; ii++){
	vaAmbient[ii] =1;
	vaProximity[ii]  =ii; 
    }

    afMotor[0] = 10;
    afMotor[1] = 10;

    iInhibit = 10; // ranges from 0 to 10

    defineColors();
    DrawStatus( );
}


void ClsEPuckDisp::defineColors(){
    colSoma.setNamedColor("darkGreen");
    colLines.setNamedColor("lightGrey" ); 
    colMotor.setNamedColor("blue");  
    colProximityWhisker.setNamedColor("yellow");
    colSensors.setNamedColor("darkGrey");
    colInhibit.setNamedColor("pink");  
    colAversive.setNamedColor("pink");  
    colAppetitive.setNamedColor("pink");  
}


void ClsEPuckDisp::DrawStatus() {


//    paintWidget->begin(this);

    paintBuffer->begin(_buffer);
    _buffer->fill( Qt::black );

    DrawSoma( paintBuffer, 2 );

    paintBuffer->end();
    repaint();

}



void ClsEPuckDisp::DrawSoma(QPainter *_Painter, int iWhiskersWidth){
    int ii;
    int iWhiskFactor = iWhiskersMaxLength / 11;
     
//       printf("+++++++++++++++++++++++\n");
//       for (ii = 0; ii<8 ; ii++){
//  	  printf("Coll: %i, ", vaAmbient[ii]);
//  	  printf("IR: %g\n", vaProximity[ii]); 
//  	  if (ii <= 2) printf("Motor: %g\n", afMotor[ii]);
//       }


    int iXCenter, iYCenter;
    int iWidth, iHeight;


    iWidth  = 250; //this->width();
    iHeight = 250; //this->height();
     
    iXCenter = iWidth / 2 ;
    iYCenter = iHeight / 2 ;



// Khepera body
    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( colSoma );
    _Painter->drawEllipse( iXCenter - iSomaDiameter / 2, 
			   iYCenter - iSomaDiameter / 2, 
			   iSomaDiameter, iSomaDiameter );
// -----------------------------

   
// Wheels     
    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( colMotor );
    /* left */


    _Painter->drawRect ( iXCenter - iSomaDiameter / 2 - iWheelWidth,  
			 iYCenter - iWheelHeight / 2, 
			 iWheelWidth, iWheelHeight);

    /* right */
    _Painter->drawRect ( iXCenter + iSomaDiameter / 2,
			 iYCenter  - iWheelHeight / 2, 
			 iWheelWidth, iWheelHeight);


    /* wheel speedbars */
    QPolygon qpaSpeedbar;
    _Painter->setBrush( QBrush( Qt::yellow, Qt::SolidPattern ) );
    /* wheel speedbar left */
    qpaSpeedbar.setPoints( 3, 
			iXCenter - iSomaDiameter / 2 - iWheelWidth,
			iYCenter,
			iXCenter - iSomaDiameter / 2 - iWheelWidth - iWheelSpeedbarHeight,
			iYCenter,
			iXCenter - iSomaDiameter / 2 - iWheelWidth - iWheelSpeedbarHeight/2,
			iYCenter -(int)((double)iWheelSpeedbarWidth*afMotor[0]));
    _Painter->drawPolygon(qpaSpeedbar);


    /* wheel speedbar right */

    qpaSpeedbar.setPoints( 3, 
			iXCenter + iSomaDiameter / 2 + iWheelWidth,
			iYCenter,
			iXCenter + iSomaDiameter / 2 + iWheelWidth + iWheelSpeedbarHeight,
			iYCenter,
			iXCenter + iSomaDiameter / 2 + iWheelWidth + iWheelSpeedbarHeight/2,
			iYCenter -(int)((double)iWheelSpeedbarWidth*afMotor[1]));
    _Painter->drawPolygon(qpaSpeedbar);



	


// ----------------------------

// Proximity and Ambient
    double fAngle;
    int iXPos, iYPos;
    int iCountSensor;
    int iAmbientColor;


    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    iCountSensor = 0;
    for ( fAngle = 205; fAngle <= 255; fAngle += 25){
	_Painter->setPen( QPen(  colLines, 1, Qt::SolidLine ) );
	iAmbientColor = (int)(vaAmbient[iCountSensor]*254.0);
//	cout << "iAmbientColor 0: " << iAmbientColor << endl;
	colCollision.setRgb(iAmbientColor,iAmbientColor,iAmbientColor);
	_Painter->setBrush( colCollision );

	iXPos = (int)(cos( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0  +
						   iAmbientSensorDiameter / 2 )) + iXCenter;
	iYPos = (int)(sin( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0  +
						   iAmbientSensorDiameter / 2 )) + iYCenter;
	  
	_Painter->drawEllipse( iXPos - iAmbientSensorDiameter / 2, 
			       iYPos - iAmbientSensorDiameter / 2 , 
			       iAmbientSensorDiameter, 
			       iAmbientSensorDiameter );
	  
	  
	_Painter->setPen( QPen( colProximityWhisker, iWhiskersWidth, Qt::SolidLine ) );
	_Painter->drawLine( iXPos, iYPos,
			    (int)(cos ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iXPos,
			    (int)(sin ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iYPos );
	iCountSensor++;
    }
     
    for ( fAngle = 285; fAngle <=335; fAngle += 25){
	_Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
	iAmbientColor = (int)(vaAmbient[iCountSensor]*254.0);
//	cout << "iAmbientColor 1: " << iAmbientColor << endl;
	colCollision.setRgb(iAmbientColor,iAmbientColor,iAmbientColor);
	_Painter->setBrush( colCollision );

	iXPos = (int)(cos( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0  +
						   iAmbientSensorDiameter / 2 )) + iXCenter;
	iYPos = (int)(sin( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0  +
						   iAmbientSensorDiameter / 2 )) + iYCenter;
	  
	_Painter->drawEllipse( iXPos - iAmbientSensorDiameter / 2, 
			       iYPos - iAmbientSensorDiameter / 2 , 
			       iAmbientSensorDiameter, 
			       iAmbientSensorDiameter );

	_Painter->setPen( QPen( colProximityWhisker, iWhiskersWidth, Qt::SolidLine ) );
	_Painter->drawLine( iXPos, iYPos,
			    (int)(cos ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iXPos,
			    (int)(sin ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iYPos );
	iCountSensor++;
    }

    for ( fAngle = 70; fAngle <=110; fAngle += 40){
	_Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
	iAmbientColor = (int)(vaAmbient[iCountSensor]*254.0);
//	cout << "iAmbientColor 2: " << iAmbientColor << endl;
	colCollision.setRgb(iAmbientColor,iAmbientColor,iAmbientColor);
	_Painter->setBrush( colCollision );

	iXPos = (int)(cos( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0  +
						   iAmbientSensorDiameter / 2 )) + iXCenter;
	iYPos = (int)(sin( fAngle * PI / 180.0) * ((double)iSomaDiameter / 2.0 +
						   iAmbientSensorDiameter / 2 )) + iYCenter;

	_Painter->drawEllipse( iXPos - iAmbientSensorDiameter / 2, 
			       iYPos - iAmbientSensorDiameter / 2 , 
			       iAmbientSensorDiameter, 
			       iAmbientSensorDiameter );
	  
	_Painter->setPen( QPen( colProximityWhisker, iWhiskersWidth, Qt::SolidLine ) );
	_Painter->drawLine( iXPos, iYPos,
			    (int)(cos ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iXPos,
			    (int)(sin ((double)aiProximitySensorPos[iCountSensor] * PI / 180.0) * (iWhiskersMaxLength-(double)vaProximity[iCountSensor] * iWhiskFactor)) + iYPos );
	iCountSensor++;
    }
// -----------------------------------

#ifdef SHOW_INHIBITION
// Inhibition
    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( colInhibit );
    _Painter->drawRect ( (int)(iXCenter - (double)iSomaDiameter * (.4 * (double)iInhibit) / 30.0),  
			 (int)(iYCenter - (double)iSomaDiameter * .4 + (double)iSomaDiameter * .15), 
			 (int)((double)iSomaDiameter * (.4 * (double)iInhibit) / 15.0),
			 (int)((double)iSomaDiameter * .8  -(double)iSomaDiameter * .6));

    iInhibit = 10;
    _Painter->setBrush( Qt::NoBrush );
    _Painter->drawRect ( (int)(iXCenter - (double)iSomaDiameter * (.4 * (double)iInhibit) / 30.0),  
			 (int)(iYCenter - (double)iSomaDiameter * .4 + (double)iSomaDiameter * .15), 
			 (int)((double)iSomaDiameter * (.4 * (double)iInhibit) / 15.0),
			 (int)((double)iSomaDiameter * .8  -(double)iSomaDiameter * .6));

// ----------------------------
#endif
     
#ifdef SHOW_APPETITIVE
// Appetitive
    double fAppetitive;
    fAppetitive = 0;
    for (ii = 0; ii < 8; ii++){
	fAppetitive += vaProximity[ii];
    }
    fAppetitive /= 8.0;

    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( colAppetitive );
    _Painter->drawRect ( (int)(iXCenter ),  
			 (int)(iYCenter - (double)iSomaDiameter * .2 + (double)iSomaDiameter * .2), 
			 (int)((double)iSomaDiameter * (.4 * (double)fAppetitive) / 15.0),
			 (int)((double)iSomaDiameter * .8  - (double)iSomaDiameter * .6));

    fAppetitive = 10.0;
    _Painter->setBrush( Qt::NoBrush );
    _Painter->drawRect ( (int)(iXCenter ),  
			 (int)(iYCenter - (double)iSomaDiameter * .2 + (double)iSomaDiameter * .2), 
			 (int)((double)iSomaDiameter * (.4 * (double)fAppetitive) / 15.0),
			 (int)((double)iSomaDiameter * .8  - (double)iSomaDiameter * .6));
// ----------------------------
#endif

#ifdef SHOW_AVERSIVE
// Aversive // to the left
    double fAversive;
    fAversive = 0;
     
    for (ii = 0; ii < 8; ii++){
	fAversive += (double)vaAmbient[ii];
    }
    fAversive /= 8.0;

    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( colAversive );
    _Painter->drawRect ( (int)(iXCenter -((double)iSomaDiameter * (.4 * (double)fAversive) / 15.0) ),  
			 (int)(iYCenter - (double)iSomaDiameter * .2 + (double)iSomaDiameter * .2), 
			 (int)((double)iSomaDiameter * (.4 * (double)fAversive) / 15.0),
			 (int)((double)iSomaDiameter * .8  - (double)iSomaDiameter * .6));
    fAversive = 10.0;
    _Painter->setBrush( Qt::NoBrush );
    _Painter->drawRect ( (int)(iXCenter -((double)iSomaDiameter * (.4 * (double)fAversive) / 15.0) ),  
			 (int)(iYCenter - (double)iSomaDiameter * .2 + (double)iSomaDiameter * .2), 
			 (int)((double)iSomaDiameter * (.4 * (double)fAversive) / 15.0),
			 (int)((double)iSomaDiameter * .8  - (double)iSomaDiameter * .6));
// ----------------------------
#endif


#ifdef USE_TRIANGLE
    /* Wheel direction */
    _Painter->setPen( QPen( colLines, 1, Qt::SolidLine ) );
    _Painter->setBrush( Qt::NoBrush  );
    _Painter->drawEllipse( iXCenter - iSomaDiameter / 3, 
			   iYCenter - iSomaDiameter / 3, 
			   (int)((double)iSomaDiameter / 1.5), 
			   (int)((double)iSomaDiameter / 1.5));

     
    /* calculate the angle ^| is 0 degree! */
    double fRest;
    double fFactor;
    fRest = atan ( 1.0 / 11.0 ) * 180.0 / PI;
    fFactor = 90.0 / ( 45.0 - fRest);

    fAngle = ((atan ((afMotor[0] + 1)/ (afMotor[1] + 1)) * 180 / PI) - fRest )* fFactor;
    fAngle -= 180;


/* draw the triangle */

    double fAngTmp;
    double fRadius;
    double fTriHeight;
    double fBaseWidth;
    int iXLeft, iXRight;
    int iYLeft, iYRight;
    int iXTip, iYTip;

    fRadius =  iSomaDiameter / 3;
    fTriHeight = iSomaDiameter / 2 - iSomaDiameter / 1.5;
    fBaseWidth =  (afMotor[0] +  afMotor[1]) * 2;  

    fAngTmp = fAngle - atan((fBaseWidth * 0.5) / fRadius) * 180 / PI;
    iXLeft = (int)(cos( fAngTmp * PI / 180.0) * fRadius ) + iXCenter;
    iYLeft = (int)(sin( fAngTmp * PI / 180.0) * fRadius ) + iYCenter;

    fAngTmp = fAngle + atan((fBaseWidth * 0.5) / fRadius) * 180 / PI;
    iXRight = (int)(cos( fAngTmp * PI / 180.0) * fRadius ) + iXCenter;
    iYRight = (int)(sin( fAngTmp * PI / 180.0) * fRadius ) + iYCenter;
     
    iXTip = (int)(cos( fAngle * PI / 180.0) * iSomaDiameter / 2 ) + iXCenter;
    iYTip = (int)(sin( fAngle * PI / 180.0) * iSomaDiameter / 2 ) + iYCenter;

    QPolygon a;
    _Painter->setBrush( QBrush( Qt::yellow, Qt::SolidPattern ) );
    a.setPoints( 3, iXLeft,iYLeft, iXTip,iYTip, iXRight,iYRight);

//     _Painter->drawPoints( a );
    _Painter->drawPolygon( a );
//    _Painter->drawPolyline ( a );

#endif /* USE_TRIANGLE */


}




void ClsEPuckDisp::paintEvent( QPaintEvent* /* event */){
//    cout << "paintEvent( QPaintEvent* event)" << endl;
//ZZZ    bitBlt (this, 0, 0, _buffer);
    
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, *_buffer);
    p.end();


};




//ZZZvoid ClsEPuckDisp::ClearImg(){
//ZZZ    _buffer->fill( Qt::black );
//ZZZ    bitBlt (this, 0, 0, _buffer);
//ZZZ}

//ZZZvoid ClsEPuckDisp::PassBack( int iPassBack ){
//ZZZ    if( iPassBack == RESET){
//ZZZ	fYMax = 0;
//ZZZ	fYMin = 1000000;
//ZZZ	_buffer->fill( Qt::black );
//ZZZ	bitBlt (this, 0, 0, _buffer);
//ZZZ	//    printf("Reset clicked\n");
//ZZZ    }
//ZZZ     
//ZZZ     
//ZZZ    emit GraphClicked( iPassBack );
//ZZZ}



//ZZZvoid ClsEPuckDisp::resizeEvent( QResizeEvent* event) {
//ZZZ    QPixmap save ( *_buffer );
//ZZZ    _buffer->resize( event->size() );
//ZZZ    _buffer->fill( Qt::black );
//ZZZ    bitBlt( _buffer, 0, 0, &save );
//ZZZ
//ZZZ//     printf("Resize\n");
//ZZZ};



void ClsEPuckDisp::customEvent( QEvent* e ) {
//    cout << "ClsEPuckDisp::customEvent( QCustomEvent* e )" << endl;

 
    if (dynamic_cast<QCEventData*>(e)){
	pair<double, double> pData = dynamic_cast<QCEventData*>(e)->getData();

	afMotor[0] = pData.first * 10.;
	afMotor[1] = pData.second * 10.;
	
//	cout << "pData.first, pData.second: " << pData.first << ", " << pData.second << endl;

	DrawStatus( );
    }

}






//cout << __FILE__ << ":" << __LINE__ << endl;
