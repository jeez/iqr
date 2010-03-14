#ifndef WLINEPLOT_HPP
#define WLINEPLOT_HPP

#include "wGraphPlot.h"

/*! The WLinePlot class inherits WGraphPlot and draws the traces as
  lines. There are two different modes, one for drawing sequences, one
  for drawing functions.*/
class WLinePlot : public WGraphPlot {
  Q_OBJECT
public:
  /*! Constructs a WLinePlot and passes all the arguments to WGraphPlot.*/
  WLinePlot(QWidget *parent, QWidget *legendParent,
	    WGraphXAxis::Position xAxisPos=WGraphXAxis::bottom,
	    WGraphYAxis::Position yAxisPos=WGraphYAxis::left,
	    int border=0);

  virtual void writeXML(QDomElement &);
  virtual void readXML(QDomElement &);

  /*! Increases the current position of the cursor by one, or cycles
    back to zero, if it was bigger then the length of the traces set
    by setTraceLength(). The new cursor position is returned.*/
  int incCursorPos(); 
  /*! Returns the current position of the cursor.*/
  int getCursorPos() const { return cursorPos; };
  
  bool isWrapAround() { return wrapAround; }

  /*! Sets the value of trace \em trace at the current cursor position
    to \em value*/
  void setCurrentValue(int trace, float value);

  void setTraceLength(unsigned int );

public slots:

  void setWrapAround(bool b) { wrapAround=b; }
  
 protected:

  virtual void drawContent(QPainter & , bool);
  
  int cursorPos;
  bool wrapAround;
};


#endif // WLINEPLOT_HPP
