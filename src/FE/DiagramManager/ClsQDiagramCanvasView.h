#ifndef CLSQDIAGRAMCANVASVIEW_H
#define CLSQDIAGRAMCANVASVIEW_H

#include <iostream>
#include <list>
#include <vector>

#include <qcanvas.h>
#include <qcursor.h> 
#include <qwmatrix.h> 



using namespace std;

#define MAX_X_CANVAS 20000
#define MAX_Y_CANVAS 20000

class ClsQCanvas;
class ClsCanvasNodeAP;
class ClsCanvasRubberBand;

class ClsQDiagramCanvasView : public QCanvasView {
    Q_OBJECT

public:
//    ClsQDiagramCanvasView(QCanvas&, QWidget* parent=0, const char* name=0, WFlags f=0);
    ClsQDiagramCanvasView(ClsQCanvas*, QWidget* parent=0, const char* name=0, WFlags f=0);
    /*ClsQDiagramCanvasView::*/ClsQDiagramCanvasView(QWidget* parent,const char* name);
    void clear();

    enum {
	ADD_CONNECTION_EXC = 1,
	ADD_CONNECTION_INH,
	ADD_CONNECTION_MOD,
	ADD_NODE,
    };
    
    void setAddState(int _iAddState);
//    void setCanvasType(int i);
    void setCanvas(QCanvas *q);
    const map<string, int> getListSelectedItems();
    void setItemInfoMode(bool b);


public slots:
//    void slotRMB( QListViewItem* Item, const QPoint & point, int );
    void slotAbortNewConnection();
    void setFocus(bool);
//--    void canvasItemDeleted(QCanvasItem*);

signals:
    void diagramViewLeftClick(int ,string,  QPoint);
    void diagramViewRightClick(int ,string,  QPoint);
    void diagramViewRightClick(int ,list<string>,  QPoint);
    void diagramViewDoubleClick(int ,string,  QPoint);
    void sigAddNewProcess( string, int, int);
    void sigAddNewGroup( string, int, int);
    void sigNewConnection(ClsCanvasNodeAP*, ClsCanvasNodeAP*, int, vector<vector<int> >);
    void sigNewConnectionStarted(ClsCanvasNodeAP* clsCanvasNodeAPSource);
    void sigNewConnectionFinished(ClsCanvasNodeAP* clsCanvasNodeAPSource, int iConnectionType);
    void sigNewConnectionAborted();

    void sigConnectionChanged(string, string, int, int);
    
    
    void sigCanvasEnlarged();
    void status(const QString&);    
    void sigGotFocus(int,string);
    

protected:
    void contentsMousePressEvent(QMouseEvent*);
    void contentsMouseDoubleClickEvent ( QMouseEvent *e);
    void contentsMouseMoveEvent(QMouseEvent*);
    void contentsMouseReleaseEvent ( QMouseEvent * );

private:
    int iAddState;
    int iSelected;
    bool bInfoMode;

    int iCanvasType;
    
    void cancelSelections();
    list<string> findSelections();
    void drawFrame ( QPainter * p );
    void createCursors();
    QColor qcolorFrame;
    

    QCanvasItem* moving;
    QPoint moving_start;
    ClsCanvasNodeAP *clsCanvasNodeAPSource;
    ClsCanvasRubberBand* clsCanvasRubberBand;

    vector<vector<int> > vectorNewConnectionPoints;
    
    list<QCanvasLine*> listNewLines;
    QValueList<QCanvasItem* > lstSelections;

    QCursor *qcursorNewGroup;
    QCursor *qcursorNewProcess;
    QCursor *qcursorNewConnection;
};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
