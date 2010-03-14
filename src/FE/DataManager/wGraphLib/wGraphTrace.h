#ifndef WGRAPHTRACE_HPP
#define WGRAPHTRACE_HPP

#include <qstring.h>
#include <qpen.h>
#include <qobject.h>
#include <qdom.h>

#include "wRange.h"

class WGraphLegend;
class QCheckBox;
class QRadioButton;

/*! The WGraphTrace is the structure in which a trace added to a
  WGraphPlot is represented.*/
class WGraphTrace : public QObject {
  Q_OBJECT
public:
  WGraphTrace(WGraphLegend *, const QString& , QPen , int );
  ~WGraphTrace();

  /*! Resizes this trace to \em length.*/
  void resize(int length) { data.fill(0,length); }
  /*! Clears the trace.*/
  void clear() { data.fill(0); }
  /*! Returns true if this trace is visible, false otherwise*/
  bool isVisible() const { return visible; };

  /*! Returns the name of this trace.*/
  const QString &getName() const { return name; };
  /*! Returns the pen for this trace.*/
  const QPen &getPen() const { return pen; };
  /*! Returns the a referennce to the data of this trace. \sa
    WGraphPlot::getTraceData()*/
  QMemArray<float> &getData() { return data; };
  
  /*! Returns the range over which the data of this trace extends.*/
  const WRange &checkRange();
  
  void writeXML(QDomElement &);
  void readXML(QDomElement &);

public slots:

  void toggleExclusive(bool );
  /*! Makes this trace visible in WGraphPlot, if \em vis = true,
    hides it otherwise.*/
  void setVisible(bool );
  
signals:

  void needRefresh();

protected:
  QString name;
  QPen    pen;
  bool    visible;
  
  QCheckBox    *checkButton;
  QRadioButton *radioButton;

  WGraphLegend *legend;

  WRange dataRange;
  QMemArray<float> data;
};

#endif//WGRAPHTRACE_HPP
