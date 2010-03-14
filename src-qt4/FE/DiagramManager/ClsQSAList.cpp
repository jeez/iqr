/****************************************************************************
 ** $Filename: ClsQSAList.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Sep  5 01:54:07 2003
 **
 *****************************************************************************/


#include <iostream>

#include <QFileDialog> 
#include <QWMatrix> 
#include <QButtonGroup> 
#include <QLayout> 
#include <QPushButton> 
#include <QMessageBox> 


#include "ClsQSAList.h"
#include "ClsQGroupStateManip.h"

namespace {
#include <up_12x12.xpm>
#include <down_12x12.xpm>
#include <invert2_12x12.xpm>
#include <trash2_12x12.xpm>
#include <fileSave.xpm>
#include <fileSaveAs.xpm>
#include <fileOpen.xpm>
}


ClsQSAList::ClsQSAList ( QWidget * parent, ClsQGroupStateManip* _clsQGroupStateManip, const char * name, Qt::WindowFlags f) : 
    QFrame ( parent, name, f ), clsQGroupStateManip(_clsQGroupStateManip)  {


    strFilename = "";
    
    QVBoxLayout* layoutMain = new QVBoxLayout( this, 5, -1, "mainL");


/* ------------------------------- */
    qlbox = new QListWidget ( this );
    qlbox->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
//    qlbox->setViewMode(QListView::IconMode);
    qlbox->setIconSize(QSize(60, 60));
    qlbox->setSpacing(10);


    qlbox->setSelectionMode(QAbstractItemView::ExtendedSelection);
   
    connect(qlbox, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotDoubleClicked(const QModelIndex &)));
    layoutMain->addWidget(qlbox,10);


    QGridLayout* layoutButtons = new QGridLayout( 2, 6, -1, "layoutButtons");
    layoutButtons->setColStretch ( 0, 50 );

/* ------------------------------- */	

    QPushButton* qpbtnMoveUp = new QPushButton (QIconSet(QPixmap(up)), "",this);
    qpbtnMoveUp->setFlat(TRUE);
    connect(qpbtnMoveUp, SIGNAL(clicked()), this, SLOT(slotMoveUp()));
    layoutButtons->addWidget(qpbtnMoveUp,0,1);
	
    QPushButton* qpbtnMoveDown = new QPushButton (QIconSet(QPixmap(down)), "",this);
    qpbtnMoveDown->setFlat(TRUE);
    connect(qpbtnMoveDown, SIGNAL(clicked()), this, SLOT(slotMoveDown()));
    layoutButtons->addWidget(qpbtnMoveDown,0,2);

    QPushButton* qpbtnInvert = new QPushButton (QIconSet(QPixmap(invert2)), "",this);
    qpbtnInvert->setFlat(TRUE);
    connect(qpbtnInvert, SIGNAL(clicked()), this, SLOT(slotInvert()));
    layoutButtons->addWidget(qpbtnInvert,0,3);

    QPushButton* qpbtnDelete = new QPushButton (QIconSet(QPixmap(trash2)), "",this);
    connect(qpbtnDelete, SIGNAL(clicked()), this, SLOT(slotDeleteItem()));
    qpbtnDelete->setFlat(TRUE);
    layoutButtons->addWidget(qpbtnDelete,0,4);

/* ------------------------------- */

    QPushButton* qpbtnSave = new QPushButton (QIconSet(QPixmap(fileSave_xpm)), "",this);
    connect(qpbtnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
    qpbtnSave->setFlat(TRUE);
    layoutButtons->addWidget(qpbtnSave,1,1);

    QPushButton* qpbtnSaveAs = new QPushButton (QIconSet(QPixmap(fileSaveAs_xpm)), "",this);
    connect(qpbtnSaveAs, SIGNAL(clicked()), this, SLOT(slotSaveAs()));
    qpbtnSaveAs->setFlat(TRUE);
    layoutButtons->addWidget(qpbtnSaveAs,1,2);

    QPushButton* qpbtnLoad = new QPushButton (QIconSet(QPixmap(fileOpen_xpm)), "",this);
    connect(qpbtnLoad, SIGNAL(clicked()), this, SLOT(slotLoad()));
    qpbtnLoad->setFlat(TRUE);
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
    
    layoutMain->addLayout(layoutButtons);
}

    
void ClsQSAList::insertSA(QPixmap qpm, string, vector <vector<double> > v, bool bReplace) {
    ClsPixmapListWidgetItem* clsListBoxSA = new ClsPixmapListWidgetItem(NULL, qpm);
    clsListBoxSA->setMatrix(v);

    if(bReplace){
	int iCurrent = qlbox->row(qlbox->currentItem ());
	if(iCurrent>=0) {
	    QListWidgetItem * lbCurrent = qlbox->takeItem(iCurrent);
	    if(lbCurrent!=NULL){
		delete lbCurrent;
	    }
	    qlbox->insertItem(iCurrent, clsListBoxSA);
	}
    } else {
	qlbox->addItem(clsListBoxSA);
    }

    qlbox->clearSelection ();
    qlbox->setCurrentItem(clsListBoxSA);
   
}


void ClsQSAList::slotMoveUp() {
//    cout << "ClsQSAList::slotMoveUp()" << endl;
    QListWidgetItem * lbCurrent = qlbox->currentItem ();
    if(lbCurrent!=NULL){
	int iCurrent = qlbox->row(lbCurrent);
	int iPrev = iCurrent-1;
	QListWidgetItem * lbCurrent = qlbox->item(iCurrent);
	qlbox->takeItem(iCurrent);
	if(iCurrent>0) {
	    qlbox->insertItem(iPrev, lbCurrent);
	} else { /* append at the end if current item is the top most item....*/
	    qlbox->addItem(lbCurrent);
	} 
	qlbox->setCurrentItem(lbCurrent);
    }


}

void ClsQSAList::slotMoveDown() {
//    cout << "ClsQSAList::slotMoveDown()" << endl;
    QListWidgetItem * lbCurrent = qlbox->currentItem ();
    if(lbCurrent!=NULL){
	int iCurrent = qlbox->row(lbCurrent);
	int iNext = iCurrent+1;
	QListWidgetItem * lbCurrent = qlbox->item(iCurrent);
	qlbox->takeItem(iCurrent);
	if(iCurrent<qlbox->count()) {
	    qlbox->insertItem(iNext, lbCurrent);
	} else { /* prepend at the top if current item is the bottom most item....*/
	    qlbox->insertItem(0, lbCurrent);
	} 
	qlbox->setCurrentItem(lbCurrent);
    }
}


void ClsQSAList::slotInvert() {
    cout << "ClsQSAList::slotInvert()" << endl;
    qlbox->setSortingEnabled ( false );
}

void ClsQSAList::slotDeleteItem() {
    int iCount = qlbox->count();
    for(int ii=(iCount-1); ii>=0; ii--){
	QListWidgetItem *item =  qlbox->item(ii);
	if(item!=NULL){
	    if(item->isSelected()){
		qlbox->takeItem(ii);
	    }
	}
    }
}

void ClsQSAList::slotDoubleClicked ( const QModelIndex & index ) {
//    cout << "ClsQSAList::slotDoubleClicked ( const QModelIndex & index )" << endl;
    
  QListWidgetItem *w = qlbox->item( index.row() );
    if(w!=NULL){
	vector <vector<double> > v = dynamic_cast<ClsPixmapListWidgetItem*>(w)->getMatrix();
	emit sigChangeMatrix(v);
    }
}

void ClsQSAList::slotSaveAs() {
    QString qstrFilename = QFileDialog::getSaveFileName(
	this,
	"save file dialog"
	"Choose a filename to save under",
	"",
	"*"
	 );

   
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
	QListWidgetItem * lbi = qlbox->item(ii);
	if(lbi!=NULL) {
	    vector <vector<double> > v = dynamic_cast<ClsPixmapListWidgetItem*>(lbi)->getMatrix(); //!!!!!!!! use valarry<double>!!
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
	this,
	"open file dialog"
	"Choose a file",
	"",
	"*"
	 );


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
    QListWidgetItem * lbi = qlbox->item(i);
    vector <vector<double> > v;
    
    if(lbi!=NULL) {
	v = dynamic_cast<ClsPixmapListWidgetItem*>(lbi)->getMatrix();    
    }
    return v;
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
