/****************************************************************************
 ** $Filename: ClsPatternSelectorRegion.h
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

#ifndef CLSPATTERNSELECTORREGION_H
#define CLSPATTERNSELECTORREGION_H    /*+ To stop multiple inclusions. +*/

#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qgroupbox.h> 
#include <qlabel.h>
#include <qpushbutton.h> 
#include <qlistbox.h> 
#include <qlineedit.h>

#include "ClsPatternBaseSelector.h"
#include "ClsSubPopulations.h"

class ClsBaseQStateArrayView;
class ClsBaseTopology;

using namespace std;


class ClsPatternSelectorRegion : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorRegion( QWidget *parent, ClsBaseAutoGroup *changeReceiver, 
			      ClsBaseTopology *_clsBaseTopologyGroup,
			      const char *name );
    
// signals:
// 	void changed();    
    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();

private:

    QGridLayout *pqlayTop;
    
    ClsBaseQStateArrayView *clsQStateArrayView;
    
    QPushButton *qbtnSet;
    QPushButton *qbtnClear;
//    QPushButton *qbtnRemove;
//    QPushButton *qbtnShow;

//    QListBox *qlstBox;

/*
    QLabel *qlblStartXValue; 
    QLabel *qlblStartYValue;
    QLabel *qlblStartWidthValue;
    QLabel *qlblStartHeightValue;
*/
    QLineEdit *qlblStartXValue; 
    QLineEdit *qlblStartYValue;
    QLineEdit *qlblStartWidthValue;
    QLineEdit *qlblStartHeightValue;

    ClsRegion clsRegion;
    int iNrCellsHorizontal, iNrCellsVertical;
    bool bKeyboardInput;
  
    
private slots:
    void addEntry();
//    void removeEntry();
    void clear();
    void textChanged ( const QString & );
};


#endif /* CLSPATTERNSELECTORREGION_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:



