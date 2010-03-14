/****************************************************************************
 ** $Filename: ClsFEGroupPlot.h
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Oct 26 01:32:22 2001
 ** $Date: 2003/10/04 09:50:48 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEGROUPPLOT_H
#define CLSFEGROUPPLOT_H

#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QEvent>
#include <QPoint>

#include <idGenerator.hpp>

#include <ClsFEPlotFramework.h>
#include <ClsFEDataClient.h>
#include <ClsQNeuronStateVariableDisplay.h>
#include <ClsBaseTopology.h>

#include "wSpacePlot.h"

class ClsFEGroupPlot :  /* public ClsFEBasePlot, */ public ClsFEPlotFramework, public ClsFEDataClient  {
    Q_OBJECT

public:
    ClsFEGroupPlot(ClsFEDataManager *clsFEDataManager, QMutex *_qmutexSysGUI, string strDataClientID, ClsFEGroup* _clsFEGroup=NULL);

    void init();
    void update();
    void setConfig(ClsDataClientConfig clsDataClientConfig);
    void groupChanged(string strGroupID);
    void groupDeleted(string strID);
    void close(){ QWidget::close();};
    ClsDataClientConfig getConfig();

public slots:
    void plotData();
    string addStateVariableDisplay(string, list<string>, string strRange);

private slots:
//--    void slotPlotMouseClick(QEvent::Type qeventtype, QPoint qpointPos, int iButton, int);
//--    void slotPlotMenu(int iMenuID);
    void slotLiveData(bool);
    void startDragging();

private:
//--    enum PLOT_MENU{
//--	MENU_SPACE_PLOT,
//--	MENU_TIME_PLOT,
//--	MENU_LIVE_DATA,
//--	MENU_AUTOSCALE
//--    };

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent( QCloseEvent* );
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange);
    void DataSinkAdded(string strID, int iColor);
    string wSpacePlotSelected2Range(QVector<int> &qvectorSelected);

    void getCellIndices(ClsFEGroup* _clsFEGroup);

    int iGroupWidth, iGroupHeight;

    /* sparse topology related */
    vector<int> vectCellIndices;
    bool bSparseTopology;
    /* ------ */

    string strGroupID;


//    ClsIDGenerator clsIDGenerator;
    WSpacePlot *wSpacePlot;

//    ClsQNeuronStateVariableDisplay *clsQNeuronStateVariableDisplay;




};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
