#ifndef CLSFETIMEPLOT_H
#define CLSFETIMEPLOT_H

#include <map>
#include <qlabel.h>
#include <qcheckbox.h>

#include <idGenerator.hpp>

//$$$#include <ClsFEBasePlot.h>
#include <ClsFEDataClient.h>

#include <ClsFEPlotFramework.h>

#include <ClsQNeuronStateVariableDisplay.h>
#include "wLinePlot.h"

class ClsFETimePlot :   /* public ClsFEBasePlot, */ public ClsFEPlotFramework,  public ClsFEDataClient  {
    Q_OBJECT

public:
    ClsFETimePlot(ClsFEDataManager *clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange);
    void init();
    void update();
    void setConfig(ClsDataClientConfig clsDataClientConfig);
    ClsDataClientConfig getConfig();
    void groupChanged(string strGroupID);
    void groupDeleted(string strID);
    void close(){ QWidget::close();};


public slots:
    void plotData();
    string addStateVariableDisplay(string, list<string>, string strRange);
    void removeStateVariableDisplay(string);
    void slotHideControls ( bool bIsRight );

private slots:

private:
//    ClsIDGenerator clsIDGenerator;

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent( QCloseEvent* );

    void DataSinkAdded(string strID, int iColor);
    void DataSinkRemoved(string strID);

    WLinePlot *wLinePlot;

    QFrame* qframeStateVariableDisplays;
    QHBoxLayout *boxlayoutStateFrames;
    map<string, ClsQNeuronStateVariableDisplay*> mapStateVariableDisplays;
    map<string, int> mapTraces;
    int iTraceLength;


};

#endif

//// Local Variables:
//// mode: c++
//// End:
