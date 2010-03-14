#ifndef WGRAPHLEGEND_HPP
#define WGRAPHLEGEND_HPP

#include <QStackedWidget>
#include <QWidget>
#include <QCheckBox>  
#include <QtXml/QDomElement>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>


class WGraphLegend : public QWidget {
  Q_OBJECT
  friend class WGraphTrace;
public:
  
  WGraphLegend(QWidget *parent);
  
  void writeXML(QDomElement &);
  void readXML(QDomElement &);
  
  bool isExclusive() const { return exclButton->isChecked(); }

  void addCheckBox(QCheckBox *qcb);
  void addRadioButton(QRadioButton *qrb);

public slots:

  void toggleExclusive(bool );
  
  
signals:

  void changed();

protected slots:

  void watchButtons(int ) { emit changed(); }

protected:

  QCheckBox *exclButton;
  QStackedWidget *legendStack;

  QButtonGroup* qbtngrpCheck;
  QGroupBox* qgrpbxCheck;
  QHBoxLayout *qlayoutCheck;


  QButtonGroup* qbtngrpRadio;
  QGroupBox* qgrpbxRadio;
  QHBoxLayout *qlayoutRadio;


  QVBoxLayout *layout;
};

#endif//WGRAPHLEGEND_HPP
