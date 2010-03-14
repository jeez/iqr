/****************************************************************************
 ** $Filename: ClsPatternSelectorTuples.cpp
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
#include <QFileDialog>

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

ClsPatternSelectorTuples::ClsPatternSelectorTuples( ClsBaseAutoGroup *changeReceiver, 
						    ClsBaseTopology *_clsBaseTopologySource, ClsBaseTopology *_clsBaseTopologyTarget,
						    const char *name ) : ClsPatternBaseSelector( changeReceiver, name )  {
    strType = ClsTagLibrary::SelectorTuples();
    
    QVBoxLayout* pqlayTop = new QVBoxLayout();
    this->setLayout(pqlayTop);
    pqlayTop->setContentsMargins( 0, 0, 0, 0 );

    QGroupBox *qgroupboxSource = new QGroupBox();
    qgroupboxSource->setTitle("Source");
    clsQStateArrayViewSource = NULL;
    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologyRect" << endl;
	clsQStateArrayViewSource = new ClsQStateArrayViewRect(NULL, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologyHex" << endl;
//***	clsQStateArrayViewSource = new ClsQStateArrayViewHex(NULL, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologySource)){
//	cout << "cast clsBaseTopologySource to ClsTopologySparse" << endl;
//***	clsQStateArrayViewSource = new ClsQStateArrayViewSparse(NULL, _clsBaseTopologySource, "source", ClsBaseQStateArrayView::LIST);
    }


    QGroupBox *qgroupboxTarget = new QGroupBox(this);
    qgroupboxTarget->setTitle("Target");
    clsQStateArrayViewTarget = NULL;
    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologyRect" << endl;
	clsQStateArrayViewTarget = new ClsQStateArrayViewRect(NULL, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologyHex" << endl;
//***	clsQStateArrayViewTarget = new ClsQStateArrayViewHex(NULL, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyTarget)){
//	cout << "cast clsBaseTopologyTarget to ClsTopologySparse" << endl;
//***	clsQStateArrayViewTarget = new ClsQStateArrayViewSparse(NULL, _clsBaseTopologyTarget, "target", ClsBaseQStateArrayView::LIST);
    }


    if(clsQStateArrayViewSource!=NULL && clsQStateArrayViewTarget!=NULL){ 
	QHBoxLayout* pqlayGrpbxSource = new QHBoxLayout();
	qgroupboxSource->setLayout(pqlayGrpbxSource);
	pqlayGrpbxSource->addWidget(clsQStateArrayViewSource);

	QHBoxLayout* pqlayGrpbxTarget = new QHBoxLayout();
	qgroupboxTarget->setLayout(pqlayGrpbxTarget);
	pqlayGrpbxTarget->addWidget(clsQStateArrayViewTarget);


	QHBoxLayout* pqlayStateArrays = new QHBoxLayout();
	pqlayTop->addLayout(pqlayStateArrays);
	pqlayStateArrays->addWidget(qgroupboxSource); 
	pqlayStateArrays->addWidget(qgroupboxTarget); 
	
/* --- */

	QHBoxLayout* pqlayButtons1 = new QHBoxLayout();
	pqlayTop->addLayout(pqlayButtons1);
	pqlayButtons1->addStretch (5);	

	qbtnAdd = new QPushButton ("Add");
	connect(qbtnAdd, SIGNAL(clicked()), this, SLOT(addEntry()));
	pqlayButtons1->addWidget(qbtnAdd);

	qbtnClear = new QPushButton ( "Clear");
	connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
	pqlayButtons1->addWidget(qbtnClear);

	pqlayButtons1->addStretch (5);
/* --- */	

	qlstBox = new QListWidget();
	connect(qlstBox, SIGNAL(itemDoubleClicked( QListWidgetItem *)), this, SLOT(showEntry( QListWidgetItem *)));
	pqlayTop->addWidget(qlstBox);

