/****************************************************************************
 ** $Filename: ClsSelectorSparse.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Jul  7 18:21:10 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>



#include "ClsSelectorSparse.h"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
#include "memberWidget.h"

/*
#include "boolParameter.hpp"
#include "doubleParameter.hpp"
#include "intParameter.hpp"
#include "optionsParameter.hpp"
#include "stringParameter.hpp"
*/

#include "ClsTopologySparse.h"



#ifdef DEBUG_CLSAUTODIALOG
static const bool bDebugAutoGroup = true;
#else
static const bool bDebugAutoGroup = false;
#endif

using namespace iqrcommon;

iqrfe::ClsSelectorSparse::ClsSelectorSparse(ClsItem &_item, bool _bModal, QWidget *_pqwgtParent, const char *_pcName, string strValue)
    : ClsBaseAutoGroup(_item, _pqwgtParent, _pcName) {

    cout << "ClsSelectorSparse::ClsSelectorSparse(...)" << endl;

    if (bDebugAutoGroup) {
	cout << "ClsSelectorSparse::ClsSelectorSparse" << endl;
    }

    string strTitle = _pcName;
    strTitle.append(" : ");
    strTitle.append(strValue);
    
    setTitle(strTitle.c_str());

//    setBackgroundColor(red);     

    
    QVBoxLayout *pqlayTop = new QVBoxLayout(this, 14, -1, "auto dialog layout");

    qtablePoints = new QTable( 0, 2, this );
    QHeader *header = qtablePoints->horizontalHeader();
    header->setLabel( 0, QObject::tr( "x" ));
    header->setLabel( 1, QObject::tr( "y" ));
    qtablePoints->setHScrollBarMode (QScrollView::AlwaysOff);
    qtablePoints->setSelectionMode(QTable::MultiRow);
     


    connect(qtablePoints, SIGNAL(valueChanged(int, int)), this, SLOT(tableChanged(int, int)));
    pqlayTop->addWidget(qtablePoints);


    clsTopologySparse = static_cast<ClsTopologySparse*>(&_item);

//--    list<pair<int, int> > lst  = clsTopologySparse->getList();;
//--    list<pair<int, int> >::iterator it;

    vector<tiPoint> lst  = clsTopologySparse->getList();;
    vector<tiPoint>::iterator it;
    

    int iRowCount = 0;
    for(it=lst.begin(); it!=lst.end();it++){
//	 qtablePoints->setNumRows(iRowCount+1);
	qtablePoints->insertRows(iRowCount, 1);
	int iX = (*it).first;
	int iY = (*it).second;
	 
	qtablePoints->setText(iRowCount, 0, iqrUtils::int2string(iX).c_str());
	qtablePoints->setText(iRowCount, 1, iqrUtils::int2string(iY).c_str());
	iRowCount++;
    }

    QHBoxLayout *pqlayTableControls = new QHBoxLayout(pqlayTop, 1, "selectors");
    qbtnAddRow = new QPushButton("add row", this, "add");
    connect(qbtnAddRow, SIGNAL(clicked()), this, SLOT(addRow()));
    pqlayTableControls->addWidget(qbtnAddRow);
     
    qbtnDeleteRow = new QPushButton("delete row(s)", this, "delete");
    connect(qbtnDeleteRow, SIGNAL(clicked()), this, SLOT(deleteRow()));
    pqlayTableControls->addWidget(qbtnDeleteRow);

/*
    QHBoxLayout *pqlayTableIO = new QHBoxLayout(pqlayTop, 1, "selectors");
    qbtnSavePoint = new QPushButton("save", this, "save");
    connect(qbtnSavePoint, SIGNAL(clicked()), this, SLOT(saveData()));
    pqlayTableIO->addWidget(qbtnSavePoint);
     
    qbtnLoadPoint = new QPushButton("load", this, "load");
    connect(qbtnLoadPoint, SIGNAL(clicked()), this, SLOT(loadData()));
    pqlayTableIO->addWidget(qbtnLoadPoint);
*/     


    if (_bModal) {
 	pqwgtButtons = new ClsModalButtons(this, QCString(_pcName)+" buttons");
 	
 	// OK in modal dialog applies changes and closes dialog.
 	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
 	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));
 
 	// Cancel closes dialog without applying changes.
 	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
 	pqwgtButtons = new ClsModelessButtons(this, QCString(_pcName)+" buttons");
 	
 	connect(this, SIGNAL(changed()), pqwgtButtons, SLOT(enableApply()));
 	connect(pqwgtButtons, SIGNAL(apply()), this, SLOT(apply()));
 	// How should the Undo button be handled?
 	connect(pqwgtButtons, SIGNAL(close()), this, SLOT(close()));
    }
 
    pqlayTop->addWidget(pqwgtButtons, BUTTON_V_STRETCH);
}

