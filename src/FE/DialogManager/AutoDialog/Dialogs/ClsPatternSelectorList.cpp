  /****************************************************************************
 ** $Filename: ClsPatternSelectorList.cpp
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

ClsPatternSelectorList::ClsPatternSelectorList( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
						ClsBaseTopology *_clsBaseTopologyGroup,
						const char *name ) : ClsPatternBaseSelector( parent, changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorList();

    
//    setBackgroundColor(green);

    if(dynamic_cast<ClsTopologyRect*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
	clsQStateArrayView = new ClsQStateArrayViewRect(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
	clsQStateArrayView = new ClsQStateArrayViewHex(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(_clsBaseTopologyGroup)){
//	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
	clsQStateArrayView = new ClsQStateArrayViewSparse(this, _clsBaseTopologyGroup, "source", ClsBaseQStateArrayView::LIST);
    }



    pqlayTop = new QGridLayout(this, 5, 2, 5, 5, name);



    pqlayTop->addMultiCellWidget(clsQStateArrayView, 0, 0,0,1, Qt::AlignHCenter | Qt::AlignBottom);

    qbtnAdd = new QPushButton ("Set", this, "add");
    connect(qbtnAdd, SIGNAL(clicked()), this, SLOT(addEntry()));
    pqlayTop->addWidget(qbtnAdd, 1, 0, Qt::AlignTop );

    qbtnClear = new QPushButton ( "Clear", this, "clear");
    connect(qbtnClear, SIGNAL(clicked()), this, SLOT(clear()));
    pqlayTop->addWidget(qbtnClear, 1,1, Qt::AlignTop );

    pqlayTop->addRowSpacing ( 1, 30 );
	

    QLabel *qlblStartHeight = new QLabel(this);
    qlblStartHeight->setText("Selection");
    pqlayTop->addWidget(qlblStartHeight, 2, 0);

    qtexteditSelection = new QTextEdit(this);
    qtexteditSelection->setWrapPolicy (QTextEdit::Anywhere);
    
    pqlayTop->addMultiCellWidget(qtexteditSelection, 3, 3,0,1, Qt::AlignTop);


    pqlayTop->setRowStretch ( 4,2 );

/*
    qlstBox = new QListBox(this);
    pqlayTop->addMultiCellWidget(qlstBox, 3,3,0,1);

    qbtnRemove = new QPushButton ("Remove", this, "remove");
    connect(qbtnRemove, SIGNAL(clicked()), this, SLOT(removeEntry()));
    pqlayTop->addWidget(qbtnRemove, 4,0);

    qbtnShow = new QPushButton ("Show", this, "remove");
    connect(qbtnShow, SIGNAL(clicked()), this, SLOT(showEntry()));
    pqlayTop->addWidget(qbtnShow, 4, 1);
*/
};

    
void ClsPatternSelectorList::addEntry() {
//    cout << "ClsPatternSelectorList::addEntry()" << endl;

    if(clsQStateArrayView->getSelected().size()>0){
	lstSelection= clsQStateArrayView->getSelected();
	string str="";
	vector<tiPoint>::iterator it;
	for(it=lstSelection.begin();it!=lstSelection.end();it++){
	    str.append("(");
	    str.append(iqrUtils::int2string((*it).first));
	    str.append(",");
	    str.append(iqrUtils::int2string((*it).second));
	    str.append(")");
	}
	qtexteditSelection->setText(str.c_str());
    }
//    emit changed();
    changeReceiver->subItemChanged();
};

// void ClsPatternSelectorList::removeEntry() {
// //    cout << "removeEntry()" << endl;
// //    cout << "current: " << qlstBox->currentItem() << endl;

//     int iCurrent = qlstBox->currentItem();
//     if(iCurrent>=0){
// 	qlstBox->removeItem(iCurrent);
//     }
// };

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
	vector<tiPoint>::iterator it;
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
//// compile-command: "cd ../ && make -k -j8 && cat /usr/share/apps/kget/sounds/finished.wav > /dev/dsp"
//// End:




