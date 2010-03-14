/****************************************************************************
 ** $Filename: ClsPatternBaseSelector.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jun  5 01:40:19 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSPATTERNBASESELECTOR_H
#define CLSPATTERNBASESELECTOR_H    /*+ To stop multiple inclusions. +*/

#include <string>

#include <qlayout.h>
#include <qframe.h>
#include <qgroupbox.h> 
#include <qlabel.h>
#include <qpushbutton.h> 
#include <qlistbox.h> 

#include "ClsSubPopulations.h"

namespace iqrfe  {
    class ClsBaseAutoGroup;
}


using namespace std;
using namespace iqrfe;


class ClsPatternBaseSelector : public QFrame  {
    Q_OBJECT
public:
    
    ClsPatternBaseSelector( QWidget *parent, ClsBaseAutoGroup *_changeReceiver, const char *name );
    string Type();

    virtual void setSubPopulation(ClsBaseSubPopulation* /*_clsBaseSubPopulation*/){};
    virtual ClsBaseSubPopulation* getSubPopulation(){
	return NULL;
    };    
signals:
	void changed();    
    


protected:
    string strType;
    ClsBaseAutoGroup *changeReceiver;
    
    
};



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

#endif /* CLSPATTERNBASESELECTOR_H */
