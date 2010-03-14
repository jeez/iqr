#ifndef CLSQSYSBRWSRFRAME_H
#define CLSQSYSBRWSRFRAME_H

#include <string>
#include <iostream>
#include <queue>

#include <QTreeWidget>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QLineEdit>
#include <QSplitter>
#include <QPushButton>



using namespace std;


class ClsQSystemBrowser;

class ClsQSysBrwsrFrame: public QFrame {
     Q_OBJECT
public:
    ClsQSysBrwsrFrame(QSplitter *_qsplitter, ClsQSystemBrowser *_browser);

private slots:
    void filterEdited();
    void clearFilter();

private:
    ClsQSystemBrowser *browser;
    QLineEdit* pqledValueWidget;
    QPushButton* qpbClear;

};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
 
