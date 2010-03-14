/****************************************************************************
 ** $Filename: ClsQGroupStateManip.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Aug 29 12:12:57 2003
 **
 *****************************************************************************/


#ifndef CLSQGROUPSTATEMANIP_H
#define CLSQGROUPSTATEMANIP_H    /*+ To stop multiple inclusions. +*/


#include <list>
#include <string>
#include <map>
#include <vector>
#include <valarray>


#include <QCheckBox>
#include <QCursor>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QSplitter>
#include <QGroupBox>

class ClsBaseTopology;
class ClsGroupManipPattern;
class ClsQSAList;



class ClsBaseQStateArrayView;



using namespace std;
//--using namespace iqrfe;

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
    QGroupBox *qgrpbxTools;
    QButtonGroup* qbtngrpTools;

    QPushButton *qpbtnRevoke;

    QDoubleSpinBox* qdblspnbx;
    QSpinBox* qspnbxIterations;
    QSpinBox* qspnbxInterval;
    QSpinBox* qspnbxStepSize;
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


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
