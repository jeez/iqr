/****************************************************************************
 ** $Filename: ClsQSynapseStateVariableDisplay.h
 ** $Header:
 **
 ** Copyright (C) 2003- by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate:
 ** $Date:
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQSYNAPSESTATEVARIABLEDISPLAY_H
#define CLSQSYNAPSESTATEVARIABLEDISPLAY_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>


#include <qmap.h>
#include <qgroupbox.h> 
#include <qevent.h>

#include <ClsInfoConnection.h> 

#include "ClsFEConnection.h"

#include "ClsQBaseStateVariableDisplay.h"

//#include <info_small.xpm>

using namespace iqrcommon;
using namespace std;

class ClsFEBasePlot;

 class ClsExclusifier  : public QObject {
     Q_OBJECT
 public:
     ClsExclusifier();
     void addButton(QRadioButton* qrb, int iID);
     void checkButton(int iID);
     void toggleButton(int iID);
     bool isChecked(int iID);
	    
 signals:
     void sigClicked(int);

 private slots:
 void buttonClicked(int iState);
  
  
 private:
     int iRecursion;
     QMap<int, QRadioButton*> mapButtons;
 };

    

class ClsQSynapseStateVariableDisplay :  public ClsQBaseStateVariableDisplay {
    Q_OBJECT

public:
    ClsQSynapseStateVariableDisplay(ClsFEDataClient *clsFEDataClient, 
				    string strID, QWidget *parent,
				    ClsFEConnection* _clsFEConnection, 
				    string strSelectedIndices);

    ~ClsQSynapseStateVariableDisplay();

    enum {
	TYPE_DISTANCE,
	TYPE_DELAY,
	TYPE_ATTENUATION,
	TYPE_SYNAPSESTATE
    };

    list<string> getSelectedStates();
    string getSelectedStatesAsString();
    void setSelectedStates(list<string> lst);
    void setSelectedStates(string strStates);

    string getItemID() { return getConnectionID();};
    string getConnectionID() { return clsFEConnection->getConnectionID(); };
    string getID() {return strID;};
    string getSelectedIndices(){ return strSelectedIndices;};

    void setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig);
    ClsStateVariableDisplayConfig getConfig();

    void connectionChanged();

    signals:
    void sigChangeType(int);
    void sigLiveData(bool);
    void sigClose(string );
    void addDisplay(string, list<string>, string );


public slots:
    void slotClose();

private slots:
    void slotLiveData(bool bToggled);
    void slotStateVariables(int ii);

    void slotSetType(int ii);

private:

    int iCurrentStateButton; // I know this is ugly, but qt doesn't provide any other mechanisms to find out which button as on before...
    void closeEvent( QCloseEvent* );

    ClsFEConnection* clsFEConnection;

    string strSelectedIndices;

    QGroupBox* qgbDisplay;
    QPushButton* qpbtnConnectionInfo;
    QString qstrDropString;

    ClsExclusifier* clsExclusifier;
};


#endif


//// Local Variables:
//// mode: c++
//// End:
