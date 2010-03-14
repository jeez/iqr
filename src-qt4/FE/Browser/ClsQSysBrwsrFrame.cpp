#include "ClsQSysBrwsrFrame.h"
#include "ClsQSystemBrowser.h"

namespace {
#include "clear_14x11.xpm"
}

ClsQSysBrwsrFrame::ClsQSysBrwsrFrame(QSplitter *_qsplitter, ClsQSystemBrowser *_browser): 
    QFrame(_qsplitter), browser(_browser) {
    QVBoxLayout* qlyt = new QVBoxLayout(this);
    qlyt->addWidget(browser);


    QHBoxLayout* qlytFilter = new QHBoxLayout();
    QLabel *lbl = new QLabel();
    lbl->setText("Filter");
    qlytFilter->addWidget(lbl);
    pqledValueWidget = new QLineEdit();
    qlytFilter->addWidget(pqledValueWidget);
    qpbClear = new QPushButton(QIconSet(QPixmap(clear)),"");
    qpbClear->setFlat(TRUE);
    qlytFilter->addWidget(qpbClear);
    
    qlyt->addLayout(qlytFilter);
    
    connect( pqledValueWidget, SIGNAL(returnPressed()), this, SLOT(filterEdited()));
    connect( qpbClear, SIGNAL(pressed()), this, SLOT(clearFilter()));

}


void ClsQSysBrwsrFrame::filterEdited(){
   browser->setFilter(pqledValueWidget->text());
}


void ClsQSysBrwsrFrame::clearFilter(){
    pqledValueWidget->setText("");
    browser->removeFilter();
    
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
 
