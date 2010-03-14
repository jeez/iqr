/****************************************************************************
 ** $Filename: ClsBaseQStateArrayView.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Jul  3 22:43:17 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASEQSTATEARRAYVIEW_H
#define CLSBASEQSTATEARRAYVIEW_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <iostream>
#include <vector>
#include <valarray>

#include <qcolor.h>

#include <qframe.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qpainter.h>

class ClsBaseTopology;
#include "ClsSubPopulations.h"

using namespace std;


class ClsBaseQStateArrayView : public QFrame {
    Q_OBJECT
public:
    ClsBaseQStateArrayView( QWidget *parent = 0, ClsBaseTopology *_clsBaseTopologyGroup = 0, const char *name = 0, unsigned int _iSelectionMode=0 );

    virtual void setValue( double /* fValue */, int /* i */, int /* j */)=0;
    virtual void addValue( double /* fValue */, int /* i */, int /* j */)=0;

    virtual void setValue(double /* fValue */, vector<tiPoint> /* lst */)=0;

    virtual void setValue(vector<pair<int, double> > /* lstIndexDist */)=0;
    virtual void addValue(vector<pair<int, double> > lstIndexDist ) = 0;

    virtual void setValue(vector<tiPoint> /* lstIndexDelay */)=0;

    virtual void setValue(valarray<size_t>, valarray<double>)=0;

    virtual void setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight);
    virtual void setRegion(double fValue, ClsRegion _clsRegion);

    virtual void markCell(int iColor, int iX, int iY);
    virtual void markCell(int iColor, list<int> lstIndex);

    virtual vector<tiPoint> getSelected(){
	vector<tiPoint> l;
	return l;
    };

    vector <vector<double> > getMatrix() {
	return vCells;
    };

    void setMatrix(vector <vector<double> > _v) {
	vCells = _v;
 	QPainter paintPM(&pmGridValues);
	drawCheckerBoard(paintPM, -BORDER, -BORDER);
	paintPM.end();
	repaint();
    }


    const ClsRegion getRegion();


    QPixmap getValuePixmap(int iImgWidth, int iImgHeight );
    QPixmap getGradientPixmap(int iImgWidth, int iImgHeight, int iColorMode = ClsBaseQStateArrayView::BLUE2RED);


    enum SELECTIONMODE {
	LIST = 0,
	REGION,
	SINGLE
    };

    enum COLORMODE {
	GRAY,
	BLUE2RED,
	HSV
    };

    void setColorMode(int i) {
	if(i == ClsBaseQStateArrayView::GRAY || i == ClsBaseQStateArrayView::BLUE2RED|| i == ClsBaseQStateArrayView::HSV){
	    iColorMode = i;
	}


    }

    void setMinValue(double f) {
	fValMin = f;
	fScaleFactor = 255. / fabs(fValMin-fValMax);
	fFixedValue = (fValMin + fValMax)/2.; //new
    };

    void setMaxValue(double f) {
	fValMax = f;
	fScaleFactor = 255. / fabs(fValMin-fValMax);
	fFixedValue = (fValMin + fValMax)/2.; //new
    };


signals:
	void cellClicked( int, int );

public slots:
     virtual void clear(){};
    void setValue(double f) { fFixedValue = f; };

protected:

    QColor getColor(double fValue);
//    virtual void drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY){};

    enum {
	MAXSIZE = 50,
	MINSIZE = 10,
	BORDER = 5
    };


    virtual void paintEvent( QPaintEvent * );
    virtual void mouseHandle( const QPoint &pos );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseHandleRegion( const QPoint &pos );
    virtual void mouseHandleEllipse( const QPoint &pos )=0;
    virtual void mouseReleaseEvent ( QMouseEvent * );

    virtual void createNullPixmap();

    virtual void drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY)=0;
    virtual int pos2index2( int x )=0;
    virtual int index2pos2( int i )=0;


    static const int iSizeMax = 200;
    ClsBaseTopology *clsBaseTopologyGroup;
    unsigned int iSelectionMode;

    int iCellSize;
    int iNrCellsX, iNrCellsY;
    double fFixedValue, fValMax, fValMin, fScaleFactor;
    vector <vector<double> > vCells;


    int iXPosStart, iYPosStart;
    QPixmap pmGridEmpty;
    QPixmap pmGridValues; 

    ClsRegion clsRegion;


    int iColorMode;
};

#endif /* CLSBASEQSTATEARRAYVIEW_H */



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
