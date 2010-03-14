#ifndef CLSQDIAGRAMVIEW_H
#define CLSQDIAGRAMVIEW_H

#include <iostream>
#include <list>


#include <QGraphicsScene>

//--#include <QMap>
#include <QMatrix> 
#include <QCursor> 
#include <QTabBar> 
#include <QLayout> 
#include <QResizeEvent>
#include <QBoxLayout>
#include <QFrame>

#include "ClsScene.h"
#include "ClsSceneView.h"

#include "ClsPanner.h"

using namespace std;

class ClsBlockDiagram;


class ClsSubDiagram : public QFrame {
    Q_OBJECT

public:
    ClsSubDiagram(QFrame *parent, ClsBlockDiagram* clsBlockDiagramParent, int iID, int i);

    void addTab(int iType, string strName, string strID) ;
    void removeTab( string strID);
    void renameTab( string strID, string strName);
    
    void setItemInfoMode(bool b); //?????????????
    
    void useSpacer(bool b);
    QGraphicsScene *scene();
    void setScene(ClsScene *q);
    
    string getNewConnectionProcessID();
    void showProcessTab(string strProcessID);
    

    const map<string, int> getListSelectedItems();
    void setAddState(int iState);

    void mark(bool);


public slots:
    void slotTabChanged(int);
    void zoomIn();
    void zoomOut();
    void clear();


 private slots:
    void slotGotFocus(int,string);
    void resizeEvent ( QResizeEvent * e );
    void pannerScroll(int, int);
    
signals:
    void status(const QString&);
    void canvasChanged(int,string);
    void sigGotFocus(int, int, string);
    void sigDiagItemActivated(int,string);

private:


    void paintEvent ( QPaintEvent * event );


    void createCursors();
    void setCursor(int iAddState, int iCanvasType);

    QCursor *qcursorNewGroup;
    QCursor *qcursorNewProcess;
    QCursor *qcursorNewConnection;

    ClsBlockDiagram *clsBlockDiagramParent;
    ClsSceneView *clsSceneView;

    QGraphicsView *qgraphicsviewMini;
    
    
    QBoxLayout * qboxlayout;
    QWidget *qwgtSpacer;  /* this widget has no function whatsoever. It's only purpose is to ensure the TabBar soesen't shrink to 0! */
    int iSpacerHeight;

    QTabBar *qtabbar;
//--    QMap<int, string> qmapTabIdentifyers;

    ClsPanner *clsclsScenepanner;

    int iAddState;
    int iID;
    bool bFocus;
    
};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:

