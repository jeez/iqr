/****************************************************************************
 ** $Filename: ClsFEPlotFramework.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Nov  5 01:02:24 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEPLOTFRAMEWORK_H
#define CLSFEPLOTFRAMEWORK_H    /*+ To stop multiple inclusions. +*/

#include <qwidget.h>
#include <qframe.h>
#include <qlayout.h>

#include "ClsFEBasePlot.h"
#include "ClsDataClientConfig.h"
#include "ClsQBaseStateVariableDisplay.h"

using namespace std;

class ClsQDivider;

class ClsFEPlotFramework : public QWidget, public ClsFEBasePlot{
    Q_OBJECT

public:
    ClsFEPlotFramework();
    virtual ClsDataClientConfig getConfig()=0;
    virtual void setConfig(ClsDataClientConfig){};
    virtual void print();
    void move(int _x, int _y){
	QWidget::move(_x,_y);
    }

    void resize(int w, int h){
	QWidget::resize(w, h);
    }


protected:
    QGridLayout *boxlayoutBase;
    ClsQBaseStateVariableDisplay *clsQBaseStateVariableDisplay;

    ClsQDivider * clsQDivider;

signals:
    void sigPlotClosed(string);

private slots:
virtual void slotHideControls ( bool bIsRight );

};

#endif /* CLSFEPLOTFRAMEWORK_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
