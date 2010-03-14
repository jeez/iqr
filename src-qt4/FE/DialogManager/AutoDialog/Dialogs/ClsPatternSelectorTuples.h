/****************************************************************************
 ** $Filename: ClsPatternSelectorTuples.h
 ** $Id$
 **
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

#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QGroupBox> 
#include <QPushButton> 
#include <QListWidget>

//--#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>

#include "ClsPatternBaseSelector.h"


class ClsBaseQStateArrayView;
class ClsBaseTopology;


using namespace std;


class ClsPatternSelectorTuples : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorTuples( ClsBaseAutoGroup *changeReceiver, 
			      ClsBaseTopology *_clsBaseTopologySource, 
			      ClsBaseTopology *_clsBaseTopologyTarget, 
			      const char *name );

    void setSubPopulation(ClsTuples* _clsTuples);
    ClsTuples* getSubPopulation();

    
private:
    
    ClsBaseQStateArrayView *clsQStateArrayViewSource;
    ClsBaseQStateArrayView *clsQStateArrayViewTarget;
    
    QPushButton *qbtnAdd;
    QPushButton *qbtnClear;
    QPushButton *qbtnRemove;
    QPushButton *qbtnShow;
    QPushButton *qbtnImport;

    QListWidget *qlstBox;

private slots:
    void addEntry();
    void removeEntry();
    void showEntry();
    void showEntry( QListWidgetItem * item);
    void clear();
    void import();
};


#endif /* CLSPATTERNSELECTORTUPLES_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k "
//// End:




