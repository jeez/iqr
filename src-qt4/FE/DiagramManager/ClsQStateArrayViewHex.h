/****************************************************************************
 ** $Filename: ClsQStateArrayViewHex.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Jul  4 19:14:28 2003
 **
 *****************************************************************************/


#ifndef CLSQSTATEARRAYVIEWHEX_H
#define CLSQSTATEARRAYVIEWHEX_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <iostream>
#include <vector>

#include <QFrame>
#include <QLabel>
#include <QPixmap>

class ClsBaseTopology;

#include "ClsBaseQStateArrayView.h"
#include "ClsBaseQStateArrayView.h"
#include "ClsSubPopulations.h"

using namespace std;

class ClsQStateArrayViewHex : public ClsBaseQStateArrayView {
    Q_OBJECT
public:
    ClsQStateArrayViewHex( QWidget *parent = 0, ClsBaseTopology *_clsBaseTopologyGroup = 0, const char *name = 0, unsigned int _iSelectionMode=0 );

    void setValue( double fValue, int i, int j );
    void addValue( double /* fValue */, int /* i */, int /* j */){};
    void setValue(double fValue, list<pair<int, int> > lst);
    void setValue(vector<pair<int, double> > /* lstIndexDist */){};
    void addValue(vector<pair<int, double> > /* lstIndexDist */){};
    void setValue(vector<pair<int, int> > /* lstIndexDelay */){};
    void setValue(valarray<size_t>, valarray<double>){};

    void setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight);
    void setRegion(double fValue, ClsRegion _clsRegion);

    void markCell(int /* iColor */, int /* iX */, int /* iY */){};

//    int		maxCol() { return maxi; }
//    int		maxRow() { return maxj; }

    list<pair<int, int> > getSelected();



public slots:
    void	clear();

protected:
//    virtual void paintEvent( QPaintEvent * );
//    virtual void mouseMoveEvent( QMouseEvent * );
//    virtual void mousePressEvent( QMouseEvent * );
//    virtual void mouseReleaseEvent ( QMouseEvent * );
//    virtual void resizeEvent( QResizeEvent * );
//    void	 mouseHandle( const QPointF &pos );
//    void	 mouseHandleRegion( const QPointF &pos );
    void mouseHandleEllipse( const QPoint& /* pos */){};


    void createNullPixmap();
    void drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY);


signals:
	void cellClicked( int, int );

private:




//     int pos2index( int x );
    int pos2index2( int x );

//     int index2pos( int i );
    int index2pos2( int i );

    void drawHexagon(QPainter &paint, int iXCenter, int iYCenter, int iVertexlength, int iOrientation);

};

//// Local Variables:
//// mode: c++
//// End:

#endif /* CLSQSTATEARRAYVIEWHEX_H */
