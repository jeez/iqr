/****************************************************************************
 ** $Filename: ClsBaseQStateArrayView.cpp
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Thu Jul  3 22:41:58 2003
 **
 *****************************************************************************/
#include <iostream>

#include <QImage>
#include <qdrawutil.h>
#include <QCheckBox>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPolygon>

#include "ClsBaseQStateArrayView.h"
#include "ClsBaseTopology.h"

//#define DEBUG_CLSBASEQSTATEARRAYVIEW

ClsBaseQStateArrayView::ClsBaseQStateArrayView( QWidget *parent,
						ClsBaseTopology *_clsBaseTopologyGroup,
						const char *_name,
						unsigned int _iSelectionMode) :
    QFrame( parent, _name, Qt::WDestructiveClose ), clsBaseTopologyGroup(_clsBaseTopologyGroup), iSelectionMode(_iSelectionMode) {
//    cout << "ClsBaseQStateArrayView::ClsBaseQStateArrayView( ... )" << endl;
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::ClsBaseQStateArrayView( QWidget *parent, ClsBaseTopology *_clsBaseTopologyGroup, const char *name, unsigned int _iSelectionMode)" << endl;
#endif

    iColorMode = ClsBaseQStateArrayView::GRAY;
    fFixedValue = 0.5;
    fValMin = 0;
    fValMax = 1.0;
    fScaleFactor = 255.0;
    iXPosStart = iYPosStart = 0;

};


QColor ClsBaseQStateArrayView::getColor(double fValue) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::getColor(double fValue)" << endl;
#endif

    QColor qc;

    if(iColorMode == ClsBaseQStateArrayView::GRAY){
	double fColorValue = (fValue - fValMin) * fScaleFactor;
	qc.setRgb((int)(fColorValue), (int)(fColorValue), (int)(fColorValue));
    }
    else if(iColorMode == ClsBaseQStateArrayView::BLUE2RED){
	double fCenter = (fValMin + fValMax)/2.;
	double fColorValue = fabs(fValue - fCenter) *fScaleFactor * 2.;
	if(fValue < fCenter ){
	    qc.setRgb(0, 0, (int)(fColorValue));
	}
	else {
	    qc.setRgb((int)(fColorValue), 0, 0);
	}
    }
    else {
	double fColorValue = (fValue - fValMin) * fScaleFactor;
	qc.setHsv((int)(fColorValue), 255,255);
    }

    return qc;
}


const ClsRegion ClsBaseQStateArrayView::getRegion(){
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::getRegion()" << endl;
#endif
    return clsRegion;
};


QPixmap ClsBaseQStateArrayView::getValuePixmap(int iImgWidth, int iImgHeight ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::getValuePixmap(int iImgWidth, int iImgHeight )" << endl;
#endif
    
/* this works but is reeeeeally slow...
    QPixmap pmTemp;
    QImage qimg2 = pmGridValues.convertToImage().copy(0, 0, 
						      pmGridValues.width()-BORDER, 
						      pmGridValues.height()-BORDER);
    
    qimg2.scale(iImgWidth, iImgHeight);
    
    pmTemp.convertFromImage(qimg2);
*/


    QPixmap pmTemp(iImgWidth, iImgHeight);
    pmTemp.fill(this->backgroundColor());
    bitBlt(&pmTemp, 0, 0, &pmGridValues, 0,0);
    return pmTemp;
}


QPixmap ClsBaseQStateArrayView::getGradientPixmap(int iImgWidth, int iImgHeight, int _iColorMode ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::getGradientPixmap(int iImgWidth, int iImgHeight)" << endl;
#endif

//    int iColorMode = ClsBaseQStateArrayView::GRAY;
//    int iColorMode = ClsBaseQStateArrayView::BLUE2RED;
//    int iColorMode = ClsBaseQStateArrayView::HSV;

    QPixmap pmGradient;
    QPainter* paintGradient = new QPainter();
    QWMatrix mxRot;
    int iDiag = (int)(sqrt(double(iImgWidth * iImgWidth + iImgHeight * iImgHeight))/2.);

    pmGradient.resize(2 * iDiag, 2 * iDiag);
    paintGradient->begin(&pmGradient);

    paintGradient->setWindow( 0, 0, 2 * iDiag, 2 * iDiag );

    int iNrSices = 50;
    for ( int i=0; i<iNrSices; i++ ) {
	paintGradient->setWorldMatrix( mxRot );
	QColor c;
	if(_iColorMode == ClsBaseQStateArrayView::GRAY){
	    c.setRgb( i* 255 / iNrSices,  i* 255 / iNrSices,  i* 255 / iNrSices );
	}
	else if(_iColorMode == ClsBaseQStateArrayView::BLUE2RED){
	    if(i<iNrSices/2){
		/* BLUE */
		c.setRgb(0, 0,  255 - i * 510/iNrSices);
	    }
	    else {
		/* RED */
		c.setRgb( (i - iNrSices/2) * 255/(iNrSices/2),  0,0);
	    }
	}
	else {
	    c.setHsv( i* 360 / iNrSices, 255, 255 );
	}
	paintGradient->setBrush( c );
	paintGradient->setPen( c );
//zzz	QPointArray a;
	QPolygon a;

	a.setPoints( 4,
		     0, 0,
		     iDiag * 2 / iNrSices, 0,
		     iDiag * 2 / iNrSices, iDiag * 2,
		     0, iDiag * 2 );
	paintGradient->drawPolygon( a );
	mxRot.translate( (double)iDiag * 2.0 / (double)iNrSices, 0.0  );
    }

    paintGradient->end();
    return pmGradient;

}

void ClsBaseQStateArrayView::setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::setRegion(double fValue, int iXStart, int iYStart, int iWidth, int iHeight)" << endl;
#endif
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


void ClsBaseQStateArrayView::setRegion(double fValue, ClsRegion _clsRegion) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::setRegion(double fValue, ClsRegion _clsRegion)" << endl;
#endif
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

void ClsBaseQStateArrayView::mouseHandle( const QPointF &_pos ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::mouseHandle( const QPointF &_pos )" << endl;
#endif
    int iX = pos2index2( _pos.x() );
    int iY = pos2index2( _pos.y() );

    if(iSelectionMode == ClsBaseQStateArrayView::SINGLE){
	clear();
    }
    setValue(fFixedValue, iX, iY);
    emit cellClicked( iX, iY);
}

void ClsBaseQStateArrayView::mouseMoveEvent( QMouseEvent *e ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::mouseMoveEvent( QMouseEvent *e )" << endl;
#endif
    if(iSelectionMode == ClsBaseQStateArrayView::LIST || iSelectionMode == ClsBaseQStateArrayView::SINGLE){
	mouseHandle( e->pos() );
    }
    else if (iSelectionMode == ClsBaseQStateArrayView::REGION){
	mouseHandleRegion( e->pos() );
    }
}

void ClsBaseQStateArrayView::mousePressEvent( QMouseEvent *e ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::mousePressEvent( QMouseEvent *e )" << endl;
#endif
    if ( e->button() == Qt::LeftButton ){
	if (iSelectionMode == ClsBaseQStateArrayView::REGION || iSelectionMode == ClsBaseQStateArrayView::SINGLE){
	    clear ();
	}
	if((iXPosStart * iYPosStart )==0){
	    iXPosStart = e->pos().x();
	    iYPosStart = e->pos().y();
	}
	if(iSelectionMode == ClsBaseQStateArrayView::LIST || iSelectionMode == ClsBaseQStateArrayView::SINGLE){
	    mouseHandle( e->pos() );
	}
	else if (iSelectionMode == ClsBaseQStateArrayView::REGION){
	    mouseHandleRegion( e->pos() );
	}
	else if (iSelectionMode == 100){
	    mouseHandleEllipse( e->pos () );
	}
    }
}

void ClsBaseQStateArrayView::mouseHandleRegion( const QPointF &_pos ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::mouseHandleRegion( const QPointF &_pos )" << endl;
#endif
//    cout << "_pos.x(), _pos.y(): " << _pos.x() << ", " << _pos.y() << endl;

    int iPosX = _pos.x();
    int iPosY = _pos.y();

    int iXMin = contentsRect().left() + 1;
    int iYMin = contentsRect().top() + 1;
    int iXMax = contentsRect().right() - 1;
    int iYMax = contentsRect().bottom() - 1;

    iPosX = (iPosX>iXMin ? iPosX : iXMin);
    iPosY = (iPosY>iYMin ? iPosY : iYMin);

    iPosX = (iPosX<iXMax ? iPosX : iXMax);
    iPosY = (iPosY<iYMax ? iPosY : iYMax);

    int iW = iPosX - iXPosStart;
    int iH = iPosY - iYPosStart;

    QBrush   qb(Qt::yellow);
    QPainter paint( this );
    qb.setColor(getColor(fFixedValue));

/* unneeded ?
    bitBlt(this, BORDER, BORDER, &pmGridEmpty);
*/

//zzz    paint.drawWinFocusRect ( iXPosStart, iYPosStart, iW, iH); //**
    paint.drawRect ( iXPosStart, iYPosStart, iW, iH); //**
}

void ClsBaseQStateArrayView::mouseReleaseEvent ( QMouseEvent * e ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::mouseReleaseEvent ( QMouseEvent * e )" << endl;
#endif
//--------------------
    int iXIndexStart = pos2index2(iXPosStart);
    iXIndexStart = (iXIndexStart <= 0 ? 1 : iXIndexStart);
    iXIndexStart = (iXIndexStart > iNrCellsX ? iNrCellsX : iXIndexStart);

    int iXIndex = pos2index2( e->pos().x());
    iXIndex = (iXIndex <= 0 ? 1 : iXIndex);
    iXIndex = (iXIndex > iNrCellsX ? iNrCellsX : iXIndex);

    int iX = (iXIndexStart < iXIndex ? iXIndexStart : iXIndex);
    int iW = abs(iXIndexStart - iXIndex) +1;
//--------------------

//--------------------
    int iYIndexStart = pos2index2(iYPosStart);
    iYIndexStart = (iYIndexStart <= 0 ? 1 : iYIndexStart);
    iYIndexStart = (iYIndexStart > iNrCellsY ? iNrCellsY : iYIndexStart);

    int iYIndex = pos2index2( e->pos().y());
    iYIndex = (iYIndex <= 0 ? 1 : iYIndex);
    iYIndex = (iYIndex > iNrCellsY ? iNrCellsY : iYIndex);

    int iY = (iYIndexStart < iYIndex ? iYIndexStart : iYIndex);
    int iH = abs(iYIndexStart - iYIndex) +1;
//--------------------

    if (iSelectionMode == ClsBaseQStateArrayView::REGION){
	clear();
	setRegion(fFixedValue, iX, iY, iW, iH);

    }

    iXPosStart = iYPosStart = 0; //**
}


void ClsBaseQStateArrayView::paintEvent( QPaintEvent *  ) {
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::paintEvent( QPaintEvent *  )" << endl;
#endif
    QPainter paint( this );

/*
    drawCheckerBoard(paint, 0, 0);
    drawFrame( &paint );
*/


   bitBlt(this, BORDER, BORDER, &pmGridValues);
   /*this leads to a crash on windows (do we need this at all???)
     drawFrame( &paint );
   */

}

void ClsBaseQStateArrayView::markCell(int iColor, int iX, int iY){
//IS THIS NEEDED AT ALL??? //#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
//IS THIS NEEDED AT ALL???     cout << "ClsBaseQStateArrayView::markCell(int iColor, int iX, int iY)" << endl;
//IS THIS NEEDED AT ALL??? //#endif
//IS THIS NEEDED AT ALL???     QPainter paint(&pmGridEmpty);
//IS THIS NEEDED AT ALL???     QPen qp;
//IS THIS NEEDED AT ALL???     QColor qc;
//IS THIS NEEDED AT ALL???     qc.setHsv(iColor, 255,255);
//IS THIS NEEDED AT ALL???     qp.setColor(qc);
//IS THIS NEEDED AT ALL???     qp.setWidth (1 );
//IS THIS NEEDED AT ALL???     paint.setPen(qp);
//IS THIS NEEDED AT ALL??? 
//IS THIS NEEDED AT ALL??? //    paint.drawRect(index2pos2( iX ) - 1 -BORDER, index2pos2( iY ) -1 -BORDER, iCellSize +1, iCellSize +1);
//IS THIS NEEDED AT ALL??? ////	paint.drawRect(index2pos2( iX ) - 1 -BORDER, index2pos2( iY ) -1 -BORDER, iCellSize +4, iCellSize +4);
//IS THIS NEEDED AT ALL??? //
//IS THIS NEEDED AT ALL??? 
//IS THIS NEEDED AT ALL??? /* unneeded ?
//IS THIS NEEDED AT ALL???     bitBlt(this, BORDER, BORDER, &pmGridEmpty);
//IS THIS NEEDED AT ALL??? */
//IS THIS NEEDED AT ALL???     paint.end();
//IS THIS NEEDED AT ALL???     repaint();

}

void ClsBaseQStateArrayView::markCell(int iColor, list<int> lstIndex){
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::markCell(int iColor, list<int> lstIndex)" << endl;
#endif

    list<int>::iterator it;
    for(it=lstIndex.begin();it!=lstIndex.end();it++){
	int iIndex = (*it);
//	cout << "iIndex: " << iIndex << endl;
	pair<int, int> pPoint = clsBaseTopologyGroup->index2pos(iIndex);
//	cout << "pPoint.first, pPoint.second: " << pPoint.first << ", " <<  pPoint.second << endl;
	markCell(iColor, pPoint.first, pPoint.second);
    }
}

void ClsBaseQStateArrayView::createNullPixmap(){
#ifdef DEBUG_CLSBASEQSTATEARRAYVIEW
    cout << "ClsBaseQStateArrayView::createNullPixmap()" << endl;
#endif

    QBrush   qb(Qt::yellow);
    pmGridEmpty.resize ( size().width()-2*BORDER, size().height()-2*BORDER);
    QPainter paint(&pmGridEmpty);
    pmGridEmpty.fill(this->backgroundColor());
    drawCheckerBoard(paint, -BORDER, -BORDER);
    paint.end();
//--    bitBlt(this, BORDER, BORDER, &pmGridEmpty);


    
    pmGridValues.resize ( size().width()-2*BORDER, size().height()-2*BORDER);
    QPainter paintPM(&pmGridValues);
    pmGridValues.fill(this->backgroundColor());
    drawCheckerBoard(paintPM, -BORDER, -BORDER);
//cout << __FILE__ << ":" << __LINE__ << endl;
    paintPM.end();

}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
