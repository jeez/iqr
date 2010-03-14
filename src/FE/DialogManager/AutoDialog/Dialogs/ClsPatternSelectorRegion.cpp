  /****************************************************************************
 ** $Filename: ClsPatternSelectorRegion.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
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

#include <qvalidator.h>

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

ClsPatternSelectorRegion::ClsPatternSelectorRegion( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
						    ClsBaseTopology *_clsBaseTopologyGroup,
						    const char *name ) : ClsPatternBaseSelector( parent, changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorRegion(); 
  
    bKeyboardInput = false;

//    setBackgroundColor(yellow);

    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
	clsQStateArrayView = new ClsQStateArrayViewRect(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
	clsQStateArrayView = new ClsQStateArrayViewHex(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
	clsQStateArrayView = new ClsQStateArrayViewSparse(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::REGION);
    }
  
    pqlayTop = new QGridLayout(this, 7, 2, 5, 5, name);

    pqlayTop->addMultiCellWidget(clsQStateArrayView, 0, 0,0,1, Qt::AlignHCenter | Qt::AlignBottom);

    qbtnSet = new QPushButton ("Set", this, "add");
    connect(qbtnSet, SIGNAL(clicked()), this, SLOT(addEntry()));
    pqlayTop->addWidget(qbtnSet, 1, 0, Qt::AlignTop );

    qbtnClear = new QPushButton ( "Clear", this, "clear");
    connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
    pqlayTop->addWidget(qbtnClear, 1,1, Qt::AlignTop );

    pqlayTop->addRowSpacing ( 1, 30 );
	

    QLabel *qlblStartX = new QLabel(this);
    qlblStartX->setText("Start X");
    pqlayTop->addWidget(qlblStartX, 2, 0);

//    qlblStartXValue = new QLabel(this);
    qlblStartXValue = new QLineEdit(this);
    pqlayTop->addWidget(qlblStartXValue, 2, 1);
    qlblStartXValue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    qlblStartXValue->setBackgroundColor(white);
//-----------------
    QLabel *qlblStartY = new QLabel(this);
    qlblStartY->setText("Start Y");
    pqlayTop->addWidget(qlblStartY, 3, 0);

//    qlblStartYValue = new QLabel(this);
    qlblStartYValue = new QLineEdit(this);
    pqlayTop->addWidget(qlblStartYValue, 3, 1);
    qlblStartYValue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    qlblStartYValue->setBackgroundColor(white);
//------------------
    QLabel *qlblStartWidth = new QLabel(this);
    qlblStartWidth->setText("Width");
    pqlayTop->addWidget(qlblStartWidth, 4, 0);

//    qlblStartWidthValue = new QLabel(this);
    qlblStartWidthValue = new QLineEdit(this);
    pqlayTop->addWidget(qlblStartWidthValue, 4, 1);
    qlblStartWidthValue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    qlblStartWidthValue->setBackgroundColor(white);
//-----------------
    QLabel *qlblStartHeight = new QLabel(this);
    qlblStartHeight->setText("Height");
    pqlayTop->addWidget(qlblStartHeight, 5, 0);

//    qlblStartHeightValue = new QLabel(this);
    qlblStartHeightValue = new QLineEdit(this);
    pqlayTop->addWidget(qlblStartHeightValue, 5, 1);
    qlblStartHeightValue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    qlblStartHeightValue->setBackgroundColor(white);
//-------



    pqlayTop->setRowStretch ( 6,2 );
    
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
//// compile-command: "cd ../ && make -k -j8"
//// End:




