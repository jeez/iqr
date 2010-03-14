/****************************************************************************
 ** $Filename: ClsFEDiagramManager.h
 ** $Id: ClsFEDiagramManager.h,v 1.2 2003/02/22 22:49:49 ulysses Exp ulysses $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jan 23 15:31:21 2003
 ** $Date: 2003/02/22 22:49:49 $
 **
 ** $Log: ClsFEDiagramManager.h,v $
 ** Revision 1.2  2003/02/22 22:49:49  ulysses
 ** *** empty log message ***
 **
 ** Revision 1.1  2003/02/08 00:36:48  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEDIAGRAMMANAGER_H
#define CLSFEDIAGRAMMANAGER_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <list>
#include <string>

#include <qobject.h>
#include <qframe.h>
#include <qwidget.h>
#include <qmainwindow.h>

#include "ClsBlockDiagram.h"


class ClsFEDiagramManager: public QObject {
    Q_OBJECT
public:


    static ClsFEDiagramManager* Instance();
    static void initializeDiagramManager(QWidget* _toplevel);

    ClsInfoDiagramIcon getDiagramIcon(string strID);
    ClsInfoDiagramLine getDiagramLine(string strID);

    void closeSystem();
    void printBlockDiagram();
    void saveBlockDiagram();
    const map<string, int> getListSelectedItems(){return clsBlockDiagram->getListSelectedItems();};
    void markItem(int iType, string strID);


    signals:
    void sigItemActivated(int,string);    


public slots:

void slotSystemChanged();
    void slotItemChanged(int iType, string strID );
    void slotItemDeleted(int iType, string strID );
    void slotItemUnDeleted(int iType, string strID );
    void slotItemAdded(int iType, string strID );
    void slotItemDuplicated(int iType, string strID );

    void slotSimulationRunning(bool);
    void DiagramCreate(int iClientType);

    void DiagramCreate(int iClientType , string strID);
    void DiagramShow(int iClientType ,  string strID);
    void slotBlockDiagramShow(string);
    void slotDiagramClosed(string);

    void saveConfig();
    void applyConfig();

private slots:
    void slotBlockDiagramLeftClicked(int ,string,  QPoint);
    void slotBlockDiagramRightClicked(int ,string,  QPoint);
    void slotBlockDiagramRightClicked(int ,list<string>,  QPoint);
    void slotBlockDiagramDoubleClicked(int i,string s,  QPoint p);
    void slotItemActivated(int, string);
    

private:
    ClsFEDiagramManager(QWidget* _toplevel);
    static ClsFEDiagramManager* _instanceDiagramManager;

    QWidget* toplevel;

    ClsBlockDiagram *clsBlockDiagram;
    
    QMap<QString, QWidget*> qmapDiagrams;
//     map<string, pair<QWidget*, ClsFEDataClient*> > mapPlots;

    /* containers
       - pointer to ClsQBlockdiagram
       - map of ClsQConnViews
    */


};

#endif /* CLSFEDIAGRAMMANAGER_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
