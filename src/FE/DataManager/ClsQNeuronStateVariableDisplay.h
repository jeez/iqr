/****************************************************************************
 ** $Filename: ClsQNeuronStateVariableDisplay.h
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataMgr/ClsQNeuronStateVariableDisplay.h,v 1.3 2002/02/10 13:21:50 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Oct 26 01:32:27 2001
 ** $Date: 2002/02/10 13:21:50 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQGROUPSTATEVARIABLEDISPLAY_H
#define CLSQGROUPSTATEVARIABLEDISPLAY_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>

#include <qevent.h>

#include "ClsQBaseStateVariableDisplay.h"
#include <ClsInfoGroup.h> //XXX

#include "ClsFEGroup.h"


//#include <info_small.xpm>

using namespace iqrcommon;
using namespace std;

class ClsFEBasePlot;
class ClsQDragHandle;
class ClsQMiniGroup;


class ClsQNeuronStateVariableDisplay :  public ClsQBaseStateVariableDisplay {
    Q_OBJECT

public:
    ClsQNeuronStateVariableDisplay(ClsFEDataClient *clsFEDataClient, string strID, QWidget *parent,
			     ClsFEGroup* _clsFEGroup, string strSelectedIndices,
			     bool bCloseEnabled, bool bAverageEnabled, bool bExclusive,
			     bool bAdditiveDisplay, bool bMiniGroupDisplay);

    ~ClsQNeuronStateVariableDisplay();

    list<string> getSelectedStates();
    string getSelectedStatesAsString();
    void setSelectedStates(list<string> lst);
    void setSelectedStates(string strStates);

    string getItemID() { return getGroupID();};
    string getGroupID() { return clsFEGroup->getGroupID(); };
    string getID() {return strID;};
    string getSelectedIndices(){ return strSelectedIndices;};

    bool useAverage() { return bAverage; };

    void setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig);
    ClsStateVariableDisplayConfig getConfig();

    void groupChanged();

signals:
    void sigClose(string );
    void addDisplay(string, list<string>, string );


public slots:
void slotClose();
    void slotLiveData(bool bToggled);

private slots:
    void slotStateVariables(int ii);
    void slotAverage(bool bToggled);
    void startDragging(bool bCopy);
    void slotDropAction(int ii);

private:

    int iCurrentStateButton; // I know this is ugly, but qt doesn't provide any other mechanisms to find out which button as on before...
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent( QCloseEvent* );

    QLabel *qLabelMiniGroup;

    ClsFEGroup* clsFEGroup;

    string strSelectedIndices;
    bool bExclusiveStates;

    bool bAdditiveDisplay;

    bool bAverage;
    QPushButton *qpbtnGroupInfo;
    QPushButton *qpbtnClose;
    QCheckBox *qchkbxAverage;
    QString qstrDropString;
};


#endif


//// Local Variables:
//// mode: c++
//// End:
