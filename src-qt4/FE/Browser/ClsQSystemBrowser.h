/****************************************************************************
 ** $Filename: ClsQSystemBrowser.h
 **
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

#include <QTreeWidget>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QLineEdit>

#include "ClsSysFileNode.h"
#include "ClsQBaseItem.h"
#include "ClsQlstItemSystem.h"
#include "ClsQlstItemConnectionHeader.h"
using namespace std;

class ClsFESystemManager;
class ClsFEGroup;

class ClsQSystemBrowser: public QTreeWidget {
     Q_OBJECT

public:
     ClsQSystemBrowser( QWidget *parent);
     ClsQBaseItem *getItem(string strID);

    void closeSystem();
    int getWidth ();
    void setFilter(QString qstrFilter);
    void removeFilter();

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
    void slotDiagItemActivated(int iType, string strID);						    
						    


private slots:
    void contextMenuEvent ( QContextMenuEvent * e );
    void startDrag(Qt::DropActions );


protected:

    void mouseDoubleClickEvent( QMouseEvent * e );

    void setRoot(string strID, string strName);
    void addProcess(string strProcessID, string strProcessName);
    void addGroup(string strProcessID, string strGroupID, string strGroupName);
    void addConnection(string strConnectionID, string strConnectionName, int iConnectionType);

     ClsQlstItemSystem *qlstvitemRootElement;
    ClsQlstItemConnectionHeader *qlstvitemConnections;

     QWidget *parent;

};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
