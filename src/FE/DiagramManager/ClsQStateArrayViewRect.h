/****************************************************************************
 ** $Filename: ClsQStateArrayView.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Jul  4 14:06:04 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQSTATEARRAYVIEW_H
#define CLSQSTATEARRAYVIEW_H    /*+ To stop multiple inclusions. +*/

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

class ClsQStateArrayViewRect : public ClsBaseQStateArrayView {
    Q_OBJECT
public:
    ClsQStateArrayViewRect( QWidget *parent = 0, ClsBaseTopology *_clsBaseTopologyGroup = 0, const char *name = 0, unsigned int _iSelectionMode=0 );

    void setValue( double fValue, int i, int j );
    void addValue( double fValue, int i, int j );
    void setValue(double fValue, vector<tiPoint> lst);
    void setValue(vector<pair<int, double> > lstIndexDist );
    void addValue(vector<pair<int, double> > lstIndexDist );
    void setValue(vector<tiPoint> lstIndexDelay);
    void setValue(valarray<size_t> vaIndices, valarray<double> vaData);

    vector<tiPoint> getSelected();

public slots:
    void clear();

protected:
    void mouseHandleEllipse( const QPoint &pos );
    void drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY);


private:
    int pos2index2( int x );
    int index2pos2( int i );

};

#endif /* CLSQSTATEARRAYVIEW_H */



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
