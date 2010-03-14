/****************************************************************************
 ** $Filename: ClsQGroupStateManip.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Aug 29 12:12:57 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQGROUPSTATEMANIP_H
#define CLSQGROUPSTATEMANIP_H    /*+ To stop multiple inclusions. +*/


#include <list>
#include <string>
#include <map>
#include <vector>
#include <valarray>

#include <qbutton.h>
#include <qcheckbox.h>
#include <qcursor.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qsplitter.h>
#include <qtabwidget.h>

#include "doubleSpinBox.hpp"
#include "intSpinBox.hpp"

class ClsBaseTopology;
class ClsGroupManipPattern;
class ClsQSAList;



class ClsBaseQStateArrayView;



using namespace std;
using namespace iqrfe;

//class ClsQGroupStateManip : public QTabWidget {
class ClsQGroupStateManip : public QFrame {
    Q_OBJECT

public:
    ClsQGroupStateManip (const char * name, string _strGroupID);
    QPixmap getPixmap(vector <vector<double> > vCells);

 public slots:
 void slotGroupChanged();

private slots:

    void slotCellClicked( int iX, int iY);
    void slotSelectTool(int);
    void slotClear();
    void slotAdd();
    void slotReplace();
    void slotRevoke();
    void slotApply();
    void slotMatrixChanged(vector <vector<double> >);
    void slotSetMode(int);
    void slotSetPlayback(int);
    void slotSetInterval(int i);
    void slotSetStepSize(int i);
    void slotSetLoops(int i);
    void slotChangeValue(double);

signals:
    void sigDiagramClosed(string);


private:

    enum TOOLID {
	TOOL_PENCIL,
	TOOL_ERASER
    };



    void closeEvent( QCloseEvent* ce );
    void createStateArray(string strGroupID);

    valarray<double> matrix2vector(bool bSparse, int iSize, vector <vector<double> > _m);
    valarray<double> matrix2vector(ClsBaseTopology* clsBaseTopology, vector <vector<double> > _m);

    static const double fMinVal(){return -1.;};
    static const double fMaxVal(){return  1.;};


    string strGroupID;

    QLabel *qlblCaption;
    QFrame *qfmStateArray;
    ClsBaseQStateArrayView* clsQStateArrayView;
    QLabel* qlblGradientPixmap;
    QButtonGroup *qbgTools;
    QPushButton *qpbtnRevoke;

    ClsDoubleSpinBox* clsDoubleSpinBoxValue;
    ClsIntSpinBox* clsIntSpinBoxIterations;
    ClsIntSpinBox* clsIntSpinBoxInterval;
    ClsIntSpinBox* clsIntSpinBoxStepSize;
    int iMode;
    int iPlayBack;
    int iInterval;
    int iLoops;
    int iStepSize;

    bool bApplied;

    QCursor *qcursorPencil;
    QCursor *qcursorEraser;

    ClsQSAList* clsQSAList;
    
    friend class ClsQSAList; /* to access clear() */

};

#endif /* CLSQGROUPSTATEMANIP_H */

/// Local Variables:
/// mode: c++
/// End:
