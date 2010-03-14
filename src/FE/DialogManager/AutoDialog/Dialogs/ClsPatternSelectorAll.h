/****************************************************************************
 ** $Filename: ClsPatternSelectorAll.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jun  6 18:37:28 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSPATTERNSELECTORALL_H
#define CLSPATTERNSELECTORALL_H    /*+ To stop multiple inclusions. +*/

#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qgroupbox.h> 
#include <qlabel.h>
#include <qpushbutton.h> 
#include <qlistbox.h> 

#include "ClsPatternBaseSelector.h"
#include "ClsSubPopulations.h"

//class ClsQStateArrayView;


using namespace std;


class ClsPatternSelectorAll : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorAll( QWidget *parent, ClsBaseAutoGroup *changeReceiver, const char *name );

    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();    
// signals:
// 	void changed();    

    
private:

    
    
};




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

#endif /* CLSPATTERNSELECTORALL_H */
