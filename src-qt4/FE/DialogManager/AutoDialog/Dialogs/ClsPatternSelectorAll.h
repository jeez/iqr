/****************************************************************************
 ** $Filename: ClsPatternSelectorAll.h
 ** $Id$
 **
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
#define CLSPATTERNSELECTORALL_H 

#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QGroupBox> 
#include <QPushButton> 

#include "ClsPatternBaseSelector.h"
#include "ClsSubPopulations.h"

//class ClsQStateArrayView;


using namespace std;


class ClsPatternSelectorAll : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorAll(ClsBaseAutoGroup *changeReceiver, const char *name );

    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();    

    
private:

    
    
};



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j2"
//// End:

#endif /* CLSPATTERNSELECTORALL_H */
