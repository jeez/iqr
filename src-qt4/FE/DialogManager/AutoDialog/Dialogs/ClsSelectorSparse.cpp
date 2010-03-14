/****************************************************************************
 ** $Filename: ClsSelectorSparse.cpp
 ** $Id$
 **
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

#include <QStringList>

#include "ClsSelectorSparse.h"
#include "autoWidget.hpp"
#include "dialogButtons.hpp"
#include "item.hpp"
#include "memberWidget.h"

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

    QVBoxLayout *pqlayTop = new QVBoxLayout(this /*, 14, -1, "auto dialog layout"*/);

    qtablePoints = new QTableWidget( 0, 2, this );
//ZZZ    QHeader *header = qtablePoints->horizontalHeader();
//ZZZ    header->setLabel( 0, QObject::tr( "x" ));
//ZZZ    header->setLabel( 1, QObject::tr( "y" ));

    QStringList qstrlstHeaders;
    qstrlstHeaders << "x";
    qstrlstHeaders << "y";
    qtablePoints->setHorizontalHeaderLabels(qstrlstHeaders);


//ZZZ FIX    qtablePoints->setHScrollBarMode (QScrollView::AlwaysOff);
//ZZZ FIX    qtablePoints->setSelectionMode(QTable::MultiRow);
     



    connect(qtablePoints, SIGNAL(valueChanged(int, int)), this, SLOT(tableChanged(int, int)));
    pqlayTop->addWidget(qtablePoints);


    clsTopologySparse = static_cast<ClsTopologySparse*>(&_item);

    QTableWidgetItem *newItem= new QTableWidgetItem();
    list<pair<int, int> > lst  = clsTopologySparse->getList();;
    list<pair<int, int> >::iterator it;
    int iRowCount = 0;
    for(it=lst.begin(); it!=lst.end();it++){
//	 qtablePoints->setNumRows(iRowCount+1);
	qtablePoints->insertRow(iRowCount);
	int iX = (*it).first;
	int iY = (*it).second;
	 
//ZZZ	qtablePoints->setText(iRowCount, 0, iqrUtils::int2string(iX).c_str());
//ZZZ	qtablePoints->setText(iRowCount, 1, iqrUtils::int2string(iY).c_str());

	newItem->setText(iqrUtils::int2string(iX).c_str());
	qtablePoints->setItem(iRowCount, 0, newItem);

	newItem->setText(iqrUtils::int2string(iY).c_str());
	qtablePoints->setItem(iRowCount, 1, newItem);


	iRowCount++;
    }

    QHBoxLayout *pqlayTableControls = new QHBoxLayout(this /*, 1, "selectors"*/);
    pqlayTop->addLayout(pqlayTableControls);

    qbtnAddRow = new QPushButton("add row", this/*, "add"*/);
    connect(qbtnAddRow, SIGNAL(clicked()), this, SLOT(addRow()));
    pqlayTableControls->addWidget(qbtnAddRow);
     
    qbtnDeleteRow = new QPushButton("delete row(s)", this/*, "delete"*/);
    connect(qbtnDeleteRow, SIGNAL(clicked()), this, SLOT(deleteRow()));
    pqlayTableControls->addWidget(qbtnDeleteRow);

    if (_bModal) {
 	pqwgtButtons = new ClsModalButtons(this /*, QCString(_pcName)+" buttons"*/);
 	
 	// OK in modal dialog applies changes and closes dialog.
 	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(apply()));
 	connect(pqwgtButtons, SIGNAL(ok()), this, SLOT(close()));
 
 	// Cancel closes dialog without applying changes.
 	connect(pqwgtButtons, SIGNAL(cancel()), this, SLOT(close()));
    } else {
 	pqwgtButtons = new ClsModelessButtons(this /*, QCString(_pcName)+" buttons"*/);
 	
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
    
    list<pair<int, int> > lst;
    for(int ii=0; ii<qtablePoints->rowCount();ii++){
	int iX = validateCell( ii, 0, false );
	int iY = validateCell( ii, 1, false );
	
//	cout << "iX, iY: " << iX << ", " << iY << endl;
	
	if(iX > 0 && iY>0){
	    pair<int, int> pairPoint(iX, iY);
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
    qtablePoints->insertRow(qtablePoints->rowCount());    
}

void iqrfe::ClsSelectorSparse::deleteRow() {
    for(int ii=0; ii<qtablePoints->rowCount();ii++){
//	cout << "qtablePoints->isRowSelected(ii): " << qtablePoints->isRowSelected(ii) << endl;
//	if(qtablePoints->isRowSelected(ii)){
	if(qtablePoints->selectionModel()->isRowSelected(ii, QModelIndex())){ //ZZZ THIS IS SICK
	    qtablePoints->removeRow(ii);
	}
    }
}

int iqrfe::ClsSelectorSparse::validateCell(int iR, int iC, bool bAllowEmpty) {
    int iReturn = -1;

//ZZZ    QString qstr = qtablePoints->text( iR, iC ).stripWhiteSpace();
    QTableWidgetItem *qtwi = qtablePoints->itemAt ( iR, iC);
    if(qtwi!=NULL){
	QString qstr = qtwi->text().trimmed();
	if(!qstr.length()<=0){
	    for(unsigned int jj=0; jj<qstr.length(); jj++) {
		QChar qc = qstr[jj];
		if(!qc.isDigit()){
//ZZZ		qtablePoints->setText(iR, iC, "invalid entry");
		    QTableWidgetItem *newItem= new QTableWidgetItem("invalid entry");
		    qtablePoints->setItem(iR, iC, newItem);
		    return iReturn;
		}
	    }
	    try{
		iReturn = iqrUtils::string2int(qstr.toStdString());
	    }
	    catch(...){
//ZZZ	    qtablePoints->setText(iR, iC, "invalid entry");
		QTableWidgetItem *newItem= new QTableWidgetItem("invalid entry");
		qtablePoints->setItem(iR, iC, newItem);
	    }
	} else {
	    if(!bAllowEmpty){
//ZZZ	    qtablePoints->setText(iR, iC, "invalid entry");
		QTableWidgetItem *newItem= new QTableWidgetItem("invalid entry");
		qtablePoints->setItem(iR, iC, newItem);
	    }
	}
	return iReturn;
    }
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j4"
//// End:



    
