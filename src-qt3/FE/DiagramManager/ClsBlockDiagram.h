/* 
   2 dos:
   - add remove groups/nodes by external entities
   - serialize 

*/

#ifndef CLSQBLOCKDIAGRAM_H
#define CLSQBLOCKDIAGRAM_H

#include <string>
#include <queue>

#include <qlabel.h>
#include <qcanvas.h>
#include <qframe.h>
#include <qintdict.h>
#include <qlayout.h> 
//#include <qmainwindow.h>
#include <qmap.h>
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qtabbar.h> 
#include <qmenubar.h> 

#include <qtoolbutton.h> 
#include <qpushbutton.h> 

#include "ClsInfoConnection.h"
#include "ClsInfoDiagramIcon.h"
#include "ClsInfoDiagramLine.h"

#include "ClsQCanvas.h"
#include "ClsQDiagramView.h"


#include <ClsFEDialogManager.h>

#include "ClsStorage.h"

using namespace std;
using namespace iqrcommon;

class ClsCanvasNode;
class ClsCanvasNodePhantom;

class ClsCanvasNodeAP;
class ClsCanvasConnection;
class ClsCanvasMetaConnection;


#define DEFAULT_NODE_WIDTH 50
#define DEFAULT_NODE_HEIGHT 50

class ClsBlockDiagram : public QFrame {
    Q_OBJECT

public:
    ClsBlockDiagram( QWidget* parent=0);
    ~ClsBlockDiagram( );

    ClsQCanvas *getCanvas(QString qstr);

    ClsInfoDiagramIcon getDiagramIcon(string strID);
    ClsInfoDiagramLine getDiagramLine(string strID);

    string getNewConnectionProcessID();
    void showProcessTab(string strProcessID);
    const map<string, int> getListSelectedItems();
    void markItem(int iType, string strID);						 

    void disableToolbarButtons(bool b);

public slots:
void clear();
    void init();
//    void enlarge(QCanvas*, int, int);
    void shrink();
    void print();
    void save();
    void slotSystemChanged();

    void slotItemAdded(int iType, string strID );
    void slotItemDuplicated(int iType, string strID );
    void slotItemChanged(int iType, string strID );
    void slotItemDeleted(int iType, string strID );
    void slotItemUnDeleted(int iType, string strID );

    void slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType, int iCanvasConnectionType);

    ClsCanvasNode* getCanvasNode( string strID);
    

private slots:
    void slotDiagramViewLeftClicked(int ,string,  QPoint);
    void slotDiagramViewRightClicked(int ,string,  QPoint);
    void slotDiagramViewRightClicked(int ,list<string>,  QPoint);
    void slotDiagramViewDoubleClicked(int i,string s,  QPoint p);

    void slotAddNode();
    void slotAddConnectionExcitatory();
    void slotAddConnectionInhibitory();
    void slotAddConnectionModulatory();
    
    void  createGroup(string strPrcID, int iX, int iY);
    void  createProcess(string strID, int iX, int iY);
    
    void slotNewConnection(ClsCanvasNodeAP*, ClsCanvasNodeAP*, int, vector<vector<int> >);

    void slotNewConnectionStarted(ClsCanvasNodeAP* clsCanvasNodeAPSource);
    void slotNewConnectionFinished(ClsCanvasNodeAP* clsCanvasNodeAPTarget, int iConnectionType);
    void slotNewConnectionAborted();
    
    void slotItemInfoMode();
    void slotSplitHorizontal();
    void slotSplitVertical();
    void slotNoSplit();


    void zoomIn();
    void zoomOut();

    void slotDiagramViewGotFocus_1(int,string);
    void slotDiagramViewGotFocus_2(int,string);
    void slotItemActivated(int,string);


    signals:
    void blockdiagramLeftClick(int ,string,  QPoint);
    void blockdiagramRightClick(int ,string,  QPoint);
    void blockdiagramRightClick(int ,list<string>,  QPoint);
    void blockdiagramDoubleClick(int ,string,  QPoint);
//--    void sigCanvasItemDeleted(QCanvasItem*);

    void sigAbortNewConnection();
    void sigItemActivated(int,string);
    

private:

    void setRoot(string strID, string strName);
    void addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon);
    void addConnection(string strConnectionID, string strConnectionName, string strSourceID, string strTargetID, 
		       int iConnectionType, ClsInfoDiagramLine clsInfoDiagramLine);
    void addConnection(ClsInfoConnection clsInfoConnection);
    

    void drawConnection(ClsCanvasConnection *clsCanvasConnection, 
			ClsCanvasNode* clsCanvasNodeStart, ClsCanvasNode* clsCanvasNodeEnd, vector<vector<int> > vectorPoints);
    void drawConnection(ClsCanvasConnection *clsCanvasConnection, 
			ClsCanvasNodeAP* clsCanvasNodeAPSource, ClsCanvasNodeAP* clsCanvasNodeAPTarget, vector<vector<int> > vectorPoints);

//    void drawPhantomConnection(ClsCanvasNode* clsCanvasNodeStart, ClsCanvasNode* clsCanvasNodeEnd);
    

    void addGroup(string strPrcID, string strGroupID, string strGroupName, ClsInfoDiagramIcon clsInfoDiagramIcon);

    ClsCanvasNodePhantom* getCanvasNodePhantom(QCanvas* qcanvasTemp, string strPhantomGroupID, string strID, string strGroupName, ClsCanvasNode* clsCanvasNodeNeighbour);
    ClsCanvasConnection* createCanvasConnectionPhantom(int iType, string strProcessID, string strConnectionID, string strSourceID, string strTargetID, int iConnectionType);

    void createMetaConnection(ClsCanvasConnection* clsCanvasConnection, string strSourceProcessID, 
			      string strTargetProcessID,
			      string strConnectionID,
			      string strSourceID, string strTargetID,
			      int iConnectionType );

    void local2IPConnection(string strConnectionID, int iEndType);
    void IP2localConnection(string strConnectionID, int iEndType, string strOldGroupProcessID);
    void rerouteMetaConnection(string strConnectionID, int iType, int iCanvasConnectionType, string strOldGroupProcessID);
    void reconnectIPConnection(string strConnectionID, int iEndType);
    void cleanupPhantomGroupAPs();
    void prunePhantomGroups();

    void deleteProcess(string strID);
    void deleteGroup(string strID);
    void deleteConnection(string strID);
    void deleteMetaConnection(string strID);

    /* delete helper */
    void deletePhantomGroups(string strID);
    
//    void resizeEvent ( QResizeEvent * e );

    void setClsCanvasNodeGroupIcon (string strGroupID, int iIconType, bool b);    
    void setGroupModuleIOIcons(string strProcessID);
    void resetGroupModuleIOIcons(string strProcessID);
    void resetGroupSynapseInputIcons(string strProcessID);


    ClsCanvasNodePhantom* getPhantomNodeByGroupID(string strGroupID);
    ClsCanvasNodePhantom* getPhantomNodeByKey(string strKey);

    QRect findRect2save(QCanvas *qcanvasTemp);
    

    QBoxLayout * qboxlayoutMain;
//    QMenuBar* qmenuBar;
    QFrame *qframeToolbar;
    

    ClsQDiagramView *clsQDiagramView;
    ClsQDiagramView *clsQDiagramView0;
    ClsQDiagramView *clsQDiagramViewActive;
    
    QPrinter* printer;

    QPushButton *qpbtnZoomIn;
    QPushButton *qpbtnZoomOut;
    QPushButton *qpbtnNewProcess;
    QPushButton *qpbtnNewGroup;
    QPushButton *qpbtnNewConnectionExcitatory;
    QPushButton *qpbtnNewConnectionModulatory;
    QPushButton *qpbtnNewConnectionInhibitory;
    QPushButton *qpbtnItemInfo;


    QPushButton *qpbtnSplitVertical;
    QPushButton *qpbtnSplitHorizontal;
    QPushButton *qpbtnNoSplit;


    QMap<string, ClsCanvasNode*> qmapNodes;
    QMap<string, ClsCanvasConnection*> qmapConnections;


    QMap<string, ClsCanvasNodePhantom*> qmapPhantomNodes;
//    QMap<string, ClsCanvasConnection*> qmapPhantomConnections;
    QMap<string, ClsCanvasMetaConnection*> qmapMetaConnections;

    /* containers for deleted objects */
    queue<ClsCanvasNode*> queueDeletedNodes;
    queue<ClsStorage<ClsCanvasNode*, ClsCanvasNode*> > queueDeletedProcesses;
/* not sure I need to store these...
   queue< ClsCanvasNodePhantom*> queueDeletedPhantomNode;
*/


    QMap<QString, ClsQCanvas*> qmapCanvases;
    QSplitter *qsplitter;

/*
  int iMenuIdAddConnectionExcitatory;
  int iMenuIdAddConnectionInhibitory;
  int iMenuIdAddConnectionModulatory;
*/

    int iCurrentActivItemType;
    bool bSimulationRunning;
    ClsCanvasNodeAP *clsCanvasNodeNewConnectionAPSource;
    QLabel *qlbl;
    
};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
