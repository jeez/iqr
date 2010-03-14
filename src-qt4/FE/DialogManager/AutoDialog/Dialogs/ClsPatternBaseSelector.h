/****************************************************************************
 ** $Filename: ClsPatternBaseSelector.h
 ** $Id$
 **
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

#include <QtGui/qlayout.h>
#include <QtGui/qframe.h>
#include <QtGui/qgroupbox.h> 
#include <QtGui/qlabel.h>
#include <QtGui/qpushbutton.h> 


#include "ClsSubPopulations.h"

namespace iqrfe  {
    class ClsBaseAutoGroup;
}


using namespace std;
using namespace iqrfe;


class ClsPatternBaseSelector : public QFrame  {
    Q_OBJECT
public:
    
    ClsPatternBaseSelector(ClsBaseAutoGroup *_changeReceiver, const char *name );
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
