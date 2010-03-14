#include "wGraphLegend.h"

#include <qradiobutton.h> 
#include <qvbuttongroup.h>
#include <qwidgetstack.h>
#include <qlayout.h>

//#include "wGraphLegend.moc"

WGraphLegend::WGraphLegend(QWidget *parent)
 : QWidget(parent) {
   
   exclButton=new QCheckBox("exclusive",this);
   legendStack=new QWidgetStack(this);
   checkGroup=new QVButtonGroup("traces", legendStack);
   radioGroup=new QVButtonGroup("traces", legendStack);
   layout=new QVBoxLayout(this,3);
   
   legendStack->addWidget(checkGroup,0);
   legendStack->addWidget(radioGroup,1);

   layout->addStretch(1); layout->addWidget(exclButton);
   layout->addStretch(1); layout->addWidget(legendStack);
   layout->addStretch(1);
   
   legendStack->raiseWidget(0);
   
   connect(exclButton,SIGNAL(toggled(bool)),this,SLOT(toggleExclusive(bool)));
   connect(exclButton,SIGNAL(clicked()),SIGNAL(changed()));
   connect(checkGroup,SIGNAL(clicked(int)),this,SLOT(watchButtons(int)));
   connect(radioGroup,SIGNAL(clicked(int)),this,SLOT(watchButtons(int)));
   
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

  QButtonGroup *group=(on?radioGroup:checkGroup);
  if (!group) return;
  legendStack->raiseWidget(group);

}
