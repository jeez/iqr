/****************************************************************************
 ** $Filename: ClsPatternSelectorList.h
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

#ifndef CLSPATTERNSELECTORLIST_H
#define CLSPATTERNSELECTORLIST_H

#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QGroupBox> 
#include <QPushButton> 
#include <QTextEdit>

#include "ClsPatternBaseSelector.h"


class ClsBaseQStateArrayView;
class ClsBaseTopology;

using namespace std;


class ClsPatternSelectorList : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorList( ClsBaseAutoGroup *changeReceiver, 
			    ClsBaseTopology *_clsBaseTopologyGroup,
			    const char *name );
    
    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();

private:

    ClsBaseQStateArrayView *clsQStateArrayView;
    
    QPushButton *qbtnAdd;
    QPushButton *qbtnClear;

    QTextEdit *qtexteditSelection;
    
    list<pair<int, int> > lstSelection;


private slots:
    void addEntry();
    void clear();
    
};


#endif /* CLSPATTERNSELECTORLIST_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j2"
//// End:




