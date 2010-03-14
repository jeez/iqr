/****************************************************************************
 ** $Filename: ClsQStateArrayViewHex.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Sep  9 17:19:10 2003
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

#include "ClsQStateArrayViewHex.h"
#include "ClsBaseTopology.h"
#include "parameter.hpp"


ClsQStateArrayViewHex::ClsQStateArrayViewHex( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup,
						const char *_name, unsigned int _iSelectionMode ) :
    ClsBaseQStateArrayView(parent, _clsBaseTopologyGroup, _name, _iSelectionMode) {

    iCellSize = 20;
    iNrCellsX = 10;
    iNrCellsY = 10;

    setBackgroundColor(yellow);


    ParameterList parameterList = _clsBaseTopologyGroup->getListParameters();

    while (parameterList.size()) {
	string strParamName = parameterList.front()->getName();
	string strParamValue = parameterList.front()->getValueAsString();

//	cout << "ParamName: " << strParamName << ": " << strParamValue << endl;

	if(!strParamName.compare(ClsTagLibrary::TopologyWidthTag())) {
	    iNrCellsX = iqrUtils::string2int(strParamValue);

	}
	else if(!strParamName.compare(ClsTagLibrary::TopologyHeightTag())) {
	    iNrCellsY =  iqrUtils::string2int(strParamValue);
	}

	parameterList.pop_front();
    }

    iCellSize = iSizeMax / (iNrCellsX > iNrCellsY ? iNrCellsX : iNrCellsY);

    vCells.resize(iNrCellsX+1);
    for(int ii=0; ii<(iNrCellsX + 1);ii++){
	vCells[ii].resize(iNrCellsY+1);
    }


    iXPosStart = iYPosStart = 0;


    fValMin = 0;
    fValMax = 1.0;
    fScaleFactor = 255.0;
    this->setFrameStyle(  QFrame::WinPanel | QFrame::Raised );

    setMinimumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );
    setMaximumSize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );

    clear();
    createNullPixmap();

//    resize( maxi * SCALE + 2 * BORDER , maxj * SCALE + 2 * BORDER );
//    resize( iNrCellsX * iCellSize + 2 * BORDER , iNrCellsY * iCellSize + 2 * BORDER );

}


void ClsQStateArrayViewHex::clear() {
    for ( int i = 0; i <= iNrCellsX; i++ ){
	for ( int j = 0; j <= iNrCellsY; j++ ){
	    vCells[i][j] = 0.;
	}
    }

    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
}



vector<tiPoint> ClsQStateArrayViewHex::getSelected() {
//    cout << "ClsQStateArrayViewHex::getSelected()" << endl;

    vector<tiPoint> lst;
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
	    if(fValue>0){
		tiPoint pairTemp(i,j);
		lst.push_back(pairTemp);
	    }

	}
    }
    return lst;
};


// void ClsQStateArrayViewHex::resizeEvent( QResizeEvent *  ) {
// }


void ClsQStateArrayViewHex::setValue( double fValue, int i, int j ) {
//    cout << "ClsQStateArrayViewHex::setValue( double fValue, int i, int j )" << endl;

    if(i>0 && j>0 && i<=iNrCellsX && j<=iNrCellsY){
	vCells[i][j] = fValue;

	QBrush   qb(yellow);
	QPainter paint( this );
	qb.setColor(getColor(fValue));
//	qDrawPlainRect (&paint, index2pos2( i ), index2pos2( j ), iCellSize -1, iCellSize - 1, black, 0, &qb);
    }
}



void ClsQStateArrayViewHex::setValue(double fValue, vector<tiPoint> lst) {
    vector<tiPoint>::iterator it;
    for(it=lst.begin();it!=lst.end();it++){
	int i, j;
	i = (*it).first;
	j = (*it).second;
	setValue(fValue, i, j);
    }
}


void ClsQStateArrayViewHex::setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight) {
/*
    clsRegion.xStart = iXStart;
    clsRegion.yStart = iYStart;
    clsRegion.width = iWidth;
    clsRegion.height = iHeight;
*/

    clsRegion.setXStart(iXStart);
    clsRegion.setYStart(iYStart);
    clsRegion.setWidth(iWidth);
    clsRegion.setHeight(iHeight);
    clsRegion.print();

    for(int ii=iXStart; ii<(iXStart+iWidth); ii++){
	for(int jj=iYStart; jj<(iYStart+iHeight); jj++){
	    setValue(fValue, ii, jj);
	}
    }
}


void ClsQStateArrayViewHex::setRegion(double fValue, ClsRegion _clsRegion) {
    clsRegion = _clsRegion;

    int iXStart = clsRegion.getXStart();
    int iYStart = clsRegion.getYStart();
    int iWidth  = clsRegion.getWidth();
    int iHeight = clsRegion.getHeight();


    for(int ii=iXStart; ii<(iXStart+iWidth); ii++){
	for(int jj=iYStart; jj<(iYStart+iHeight); jj++){
	    setValue(fValue, ii, jj);
	}
    }
}


// void ClsQStateArrayViewHex::mouseHandle( const QPoint &pos ) {
//     int iX = pos2index2( pos.x() );
//     int iY = pos2index2( pos.y() );

//     setValue(fFixedValue, iX, iY);

//     emit cellClicked( iX, iY);
// }

// void ClsQStateArrayViewHex::mouseHandleRegion( const QPoint &pos ) {
//     cout << "ClsQStateArrayViewHex::mouseHandleRegion( const QPoint &pos )" << endl;


//     int iPosX = pos.x();
//     iPosX = ( iPosX < BORDER ? BORDER : iPosX);

//     int iPosY = pos.y();
//     iPosY = ( iPosY < BORDER ? BORDER : iPosY);


//     int iX = pos2index2( iPosX );
//     int iY = pos2index2( iPosY );

//     int iW = iPosX - iXPosStart;
//     int iH = iPosY - iYPosStart;

//     if(iX>0 && iY>0 && iX<=iNrCellsX && iY<=iNrCellsY){

//	QBrush   qb(yellow);
//	QPainter paint( this );
//	qb.setColor(getColor(fFixedValue));
//	bitBlt(this, BORDER, BORDER, &pm);
//	paint.drawWinFocusRect ( iXPosStart, iYPosStart, iW, iH); //**
//     }

// }


// void ClsQStateArrayViewHex::mouseMoveEvent( QMouseEvent *e ) {

//     if(iSelectionMode == ClsBaseQStateArrayView::LIST){
//	mouseHandle( e->pos() );
//     }
//     else if (iSelectionMode == ClsBaseQStateArrayView::REGION){
//	mouseHandleRegion( e->pos() );
//     }
// }


// void ClsQStateArrayViewHex::mousePressEvent( QMouseEvent *e ) {
//     if ( e->button() == QMouseEvent::LeftButton ){

//	if (iSelectionMode == ClsBaseQStateArrayView::REGION){
//	    clear ();//**
//	}
//	if((iXPosStart * iYPosStart )==0){
// //	    cout << "set" << endl;

//	    iXPosStart = e->pos().x(); //**
//	    iYPosStart = e->pos().y(); //**

//	}
//	if(iSelectionMode == ClsBaseQStateArrayView::LIST){
//	    mouseHandle( e->pos() );
//	}
//	else if (iSelectionMode == ClsBaseQStateArrayView::REGION){
//	    mouseHandleRegion( e->pos() );
//	}
//     }
// }

// void ClsQStateArrayViewHex::mouseReleaseEvent ( QMouseEvent * e ) {
// //    cout << "ClsQStateArrayViewHex::mouseReleaseEvent ( QMouseEvent * e )" << endl;

// //--------------------
//     int iXIndexStart = pos2index2(iXPosStart);
//     iXIndexStart = (iXIndexStart <= 0 ? 1 : iXIndexStart);
//     iXIndexStart = (iXIndexStart > iNrCellsX ? iNrCellsX : iXIndexStart);

//     int iXIndex = pos2index2( e->pos().x());
//     iXIndex = (iXIndex <= 0 ? 1 : iXIndex);
//     iXIndex = (iXIndex > iNrCellsX ? iNrCellsX : iXIndex);

//     int iX = (iXIndexStart < iXIndex ? iXIndexStart : iXIndex);
//     int iW = abs(iXIndexStart - iXIndex) +1;
// //--------------------

// //--------------------
//     int iYIndexStart = pos2index2(iYPosStart);
//     iYIndexStart = (iYIndexStart <= 0 ? 1 : iYIndexStart);
//     iYIndexStart = (iYIndexStart > iNrCellsY ? iNrCellsY : iYIndexStart);

//     int iYIndex = pos2index2( e->pos().y());
//     iYIndex = (iYIndex <= 0 ? 1 : iYIndex);
//     iYIndex = (iYIndex > iNrCellsY ? iNrCellsY : iYIndex);

