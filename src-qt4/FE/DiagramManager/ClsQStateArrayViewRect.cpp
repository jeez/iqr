/****************************************************************************
 ** $Filename: ClsQStateArrayViewRect.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Sep  9 17:19:01 2003
 **
 *****************************************************************************/


#include <QPainter>
#include <QCheckBox>
#include <QEvent>
#include <QApplication>
#include <QColor>
#include <QToolTip>

#include <qdrawutil.h>

#include "ClsQStateArrayViewRect.h"
#include "ClsBaseTopology.h"
#include "parameter.hpp"

//#define DEBUG_CLSQSTATEARRAYVIEWRECT


ClsQStateArrayViewRect::ClsQStateArrayViewRect( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup,
						const char *_name, unsigned int _iSelectionMode ) :
    ClsBaseQStateArrayView(parent, _clsBaseTopologyGroup, _name, _iSelectionMode) {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup, const char *_name, unsigned int _iSelectionMode ) " << endl;
#endif

    iCellSize = 20;
    iNrCellsX = 10;
    iNrCellsY = 10;

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
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::clear()" << endl;
#endif

    for ( int i = 0; i <= iNrCellsX; i++ ){
	for ( int j = 0; j <= iNrCellsY; j++ ){
	    vCells[i][j] = 0.;
	}
    }
    bitBlt(&pmGridValues, 0, 0, &pmGridEmpty);
    repaint();

}

list<pair<int, int> > ClsQStateArrayViewRect::getSelected() {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::getSelected()" << endl;
#endif

    list<pair<int, int> > lst;
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
	    if(fValue>0){
		pair<int, int> pairTemp(i,j);
		lst.push_back(pairTemp);
	    }
	}
    }
    return lst;
};

void ClsQStateArrayViewRect::setValue( double fValue, int i, int j ) {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::setValue( double fValue, int i, int j )" << endl;
#endif

    if(i>0 && j>0 && i<=iNrCellsX && j<=iNrCellsY){
	vCells[i][j] = fValue;
	QBrush   qb(Qt::yellow);
	qb.setColor(getColor(fValue));

 	QPainter paintPM(&pmGridValues);
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, Qt::black, 0, &qb);
	paintPM.end();

	repaint();

    }
}

void ClsQStateArrayViewRect::addValue( double fValue, int i, int j ) {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::addValue( double fValue, int i, int j )" << endl;
#endif

    if(i>0 && j>0 && i<=iNrCellsX && j<=iNrCellsY){
	vCells[i][j] += fValue;
	QBrush   qb(Qt::yellow);
	QPainter paintPM(&pmGridValues );
	qb.setColor(getColor(fValue));
	qDrawPlainRect (&paintPM, index2pos2( i )-BORDER, index2pos2( j )-BORDER, iCellSize -1, iCellSize - 1, Qt::black, 0, &qb);
	paintPM.end();
    }
}

void ClsQStateArrayViewRect::setValue(double fValue, list<pair<int, int> > lst) {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::setValue(double fValue, list<pair<int, int> > lst)" << endl;
#endif

    list<pair<int, int> >::iterator it;
    for(it=lst.begin();it!=lst.end();it++){
	int i, j;
	i = (*it).first;
	j = (*it).second;
	setValue(fValue, i, j);
    }
}

void ClsQStateArrayViewRect::setValue(vector<pair<int, double> > lstIndexDist ){
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::setValue(vector<pair<int, double> > lstIndexDist )" << endl;
#endif
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	pair<int, int> pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewRect::addValue(vector<pair<int, double> > lstIndexDist ){
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::addValue(vector<pair<int, double> > lstIndexDist )" << endl;
#endif
    vector<pair<int, double> >::iterator it;
    for(it=lstIndexDist.begin();it!=lstIndexDist.end();it++){
	int iIndex = (*it).first;
	double fValue = (*it).second;
	pair<int, int> pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	addValue(fValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewRect::setValue(vector<pair<int, int> > lstIndexDelay){
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::setValue(vector<pair<int, int> > lstIndexDelay)" << endl;
#endif
    vector<pair<int, int> >::iterator it;
    for(it=lstIndexDelay.begin();it!=lstIndexDelay.end();it++){
	int iIndex = (*it).first;
	int iValue = (*it).second;
	pair<int, int> pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue((double)iValue, pPoint.first, pPoint.second);
    }
}

void ClsQStateArrayViewRect::setValue(valarray<size_t> vaIndices, valarray<double> vaData){
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::setValue(valarray<size_t> vaIndices, valarray<double> vaData)" << endl;
#endif
    for(unsigned int ii=0; ii<vaIndices.size(); ii++){
	int iIndex = vaIndices[ii];
	double fValue = vaData[ii];
	pair<int, int> pPoint = clsBaseTopologyGroup->index2pos(iIndex);
	setValue(fValue, pPoint.first, pPoint.second);
    }
}


void ClsQStateArrayViewRect::mouseHandleEllipse( const QPointF &_pos ) {
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::mouseHandleEllipse( const QPointF &_pos )" << endl;
#endif

    int iX = pos2index2( _pos.x() );
    int iY = pos2index2( _pos.y() );

    int iWidth = 9;
    int iHeight = 5;
    bool bUseFloat = false;

    list<pair<int, int> > lst0;
    list<pair<int, int> > lst1;
    list<pair<int, int> > lst;

    QPainter paint( this ); QPen   qp(Qt::yellow); paint.setPen(qp);
    if(bUseFloat){
	lst = clsBaseTopologyGroup->getPoints4Ellipse((double)iX +.5, (double)iY + .5, (double)iWidth, (double)iHeight);
//    lst = getPoints4Rect((double)iX +.5, (double)iY + .5, iWidth, iHeight);

	setValue(.5, lst);

	paint.drawRect ( index2pos2(iX) + iCellSize/2,
			 index2pos2(iY) + iCellSize/2,
			 iCellSize, iCellSize);


	paint.drawEllipse ( index2pos2(iX) + iCellSize - index2pos2(iWidth)/2,  
			    index2pos2(iY) + iCellSize - index2pos2(iHeight)/2 , 
			    index2pos2(iWidth), index2pos2(iHeight));
    }
    else {
	lst = clsBaseTopologyGroup->getPoints4Ellipse((double)iX, (double)iY, (double)iWidth, (double)iHeight);
	setValue(fFixedValue, lst);
	paint.drawRect ( index2pos2(iX),
			 index2pos2(iY),
			 iCellSize, iCellSize);

	paint.drawEllipse ( index2pos2(iX) + iCellSize/2 - index2pos2(iWidth)/2,  
			    index2pos2(iY) + iCellSize/2 - index2pos2(iHeight)/2 , 
			    index2pos2(iWidth), index2pos2(iHeight));
    }
}

void ClsQStateArrayViewRect::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY){
#ifdef DEBUG_CLSQSTATEARRAYVIEWRECT
    cout << "ClsQStateArrayViewRect::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY)" << endl;
#endif

    QBrush   qb(Qt::yellow);
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
	    qb.setColor(getColor(fValue));
	    qDrawPlainRect (&paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1, Qt::black, 0, &qb);
	    QToolTip::add (this, QRect(BORDER+ index2pos2( i )+iOffsetX, BORDER +index2pos2( j )+iOffsetY, 
				       iCellSize -1, iCellSize - 1), iqrUtils::int2string((j-1)*iNrCellsX + i).c_str());
	}
    }
}

int ClsQStateArrayViewRect::pos2index2( int _x ) {
    return ( _x - BORDER ) / iCellSize + 1;
}

int ClsQStateArrayViewRect::index2pos2( int i ) {
    return	( i - 1 ) * iCellSize + BORDER;
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
