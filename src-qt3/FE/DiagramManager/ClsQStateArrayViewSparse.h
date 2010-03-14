/****************************************************************************
 ** $Filename: ClsQStateArrayViewSparse.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Jul  4 19:14:00 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQSTATEARRAYVIEWSPARSE_H
#define CLSQSTATEARRAYVIEWSPARSE_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <iostream>
#include <vector>

#include <qframe.h>
#include <qlabel.h>
#include <qpixmap.h>

class ClsBaseTopology;

#include "ClsBaseQStateArrayView.h"
#include "ClsBaseQStateArrayView.h"
#include "ClsSubPopulations.h"

using namespace std;

class ClsQStateArrayViewSparse : public ClsBaseQStateArrayView {
    Q_OBJECT
public:
    ClsQStateArrayViewSparse( QWidget *parent = 0, ClsBaseTopology *_clsBaseTopologyGroup = 0, const char *name = 0, unsigned int _iSelectionMode=0 );

    void setValue( double fValue, int i, int j );
    void addValue( double fValue, int i, int j );
    void setValue(double fValue, vector<tiPoint> lst);
    void setValue(vector<pair<int, double> > lstIndexDist );
    void addValue(vector<pair<int, double> > lstIndexDist );
    void setValue(vector<tiPoint> lstIndexDelay);
    void setValue(valarray<size_t> vaIndices, valarray<double> vaData);

    void setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight);
    void setRegion(double fValue, ClsRegion _clsRegion);

    vector<tiPoint> getSelected();



public slots:
    void	clear();

protected:
    void mouseHandleEllipse( const QPoint& /* pos */){};

    void drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY);


private:
    vector<tiPoint> lstPoints;

    bool isValidCell(int iX, int iY);

    int pos2index2( int x );
    int index2pos2( int i );

};

#endif /* CLSQSTATEARRAYVIEWSPARSE_H */

//// Local Variables:
//// mode: c++
//// End:
