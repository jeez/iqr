/****************************************************************************
 ** $Filename: ClsFEBasePlot.h
 ** $Header$
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Sun Sep  2 12:09:46 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEBASEPLOT_H
#define CLSFEBASEPLOT_H    /*+ To stop multiple inclusions. +*/

#include <string>

#include <qobject.h>
#include <qwidget.h>
#include <qframe.h>
#include <qlayout.h>

#include "ClsDataClientConfig.h"

using namespace std;

class ClsQDivider;

//$$$class ClsFEBasePlot  { //:  public QWidget{
class ClsFEBasePlot  {//:  public QObject{
//    Q_OBJECT

public:
    ClsFEBasePlot(){};
    virtual ~ClsFEBasePlot(){};

//    virtual void plotData()=0;
    virtual void update()=0;
    virtual void print()=0;
    virtual void init()=0;
    virtual void close()=0;//{ cout << "ClsFEBasePlot::close()" << endl; };

    virtual ClsDataClientConfig getConfig()=0;
    virtual void setConfig(ClsDataClientConfig)=0;
    virtual void groupChanged(string)=0;
    virtual void groupDeleted(string)=0;

    virtual void connectionChanged(string){};
    virtual void connectionDeleted(string){};

    virtual void resize(int, int)=0;
    virtual void move(int, int)=0;


//signals:
//    void sigDataClientClosed(string);

protected:
    string strTitle;

//$$$    QBoxLayout *boxlayoutPlot;
//$$$    QFrame *qframeControls;
//$$$    QHBoxLayout *boxlayoutStateFrames;
//$$$
//$$$    ClsQDivider * clsQDivider;

//$$$    virtual void closeEvent( QCloseEvent* )=0;

//private slots:
//    virtual void slotHideControls ( bool bIsRight )=0;

private:


};

#endif /* CLSFEBASEPLOT_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:

