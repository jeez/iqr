/****************************************************************************
 ** $Filename: ClsPatternSelectorAll.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jun  6 18:37:35 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>
#include <qlabel.h>
#include "tagLibrary.hpp"
#include "ClsPatternSelectorAll.h"
//#include "ClsQStateArrayView.h"
#include "ClsListItems.h"
#include "ClsBaseAutoGroup.h"
#include <iqrUtils.h>
using namespace iqrcommon;

ClsPatternSelectorAll::ClsPatternSelectorAll( QWidget *parent, ClsBaseAutoGroup *changeReceiver, const char *name ) : 
    ClsPatternBaseSelector( parent, changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorAll();
    
//    setBackgroundColor(blue);
    QLabel *qlbl = new QLabel(this);
    qlbl->setText("   All");
    
    
};

void ClsPatternSelectorAll::setSubPopulation(ClsBaseSubPopulation* /*_clsBaseSubPopulation*/) {
}

ClsBaseSubPopulation* ClsPatternSelectorAll::getSubPopulation() {
    ClsAll* clsAll = new ClsAll();
    return clsAll;
}
    
 


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8 && cat /usr/share/apps/kget/sounds/finished.wav > /dev/dsp"
//// End:




