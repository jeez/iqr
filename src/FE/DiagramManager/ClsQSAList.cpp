/****************************************************************************
 ** $Filename: ClsQSAList.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Sep  5 01:54:07 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


/*

stuff to do:

- store lists from SA
- reorder SA
- return data from SA (on double click...)

*/
#include <iostream>

#include <qfiledialog.h> 
#include <qwmatrix.h> 
#include <qtooltip.h>
#include <qbuttongroup.h> 
#include <qlayout.h> 
#include <qpushbutton.h> 
#include <qmessagebox.h> 


#include "ClsQSAList.h"
#include "ClsQGroupStateManip.h"

namespace {
#include <up_12x12.xpm>
#include <down_12x12.xpm>
#include <invert2_12x12.xpm>
#include <trash2_12x12.xpm>
//#include <filesave_14x14.xpm>
#include <filesave_12x12.xpm>
//#include <filesaveas_16x14.xpm>
#include <filesaveas_12x12.xpm>
//#include <fileopen_16x14.xpm>
#include <fileopen_12x12.xpm>
}


ClsQSAList::ClsQSAList ( QWidget * parent, ClsQGroupStateManip* _clsQGroupStateManip, const char * name, WFlags f) : 
    QFrame ( parent, name, f ), clsQGroupStateManip(_clsQGroupStateManip)  {


    strFilename = "";///home/ulysses/STATESEQ";
    
    QVBoxLayout* layoutMain = new QVBoxLayout( this, 5, -1, "mainL");


/* ------------------------------- */
    qlbox = new QListBox( this );
    qlbox->setVScrollBarMode(QScrollView::AlwaysOn);
//    qlbox->setHScrollBarMode(QScrollView::AlwaysOff);
//    qlbox->setColumnMode ( QListBox::FitToWidth );
//    qlbox->setColumnMode ( QListBox::FixedNumber );
    qlbox->setFixedWidth ( 150 );
//    qlbox->setResizePolicy(QScrollView::AutoOne);

    qlbox->setSelectionMode(QListBox::Extended);

    cQListBoxToolTip = new GQListBoxToolTip(qlbox);
   
    connect(qlbox, SIGNAL(doubleClicked(QListBoxItem*)), this, SLOT(slotDoubleClicked(QListBoxItem*)));
    layoutMain->addWidget(qlbox,10);
//    layoutMain->addMultiCellWidget(qlbox, 0, 0,0,3);


    QGridLayout* layoutButtons = new QGridLayout( 2, 6, -1, "layoutButtons");
    layoutButtons->setColStretch ( 0, 50 );

/* ------------------------------- */	
//--    QHBoxLayout* qlayoutEdit = new QHBoxLayout( layoutMain);

    QPushButton* qpbtnMoveUp = new QPushButton (QIconSet(QPixmap(up)), "",this);
    qpbtnMoveUp->setFlat(TRUE);
    connect(qpbtnMoveUp, SIGNAL(clicked()), this, SLOT(slotMoveUp()));
//--    qlayoutEdit->addWidget(qpbtnMoveUp);
    layoutButtons->addWidget(qpbtnMoveUp,0,1);
	
    QPushButton* qpbtnMoveDown = new QPushButton (QIconSet(QPixmap(down)), "",this);
    qpbtnMoveDown->setFlat(TRUE);
    connect(qpbtnMoveDown, SIGNAL(clicked()), this, SLOT(slotMoveDown()));
//--    qlayoutEdit->addWidget(qpbtnMoveDown);
    layoutButtons->addWidget(qpbtnMoveDown,0,2);

    QPushButton* qpbtnInvert = new QPushButton (QIconSet(QPixmap(invert2)), "",this);
    qpbtnInvert->setFlat(TRUE);
    connect(qpbtnInvert, SIGNAL(clicked()), this, SLOT(slotInvert()));
//--    qlayoutEdit->addWidget(qpbtnInvert);
    layoutButtons->addWidget(qpbtnInvert,0,3);

    QPushButton* qpbtnDelete = new QPushButton (QIconSet(QPixmap(trash2)), "",this);
    connect(qpbtnDelete, SIGNAL(clicked()), this, SLOT(slotDeleteItem()));
    qpbtnDelete->setFlat(TRUE);
//--    qlayoutEdit->addWidget(qpbtnDelete);
    layoutButtons->addWidget(qpbtnDelete,0,4);

/* ------------------------------- */
//--    QHBoxLayout* qlayout2 = new QHBoxLayout( layoutMain);

    QPushButton* qpbtnSave = new QPushButton (QIconSet(QPixmap(filesave)), "",this);
    connect(qpbtnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
    qpbtnSave->setFlat(TRUE);
//--    qlayout2->addWidget(qpbtnSave);
    layoutButtons->addWidget(qpbtnSave,1,1);

    QPushButton* qpbtnSaveAs = new QPushButton (QIconSet(QPixmap(filesaveas)), "",this);
    connect(qpbtnSaveAs, SIGNAL(clicked()), this, SLOT(slotSaveAs()));
    qpbtnSaveAs->setFlat(TRUE);
//--    qlayout2->addWidget(qpbtnSaveAs);
    layoutButtons->addWidget(qpbtnSaveAs,1,2);

    QPushButton* qpbtnLoad = new QPushButton (QIconSet(QPixmap(fileopen)), "",this);
    connect(qpbtnLoad, SIGNAL(clicked()), this, SLOT(slotLoad()));
    qpbtnLoad->setFlat(TRUE);
//--    qlayout2->addWidget(qpbtnLoad);
    layoutButtons->addWidget(qpbtnLoad,1,4);
    
    layoutButtons->setColStretch ( 5, 50 );
    
    QToolTip::add(qpbtnMoveUp, "Move up");
    QToolTip::add(qpbtnMoveDown, "Move down");
    QToolTip::add(qpbtnInvert, "Invert order");
    QToolTip::add(qpbtnDelete, "Delete");
    QToolTip::add(qpbtnSave, "Save Sequence");
    QToolTip::add(qpbtnSaveAs, "Save Sequence under new name");
    QToolTip::add(qpbtnLoad, "Load Sequence");

    qpbtnMoveUp->setMaximumSize (QSize(22,22));
    qpbtnMoveDown->setMaximumSize (QSize(22,22));
    qpbtnInvert->setMaximumSize (QSize(22,22));
    qpbtnDelete->setMaximumSize (QSize(22,22));
    qpbtnSave->setMaximumSize (QSize(22,22));
    qpbtnSaveAs->setMaximumSize (QSize(22,22));
    qpbtnLoad->setMaximumSize (QSize(22,22));
    

//    layoutMain->setResizeMode (QLayout::Fixed);

    layoutMain->addLayout(layoutButtons);
//    this->setFixedWidth(this->sizeHint().width());
//    cout << "qlbox->visibleWidth (): " << qlbox->visibleWidth () << endl;
    iListViewVisibleWidth = qlbox->visibleWidth () + 5;//- qlbox->verticalScrollBar()->width();
}
    
void ClsQSAList::insertSA(QPixmap qpm, string, vector <vector<double> > v, bool bReplace) {
//    cout << "ClsQSAList::insertSA(QPixmap qpm, string, vector <vector<double> > v)" << endl;
    double fFactor = (double)iListViewVisibleWidth / (double) qpm.width();
    
    QWMatrix m;
    m.scale(fFactor, fFactor );
    QPixmap pmS = qpm.xForm( m ); 

    ClsListBoxSA* clsListBoxSA = new ClsListBoxSA(qlbox, pmS);
    clsListBoxSA->setMatrix(v);


    if(bReplace){
	int iCurrent = qlbox->currentItem();
	if(iCurrent>=0) {
	    qlbox->removeItem(iCurrent);
	}
	qlbox->insertItem(clsListBoxSA, iCurrent);
    } else {
	qlbox->insertItem(clsListBoxSA);
    }

    qlbox->setCurrentItem(clsListBoxSA);
    qlbox->clearSelection ();
    qlbox->setSelected(clsListBoxSA, true);
}


void ClsQSAList::slotMoveUp() {
//    cout << "ClsQSAList::slotMoveUp()" << endl;
    int iCurrent = qlbox->currentItem();
    if(iCurrent>=0) {
	QListBoxItem * lbCurrent = qlbox->item(iCurrent);
	QListBoxItem * lbPrev = lbCurrent->prev();
	if(lbPrev!=NULL) {
	    int iPrev = qlbox->index(lbPrev);
	    qlbox->takeItem(lbCurrent);
	    qlbox->insertItem(lbCurrent, iPrev);
	    qlbox->setCurrentItem ( iPrev );
	}
    }
    
}

void ClsQSAList::slotMoveDown() {
//    cout << "ClsQSAList::slotMoveDown()" << endl;
    
    int iCurrent = qlbox->currentItem();
    if(iCurrent>=0) {
	QListBoxItem * lbCurrent = qlbox->item(iCurrent);
	QListBoxItem * lbNext = lbCurrent->next();
	if(lbNext!=NULL) {
	    int iNext = qlbox->index(lbNext);
	    qlbox->takeItem(lbCurrent);
	    qlbox->insertItem(lbCurrent, iNext);
	    qlbox->setCurrentItem ( iNext );
	}
    }
}


void ClsQSAList::slotInvert() {
//    cout << "ClsQSAList::slotInvert()" << endl;
    qlbox->sort( FALSE );
}

void ClsQSAList::slotDeleteItem() {

/*
    int iCurrent = qlbox->currentItem();
    if(iCurrent>=0) {
	qlbox->removeItem(iCurrent);
    }
*/
    int iCount = qlbox->count();
    for(int ii=(iCount-1); ii>=0; ii--){
	if(qlbox->isSelected(ii)){
	    qlbox->removeItem(ii);
	}
    }

}

void ClsQSAList::slotDoubleClicked ( QListBoxItem * item ) {
//    cout << "ClsQSAList::slotDoubleClicked ( QListBoxItem * item )" << endl;
    vector <vector<double> > v = dynamic_cast<ClsListBoxSA*>(item)->getMatrix();
    emit sigChangeMatrix(v);
}

void ClsQSAList::slotSaveAs() {
//    cout << "ClsQSAList::slotSaveAs()" << endl;
    QString qstrFilename = QFileDialog::getSaveFileName(
	"",
	"*", 
	this,
	"save file dialog"
	"Choose a filename to save under" );
    
    if(qstrFilename.length()<=0){
	return;
    }
    if ( QFile::exists(qstrFilename) ) { 
/*      warning on overwrite */
	int iReturn;
	iReturn = QMessageBox::warning( this, "IQR",
					"A file with the seleteced name exists\n",
					"Overwrite", "Cancel", 0, 0, 1 );
	if(iReturn == 1){
	    return;
	}
    }
    strFilename = qstrFilename.latin1();
//    cout << "strFilename: " << strFilename << endl;
    slotSave();
}

void ClsQSAList::slotSave() {
//    cout << "ClsQSAList::slotSave()" << endl;
    if(strFilename.length()<=0) {
	slotSaveAs();
    }
    
    ofstream ofstr;
    ofstr.open(strFilename.c_str());

    if(!ofstr) {
	cerr << "ERROR saving to: " << strFilename << endl;
	return;
    }
        
    int iCount = getCount();
    for(int ii=0; ii<iCount; ii++) {
	QListBoxItem * lbi = qlbox->item(ii);
	if(lbi!=NULL) {
	    vector <vector<double> > v = dynamic_cast<ClsListBoxSA*>(lbi)->getMatrix(); //!!!!!!!! use valarry<double>!!
	    ofstr << v;
	    if(ii+1 < iCount) {
		ofstr << endl;
	    }
	    ofstr << flush;
	}
    }
    ofstr.close();
}

void ClsQSAList::slotLoad() {
//	cout << "ClsQSAList::slotLoad()" << endl;

    QString qstrFilename = QFileDialog::getOpenFileName(
	"",
	"*",
	this,
	"open file dialog"
	"Choose a file" );

    if(qstrFilename.length()>0){
	strFilename = qstrFilename.latin1();
	qlbox->clear();
	ifstream ifstr;
	ifstr.open(strFilename.c_str(), std::ios::in );
	if(!ifstr) {
	    cerr << "ERROR loading: " << strFilename << endl;
	    return;
	}

	while(ifstr) {
	    vector <vector<double> > v;
	    ifstr >> v;
//	    cout << v << endl;
//	    cout << "++++++++++++++++" << endl;
//	    cout << "v.size(): " << v.size() << endl;
	    
	    if(v.size()>0){
	    clsQGroupStateManip->slotClear();
	    QPixmap qpm = clsQGroupStateManip->getPixmap(v);
	    insertSA(qpm, "", v);
	    }
	}
	clsQGroupStateManip->slotClear();
	ifstr.close();
    }
}


vector <vector<double> > ClsQSAList::getVectorAt(int i) {
    QListBoxItem * lbi = qlbox->item(i);
    vector <vector<double> > v;
    
    if(lbi!=NULL) {
	v = dynamic_cast<ClsListBoxSA*>(lbi)->getMatrix();    
    }
    return v;
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k"
//// End:
