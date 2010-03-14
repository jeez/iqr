#include "wGraphLegend.h"

#include <QRadioButton> 
#include <QLayout>
#include <QVBoxLayout>


WGraphLegend::WGraphLegend(QWidget *parent) : QWidget(parent) {
   
    exclButton=new QCheckBox("exclusive",this);
    legendStack=new QStackedWidget(this);

    qgrpbxCheck=new QGroupBox("traces", legendStack);
    qlayoutCheck = new QHBoxLayout();
    qgrpbxCheck->setLayout(qlayoutCheck);

    qgrpbxRadio=new QGroupBox("traces", legendStack);
    qlayoutRadio = new QHBoxLayout();
    qgrpbxRadio->setLayout(qlayoutRadio);


    layout=new QVBoxLayout(this);
   
    legendStack->addWidget(qgrpbxCheck);
    legendStack->addWidget(qgrpbxRadio);

//   layout->addStretch(1); 
    layout->addWidget(exclButton);
//   layout->addStretch(1); 
    layout->addWidget(legendStack);
//   layout->addStretch(1);
   
    legendStack->setCurrentIndex(0);
    legendStack->setCurrentWidget(qgrpbxCheck);
   
    connect(exclButton,SIGNAL(toggled(bool)),this,SLOT(toggleExclusive(bool)));
    connect(exclButton,SIGNAL(clicked()),SIGNAL(changed()));

    qbtngrpCheck = new QButtonGroup();
    connect(qbtngrpCheck,SIGNAL(buttonClicked(int)),this,SLOT(watchButtons(int)));
    
    qbtngrpRadio = new QButtonGroup();
    connect(qbtngrpCheck,SIGNAL(buttonClicked(int)),this,SLOT(watchButtons(int)));
   
}

void WGraphLegend::writeXML(QDomElement &elem) {
    elem.setAttribute("exclusive",exclButton->isChecked()?"true":"false");
}

void WGraphLegend::readXML(QDomElement &elem) {
    exclButton->setChecked(elem.attribute("exclusive")=="true");
}

void WGraphLegend::toggleExclusive(bool on) {

    exclButton->blockSignals(true);
    exclButton->setChecked(on);
    exclButton->blockSignals(false);

    QGroupBox *group=(on?qgrpbxRadio:qgrpbxCheck);
    if (!group) {
	return;
    }
    legendStack->setCurrentIndex(group);
    legendStack->setCurrentWidget(group);

}



void WGraphLegend::addCheckBox(QCheckBox *qcb){
    if(qcb!=NULL){
	qlayoutCheck->addWidget(qcb);
	qbtngrpCheck->addButton(qcb);
    }
};

void WGraphLegend::addRadioButton(QRadioButton *qrb){
    if(qrb!=NULL){
	qlayoutRadio->addWidget(qrb);
	qbtngrpRadio->addButton(qrb);
    }
};


