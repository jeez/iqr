#ifndef WRANGEEDIT_HPP
#define WRANGEEDIT_HPP

#include "wRange.h"

#include <qlineedit.h>
#include <qvalidator.h>

class WDoubleValidator : public QDoubleValidator {
 public:
  WDoubleValidator(QWidget *parent, const char *name=0) 
    : QDoubleValidator(parent,name) {}
  
  void fixup(QString &input) const {
    if (input.toFloat() < bottom()) input.setNum(bottom());
    if (input.toFloat() > top()) input.setNum(top());
  }
};

class WDoubleEdit : public QLineEdit {
  Q_OBJECT
public:
  WDoubleEdit(QWidget *parent, const char *name=0) 
    : QLineEdit(parent,name) { 
      setValidator(valid = new WDoubleValidator(this)); 
      setFixedWidth(60);
    }
  
  void setRange(float min, float max) { valid->setRange(min,max,5); }
  void setRange(WRange r) { valid->setRange(r.min(),r.max(),5); }
  
  virtual void setValue(float v) {
    validateAndSet((const char*)QString::number(v,'g',5),0,0,0);
    emit valueChanged(v);
  }
  
  float getValue() { return text().toFloat(); }
  
signals:

  void valueChanged(float);
  
 protected:
  
  WDoubleValidator *valid;  
};

class WRangeEdit : public QWidget {
  Q_OBJECT
public:
  WRangeEdit(QWidget *parent);
  
  void setEnvelope(WRange e) { envelope=e; setRange(getRange()); }
  WRange getRange();
  
  void clear() {
    minValue->clear();
    maxValue->clear();
  }

signals:
  void rangeChanged(WRange);

public slots:

  void catchReturnPressed();
  void setRange(WRange );

protected:
  WRange envelope;
  WDoubleEdit *minValue, *maxValue;
};

#endif // WRANGEEDIT_HPP
