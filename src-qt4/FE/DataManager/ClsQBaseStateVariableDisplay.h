/****************************************************************************
 ** $Filename: ClsQBaseStateVariableDisplay.h
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Feb  4 13:27:50 2004
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSQBASESTATEVARIABLEDISPLAY_H
#define CLSQBASESTATEVARIABLEDISPLAY_H    /*+ To stop multiple inclusions. +*/

#include <qframe.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QButtonGroup>


#include "ClsDataClientConfig.h"
#include <ClsFEDataClient.h>
#include <iqrUtils.h>

using namespace iqrcommon;
using namespace std;

class ClsQBaseStateVariableDisplay :  public QFrame   {
public:
    ClsQBaseStateVariableDisplay(ClsFEDataClient *_clsFEDataClient, string _strID, QWidget *_parent);
    
    bool isLive();
    virtual void setConfig(ClsStateVariableDisplayConfig clsStateVariableDisplayConfig)=0;
    virtual ClsStateVariableDisplayConfig getConfig()=0;
    virtual string getItemID()=0;

    signals:
    
public slots:
virtual void slotClose()=0;
virtual void slotLiveData(bool bToggled)=0;
    
private slots:
    virtual void slotStateVariables(int ii)=0;
    
protected:
    virtual void closeEvent( QCloseEvent* )=0;
    
    QWidget *parent;
    ClsFEDataClient *clsFEDataClient;
    string strID;

    QLabel* lblCaption;    
    QButtonGroup* qbtngrpStateVariables;
    QGroupBox* qgrpbxStateVariables;
    QVBoxLayout *qlayoutStateVariables;
    QCheckBox* qchkbxLiveData;
    
};

#endif /* CLSQBASESTATEVARIABLEDISPLAY_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
