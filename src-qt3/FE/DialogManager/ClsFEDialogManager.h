/****************************************************************************
 ** $Filename: ClsFEDialogManager.h
 ** $Id: ClsFEDialogManager.h,v 1.4 2005/10/29 20:18:23 ulysses Exp ulysses $
 **
 ** Copyright (C) 2002 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Feb  5 13:40:26 2002
 ** $Date: 2005/10/29 20:18:23 $
 **
 ** $Log: ClsFEDialogManager.h,v $
 ** Revision 1.4  2005/10/29 20:18:23  ulysses
 ** *** empty log message ***
 **
 ** Revision 1.3  2003/04/29 11:59:38  ulysses
 ** before using the mediator
 **
 ** Revision 1.2  2003/03/11 09:57:28  ulysses
 ** before implementing property dialogs
 **
 ** Revision 1.1  2002/02/20 13:52:00  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSFEPOPUPMANAGER_H
#define CLSFEPOPUPMANAGER_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include <list>
#include <qpopupmenu.h>
#include <qpoint.h>
#include <qwidget.h>
#include <qmap.h> 

#include "idGenerator.hpp"

class iqrMainWindow;

namespace iqrfe {
    class ClsDialog;
}


using namespace std;
using namespace iqrfe;


class ClsFEDialogManager : public QWidget {
     Q_OBJECT

public:

    static ClsFEDialogManager* Instance();

    static void initializeDialogManager(iqrMainWindow* _parent);


    void closeAllDialogs();

    signals:
     void sigSubTypesChangable(bool);
    
     
public slots:
     void createPopup(int, string, QPoint);
     void createPopup(int, list<string>, QPoint);
    int createPropertyDialog(int, string, QPoint);
    int createSettingsDialog();
    void slotSimulationRunning(bool);
    string createSaveDialog(string strFileTypeDescription, string strExtension);
    void createInfoTip(int, string, QPoint);
    void createAboutDialog();
    void createReleaseNotesDialog();

private slots:
    void slotItemChanged(int iType, string strID);
    void slotItemDeleted(int iType, string strID );
    void slotDialogClosed(string);

    
     
private:


    ClsFEDialogManager(iqrMainWindow* _parent);
    static ClsFEDialogManager* _instanceDialogManager;

     void createSystemPopup(string strID, QPoint qpLocation);
     void createProcessPopup(string strID, QPoint qpLocation);
     void createGroupPopup(string strID, QPoint qpLocation);
    void createConnectionPopup(string strID, QPoint qpLocation);
    void createMultiGroupPopup(list<string> listIDs, QPoint qpLocation);
//    void createConnectionPopup(list<string> listIDs, QPoint qpLocation);


    void createProcessInfoTip(string strID, QPoint qpLocation);
    void createGroupInfoTip(string strID, QPoint qpLocation);
    void createConnectionInfoTip(string strID, QPoint qpLocation);




    int createSystemPropertyDialog(string strID, QPoint qpLocation);
//--    int createProcessPropertyDialog_OLD(string strID, QPoint qpLocation);
    int createProcessPropertyDialog(string strID, QPoint qpLocation);
    int createGroupPropertyDialog(string strID, QPoint qpLocation);
    int createConnectionPropertyDialog(string strID, QPoint qpLocation);
    int createConnectionArborizationPropertyDialog(string strID, QPoint qpLocation);
    

    void createItemDeleteDialog( int iType, string strID);
    
    QMap<string, ClsDialog*> qmapAutoDialogs;
    bool bSubTypesChangable;

    iqrMainWindow *parent;

};

#endif /* CLSFEPOPUPMANAGER_H */

//// Local Variables: 
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End: 