/* --- */

	QHBoxLayout* pqlayButtons2 = new QHBoxLayout();
	pqlayTop->addLayout(pqlayButtons2);
	pqlayButtons2->addStretch (5);

	qbtnImport = new QPushButton ("Import");
	connect(qbtnImport, SIGNAL(clicked()), this, SLOT(import()));
	pqlayButtons2->addWidget(qbtnImport);

	qbtnRemove = new QPushButton ("Remove");
	connect(qbtnRemove, SIGNAL(clicked()), this, SLOT(removeEntry()));
	pqlayButtons2->addWidget(qbtnRemove);

	qbtnShow = new QPushButton ("Show");
	connect(qbtnShow, SIGNAL(clicked()), this, SLOT(showEntry()));
	pqlayButtons2->addWidget(qbtnShow);

	pqlayButtons2->addStretch (5);

    }
};

    
void ClsPatternSelectorTuples::addEntry() {
//    cout << "ClsPatternSelectorTuples::addEntry()" << endl;
    
    list<pair<int, int> > lstSource = clsQStateArrayViewSource->getSelected();
    list<pair<int, int> > lstTarget = clsQStateArrayViewTarget->getSelected();

    if(lstSource.size()>0 && lstTarget.size()>0){
	ClsListItemTuples *clsListItemList = new ClsListItemTuples(qlstBox, lstSource, lstTarget);
    } else {
	cout << "LISTS ARE 0" << endl;
    }

    changeReceiver->subItemChanged();
};

void ClsPatternSelectorTuples::removeEntry() {
//    cout << "ClsPatternSelectorTuples::removeEntry()" << endl;
    int iCount = qlstBox->count();
    for(int ii=(iCount-1); ii>=0; ii--){
	QListWidgetItem *item =  qlstBox->item(ii);
	if(item!=NULL){
	    if(item->isSelected()){
		qlstBox->takeItem(ii);
	    }
	}
    }
    changeReceiver->subItemChanged();
};

void ClsPatternSelectorTuples::showEntry() {
//    cout << "showEntry()" << endl;
    clear();
    QList<QListWidgetItem *> ql = qlstBox->selectedItems();
    QList<QListWidgetItem *>::iterator it;
    for(it=ql.begin(); it!=ql.end();it++){
	ClsListItemTuples *clsListItemList = dynamic_cast<ClsListItemTuples *>(*it);
	if(clsListItemList!=NULL){
	    clsQStateArrayViewSource->setValue(.5, clsListItemList->getListSource());
	    clsQStateArrayViewTarget->setValue(.5, clsListItemList->getListTarget());
	}
	
    }
};

void ClsPatternSelectorTuples::showEntry( QListWidgetItem * item) {
    clear();
    ClsListItemTuples *clsListItemList = dynamic_cast<ClsListItemTuples*>(item);
    if(clsListItemList!=NULL){
  	clsQStateArrayViewSource->setValue(.5, clsListItemList->getListSource());
  	clsQStateArrayViewTarget->setValue(.5, clsListItemList->getListTarget());
    }
}
 


void ClsPatternSelectorTuples::clear() {
    clsQStateArrayViewSource->clear();
    clsQStateArrayViewTarget->clear();
}

void ClsPatternSelectorTuples::setSubPopulation(ClsTuples* _clsTuples) {
//    cout << "ClsPatternSelectorTuples::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation)" << endl;
   
    if(_clsTuples!=NULL){
	list<pair<tListOfPairs, tListOfPairs> > lstTuples = _clsTuples->getData();
	list<pair<tListOfPairs, tListOfPairs> >::iterator it;
	
	for(it=lstTuples.begin(); it!=lstTuples.end(); it++){
	    ClsListItemTuples *clsListItemList = new ClsListItemTuples(qlstBox, (*it).first, (*it).second);
	}
    }
     
}

ClsTuples* ClsPatternSelectorTuples::getSubPopulation() {
    ClsTuples* clsTuples = new ClsTuples();

    for(unsigned int ii=0; ii<qlstBox->count (); ii++){
	ClsListItemTuples* clsListItemTuples = static_cast<ClsListItemTuples*>(qlstBox->item(ii));
	tListOfPairs lstSource = clsListItemTuples->getListSource();
	tListOfPairs lstTarget = clsListItemTuples->getListTarget();
	clsTuples->addData(lstSource, lstTarget);
    }

    return clsTuples;
    
}

void ClsPatternSelectorTuples::import() {
//     cout << "ClsPatternSelectorTuples::import()" << endl;
    QString qstrFileName = QFileDialog::getOpenFileName(
	this,
	"Open File"
	"Choose a file",
	"",
	"*" );
     
    if(qstrFileName.length()>0){
	string strFileName = qstrFileName.toStdString();
	list<pair<tListOfPairs, tListOfPairs> > lstTuples = ClsTuplesImporter::parseFile(strFileName);
	list<pair<tListOfPairs, tListOfPairs> >::iterator it;
	
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



