/****************************************************************************
 ** Header file for main source for the TraXX project 
 ** 
 ** started 1999.01 
 ** 
 ** All rights with Ulysses Bernardet
 ** 
 *****************************************************************************/


#ifndef READIMAGE_H
#define READIMAGE_H

#include <ctype.h>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <vector>
#include <valarray>

#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QtCore/QEvent>
#include <QPolygon>
#include <QPixmap>
#include <QEvent>
#include <QPaintEvent>


using namespace std;

enum ePassBack {
    RESET,
    TEST,
    PRINT,
    SAVE,
    CHANGE_FONT,
    CLEAR,
    Y_AUTOSCALE,
    DRAW_GRID,
    QUIT,
    SET_SETTINGS
};



class QCEventData : public QEvent {
public:
    QCEventData( pair<double,double> _pData ) : QEvent( (QEvent::Type)65433 ), pData(_pData) {
    }
    pair<double,double> getData(){
	return pData;
    }
private:
    pair<double,double> pData;
};



class ClsKhepDisp : public QWidget {
    Q_OBJECT
public:
    ClsKhepDisp( const char *caption);

    
//ZZZ    void resizeEvent( QResizeEvent* );
     
     
    void setAmbient( valarray<double>& _vaAmbient ){ 
	vaAmbient = _vaAmbient; 
    }

     
    void setProximity( valarray<double>& _vaProximity ){ 
	vaProximity = _vaProximity * 10.; 
    }
    

    void setMotor( double _afMotor[2] ) { 
	int ii;
	for (ii = 0 ; ii < 2; ii++){
	    afMotor[ii] = _afMotor[ii]; 
	}
    };
    void setInhibition( int _iInhibit ) { iInhibit = _iInhibit; };
     


    signals:
    void GraphClicked( int iPassBack);
     
public slots:
    void DrawStatus( );
//ZZZ    void ClearImg();
//ZZZ    void PassBack( int iPassBack );


     
private:
    void customEvent( QEvent * e );

    void defineColors();
    void DrawSoma(QPainter *_Painter, 
		  /*int aiColl[8], double afIR[8], double afMotor[2], int iInhibit, */
		  int iWhiskersWidth);

    void paintEvent( QPaintEvent* );

    int iXCoorOld;
    int iYCoorOld;
    double fYMin;
    double fYMax;
    int iXMin; // not used yet...
    int iXMax;
     
    int iSomaDiameter, iAmbientSensorDiameter;
    int iWheelHeight, iWheelWidth;
    int iWheelSpeedbarHeight, iWheelSpeedbarWidth;

    int iWhiskersMaxLength;
    int aiProximitySensorPos[8];  

    double afMotor[2];

    valarray<double> vaProximity;
    valarray<double> vaAmbient;


    int iInhibit;

    QColor colSoma;
    QColor colLines;
    QColor colMotor;
    QColor colCollision;
    QColor colProximityWhisker;
    QColor colInhibit;
    QColor colAversive;
    QColor colAppetitive;
    QColor colSensors;



    int iImgWidth, iImgHeight;
    QPushButton *btnClear;
    QPainter *paintBuffer;
    QPainter *paintWidget;
    QFont *fontPlot;
    QPixmap *_buffer;

};




#endif










