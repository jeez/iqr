/****************************************************************************
 ** $Filename: ClsPatternSelectorList.cpp
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

#include "tagLibrary.hpp"

#include "ClsPatternSelectorList.h"

#include "ClsQStateArrayViewRect.h"
#include "ClsQStateArrayViewHex.h"
#include "ClsQStateArrayViewSparse.h"

#include "ClsTopologyRect.h"
#include "ClsTopologyHex.h"
#include "ClsTopologySparse.h"

#include "ClsBaseAutoGroup.h"
#include "ClsBaseTopology.h"

#include "ClsListItems.h"

using namespace iqrcommon;

ClsPatternSelectorList::ClsPatternSelectorList( ClsBaseAutoGroup *changeReceiver, 
						ClsBaseTopology *_clsBaseTopologyGroup,
						const char *name ) : ClsPatternBaseSelector( changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorList();

    clsQStateArrayView = NULL;
    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
	clsQStateArrayView = new ClsQStateArrayViewRect(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
//****	clsQStateArrayView = new ClsQStateArrayViewHex(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
//****	clsQStateArrayView = new ClsQStateArrayViewSparse(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }

    if(clsQStateArrayView!=NULL){
	QVBoxLayout* pqlayTop = new QVBoxLayout();
	this->setLayout(pqlayTop);
	pqlayTop->setContentsMargins( 0, 0, 0, 0 );
	
	pqlayTop->addWidget(clsQStateArrayView);
	
	QHBoxLayout* pqlayButtons = new QHBoxLayout();
	pqlayTop->addLayout(pqlayButtons);
	pqlayButtons->addStretch (5);	
	
	qbtnAdd = new QPushButton ("Set");
	connect(qbtnAdd, SIGNAL(clicked()), this, SLOT(addEntry()));
	pqlayButtons->addWidget(qbtnAdd );
	
	qbtnClear = new QPushButton ( "Clear");
	connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
	pqlayButtons->addWidget(qbtnClear);
	pqlayButtons->addStretch (5);	
	
/* --- */

	QLabel *qlblStartHeight = new QLabel();
	qlblStartHeight->setText("Selection");
	pqlayTop->addWidget(qlblStartHeight);

/* --- */
	
	qtexteditSelection = new QTextEdit();
	pqlayTop->addWidget(qtexteditSelection);
	qtexteditSelection->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Maximum );

	pqlayTop->addStretch (5);	



    }
};

    
void ClsPatternSelectorList::addEntry() {
//    cout << "ClsPatternSelectorList::addEntry()" << endl;

    if(clsQStateArrayView->getSelected().size()>0){
	lstSelection= clsQStateArrayView->getSelected();
	string str="";
	list<pair<int, int> >::iterator it;
	for(it=lstSelection.begin();it!=lstSelection.end();it++){
	    str.append("(");
	    str.append(iqrUtils::int2string((*it).first));
	    str.append(",");
	    str.append(iqrUtils::int2string((*it).second));
	    str.append(")");
	}
	qtexteditSelection->setText(str.c_str());
    }
    changeReceiver->subItemChanged();
};


void ClsPatternSelectorList::clear() {
    clsQStateArrayView->clear();
    qtexteditSelection->setText("");
    lstSelection.clear();

    changeReceiver->subItemChanged();    
};

void ClsPatternSelectorList::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation) {
//    cout << "ClsPatternSelectorList::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation)" << endl;
//    cout << "_clsBaseSubPopulation->Type(): " << _clsBaseSubPopulation->Type() << endl;
    
    if(_clsBaseSubPopulation!=NULL){
	ClsList *_clsList= static_cast<ClsList*>(_clsBaseSubPopulation);
	lstSelection = _clsList->getData();
	
	string str="";
	list<pair<int, int> >::iterator it;
	for(it=lstSelection.begin();it!=lstSelection.end();it++){
	    str.append("(");
	    str.append(iqrUtils::int2string((*it).first));
	    str.append(",");
	    str.append(iqrUtils::int2string((*it).second));
	    str.append(")");
	}
	qtexteditSelection->setText(str.c_str());
	
	clsQStateArrayView->setValue(.5, lstSelection);
    }
//    cout << "end:ClsPatternSelectorList::setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation)" << endl;
}

ClsBaseSubPopulation* ClsPatternSelectorList::getSubPopulation() {
//    cout << "ClsPatternSelectorList::getSubPopulation()" << endl;
    ClsList* clsList = new ClsList();
    clsList->setData(lstSelection);

    return clsList;
}

  
 

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k "
//// End:





