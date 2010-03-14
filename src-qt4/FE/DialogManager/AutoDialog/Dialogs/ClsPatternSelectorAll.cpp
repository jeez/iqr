/****************************************************************************
 ** $Filename: ClsPatternSelectorAll.cpp
 ** $Id$
 **
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

#include <QLabel>


#include "tagLibrary.hpp"
#include "ClsPatternSelectorAll.h"
#include "ClsListItems.h"
#include "ClsBaseAutoGroup.h"
#include <iqrUtils.h>

using namespace iqrcommon;

ClsPatternSelectorAll::ClsPatternSelectorAll(ClsBaseAutoGroup *changeReceiver, const char *name ) : 
    ClsPatternBaseSelector( changeReceiver, name ) {
    strType = ClsTagLibrary::SelectorAll();
    
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
//// compile-command: "cd ../ && make -k -j2"
//// End:


