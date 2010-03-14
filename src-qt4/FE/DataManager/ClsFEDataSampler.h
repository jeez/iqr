/****************************************************************************
 ** $Filename: ClsFEDataSampler.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Nov  2 15:18:33 2001
 ** $Date: 2003/10/04 10:06:19 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEDATASAMPLER_H
#define CLSFEDATASAMPLER_H    /*+ To stop multiple inclusions. +*/

#include <iostream>
#include <fstream>

#include <QBuffer>
#include <QCheckBox>
#include <QEvent>
#include <QLabel>
#include <QMovie>
#include <QMutex>
#include <QPoint>
#include <QTimer>

#include <QButtonGroup>
#include <QGroupBox>


#include <qlcdnumber.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qworkspace.h>

#include <ClsFEDataClient.h>

#include <idGenerator.hpp>
#include <ClsQNeuronStateVariableDisplay.h>
#include "ClsDataClientConfig.h"
#include <ClsFEPlotFramework.h>

class ClsQDivider;

/*
  class ColorChangeEvent : public QEvent
  {
  public:
  ColorChangeEvent( QColor color ) : QEvent( 65432 ), c( color ) {};
  QColor color() const { return c; }
  private:
  QColor c;
  };
*/

class ClsQProgress :  public QFrame  {

public:
    ClsQProgress(QWidget *parent, int _iGaugeType);
    void setGaugeType(int _iGaugeType);
    void setValue( int ii);
    void setMaxValue(int iMax);
    void resize(QSize qsize);

    enum GAUGE_TYPE{
	GAUGE_THERMO,
	GAUGE_LCD
    };

private:
    int iGaugeType;
    QLCDNumber *qLCD;
    QProgressBar *qProgressBar;
};


//class ClsFEDataSampler :  public QWidget, public ClsFEDataClient  {
class ClsFEDataSampler :  public ClsFEPlotFramework,  public ClsFEDataClient  {
    Q_OBJECT

public:
    ClsFEDataSampler(ClsFEDataManager *clsFEDataManager, QMutex *_qmutexSysGUI, string strDataClientID);

    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange);
    string addStateVariableDisplay(string strGroupID, string strRange);

    bool isAutoStart(){return bAutoStart;};
    ClsDataClientConfig getConfig();
    void setConfig(ClsDataClientConfig clsDataClientConfig);
    void groupChanged(string strGroupID); 
    void groupDeleted(string strID);
    void init();
    void close(){ QWidget::close();};

    void customEvent(QEvent *);

    enum EVENT_TYPES {
	EVENT_SAVEDATA = 65432,
	EVENT_STOP
    };

public slots:
    string addStateVariableDisplay(string, list<string>, string strRange);
    void removeStateVariableDisplay(string);
    void slotStartSampling();
    void slotStopSampling();

    void update(){};

private slots:
    void slotSettingsCounting( int );
    void slotSettingsTarget(int );
    void slotRunControl();
    void slotFileDlg();
    void slotSpinCounting(const  QString&);
    void slotSpinLocking(const  QString&);
    void slotFilenameChanged(const  QString&);
    void slotHideControls( bool bIsRight );
    void slotToggleAutoStart(int i);


signals:

private:

    enum CHECK_STATES {
	LOCKING_CYCLES,
	COUNTING_CONTINUOUS,
	COUNTING_STEPS,
	COUNTING_TIME,
	TARGET_OVERWRITE,
	TARGET_APPEND,
	TARGET_SEQUENCE
    };



    void saveData( );
    void setSettingsAcquistion( int i );
    void setSettingTarget( int i );
    void setSettingAutoStart ( bool b );
    void setAcquisition( int i );
    void setSampling( int i );
    void setFileName( string str );


    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    void prepareOutStream();
    void closeEvent( QCloseEvent* ce );


    void checkState(bool ok = true);


    bool bAutoStart;
    bool bFirstRound;
    int iCOUNTING_MODE;
    int iTARGET_MODE;
    double fSamplingRate;
    int iAmount;
    int iStepCounter;
    int iFileSequenceCounter;
    bool bSampling;

    ofstream ofstreamFile;

    string strTitle;
    string strFileName;

    map<string, ClsQNeuronStateVariableDisplay*> mapStateVariableDisplays;
    QTimer *qtimerStop;

    ClsFEDataSink *clsFEDataSink;


    /* general gui */
    QHBoxLayout *boxlayoutStateFrames;
    QFrame *qframeStateVariableDisplays;
    QPushButton *qbtnAction;

    /* settings frame */
    QSpinBox *qspinSamplePerCycle;
    QSpinBox *qspinSteps;
    QLineEdit *leditFileName;
    QCheckBox *qchkAutoStart;

    QGroupBox *qgrpbxSampling;

    QGroupBox *qgrpbxAcquisition;
    QButtonGroup* qbtngrpAcquisition;

    QGroupBox *qgrpbxSampleTarget;
    QButtonGroup* qbtngrpSampleTarget;

    QGroupBox *qgrpbxMiscOptions;


    /* runtime frame */
    QFrame *qframeRuntime;
    QLabel *qlblTargetRunTime;
    QLabel *qlblAcquitionUnit;
    QLabel *qlblStatus;

    QLineEdit *leditFileNameRun;
    ClsQProgress *clsQProgress;



};


#endif /* CLSFEDATASAMPLER_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
