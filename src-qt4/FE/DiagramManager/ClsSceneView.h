#ifndef CLSQDIAGRAMCANVASVIEW_H
#define CLSQDIAGRAMCANVASVIEW_H

#include <iostream>
#include <list>
#include <vector>

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QCursor> 
#include <QMatrix> 
#include <QMouseEvent>



using namespace std;

#define MAX_X_CANVAS 20000
#define MAX_Y_CANVAS 20000

class ClsScene;

class ClsSceneView : public QGraphicsView {
    Q_OBJECT

public:
    ClsSceneView(QWidget* parent);
    void clear();
    const map<string, int> getListSelectedItems();

signals:
    void sigGotFocus(int,string);
  
private:
    void mousePressEvent ( QMouseEvent * event);

};


#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