//     int iY = (iYIndexStart < iYIndex ? iYIndexStart : iYIndex);
//     int iH = abs(iYIndexStart - iYIndex) +1;
// //--------------------



//     if (iSelectionMode == ClsBaseQStateArrayView::REGION){
//	clear();
//	setRegion(fFixedValue, iX, iY, iW, iH);
//     }

//     iXPosStart = iYPosStart = 0; //**
// }


// void ClsQStateArrayViewHex::paintEvent( QPaintEvent *  ) {
// //    cout << "ClsQStateArrayViewHex::paintEvent( QPaintEvent *  )" << endl;

//     QPainter paint( this );
//     drawCheckerBoard(paint, 0, 0);
//     drawFrame( &paint );
// }

void ClsQStateArrayViewHex::createNullPixmap(){
    QBrush   qb(yellow);
//    QColor qc;

    pmGridEmpty.resize ( size().width()-2*BORDER, size().height()-2*BORDER);
    QPainter paint(&pmGridEmpty);;
    pmGridEmpty.fill(this->backgroundColor());
    drawCheckerBoard(paint, -BORDER, -BORDER);
    paint.end();
}

void ClsQStateArrayViewHex::drawCheckerBoard(QPainter &paint, int iOffsetX, int iOffsetY){
    QBrush   qb(yellow);
//    QColor qc;

/*
    for ( int i = 1; i <= iNrCellsX; i++ ) {
	for ( int j = 1; j <= iNrCellsY; j++ ) {
	    double fValue = vCells[i][j];
	    qc.setRgb((int)(fValue * fScaleFactor), (int)(fValue * fScaleFactor), (int)(fValue * fScaleFactor));
	    qb.setColor(qc);
	    qDrawPlainRect (&paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1, black, 0, &qb);
	}
    }
*/
    for ( int i = 1; i <= iNrCellsX; i++ ) {
//	for ( int j = 1; j <= iNrCellsY; j++ ) {
	int j = 1;
	    double fValue = vCells[i][j];
//	    qc.setRgb((int)(fValue * fScaleFactor), (int)(fValue * fScaleFactor), (int)(fValue * fScaleFactor));
//	    qb.setColor(qc);

	    qb.setColor(getColor(fValue));
	    paint.setBrush(qb);
//	    qDrawPlainRect (&paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize -1, iCellSize - 1, black, 0, &qb);

	    drawHexagon(paint, index2pos2( i )+iOffsetX, index2pos2( j )+iOffsetY, iCellSize, 1);

//	}
    }




}


void ClsQStateArrayViewHex::drawHexagon(QPainter &paint, int iXCenter, int iYCenter, int iVertexlength, int /* iOrientation */){

    const double f = 1.7320508;

    iXCenter = iXCenter + (int)((double)iVertexlength/4. * f);
    iYCenter = iYCenter + (int)((double)iVertexlength/2.) ;


    QPointArray pa(6);

/*
    pa[0] = QPoint(iVertexlength / 2,0);
    pa[1] = QPoint(iVertexlength / 4 ,-iVertexlength / 4 * f);
    pa[2] = QPoint(-iVertexlength / 4,-iVertexlength / 4 * f);
    pa[3] = QPoint(-iVertexlength / 2,0);
    pa[4] = QPoint(-iVertexlength / 4,iVertexlength / 4 * f);
    pa[5] = QPoint(iVertexlength / 4 ,iVertexlength / 4 * f);
*/

    pa[0] = QPoint(0,                             (int)(-iVertexlength/2));
    pa[1] = QPoint((int)(iVertexlength / 4 * f),  (int)(-iVertexlength / 4));
    pa[2] = QPoint((int)(iVertexlength / 4 * f),  (int)(iVertexlength / 4));
    pa[3] = QPoint(0,                             (int)(iVertexlength/2));
    pa[4] = QPoint((int)(-iVertexlength / 4 * f), (int)(iVertexlength / 4));
    pa[5] = QPoint((int)(-iVertexlength / 4 * f), (int)(-iVertexlength / 4));

    pa.translate(iXCenter, iYCenter);

    paint.drawPolygon ( pa, false );
}


int ClsQStateArrayViewHex::pos2index2( int _x ) {
    return ( _x - BORDER ) / iCellSize + 1;
}


int ClsQStateArrayViewHex::index2pos2( int i ) {
	return	( i - 1 ) * iCellSize + BORDER;
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../SystemGUI/ && make -k -j8 && cat /usr/share/apps/kget/sounds/finished.wav > /dev/dsp"
//// End:
