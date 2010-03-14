#ifndef CLSQDIAGRAMVIEW_H
#define CLSQDIAGRAMVIEW_H

#include <iostream>
#include <list>

#include <qmap.h>
#include <qcanvas.h>
#include <qwmatrix.h> 
#include <qcursor.h> 
#include <qtabbar.h> 
#include <qlayout.h> 

#include "ClsQCanvas.h"
#include "ClsQDiagramCanvasView.h"

#define USEPANNER

#ifdef USEPANNER
#include "ClsQCanvasPanner.h"
#endif

using namespace std;

class ClsBlockDiagram;


class ClsQDiagramView : public QFrame {
    Q_OBJECT

public:
    ClsQDiagramView(QFrame *parent, ClsBlockDiagram* clsBlockDiagramParent,const char* name, int i);

    void setAddState(int _iAddState) ;
    void addTab(int iType, string strName, string strID) ;
    void removeTab( string strID);
    void renameTab( string strID, string strName);
    
    void setItemInfoMode(bool b);
    
    void useSpacer(bool b);
    QCanvas *canvas();
    void setCanvas(ClsQCanvas *q);
    
    string getNewConnectionProcessID();
    void showProcessTab(string strProcessID);
    

//XXX     bool hasFocus() { 
//XXX 	cout << "ClsQDiagramView:hasFocus()" << endl;
//XXX 	return bFocus; 
//XXX     };
    
    void setFocus(bool b);
    const map<string, int> getListSelectedItems(){return clsQDiagramCanvasView->getListSelectedItems();};


public slots:
    void slotTabChanged(int);
    void zoomIn();
    void zoomOut();
    void clear();
//--    void slotCanvasItemDeleted(QCanvasItem*);

 private slots:
 void slotDiagramCanvasViewGotFocus(int,string);
     void resizeEvent ( QResizeEvent * e );
    
signals:
    void diagramViewRightClick(int ,string,  QPoint);
    void diagramViewRightClick(int ,list<string>,  QPoint);

    void status(const QString&);
    void canvasChanged(int,string);
    void sigGotFocus(int,string);

private:


    ClsBlockDiagram *clsBlockDiagramParent;
    int iAddState;
    ClsQDiagramCanvasView *clsQDiagramCanvasView;

    QCanvasView *qcanvasviewMini;
    
    
    QBoxLayout * qboxlayout;
    QWidget *qwgtSpacer;  /* this widget has no function whatsoever. It's only purpose is to ensure the TabBar soesen't shrink to 0! */
    int iSpacerHeight;

    QTabBar *qtabbar;
    QMap<int, string> qmapTabIdentifyers;
    bool bFocus;
    
    

#ifdef USEPANNER
    ClsQCanvasPanner *clsqcanvaspanner;
#endif
    
};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
