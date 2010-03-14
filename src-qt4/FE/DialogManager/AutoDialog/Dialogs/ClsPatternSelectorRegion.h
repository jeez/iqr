/****************************************************************************
 ** $Filename: ClsPatternSelectorRegion.h
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

#ifndef CLSPATTERNSELECTORREGION_H
#define CLSPATTERNSELECTORREGION_H    /*+ To stop multiple inclusions. +*/

#include <QLineEdit>
#include <QLayout>
#include <QFrame>
#include <QLabel>
#include <QGroupBox> 
#include <QPushButton> 



#include "ClsPatternBaseSelector.h"
#include "ClsSubPopulations.h"

class ClsBaseQStateArrayView;
class ClsBaseTopology;

using namespace std;


class ClsPatternSelectorRegion : public ClsPatternBaseSelector  {
    Q_OBJECT
public:
    ClsPatternSelectorRegion( ClsBaseAutoGroup *changeReceiver, 
			      ClsBaseTopology *_clsBaseTopologyGroup,
			      const char *name );
    
    void setSubPopulation(ClsBaseSubPopulation* _clsBaseSubPopulation);
    ClsBaseSubPopulation* getSubPopulation();

private:

    ClsBaseQStateArrayView *clsQStateArrayView;
    
    QPushButton *qbtnSet;
    QPushButton *qbtnClear;

    QLineEdit *qlblStartXValue; 
    QLineEdit *qlblStartYValue;
    QLineEdit *qlblStartWidthValue;
    QLineEdit *qlblStartHeightValue;

    ClsRegion clsRegion;
    int iNrCellsHorizontal, iNrCellsVertical;
    bool bKeyboardInput;
  
    
private slots:
    void addEntry();
    void clear();
    void textChanged ( const QString & );
};


#endif /* CLSPATTERNSELECTORREGION_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j2"
//// End:



