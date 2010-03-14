#ifndef WGRAPHLEGEND_HPP
#define WGRAPHLEGEND_HPP

#include <qwidget.h>
#include <qcheckbox.h>  
#include <qdom.h>

class QWidgetStack;
class QVButtonGroup;
class QVBoxLayout;

class WGraphLegend : public QWidget {
  Q_OBJECT
  friend class WGraphTrace;
public:
  
  WGraphLegend(QWidget *parent);
  
  void writeXML(QDomElement &);
  void readXML(QDomElement &);
  
  bool isExclusive() const { return exclButton->isChecked(); }
  
public slots:

  void toggleExclusive(bool );
  
signals:

  void changed();

protected slots:

  void watchButtons(int ) { emit changed(); }

protected:

  QCheckBox *exclButton;
  QWidgetStack *legendStack;
  QVButtonGroup *checkGroup, *radioGroup;
  QVBoxLayout *layout;
};

#endif//WGRAPHLEGEND_HPP
