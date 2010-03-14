#ifndef IQRMAINWINDOW_H
#define IQRMAINWINDOW_H    /*+ To stop multiple inclusions. +*/

#include <qapplication.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmovie.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprogressbar.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qtextview.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qmutex.h>
#include <qwaitcondition.h>
#include <qaccel.h>

#include <string>
#include <fstream>
#include <unistd.h>
#include <list>
#include <deque>

#include "ipcserver.h"

namespace iqrcommon  {
    class ClsSysFileParser;
}


class ClsFESerializer;
class ClsFESystemManager;
class ClsFESystemBuilder;
class ClsBaseDataBroker;
class ClsFECommHandlerLocal;
class ClsQSystemBrowser;



#define COUT cout << "[01;34m"
#define ENDL "[00m" << endl

static const Q_UINT16 ipcPort = 54923;

using namespace std;
using namespace iqrcommon;


class iqrMainWindow: public QMainWindow {
    Q_OBJECT
public:
    iqrMainWindow(QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);

/*
  ~iqrMainWindow();
*/

    void newSystem();
    void openSystem(string s);
    void customEvent( QCustomEvent * e );
    void autoStartSimulation();
    void openConfig(string strConfigFileName);
    void openHarborConfig(string strHarborConfigFileName);
    void copyItemsToClipboard(int iType, list<string> lstIDs);
    void copyItemsToClipboard(list<string> lstGroupIDs,  list<string> lstConnectionIDs);
    void exportProcess(string strID);


signals:
    void sigSimulationRunning(bool);


public slots:
bool slotQuitIQR();

private slots:
    void slotOpenSystem();
    void slotOpenOldFile(int);

    void slotRunControl();

    void slotSaveSystem();
    void slotSaveSystemAs();
    void slotAutoSaveSystem();
    void slotDebugSaveSystem();
    void slotExportSystemForAER();
    void slotImportProcess();
    void slotLinkProcess();
    void slotSaveToBackup();

    void slotSaveDiagram();
    void slotPrintDiagram();


    void slotNewSystem();
    bool slotCloseSystem();

    void slotSettings();
    bool buildSystem();
    void slotSystemBuildt();
    void slotShowLogWindow();
    void advanceProgress();
    void showLoadProgress(string s);
    void slotValidateSystem();

    void slotItemActivated(int,string);

/* dialogs */
    void slotSystemProperties();
    void slotDAVMenu(int iMenuID);
    void slotHelpMenu(int iMenuID);
/* clipboard */


//--    void cutItemsToClipboard(int iType, list<string> lstIDs);
//--    void pasteItemsFromClipboard(int iType, list<string> lstIDs);
    void slotPaste();
    void slotClipboardChanged();

    void slotRemoteCommand(const QString&);

    void slotAccel(int);

private:
    void createAnimation();
    void initLFO(list<string> &);
    void updateLFO(string);
    void closeEvent( QCloseEvent *e);
    void changeState();

    int startSimulation();
    void stopSimulation();

    void setupRCServer();
    void startPdfHelp(int iType);

    void importORLinkProcess(bool);
    bool isValidSystem(bool bConfirm);

    enum PLOT_MENU{
	MENU_SPACE_PLOT,
	MENU_TIME_PLOT,
	MENU_DATA_SAMPLER,
	MENU_PARAMRELAIS
    };

    enum FILE_MENU{
	FILE_OPEN,
	FILE_NEW_SYSTEM,
	FILE_CLOSE_SYSTEM,
	FILE_SAVE_SYSTEM,
	FILE_SAVE_SYSTEM_AS,
	FILE_SYSTEM_PROPERTIES,
	FILE_QUIT,
	FILE_EXPORT_FOR_AER,
	FILE_SAVE_TO_BACKUP,
	FILE_IMPORT_PROCESS,
	FILE_LINK_PROCESS
    };

    enum ABOUT_MENU{
	HELP_ABOUT,
	HELP_RELEASENOTES,
	HELP_MANUAL,
	HELP_USERDEFINED_TYPES
    };

    enum MENUBAR_ITEMS{
	ITEM_PASTE
    };

    enum ACCEL_IDS{
	ACCEL_COPY,
	ACCEL_CUT,
	ACCEL_PASTE,
	ACCEL_DELETE
    };


    string strSystemFileName;
    QWaitCondition qWaitCondition;
    QMutex qmutexSysGUI;
    QTimer* qtimerAutoSave;
    int iActiveItemType;
    int iClipboardType;
    string strActiveItemID;


/* core modules */
    ClsFESerializer *clsFESerializer;
    ClsBaseDataBroker *clsFECommHandler;
    ClsQSystemBrowser *clsQSystemBrowser;





/* GUI related stuff */
    QPopupMenu * file;
    QPopupMenu * edit;
    QPopupMenu * view;

    QToolBar *qtoolbarToolbar;
    QStatusBar *qstatusBar;

    QLabel *qlblStatus;

    QToolButton *qtoolbtnFileSave;
    QProgressBar* progress;
    QLabel* lblStatusBar;
    QSplitter *qsplitter;
    QToolButton *qtoolbtnRunControl;

//    map<int, string> mapLFO;
    deque<string> dequeLFO;

    QAccel *qaccelMainWindow;

    IpcServer *rcServer;


    bool bSimulationRunning;
    friend class ClsFEDialogManager;
};





#endif /* IQRMAINWINDOW_H */

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
