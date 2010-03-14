/* 
   2 dos:
   - add remove groups/nodes by external entities
   - serialize 

*/

#ifndef CLSQBLOCKDIAGRAM_H
#define CLSQBLOCKDIAGRAM_H

#include <string>
#include <deque>

#include <QGraphicsScene>

#include <QBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLayout> 
#include <QMap>
#include <QMultiMap>
#include <QPushButton>
#include <QSplitter>
#include <QGraphicsEllipseItem>

#include "ClsInfoConnection.h"
#include "ClsInfoDiagramIcon.h"
#include "ClsInfoDiagramLine.h"

#include "ClsScene.h"
#include "ClsSubDiagram.h"


#include <ClsFEDialogManager.h>

#include "ClsStorage.h"

using namespace std;
using namespace iqrcommon;

class ClsDiagItem;
class ClsDiagItemPhantom;

class ClsDiagItemAP;
class ClsDiagHyperConnection;


#define DEFAULT_NODE_WIDTH 50
#define DEFAULT_NODE_HEIGHT 50

class ClsBlockDiagram : public QFrame {
    Q_OBJECT

public:

    enum {
	ADD_CONNECTION_EXC = 1,
	ADD_CONNECTION_INH,
	ADD_CONNECTION_MOD,
	ADD_NODE,
    };

    enum {
	DIAG_00,
	DIAG_01,
    };


    static ClsBlockDiagram* Instance();
    static void initializeBlockDiagram(QWidget* _toplevel);


    ~ClsBlockDiagram( );

    void cleanup();


    ClsScene* getCanvas(QString qstr);
    ClsScene* getSystemCanvas();


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
    void print();
    void save();
    void slotSystemChanged();

    void slotItemAdded(int iType, string strID );
    void slotItemDuplicated(int iType, string strID );
    void slotItemChanged(int iType, string strID );
    void slotItemDeleted(int iType, string strID );
    void slotItemUnDeleted(int iType, string strID );

    void slotConnectionChanged(string strConnectionID, string strGroupID, int iEndType, int iCanvasConnectionType);

    ClsDiagItem* getCanvasNode( string strID);


/* NEW STUFF */
    void mouseLeftClickAP(ClsDiagItemAP* ap);
    void mouseLeftClickPane(string strSceneID, QPointF p);
    void mouseRightClickPane(string strSceneID, QPointF p);
    bool infoMode();
    void resetAddState();
    ClsDiagHyperConnection* getCanvasHyperConnection(string strID);
							




private slots:
    void slotAddNode(bool);
    void slotAddConnectionExcitatory(bool);
    void slotAddConnectionInhibitory(bool);
    void slotAddConnectionModulatory(bool);
    
    void  createGroup(string strPrcID, int iX, int iY);
    void  createProcess(string strID, int iX, int iY);
    
    void slotSplitHorizontal();
    void slotSplitVertical();
    void slotNoSplit();

    void zoomIn();
    void zoomOut();

    void slotDiagViewGotFocus(int,int,string);
    void slotDiagViewActivated(int,string);



signals:
    void sigAbortNewConnection();
    void sigDiagItemActivated(int,string);
    void sigDiagViewActivated(int,string);
    

private:
    ClsBlockDiagram( QWidget* parent=0);
    static ClsBlockDiagram* _instanceBlockDiagram;


    void setRoot(string strID, string strName);
    void addProcess(string strPrcID, string strPrcName, ClsInfoDiagramIcon clsInfoDiagramIcon);
    void addConnection(string strConnectionID, ClsInfoDiagramLine clsInfoDiagramLine);
    


    void addGroup(string strPrcID, string strGroupID, string strGroupName, ClsInfoDiagramIcon clsInfoDiagramIcon);

    ClsDiagItemPhantom* getDiagItemPhantom(QGraphicsScene* clsSceneTemp, string strID, ClsDiagItem* clsDiagItemNeighbour);

    void prunePhantomGroups();

    void deleteProcess(string strID);
    void deleteGroup(string strID);
    void deleteConnection(string strID);

    /* delete helper */
    void deletePhantomGroups(string strID);
    

    void setClsDiagItemGroupIcon (string strGroupID, int iIconType, bool b);    
    void setGroupModuleIOIcons(string strProcessID);
    void resetGroupModuleIOIcons(string strProcessID);
    void resetGroupSynapseInputIcons(string strProcessID);

    QBoxLayout * qboxlayoutMain;
    QFrame *qframeToolbar;
    

    ClsSubDiagram *clsQDiagramView00;
    ClsSubDiagram *clsQDiagramView01;
    ClsSubDiagram *clsQDiagramViewActive;
    
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


    QMap<string, ClsDiagItem*> qmapNodes;


    QMultiMap<string, ClsDiagItemPhantom*> qmapPhantomNodes;
    QMap<string, ClsDiagHyperConnection*> qmapHyperConnections;

    QMap<QString, ClsScene*> qmapCanvases;
    QSplitter *qsplitter;


    int iCurrentActivItemType;
    bool bSimulationRunning;
    ClsDiagItemAP *clsDiagItemAPSource;

//--    QLabel *qlbl;
    int iAddState;

    deque<pair<string, QPointF> > queueConnectionPoints;
    deque<QGraphicsEllipseItem*> queueConnMarkers;

    friend class ClsDiagHyperConnection;
};


#endif



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
