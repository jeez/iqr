#ifndef CLSQCONNECTIONDIAGRAM_HPP
#define CLSQCONNECTIONDIAGRAM_HPP

#include <list>
#include <string>
#include <map>

#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>

#include <ClsFEDataClient.h>
#include <ClsFEPlotFramework.h>

#include "ClsFEBasePlot.h"
#include "ClsHyperLists.h"
#include "ClsBaseConnection.h"

#include "ClsQSynapseStateVariableDisplay.h"

class ClsFEConnection;
class ClsBaseQStateArrayView;
class ClsQSynapseStateVariableDisplay;


using namespace std;
using namespace iqrcommon;


#define GRADIENT_HEIGHT 15
#define GRADIENT_WIDTH 150

class ClsRangeGradient : public QFrame {

public:
    ClsRangeGradient(QWidget *parent) : QFrame(parent){
	QHBoxLayout *qlayoutGradient = new QHBoxLayout( this);

	qlblMinValue = new QLabel(this);
	qlblMaxValue = new QLabel(this);
	qlblGradientPixmap = new QLabel(this);;
	qlblGradientPixmap->setFixedHeight(GRADIENT_HEIGHT);
	qlblGradientPixmap->setFixedWidth(GRADIENT_WIDTH);

	qlayoutGradient->addWidget(qlblMinValue, 0, Qt::AlignRight);
	qlayoutGradient->addSpacing(5);
	qlayoutGradient->addWidget(qlblGradientPixmap, 1, Qt::AlignHCenter);
	qlayoutGradient->addSpacing(5);
	qlayoutGradient->addWidget(qlblMaxValue);

	setMinValue(0);
	setMaxValue(1);
    }

    void setMinValue(double fVal){
	QString qstr;
	qstr.setNum(fVal, 'f', 3);
	qlblMinValue->setText(qstr);
    }

    void setMaxValue(double fVal){
	QString qstr;
	qstr.setNum(fVal, 'f', 3);
	qlblMaxValue->setText(qstr);
    }

    void setPixmap(const QPixmap &pm){
	qlblGradientPixmap->setPixmap(pm);
    }

private:
    QLabel* qlblMinValue;
    QLabel* qlblMaxValue;
    QLabel* qlblGradientPixmap;


};


//class ClsFEConnectionDiagram : public QFrame, public ClsFEBasePlot {
class ClsFEConnectionDiagram : public ClsFEPlotFramework,  public ClsFEDataClient {
    Q_OBJECT

public:
    ClsFEConnectionDiagram (ClsFEDataManager *_clsFEDataManager, QMutex *_qmutexSysGUI, string _strMyID, string _strConnectionID);

    void createDiagram( );
    string addStateVariableDisplay(ClsFEConnection* _clsFEConnection);
    void init();
    void update();
    void print();
    void close(){ QWidget::close();};
    ClsDataClientConfig getConfig();
    void setConfig(ClsDataClientConfig);

    void groupChanged(string);
    void groupDeleted(string);    

    void connectionChanged(string strID);
    void connectionDeleted(string);

/*
    void move(int x, int y){
	QFrame::move(x,y);
    }

    void resize(int w, int h){
	QFrame::resize(w, h);
    }
*/


public slots:

signals:
//--    void sigPlotClosed(string);

private slots:

void slotSourceCellClicked( int iX, int iY);
    void slotTargetCellClicked( int iX, int iY);
    void slotSetType(int);
    void slotLiveData(bool);

private:

    ClsBaseQStateArrayView* createStateArray(string strGroupID, QFrame* qfmParent);
    void appendSourceStateArray(string strGroupID);
    void appendTargetStateArray(string strGroupID);

    void closeEvent( QCloseEvent* ce );

    double getDistMax(string strGroupID);
    void createPre2Post();
    void createPost2Pre();



    string strMyID;
    string strConnectionID;

    ClsBaseQStateArrayView* clsQStateArrayViewSource;
    ClsBaseQStateArrayView* clsQStateArrayViewTarget;

    QLabel *qlblSource;
    QLabel *qlblTarget;

    QFrame *qfmSASource;
    QFrame *qfmSATarget;


    ClsRangeGradient* clsRangeGradient;

    static const int iMarkColor=64;
    ListIndexQuadruples listIndexQuadruple;

    vector<Pre2PostType> pre2post;
    vector<Post2PreType> post2pre;

    int iType;
    bool bLiveData;

    double fDistMaxSource, fDistMaxTarget;
    int iDelayMax;
    double fAttenuationMax;

    int iIndexSourceCell, iIndexTargetCell;

};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
