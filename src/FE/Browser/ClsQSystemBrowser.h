/****************************************************************************
 ** $Filename: ClsQSystemBrowser.h
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ClsQSystemBrowser.h,v 1.1 2002/12/08 17:33:45 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Mon Sep 10 10:53:54 2001
 ** $Date: 2002/12/08 17:33:45 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSQSYSBROWSER_H
#define CLSQSYSBROWSER_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <iostream>
#include <queue>

#include <qlistview.h>
#include <qpopupmenu.h>
#include <qdragobject.h>

#include "ClsSysFileNode.h"
#include "ClsQBaseItem.h"
#include "ClsQlstItemSystem.h"
#include "ClsQlstItemConnectionHeader.h"
using namespace std;

class ClsFESystemManager;
class ClsFEGroup;

class ClsQSystemBrowser: public QListView {
     Q_OBJECT

public:
     ClsQSystemBrowser( QWidget *parent);
     ClsQBaseItem *getItem(string strID);

    void closeSystem();


signals:
     void sigDataDisplay(int, string, string, string);
     void sysbrowserRightClick(int ,string,  QPoint);
    void sigShowBlockDiagram(string);


public slots:
     void slotSystemChanged();
    void slotItemAdded(int iType, string strID );
    void slotItemDeleted(int iType, string strID );
    void slotItemChanged(int iType, string strID );
    void slotItemUnDeleted(int iType, string strID );



private slots:
     void slotRMB( QListViewItem* Item, const QPoint & point, int );
     void startDrag();


protected:
     /* needed in qt 2.3.x */
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMouseDoubleClickEvent ( QMouseEvent * e );

     void setRoot(string strID, string strName);
     void addProcess(string strProcessID, string strProcessName);
    void addGroup(string strProcessID, string strGroupID, string strGroupName);
    void addConnection(string strConnectionID, string strConnectionName, int iConnectionType);

//***     QPopupMenu* menuBrowser;
     ClsQlstItemSystem *qlstvitemRootElement;
    ClsQlstItemConnectionHeader *qlstvitemConnections;

     QWidget *parent;
//     ClsFESystemManager *clsFESystemManager;

};


#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
