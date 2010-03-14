/****************************************************************************
 ** $Filename: ClsPatternSelectorRegion.cpp
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Jun  1 00:17:17 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>

#include <QtGui/qvalidator.h>

#include "tagLibrary.hpp"
#include "ClsPatternSelectorRegion.h"
#include "ClsListItems.h"
#include "ClsBaseAutoGroup.h"
#include "ClsBaseTopology.h"

#include "ClsTopologyRect.h"
#include "ClsTopologyHex.h"
#include "ClsTopologySparse.h"

#include "ClsQStateArrayViewRect.h"
#include "ClsQStateArrayViewHex.h"
#include "ClsQStateArrayViewSparse.h"

#include <iqrUtils.h>
using namespace iqrcommon;

ClsPatternSelectorRegion::ClsPatternSelectorRegion( ClsBaseAutoGroup *changeReceiver, 
						    ClsBaseTopology *_clsBaseTopologyGroup,
						    const char *name ) : ClsPatternBaseSelector(changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorRegion(); 
  
    bKeyboardInput = false;

    clsQStateArrayView = NULL;
    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
	clsQStateArrayView = new ClsQStateArrayViewRect(NULL, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
//****	clsQStateArrayView = new ClsQStateArrayViewHex(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
//****	clsQStateArrayView = new ClsQStateArrayViewSparse(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }
 
    if(clsQStateArrayView!=NULL){
 
	QVBoxLayout* pqlayTop = new QVBoxLayout();

	this->setLayout(pqlayTop);
	pqlayTop->setContentsMargins( 0, 0, 0, 0 );

//    pqlayTop->addWidget(clsQStateArrayView, 0, 0, 0, 1, Qt::AlignHCenter | Qt::AlignBottom);
	pqlayTop->addWidget(clsQStateArrayView);

/* --- */

	QHBoxLayout* pqlayButtons = new QHBoxLayout();
	pqlayTop->addLayout(pqlayButtons);
	pqlayButtons->addStretch (5);	

	qbtnSet = new QPushButton ("Set");
	connect(qbtnSet, SIGNAL(clicked()), this, SLOT(addEntry()));
	pqlayButtons->addWidget(qbtnSet);

	qbtnClear = new QPushButton ( "Clear");
	connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
	pqlayButtons->addWidget(qbtnClear);
	pqlayButtons->addStretch (5);	

/* --- */

	QGridLayout* pqlayGrid = new QGridLayout();
	pqlayTop->addLayout(pqlayGrid);

	QPalette palette;
	

	QLabel *qlblStartX = new QLabel();
	qlblStartX->setText("Start X");
	pqlayGrid->addWidget(qlblStartX, 0, 0, Qt::AlignRight);

	qlblStartXValue = new QLineEdit();
	qlblStartXValue->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Maximum );

	pqlayGrid->addWidget(qlblStartXValue, 0, 1);
	palette.setColor(qlblStartXValue->backgroundRole(), Qt::white);
	qlblStartXValue->setPalette(palette);


//-----------------
	QLabel *qlblStartY = new QLabel();
	qlblStartY->setText("Start Y");
	pqlayGrid->addWidget(qlblStartY, 1, 0, Qt::AlignRight);

//    qlblStartYValue = new QLabel(this);
	qlblStartYValue = new QLineEdit();
	qlblStartYValue->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Maximum );

	pqlayGrid->addWidget(qlblStartYValue, 1, 1);
	palette.setColor(qlblStartYValue->backgroundRole(), Qt::white);
	qlblStartYValue->setPalette(palette);


//------------------
	QLabel *qlblStartWidth = new QLabel();
	qlblStartWidth->setText("Width");
	pqlayGrid->addWidget(qlblStartWidth, 2, 0, Qt::AlignRight);

	qlblStartWidthValue = new QLineEdit(this);
	qlblStartWidthValue->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Maximum );

	pqlayGrid->addWidget(qlblStartWidthValue, 2, 1);
	palette.setColor(qlblStartWidthValue->backgroundRole(), Qt::white);
	qlblStartWidthValue->setPalette(palette);

//-----------------
	QLabel *qlblStartHeight = new QLabel();
	qlblStartHeight->setText("Height");
	pqlayGrid->addWidget(qlblStartHeight, 3, 0, Qt::AlignRight);

	qlblStartHeightValue = new QLineEdit();
	qlblStartHeightValue->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Maximum );

	pqlayGrid->addWidget(qlblStartHeightValue, 3, 1);
	palette.setColor(qlblStartHeightValue->backgroundRole(), Qt::white);
	qlblStartHeightValue->setPalette(palette);

	pqlayTop->addStretch (5);

//-------

    
	iNrCellsHorizontal = _clsBaseTopologyGroup->nrCellsHorizontal();
	iNrCellsVertical =  _clsBaseTopologyGroup->nrCellsVertical();
	QValidator* validatorHorizontal = new QIntValidator( 1, iNrCellsHorizontal, this );
	qlblStartXValue->setValidator( validatorHorizontal );
	qlblStartWidthValue->setValidator( validatorHorizontal );

	QValidator* validatorVertical = new QIntValidator( 1, iNrCellsVertical, this );
	qlblStartYValue->setValidator( validatorVertical );
	qlblStartHeightValue->setValidator( validatorVertical );


	connect(qlblStartXValue, SIGNAL(textChanged( const QString & )), this, SLOT(textChanged( const QString & )));
	connect(qlblStartYValue, SIGNAL(textChanged( const QString & )), this, SLOT(textChanged( const QString & )));
	connect(qlblStartWidthValue, SIGNAL(textChanged( const QString & )), this, SLOT(textChanged( const QString & )));
	connect(qlblStartHeightValue, SIGNAL(textChanged( const QString & )), this, SLOT(textChanged( const QString & )));
    }
};

    
void ClsPatternSelectorRegion::addEntry() {
//    cout << "ClsPatternSelectorRegion::addEntry()" << endl;

    if(bKeyboardInput) {
	bool ok;
	
	int iStartX = qlblStartXValue->text().toInt( &ok, 10 );;
	if(!ok){
	    iStartX = 1;
	    qlblStartXValue->setText(iqrUtils::int2string(iStartX).c_str());
	}

	int iStartY = qlblStartYValue->text().toInt( &ok, 10 );
	if(!ok){
	    iStartY = 1;
	    qlblStartYValue->setText(iqrUtils::int2string(iStartY).c_str());
	}

	int iWidth = qlblStartWidthValue->text().toInt( &ok, 10 );
	if(!ok){
	    iWidth = 1;
	    qlblStartWidthValue->setText(iqrUtils::int2string(iWidth).c_str());
	}

	int iHeight = qlblStartHeightValue->text().toInt( &ok, 10 );
	if(!ok){
	    iHeight = 1;
	    qlblStartHeightValue->setText(iqrUtils::int2string(iHeight).c_str());
	}

	if(iStartX+iWidth>(iNrCellsHorizontal)){
	    iWidth = iNrCellsHorizontal-iStartX+1;
	    qlblStartWidthValue->setText(iqrUtils::int2string(iWidth).c_str());
	}
	
	if(iStartY+iHeight>(iNrCellsVertical)){
	    iHeight = iNrCellsVertical-iStartY+1;
	    qlblStartHeightValue->setText(iqrUtils::int2string(iHeight).c_str());
	}

	clsRegion.setXStart(iStartX);
	clsRegion.setYStart(iStartY);
	clsRegion.setWidth(iWidth);
	clsRegion.setHeight(iHeight);

	clsQStateArrayView->clear();
	clsQStateArrayView->setRegion(.5, clsRegion);
	
    } else {
	clsRegion = clsQStateArrayView->getRegion();
	
	
	qlblStartXValue->setText(    iqrUtils::int2string(clsRegion.getXStart() ).c_str());
	qlblStartYValue->setText(    iqrUtils::int2string(clsRegion.getYStart() ).c_str());
	qlblStartWidthValue->setText(iqrUtils::int2string(clsRegion.getWidth()  ).c_str());
	qlblStartHeightValue->setText(iqrUtils::int2string(clsRegion.getHeight()).c_str());
    }

    bKeyboardInput = false;
    changeReceiver->subItemChanged();
};


void ClsPatternSelectorRegion::clear() {
    cout << "ClsPatternSelectorRegion::clear()" << endl;
    clsQStateArrayView->clear();

    qlblStartXValue->setText("");
    qlblStartYValue->setText("");
    qlblStartWidthValue->setText("");
    qlblStartHeightValue->setText("");

    clsRegion.setXStart(0);
    clsRegion.setYStart(0);
    clsRegion.setWidth(0);
    clsRegion.setHeight(0);

    changeReceiver->subItemChanged();
    
};



void ClsPatternSelectorRegion::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation) {
//    cout << "ClsPatternSelectorRegion::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation)" << endl;

    if(_clsBaseSubPopulation!=NULL){
	ClsRegion *_clsRegion= static_cast<ClsRegion*>(_clsBaseSubPopulation);
	clsRegion  = *_clsRegion;
	
	qlblStartXValue->setText(    iqrUtils::int2string(clsRegion.getXStart() ).c_str());
	qlblStartYValue->setText(    iqrUtils::int2string(clsRegion.getYStart() ).c_str());
	qlblStartWidthValue->setText(iqrUtils::int2string(clsRegion.getWidth()  ).c_str());
	qlblStartHeightValue->setText(iqrUtils::int2string(clsRegion.getHeight()).c_str());
	
	bKeyboardInput = false;
	clsQStateArrayView->setRegion(.5, clsRegion);
    }
}

    
ClsBaseSubPopulation* ClsPatternSelectorRegion::getSubPopulation() {
//    cout << "ClsPatternSelectorRegion::getSubPopulation()" << endl;
    ClsRegion* _clsRegion = new ClsRegion();
    *_clsRegion = clsRegion;
    return _clsRegion;
}
 
void ClsPatternSelectorRegion::textChanged( const QString & ){
//    cout << "ClsPatternSelectorRegion::textChanged" << endl;
    bKeyboardInput = true;
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j2"
//// End:




