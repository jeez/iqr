/****************************************************************************
 ** $Filename: ClsPatternSelectorTuples.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat May 31 01:08:26 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSPATTERNSELECTORTUPLES_H
#define CLSPATTERNSELECTORTUPLES_H    /*+ To stop multiple inclusions. +*/

#include <qlayout.h>
#include <qframe.h>
#include <qgroupbox.h> 
#include <qlabel.h>
#include <qpushbutton.h> 
#include <qlistbox.h> 
#include "ClsPatternBaseSelector.h"


class ClsBaseQStateArrayView;
class ClsBaseTopology;


using namespace std;


class ClsPatternSelectorTuples : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorTuples( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
			      ClsBaseTopology *_clsBaseTopologySource, 
			      ClsBaseTopology *_clsBaseTopologyTarget, 
			      const char *name );

    


//    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    void setSubPopulation(ClsTuples* _clsTuples);
//    ClsBaseSubPopulation* getSubPopulation();
    ClsTuples* getSubPopulation();
// signals:
// 	void changed();    

    
private:
    
    QGridLayout *pqlayTop;

//    ClsBaseTopology *clsBaseTopologySource;
//    ClsBaseTopology *clsBaseTopologyTarget;
    
    ClsBaseQStateArrayView *clsQStateArrayViewSource;
    ClsBaseQStateArrayView *clsQStateArrayViewTarget;
    
    QPushButton *qbtnAdd;
    QPushButton *qbtnClear;
    QPushButton *qbtnRemove;
    QPushButton *qbtnShow;
    QPushButton *qbtnImport;

    QListBox *qlstBox;

private slots:
    void addEntry();
    void removeEntry();
    void showEntry();
    void showEntry( QListBoxItem * item);
    void clear();
    void import();
    
    
};


#endif /* CLSPATTERNSELECTORTUPLES_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k "
//// End:




