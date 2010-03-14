/****************************************************************************
 ** $Filename: ClsQStateArrayViewSparse.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Sep  9 17:18:57 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <qpainter.h>
#include <qdrawutil.h>
#include <qcheckbox.h>
#include <qevent.h>
#include <qapplication.h>
#include <qcolor.h>

#include "ClsQStateArrayViewSparse.h"
#include "ClsTopologySparse.h"
#include "parameter.hpp"


ClsQStateArrayViewSparse::ClsQStateArrayViewSparse( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup,
						    const char *name, unsigned int _iSelectionMode ) :
    ClsBaseQStateArrayView(parent, _clsBaseTopologyGroup, name, _iSelectionMode) {

    iCellSize = 20;
    iNrCellsX = 10;
    iNrCellsY = 10;

    ParameterList parameterList = _clsBaseTopologyGroup->getListParameters();

    int iXMax = 0;
//    int iXMin = 10000;

    int iYMax = 0;
//    int iYMin = 10000;

    if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup)){
	lstPoints = (dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup))->getList();
	vector<tiPoint>::iterator it;
	for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	    int iX = (*it).first;
	    int iY = (*it).second;
//	    cout << "iX, iY: " << iX << ", " << iY << endl;

	    iXMax = (iX > iXMax ? iX : iXMax);
//	    iXMin = (iX < iXMin ? iX : iXMin);

	    iYMax = (iY > iYMax ? iY : iYMax);
//	    iYMin = (iY < iYMin ? iY : iYMin);
	}
    }

    iNrCellsX = iXMax;
    iNrCellsY = iYMax;

    iCellSize = iSizeMax / (iNrCellsX > iNrCellsY ? iNrCellsX : iNrCellsY);

    vCells.resize(iNrCellsX+1);
    for(int ii=0; ii<(iNrCellsX + 1);ii++){
	vCells[ii].resize(iNrCellsY+1);
    }

    for ( int i = 0; i <= iNrCellsX; i++ ){
	for ( int j = 0; j <= iNrCellsY; j++ ){
	    vCells[i][j] = -99; //!!!!!!!!!!!!!
	}
    }

    vector<tiPoint>::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	vCells[iX][iY] = 0.;
    }


    this->setFrameStyle(  QFrame::WinPanel | QFrame::Raised );

    setMinimumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );
    setMaximumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );

    clear();
    createNullPixmap();
//    resize( maxi * SCALE + 2 * BORDER , maxj * SCALE + 2 * BORDER );
//    resize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );
}

void ClsQStateArrayViewSparse::clear() {
    for ( int i = 0; i <= iNrCellsX; i++ ){
	for ( int j = 0; j <= iNrCellsY; j++ ){
	    vCells[i][j] = -99; //!!!!!!!!!!!!!
	}
    }

    vector<tiPoint>::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	vCells[iX][iY] = 0.;
    }
    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
    bitBlt(&pmGridValues, 0, 0, &pmGridEmpty);
}

vector<tiPoint> ClsQStateArrayViewSparse::getSelected() {
    cout << "ClsQStateArrayViewSparse::getSelected()" << endl;
    vector<tiPoint> lst;
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
//	    cout << "i, j, fValue: " << i << ", " << j << ", " << fValue << endl;
	    if(fValue != -99 && fValue != 0){
//		cout << "push_back" << endl;
		tiPoint pairTemp(i,j);
		lst.push_back(pairTemp);
	    }
	}
    }
    return lst;
};

void ClsQStateArrayViewSparse::setValue( double fValue, int i, int j ) {
//    cout << "ClsQStateArrayViewSparse::setValue( double fValue, int i, int j )" << endl;

    if(isValidCell(i, j)){
	vCells[i][j] = fValue;
	QBrush   qb(yellow);
	QPainter paintPM( &pmGridValues );
	qb.setColor(getColor(fValue));
//	qDrawPlainRect (&paint,   index2pos2( i ),        index2pos2( j ),        iCellSize -1, iCellSize - 1, black, 0, &qb);
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, black, 0, &qb);
	paintPM.end();
	bitBlt(this, BORDER, BORDER, &pmGridValues); 
    }
}

void ClsQStateArrayViewSparse::addValue( double fValue, int i, int j ) {
    if(isValidCell(i, j)){
//	cout << "valid cell: " << i << ", " << j << endl;
	vCells[i][j] += fValue;
	QBrush   qb(yellow);
	QPainter paintPM(&pmGridValues );
	qb.setColor(getColor(fValue));
//	qDrawPlainRect (&paint, index2pos2( i ), index2pos2( j ), iCellSize -1, iCellSize - 1, black, 0, &qb);
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, black, 0, &qb);
	paintPM.end();
    }
}


void ClsQStateArrayViewSparse::setValue(double fValue, vector<tiPoint> lst) {
    vector<tiPoint>::iterator it;
    for(it=lst.begin();it!=lst.end();it++){
	int i, j;
	i = (*it).first;
	j = (*it).second;
	setValue(fValue, i, j);
    }
}

void ClsQStateArrayViewSparse::setValue(vector<pair<int, double> > lstIndexDist ){
//    cout << "ClsQStateArrayViewSparse::setValue(vector<pair<int, double> > lstIndexDist )" << endl;
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewSparse::addValue(vector<pair<int, double> > lstIndexDist ){
//    cout << "ClsQStateArrayViewSparse::addValue(vector<pair<int, double> > lstIndexDist )" << endl;
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	addValue(fValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewSparse::setValue(vector<tiPoint> lstIndexDelay){
    vector<tiPoint>::iterator it;
    for(it=lstIndexDelay.begin();it!=lstIndexDelay.end();it++){
	int iIndex = (*it).first;
	int iValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(iValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewSparse::setValue(valarray<size_t> vaIndices, valarray<double> vaData){
    for(unsigned int ii=0; ii<vaIndices.size(); ii++){
	int iIndex = vaIndices[ii];
	double fValue = vaData[ii];
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewSparse::setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight) {
//    cout << "ClsQStateArrayViewSparse::setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight)" << endl;
    /* 1. find cells within region */
    vector<tiPoint> lstPointsWithin;
    vector<tiPoint>::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	if(iX>=iXStart && iX<(iXStart+iWidth) && iY>=iYStart && iY<(iYStart+iHeight)){
	    lstPointsWithin.push_back(*it);
	}
    }

/* 2. find min/max x/y for these cells */
    int iHMax = 0, iVMax = 0;
    int iHMin = 99999, iVMin = 99999;

    vector<tiPoint>::iterator it1;
    for(it1=lstPointsWithin.begin(); it1!=lstPointsWithin.end();it1++){
	int iX = (*it1).first;
	iHMax = (iX > iHMax ? iX : iHMax);
	iHMin = (iX < iHMin ? iX : iHMin);
	int iY = (*it1).second;
	iVMax = (iY > iVMax ? iY : iVMax);
	iVMin = (iY < iVMin ? iY : iVMin);
    }

/* 3. reshape region */
    iXStart = iHMin;
    iYStart = iVMin;
    iWidth = iHMax - iHMin + 1;
    iHeight = iVMax - iVMin +1;

    clsRegion.setXStart(iXStart);
    clsRegion.setYStart(iYStart);
    clsRegion.setWidth(iWidth);
    clsRegion.setHeight(iHeight);

    for(int ii=iXStart; ii<(iXStart+iWidth); ii++){
	for(int jj=iYStart; jj<(iYStart+iHeight); jj++){
	    setValue(fValue, ii, jj);
	}
    }
}

void ClsQStateArrayViewSparse::setRegion(double fValue, ClsRegion _clsRegion){
    int iXStart = _clsRegion.getXStart();
    int iYStart = _clsRegion.getYStart();
    int iWidth  = _clsRegion.getWidth();
    int iHeight = _clsRegion.getHeight();

    setRegion(fValue, iXStart, iYStart, iWidth, iHeight);
}

void ClsQStateArrayViewSparse::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY){
//    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
    QBrush   qb(yellow);
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
//	    if(fValue>=0){
	    if(fValue==-99){
		qb.setColor(backgroundColor ());
	    }
	    else{
		qb.setColor(getColor(fValue));
	    }
	    qDrawPlainRect (&paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1, black, 0, &qb);
	}
    }
}

bool ClsQStateArrayViewSparse::isValidCell(int _iX, int _iY) {
    vector<tiPoint>::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
	int iX = (*it).first;
	int iY = (*it).second;
	if(_iX == iX && _iY == iY){
	    return true;
	}
    }
    return false;
}

int ClsQStateArrayViewSparse::pos2index2( int x ) {
    return ( x - BORDER ) / iCellSize + 1;
}

int ClsQStateArrayViewSparse::index2pos2( int i ) {
    return	( i - 1 ) * iCellSize + BORDER;
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../SystemGUI/ && make -k -j8 && cat /usr/share/apps/kget/sounds/finished.wav > /dev/dsp"
//// compile-command: "cd ../SystemGUI/ && make -k -j8"
//// End:
