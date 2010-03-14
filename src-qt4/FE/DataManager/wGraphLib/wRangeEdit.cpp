#include "wRangeEdit.h"

#include <QHBoxLayout>
#include <QLabel>


WRangeEdit::WRangeEdit (QWidget *parent) : QWidget(parent) /* QWidgetAction(parent)*/ {
    
    QHBoxLayout *topLayout = new QHBoxLayout(this/*,3*/);
    topLayout->addWidget(new QLabel("range:",this));
    topLayout->addWidget(minValue = new WDoubleEdit(this));
    topLayout->addWidget(new QLabel("-",this));
    topLayout->addWidget(maxValue = new WDoubleEdit(this));
    setEnvelope(WRange(-FLT_MAX,FLT_MAX));
 
   
    connect(minValue,SIGNAL(returnPressed()), this, SLOT(catchReturnPressed()));
    connect(maxValue,SIGNAL(returnPressed()), this, SLOT(catchReturnPressed()));
  }

void WRangeEdit::setRange(WRange r) {
  
  minValue->blockSignals(true);
  maxValue->blockSignals(true);
  minValue->setRange(WRange(envelope.min(),r.max()));
  maxValue->setRange(WRange(r.min(),envelope.max()));
  minValue->setValue(r.min());
  maxValue->setValue(r.max());
  minValue->blockSignals(false);
  maxValue->blockSignals(false);
}

WRange WRangeEdit::getRange() {
  return WRange(minValue->getValue(),maxValue->getValue());
}

void WRangeEdit::catchReturnPressed() {
  emit rangeChanged(getRange());
  parentWidget()->close();
}
