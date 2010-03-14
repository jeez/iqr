#ifndef CLSIQRGUI_H
#define CLSIQRGUI_H

#include <string>
#include <fstream>
#include <unistd.h>
#include <list>

#include <qapplication.h>
#include <qcanvas.h>
#include <qclipboard.h>
#include <qcstring.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qframe.h>
#include <qimage.h>
#include <qintdict.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qmap.h>
#include <qmenubar.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmovie.h>
#include <qmutex.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qprogressbar.h>
#include <qprogressdialog.h>
#include <qregexp.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qstatusbar.h>
#include <qtabbar.h>
#include <qtextview.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwaitcondition.h>

#include "diagramTypes.h"

class ClsFESystemManager;
class ClsFESystemBuilder;


class ClsBaseDataBroker;
class ClsFECommHandlerLocal;
class ClsQSystemBrowser;

namespace iqrcommon  {
    class ClsSysFileParser;
}


class ClsFESerializer;



#define COUT cout << "[01;34m"
#define ENDL "[00m" << endl

using namespace std;
using namespace iqrcommon;



using namespace std;

class ClsIqrGUI {
    Q_OBJECT

public:
    ClsIqrGUI( QWidget* parent=0, const char* name=0, Qt::WindowFlags f=WType_TopLevel);
    ~ClsIqrGUI();

    void openConfig(string strConfigFileName);
    void autoStartSimulation();

    void newSystem();
    void openSystem(string s);
    void customEvent( QCustomEvent * e );


signals:
    void sigSimulationRunning(bool);


private slots:
    bool buildSystem();
    void advanceProgress();
    void slotDebugSaveSystem();
    void slotNewSystem();
    void slotPrintDiagram();
    void slotRunControl();
    void slotSystemBuildt();
    void slotValidateSystem();

/* opening */
    void slotOpenOldFile(int);
    void slotOpenSystem();


/* saving */    
    void slotSaveSystem();
    void slotSaveSystemAs();
    void slotAutoSaveSystem();
    void slotExportSystemForAER();

    void slotSaveDiagram();

/* close and quit */
    bool slotCloseSystem();
    bool slotQuitIQR();


/* dialogs */
    void slotDAVMenu(int iMenuID);
    void slotItemActivated(int,string);
    void slotShowLogWindow();
    void slotSystemProperties();
    void slotSettings();


/* clipboard */
    void slotClipboardChanged();
    void copyItemsToClipboard(int iType, list<string> lstIDs);
    void cutItemsToClipboard(int iType, list<string> lstIDs);
    void pasteItemsFromClipboard(int iType, list<string> lstIDs);
    void slotPaste();



private:

    int startSimulation();
    void stopSimulation();
    void changeState();

    void createAnimation();
    void createMenubar();
    void createToolbar();

    void closeEvent( QCloseEvent *e);


    enum PLOT_MENU{
	MENU_SPACE_PLOT,
	MENU_TIME_PLOT,
	MENU_DATA_SAMPLER,
    };

    enum FILE_MENU{
	FILE_OPEN,
	FILE_NEW_SYSTEM,
	FILE_CLOSE_SYSTEM,
	FILE_SAVE_SYSTEM,
	FILE_SAVE_SYSTEM_AS,
	FILE_SYSTEM_PROPERTIES,
	FILE_QUIT,
	FILE_EXPORT_FOR_AER
    };

    enum MENUBAR_ITEMS{
	ITEM_PASTE
    };


/* core modules */
    ClsFESerializer *clsFESerializer;
    ClsBaseDataBroker *clsFECommHandler;
    ClsQSystemBrowser *clsQSystemBrowser;




    int iActiveItemType;
    int iClipboardType;
    string strActiveItemID;
    QWaitCondition qWaitCondition;
    QMutex qmutexSysGUI;
    QTimer* qtimerAutoSave;
    string strSystemFileName;



/* GUI related stuff */
    QPopupMenu * file;
    QPopupMenu * edit;
    QPopupMenu * view;

    QSplitter *qsplitter;
    QToolButton *qtoolbtnRunControl;

    QToolBar *qtoolbarToolbar;
    QStatusBar *qstatusBar;

    QLabel *qlblStatus;

    QToolButton *qtoolbtnFileSave;
    QProgressBar* progress;
    QLabel* lblStatusBar;

    friend class ClsFEDialogManager;


};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
