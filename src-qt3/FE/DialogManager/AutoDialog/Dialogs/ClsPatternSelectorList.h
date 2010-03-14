/****************************************************************************
 ** $Filename: ClsPatternSelectorList.h
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

#ifndef CLSPATTERNSELECTORLIST_H
#define CLSPATTERNSELECTORLIST_H    /*+ To stop multiple inclusions. +*/

#include <qlayout.h>
#include <qframe.h>
#include <qgroupbox.h> 
#include <qlabel.h>
#include <qpushbutton.h> 
#include <qlistbox.h> 
#include <qtextedit.h> 
#include "ClsPatternBaseSelector.h"


class ClsBaseQStateArrayView;
class ClsBaseTopology;

using namespace std;


class ClsPatternSelectorList : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorList( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
			    ClsBaseTopology *_clsBaseTopologyGroup,
			    const char *name );
    
    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();

private:

    QGridLayout *pqlayTop;
    
    ClsBaseQStateArrayView *clsQStateArrayView;
    
    QPushButton *qbtnAdd;
    QPushButton *qbtnClear;
//    QPushButton *qbtnRemove;
//    QPushButton *qbtnShow;

//    QListBox *qlstBox;

    QTextEdit *qtexteditSelection;
    
    vector<tiPoint> lstSelection;


private slots:
    void addEntry();
//    void removeEntry();
    void clear();
    
    
};


#endif /* CLSPATTERNSELECTORLIST_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:




