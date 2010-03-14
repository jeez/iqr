/****************************************************************************
 ** $Filename: ClsFEDataSampler.h
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataManager/ClsFEDataSampler.h,v 1.1 2003/10/04 10:06:19 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
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

#include <qworkspace.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qevent.h>
#include <qpoint.h>
#include <qlineedit.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <qlcdnumber.h>
#include <qmovie.h>
#include <qtimer.h>
#include <qevent.h>
#include <qmutex.h>

#include <qprogressbar.h>

#include <ClsFEDataClient.h>

#include <idGenerator.hpp>
#include <ClsQNeuronStateVariableDisplay.h>
#include "ClsDataClientConfig.h"
#include <ClsFEPlotFramework.h>

class ClsQDivider;

/*
  class ColorChangeEvent : public QCustomEvent
  {
  public:
  ColorChangeEvent( QColor color ) : QCustomEvent( 65432 ), c( color ) {};
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

    void customEvent(QCustomEvent *);

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
//    void slotSettingsLocking( int );
    void slotSettingsTarget(int );
    void slotSettingsMisc(int);
    void slotRunControl();
    void slotFileDlg();
    void slotSpinCounting(const  QString&);
    void slotSpinLocking(const  QString&);
    void slotFilenameChanged(const  QString&);
    void slotHideControls( bool bIsRight );

signals:
//    void sigPlotClosed(string);

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


    enum MISC_OPTIONS {
	OPT_AUTOSTART = 200
    };

    void ClsFEDataSampler::saveData( );
//--    void setSettingsSampling( int i );
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
//    int iLOCKING_MODE;
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
//    ClsIDGenerator clsIDGenerator;
    QTimer *qtimerStop;

    ClsFEDataSink *clsFEDataSink;


    /* general gui */
//--    QTabWidget *qTabWidget;
    QHBoxLayout *boxlayoutStateFrames;
//--    ClsQDivider * clsQDivider;
    QFrame *qframeStateVariableDisplays;
    QPushButton *qbtnAction;

    /* settings frame */
//--    QFrame *qframeSettings;
    QSpinBox *qspinSamplePerCycle;
    QSpinBox *qspinSteps;
//TEMP    QSpinBox *qspinTime;
    QLineEdit *leditFileName;


    QButtonGroup *qbtngrpSampling;
    QButtonGroup *qbtngrpAcquisition;
    QButtonGroup *qbtngrpSampleTarget;
    QButtonGroup *qbtngrpMiscOptions;


    /* runtime frame */
    QFrame *qframeRuntime;
    QLabel *qlblTargetRunTime;
    QLabel *qlblAcquitionUnit;
    QLineEdit *leditFileNameRun;
    ClsQProgress *clsQProgress;
    QMovie *qmovieAnimation;


//    friend class ClsFEComputeEngine;
};


#endif /* CLSFEDATASAMPLER_H */



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:

