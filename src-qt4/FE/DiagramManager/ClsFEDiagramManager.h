/****************************************************************************
 ** $Filename: ClsFEDiagramManager.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jan 23 15:31:21 2003
 **
 *****************************************************************************/


#ifndef CLSFEDIAGRAMMANAGER_H
#define CLSFEDIAGRAMMANAGER_H    /*+ To stop multiple inclusions. +*/

#include <map>
#include <list>
#include <string>

#include <QObject>
#include <QFrame>
#include <QWidget>
#include <QMainWindow>

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
    const map<string, int> getListSelectedItems(){return ClsBlockDiagram::Instance()->getListSelectedItems();};
    void markItem(int iType, string strID);


    signals:
    void sigDiagItemActivated(int,string);    
    void sigDiagViewActivated(int,string);    


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

    

private:
    ClsFEDiagramManager(QWidget* _toplevel);
    static ClsFEDiagramManager* _instanceDiagramManager;

    QWidget* toplevel;
    QMap<QString, QWidget*> qmapDiagrams;


};

#endif /* CLSFEDIAGRAMMANAGER_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:


//
