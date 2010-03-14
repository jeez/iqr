#ifndef WBARPLOT_H
#define WBARPLOT_H

#include "wGraphPlot.h"



/*! The WBarPlot class inherits WGraphPlot and draws the traces as
  bars.*/
class WBarPlot : public WGraphPlot {
  Q_OBJECT
    
public:
  /*! Constructs a WBarPlot and passes all the arguments to WGraphPlot.*/
  WBarPlot(QWidget *, QWidget * =0,
	   WGraphXAxis::Position =WGraphXAxis::bottom,
	   WGraphYAxis::Position =WGraphYAxis::left,
	   int=0);
  ~WBarPlot();

  virtual void writeXML(QDomElement &);
  virtual void readXML(QDomElement &);

  /*! Sets the origin of the bars within the plot to \em v. This has
    no effect, if the bar origin is not enabled. \sa
    setOrginiEnabled()*/
  void setBarQrigin(float v) { barOrigin=v; updateGraph(); }
  /*! Enables the bar origin, if \em origin = true, disables it
    otherwise. Enabled means, that all bars will originate from the
    value set by setBarOrigin. So, if for example the origin was set
    to zero, bars will raise/descend from zero for positive/negative
    values respectively.*/
  void setOriginEnabled(bool origin) { originEnabled=origin; updateGraph(); };
  /*! Sets the gap between bars to \em gap pixels. */
  void setBarGap(int gap) { barGap=gap; updateGraph(); };

  void setXDataRange(WRange );

public slots:

  void checkRange();
  
protected:

  virtual void drawContent(QPainter & , bool);

  float barOrigin;
  bool  originEnabled;
  int   barGap;
};


#endif // WBARPLOT_H