iqrfe::ClsSelectorSparse::~ClsSelectorSparse() {
    if (bDebugAutoGroup) {
	cout << "ClsSelectorSparse::~ClsSelectorSparse" << endl;
    }
}

void iqrfe::ClsSelectorSparse::apply() {
    if (bDebugAutoGroup) {
	cout << "ClsSelectorSparse::apply" << endl;
    }
    
//--    list<pair<int, int> > lst;
    vector<tiPoint> lst;
    
    
    for(int ii=0; ii<qtablePoints->numRows();ii++){
	
	int iX = validateCell( ii, 0, false );
	int iY = validateCell( ii, 1, false );
	
	cout << "iX, iY: " << iX << ", " << iY << endl;
	
	if(iX > 0 && iY>0){
//--	    pair<int, int> pairPoint(iX, iY);
	    tiPoint pairPoint;
	    pairPoint.first = iX;
	    pairPoint.second = iY;
	    lst.push_back(pairPoint);
	}
	
	
    }
    
    clsTopologySparse->setList(lst);
    
}
    

void iqrfe::ClsSelectorSparse::closeEvent ( QCloseEvent *  ) {
    cout << "ClsSelectorSparse::closeEvent ( QCloseEvent * e )" << endl;
    emit autoGroupClosed(strName, "", ClsMemberWidget::HIDE);
}

void iqrfe::ClsSelectorSparse::tableChanged(int iR, int iC) {
    validateCell(iR, iC, true);
    emit changed();
}


void iqrfe::ClsSelectorSparse::addRow() {
    qtablePoints->insertRows(qtablePoints->numRows(), 1);    
}

void iqrfe::ClsSelectorSparse::deleteRow() {
    for(int ii=0; ii<qtablePoints->numRows();ii++){
	cout << "qtablePoints->isRowSelected(ii): " << qtablePoints->isRowSelected(ii) << endl;
	if(qtablePoints->isRowSelected(ii)){
	    qtablePoints->removeRow(ii);
	}
    }
}

int iqrfe::ClsSelectorSparse::validateCell(int iR, int iC, bool bAllowEmpty) {
    int iReturn = -1;

    QString qstr = qtablePoints->text( iR, iC ).stripWhiteSpace();

    if(!qstr.length()<=0){
	for(unsigned int jj=0; jj<qstr.length(); jj++) {
	    QChar qc = qstr[jj];
	    if(!qc.isDigit()){
		qtablePoints->setText(iR, iC, "invalid entry");
		return iReturn;
	    }
	}
	try{
	    iReturn = iqrUtils::string2int(qstr.latin1());
	}
	catch(...){
	    qtablePoints->setText(iR, iC, "invalid entry");
	}
    } else {
	if(!bAllowEmpty){
	    qtablePoints->setText(iR, iC, "invalid entry");
	}
    }
    return iReturn;
}

/*
void iqrfe::ClsSelectorSparse::saveData() {
    cout << "ClsSelectorSparse::saveData()" << endl;
    
}
*/

/*
void iqrfe::ClsSelectorSparse::loadData() {
    cout << "ClsSelectorSparse::loadData()" << endl;


}
*/



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:



    
