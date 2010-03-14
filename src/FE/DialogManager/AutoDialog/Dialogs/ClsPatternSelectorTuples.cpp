  /****************************************************************************
 ** $Filename: ClsPatternSelectorTuples.cpp
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
#include <qfiledialog.h>

#include "tagLibrary.hpp"

#include "ClsTopologyRect.h"
#include "ClsTopologyHex.h"
#include "ClsTopologySparse.h"

#include "ClsPatternSelectorTuples.h"
#include "ClsTuplesImporter.h"

#include "ClsQStateArrayViewRect.h"
#include "ClsQStateArrayViewHex.h"
#include "ClsQStateArrayViewSparse.h"

#include "ClsBaseAutoGroup.h"
#include "ClsListItems.h"
#include "ClsBaseTopology.h"


using namespace iqrcommon;

ClsPatternSelectorTuples::ClsPatternSelectorTuples( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
						    ClsBaseTopology *_clsBaseTopologySource, ClsBaseTopology *_clsBaseTopologyTarget,
						    const char *name ) : ClsPatternBaseSelector( parent, changeReceiver, name )  {
    strType = ClsTagLibrary::SelectorTuples();
    



//    setBackgroundColor(blue);

    pqlayTop = new QGridLayout(this, 4, 4, 0, 5, name);

    QGroupBox *qgroupboxSource = new QGroupBox(this);
    qgroupboxSource->setTitle("Source");
    QVBoxLayout *pqlaySourceGB = new QVBoxLayout(qgroupboxSource, 5, 0, "source gb layout");
    pqlaySourceGB->setAutoAdd (true);
    pqlaySourceGB->insertSpacing(0, 10);

//    clsQStateArrayViewSource = new ClsQStateArrayViewRect(qgroupboxSource, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);

    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologyRect" << endl;
	clsQStateArrayViewSource = new ClsQStateArrayViewRect(qgroupboxSource, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologyHex" << endl;
	clsQStateArrayViewSource = new ClsQStateArrayViewHex(qgroupboxSource, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologySparse" << endl;
	clsQStateArrayViewSource = new ClsQStateArrayViewSparse(qgroupboxSource, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }



    QGroupBox *qgroupboxTarget = new QGroupBox(this);
    qgroupboxTarget->setTitle("Target");
    QVBoxLayout *pqlayTargetGB = new QVBoxLayout(qgroupboxTarget, 5, 0, "source gb layout");
    pqlayTargetGB->setAutoAdd (true);
    pqlayTargetGB->insertSpacing(0, 10);
//    clsQStateArrayViewTarget = new ClsQStateArrayViewRect(qgroupboxTarget, _clsBaseTopologyTarget, "source", ClsBaseQStateArrayView::LIST);

    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologyRect" << endl;
	clsQStateArrayViewTarget = new ClsQStateArrayViewRect(qgroupboxTarget, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologyHex" << endl;
	clsQStateArrayViewTarget = new ClsQStateArrayViewHex(qgroupboxTarget, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologySparse" << endl;
	clsQStateArrayViewTarget = new ClsQStateArrayViewSparse(qgroupboxTarget, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }
	
    pqlayTop->addMultiCellWidget(qgroupboxSource, 0, 0, 0, 1, Qt::AlignHCenter | Qt::AlignBottom);
    pqlayTop->addMultiCellWidget(qgroupboxTarget, 0, 0, 2, 3, Qt::AlignHCenter | Qt::AlignBottom);




    qbtnAdd = new QPushButton ("Add", this, "add");
    connect(qbtnAdd, SIGNAL(clicked()), this, SLOT(addEntry()));
    pqlayTop->addWidget(qbtnAdd, 1, 2, Qt::AlignTop );

    qbtnClear = new QPushButton ( "Clear", this, "clear");
    connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
    pqlayTop->addWidget(qbtnClear, 1, 3, Qt::AlignTop );

    pqlayTop->addRowSpacing ( 1, 50 );
	

    qlstBox = new QListBox(this);
    connect(qlstBox, SIGNAL(doubleClicked( QListBoxItem *)), this, SLOT(showEntry( QListBoxItem *)));
    pqlayTop->addMultiCellWidget(qlstBox, 2,2,0,3);

    qbtnImport = new QPushButton ("Import", this, "import");
    connect(qbtnImport, SIGNAL(clicked()), this, SLOT(import()));
    pqlayTop->addWidget(qbtnImport, 3,1);


    qbtnRemove = new QPushButton ("Remove", this, "remove");
    connect(qbtnRemove, SIGNAL(clicked()), this, SLOT(removeEntry()));
    pqlayTop->addWidget(qbtnRemove, 3,2);

    qbtnShow = new QPushButton ("Show", this, "remove");
    connect(qbtnShow, SIGNAL(clicked()), this, SLOT(showEntry()));
    pqlayTop->addWidget(qbtnShow, 3, 3);
};

    
void ClsPatternSelectorTuples::addEntry() {
//    cout << "ClsPatternSelectorTuples::addEntry()" << endl;
    
    vector<tiPoint> lstSource = clsQStateArrayViewSource->getSelected();
    vector<tiPoint> lstTarget = clsQStateArrayViewTarget->getSelected();

    if(lstSource.size()>0 && lstTarget.size()>0){
	ClsListItemTuples *clsListItemList = new ClsListItemTuples(qlstBox, lstSource, lstTarget);
    } else {
	cout << "LISTS ARE 0" << endl;
    }

//    emit changed();
    changeReceiver->subItemChanged();
};

void ClsPatternSelectorTuples::removeEntry() {
//    cout << "ClsPatternSelectorTuples::removeEntry()" << endl;

    int iCurrent = qlstBox->currentItem();
    if(iCurrent>=0){
	qlstBox->removeItem(iCurrent);
    }
//    emit changed();
    changeReceiver->subItemChanged();
};

void ClsPatternSelectorTuples::showEntry() {
//    cout << "showEntry()" << endl;

    clear();
    ClsListItemTuples *clsListItemList = dynamic_cast<ClsListItemTuples*>(qlstBox->selectedItem());
    if(clsListItemList!=NULL){
  	clsQStateArrayViewSource->setValue(.5, clsListItemList->getVectorSource());
  	clsQStateArrayViewTarget->setValue(.5, clsListItemList->getVectorTarget());
    }
};

void ClsPatternSelectorTuples::showEntry( QListBoxItem * item) {
    clear();
    ClsListItemTuples *clsListItemList = dynamic_cast<ClsListItemTuples*>(item);
    if(clsListItemList!=NULL){
  	clsQStateArrayViewSource->setValue(.5, clsListItemList->getVectorSource());
  	clsQStateArrayViewTarget->setValue(.5, clsListItemList->getVectorTarget());
    }
    
}



void ClsPatternSelectorTuples::clear() {
    clsQStateArrayViewSource->clear();
    clsQStateArrayViewTarget->clear();
}

//void ClsPatternSelectorTuples::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation) {
void ClsPatternSelectorTuples::setSubPopulation(ClsTuples* _clsTuples) {
//    cout << "ClsPatternSelectorTuples::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation)" << endl;
    
//    ClsTuples *_clsTuples= static_cast<ClsTuples*>(_clsBaseSubPopulation);

    if(_clsTuples!=NULL){
	list<pair<tListOfPoints, tListOfPoints> > lstTuples = _clsTuples->getData();
	list<pair<tListOfPoints, tListOfPoints> >::iterator it;
	
	for(it=lstTuples.begin(); it!=lstTuples.end(); it++){
	    ClsListItemTuples *clsListItemList = new ClsListItemTuples(qlstBox, (*it).first, (*it).second);
	}
    }
     
}

//ClsBaseSubPopulation* ClsPatternSelectorTuples::getSubPopulation() {
ClsTuples* ClsPatternSelectorTuples::getSubPopulation() {
    ClsTuples* clsTuples = new ClsTuples();

    for(unsigned int ii=0; ii<qlstBox->count (); ii++){
	ClsListItemTuples* clsListItemTuples = static_cast<ClsListItemTuples*>(qlstBox->item(ii));
	tListOfPoints lstSource = clsListItemTuples->getVectorSource();
	tListOfPoints lstTarget = clsListItemTuples->getVectorTarget();
	clsTuples->addData(lstSource, lstTarget);
    }

//         tListOfPoints getVectorSource() { return lstSource; };
//     tListOfPoints getVectorTarget() { return lstTarget; };

//qlstBox->

    return clsTuples;
    
    
}

void ClsPatternSelectorTuples::import() {
//     cout << "ClsPatternSelectorTuples::import()" << endl;
     QString qstrFileName = QFileDialog::getOpenFileName(
	 "",
	 "*",
	 this,
	 "Open File"
	 "Choose a file" );
     
     if(qstrFileName.length()>0){
	 string strFileName = qstrFileName.latin1();
	 list<pair<tListOfPoints, tListOfPoints> > lstTuples = ClsTuplesImporter::parseFile(strFileName);
	 list<pair<tListOfPoints, tListOfPoints> >::iterator it;
	
	 for(it=lstTuples.begin(); it!=lstTuples.end(); it++){
	     ClsListItemTuples *clsListItemList = new ClsListItemTuples(qlstBox, (*it).first, (*it).second);
	 }
	 changeReceiver->subItemChanged();
     }
};

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k "
//// End:



