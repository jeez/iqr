/****************************************************************************
 ** $Filename: ClsQStateArrayViewRect.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Sep  9 17:19:01 2003
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
#include <qtooltip.h>

#include "ClsQStateArrayViewRect.h"
#include "ClsBaseTopology.h"
#include "parameter.hpp"




ClsQStateArrayViewRect::ClsQStateArrayViewRect( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup,
						const char *_name, unsigned int _iSelectionMode ) :
    ClsBaseQStateArrayView(parent, _clsBaseTopologyGroup, _name, _iSelectionMode) {

    iCellSize = 20;
    iNrCellsX = 10;
    iNrCellsY = 10;

//iSelectionMode = 100;

    ParameterList parameterList = _clsBaseTopologyGroup->getListParameters();

    while (parameterList.size()) {
	string strParamName = parameterList.front()->getName();
	string strParamValue = parameterList.front()->getValueAsString();
	if(!strParamName.compare(ClsTagLibrary::TopologyWidthTag())) {
	    iNrCellsX = iqrUtils::string2int(strParamValue);
	}
	else if(!strParamName.compare(ClsTagLibrary::TopologyHeightTag())) {
	    iNrCellsY =  iqrUtils::string2int(strParamValue);
	}
	parameterList.pop_front();
    }

    iCellSize = iSizeMax / (iNrCellsX > iNrCellsY ? iNrCellsX : iNrCellsY);
    if(iCellSize <=1){
	iCellSize = 2;
    }

    vCells.resize(iNrCellsX+1);
    for(int ii=0; ii<(iNrCellsX + 1);ii++){
	vCells[ii].resize(iNrCellsY+1);
    }

    this->setFrameStyle(  QFrame::WinPanel | QFrame::Raised );

    setMinimumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );
    setMaximumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );

    clear();
    createNullPixmap();


}

void ClsQStateArrayViewRect::clear() {
//    cout << "ClsQStateArrayViewRect::clear()" << endl;

    for ( int i = 0; i <= iNrCellsX; i++ ){
	for ( int j = 0; j <= iNrCellsY; j++ ){
	    vCells[i][j] = 0.;
	}
    }
    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
    bitBlt(&pmGridValues, 0, 0, &pmGridEmpty);
}

vector<tiPoint> ClsQStateArrayViewRect::getSelected() {
//    cout << "ClsQStateArrayViewRect::getSelected()" << endl;

    vector<tiPoint> lst;
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {

//---    for ( int i = 0; i <= iNrCellsX; i++ ) {
//---	for ( int j = 0; j <= iNrCellsY; j++ ) {

	    double fValue = vCells[i][j];
	    if(fValue>0){
		tiPoint pairTemp(i,j);
		lst.push_back(pairTemp);
	    }
	}
    }
    return lst;
};

void ClsQStateArrayViewRect::setValue( double fValue, int i, int j ) {
//    cout << "ClsQStateArrayViewRect::setValue( double fValue, int i, int j )" << endl;

    if(i>0 && j>0 && i<=iNrCellsX && j<=iNrCellsY){
	vCells[i][j] = fValue;
	QBrush   qb(yellow);
	qb.setColor(getColor(fValue));

/*
	QPainter paint( this );
	qDrawPlainRect (&paint, index2pos2( i ), index2pos2( j ), iCellSize -1, iCellSize - 1, black, 0, &qb);
*/

 	QPainter paintPM(&pmGridValues);
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, black, 0, &qb);
	paintPM.end();

	bitBlt(this, BORDER, BORDER, &pmGridValues); 


    }
}

void ClsQStateArrayViewRect::addValue( double fValue, int i, int j ) {
    if(i>0 && j>0 && i<=iNrCellsX && j<=iNrCellsY){
	vCells[i][j] += fValue;
	QBrush   qb(yellow);
	QPainter paintPM(&pmGridValues );
	qb.setColor(getColor(fValue));
//	qDrawPlainRect (&paint,   index2pos2( i ),        index2pos2( j ),        iCellSize -1, iCellSize - 1, black, 0, &qb);
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, black, 0, &qb);
	paintPM.end();
    }
}

void ClsQStateArrayViewRect::setValue(double fValue, vector<tiPoint> lst) {
    vector<tiPoint>::iterator it;
    for(it=lst.begin();it!=lst.end();it++){
	int i, j;
	i = (*it).first;
	j = (*it).second;
	setValue(fValue, i, j);
    }
}

void ClsQStateArrayViewRect::setValue(vector<pair<int, double> > lstIndexDist ){
//    cout << "ClsQStateArrayViewRect::setValue(vector<pair<int, double> > lstIndexDist )" << endl;
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewRect::addValue(vector<pair<int, double> > lstIndexDist ){
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	addValue(fValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewRect::setValue(vector<tiPoint> lstIndexDelay){
    vector<tiPoint>::iterator it;
    for(it=lstIndexDelay.begin();it!=lstIndexDelay.end();it++){
	int iIndex = (*it).first;
	int iValue = (*it).second;
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue((double)iValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewRect::setValue(valarray<size_t> vaIndices, valarray<double> vaData){
    for(unsigned int ii=0; ii<vaIndices.size(); ii++){
	int iIndex = vaIndices[ii];
	double fValue = vaData[ii];
	tiPoint pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewRect::mouseHandleEllipse( const QPoint &_pos ) {

    int iX = pos2index2( _pos.x() );
    int iY = pos2index2( _pos.y() );

    int iWidth = 9;
    int iHeight = 5;
    bool bUseFloat = false;

//    int iWidth1 = 3;
//    int iHeight1 = 6;

    vector<tiPoint> lst0;
    vector<tiPoint> lst1;
    vector<tiPoint> lst;

//    lst0 = clsBaseTopologyGroup->getPoints4Ellipse(iX, iY, iWidth, iHeight);
//    lst0 = clsBaseTopologyGroup->getPoints4Rect(iX, iY, iWidth, iHeight);
//  lst1 = clsBaseTopologyGroup->getPoints4Ellipse(iX, iY, iWidth1, iHeight1);
//    lst1 = clsBaseTopologyGroup->getPoints4Rect(iX, iY, iWidth1, iHeight1);
//    lst = ClsBaseTopology::XOrLists(lst0, lst1);

    QPainter paint( this ); QPen   qp(yellow); paint.setPen(qp);
    if(bUseFloat){
	lst = clsBaseTopologyGroup->getPoints4Ellipse((double)iX +.5, (double)iY + .5, (double)iWidth, (double)iHeight);
//    lst = getPoints4Rect((double)iX +.5, (double)iY + .5, iWidth, iHeight);

	setValue(.5, lst);

	paint.drawRect ( index2pos2(iX) + iCellSize/2,
			 index2pos2(iY) + iCellSize/2,
			 iCellSize, iCellSize);


	paint.drawEllipse ( index2pos2(iX) + iCellSize - index2pos2(iWidth)/2,  index2pos2(iY) + iCellSize - index2pos2(iHeight)/2 , index2pos2(iWidth), index2pos2(iHeight));
//    paint.drawRect ( index2pos2(iX) - index2pos2(iWidth)/2  + iCellSize/2 , index2pos2(iY) - index2pos2(iHeight)/2  + iCellSize/2, index2pos2(iWidth), index2pos2(iHeight));
    }
    else {
	lst = clsBaseTopologyGroup->getPoints4Ellipse((double)iX, (double)iY, (double)iWidth, (double)iHeight);
//    lst = getPoints4Rect((double)iX, (double)iY, iWidth, iHeight);

	setValue(fFixedValue, lst);
	paint.drawRect ( index2pos2(iX),
			 index2pos2(iY),
			 iCellSize, iCellSize);

	paint.drawEllipse ( index2pos2(iX) + iCellSize/2 - index2pos2(iWidth)/2,  index2pos2(iY) + iCellSize/2 - index2pos2(iHeight)/2 , index2pos2(iWidth), index2pos2(iHeight));
//    paint.drawEllipse ( index2pos2(iX) + iCellSize/2 - index2pos2(iWidth1)/2, index2pos2(iY) + iCellSize/2 - index2pos2(iHeight1)/2 , index2pos2(iWidth1), index2pos2(iHeight1));

//    paint.drawRect ( index2pos2(iX) - index2pos2(iWidth)/2, index2pos2(iY) - index2pos2(iHeight)/2 , index2pos2(iWidth), index2pos2(iHeight));
//    paint.drawRect ( index2pos2(iX) - index2pos2(iWidth1)/2, index2pos2(iY) - index2pos2(iHeight1)/2 , index2pos2(iWidth1), index2pos2(iHeight1));

    }
}

void ClsQStateArrayViewRect::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY){
//    cout << "ClsQStateArrayViewRect::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY)" << endl;
//    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
    QBrush   qb(yellow);
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
	    qb.setColor(getColor(fValue));
	    qDrawPlainRect (&paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1, black, 0, &qb);
	    QToolTip::add (this, QRect(BORDER+ index2pos2( i )+iOffsetX, BORDER +index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1), iqrUtils::int2string((j-1)*iNrCellsX + i));
	}
    }
}


/*     ClsQStateArrayViewRect::int pos2index( int x ) { */
/*	return ( x - BORDER ) / SCALE + 1; */
/*     } */

int ClsQStateArrayViewRect::pos2index2( int _x ) {
    return ( _x - BORDER ) / iCellSize + 1;
//--    return ( x - BORDER ) / iCellSize;// + 1;
}

/*     int ClsQStateArrayViewRect::index2pos( int i ) { */
/*	return	( i - 1 ) * SCALE + BORDER; */
/*     } */


int ClsQStateArrayViewRect::index2pos2( int i ) {
    return	( i - 1 ) * iCellSize + BORDER;
//--    return	i * iCellSize + BORDER;
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
