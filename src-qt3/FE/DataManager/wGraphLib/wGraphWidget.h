#ifndef WGRAPHWIDGET_HPP
#define WGRAPHWIDGET_HPP

#include "wGraphAxis.h"
#include "wGraphFrame.h"

class QToolButton;

/*! The WGraphWidget class is the base class of all widgets for the
  grphical visualization of data.

  It conists of a WGraphFrame, which is the drawing area, a horizontal
  (WGraphXAxis) and vertical (WGraphYAxis) axis, which are responsible
  for the control of data ranges and visualization of tick marks and
  their labeling, and two axis labels (WGraphAxisLabel), one for each
  axis.
*/

class WGraphWidget : public QWidget {
  Q_OBJECT
  friend class WGraphFrame;
public:
  /*! Constructs a WGraphWidget with a horizontal axis at \em
    xAxisPos and a vertical axis at \em yAxisPos. \em border is the
    amount of pixels around the drawing frame where no axis is
    present. */
  WGraphWidget(QWidget *parent,
	       WGraphXAxis::Position xAxisPos=WGraphXAxis::bottom,
	       WGraphYAxis::Position yAxisPos=WGraphYAxis::left,
	       int border=0);
  
  /*! Sets the label of the horizontal axis to \em label and shows
    it. If \em label is empty, the axis has no label.*/
  void setXLabel(const QString& label); 
  /*! Sets the label of the vertical axis to \em label and shows
    it. If \em label is empty, the axis has no label.*/
  void setYLabel(const QString& label);
  /*! Writes the current internal settings of the horizontal and
    vertical axis, the drawing frame but also its own settings into
    \em s.*/

  virtual void writeXML(QDomElement &);
  virtual void readXML(QDomElement &);

  /*! See WGraphAxis::getVisRange()*/
  WRange getXVisRange() const { return xGraphAxis->getVisRange(); };
  /*! See WGraphAxis::getVisRange()*/
  WRange getYVisRange() const { return yGraphAxis->getVisRange(); };
  /*! See WGraphAxis::getDataRange()*/
  WRange getXDataRange() const { return xGraphAxis->getDataRange(); };
  /*! See WGraphAxis::getDataRange()*/
  WRange getYDataRange() const { return yGraphAxis->getDataRange(); };
  
  /*! Sets the background color of the drawing frame to \em color.*/
  void setBackgroundColor(QColor color) { 
    drawingFrame->setBackgroundColor(color); 
  };

  /*! Sets the mouse cursor for the drawing frame to \em cursor.*/
  void setCursor(QCursor cursor) { drawingFrame->setCursor(cursor); }

  /*! Sets the mouse cursor for the drawing frame back to default.*/
  void unsetCursor() { drawingFrame->unsetCursor(); }
  
  /*! Returns the size of the drawing frame.*/
  QSize getFrameSize() const { return drawingFrame->contentsRect().size(); };
  
  QPoint mapToViewCoords(float,float) const;
  void mapFromViewCoords(QPoint , float &, float &) const;
  
  /*! See WGraphFrame::isZoomEnabled()*/
  bool isZoomEnabled() const { return drawingFrame->isZoomEnabled(); }
  
  /*! See WGraphFrame::isXGridVisible()*/
  bool isXGridVisible() const { return drawingFrame->isXGridVisible(); }
  /*! See WGraphFrame::isYridVisible()*/
  bool isYGridVisible() const { return drawingFrame->isYGridVisible(); }
  
  QPopupMenu *gridPopup() const { return showGridPopup; }
  
  /*! Returns a pointer to the x-axis.*/
  WGraphXAxis *xAxis() const { return xGraphAxis; }
  /*! Returns a pointer to the y-axis.*/
  WGraphYAxis *yAxis() const { return yGraphAxis; }

 public slots:
	    
  /*! If \em show = true, the horizontal axis is shown at the position
    specified in WGraphWidget(). If false, it is hidden.*/
  void showXAxis(bool show);
  /*! If \em show = true, the vertical axis is shown at the position
    specified in WGraphWidget(). If false, it is hidden.*/
  void showYAxis(bool show);

  /*! See WGraphFrame::showXGrid()*/
  void showXGrid(bool show) { drawingFrame->showXGrid(show); updateGraph(); };
  /*! See WGraphFrame::showYGrid()*/
  void showYGrid(bool show) { drawingFrame->showYGrid(show); updateGraph(); };
  /*! Added for convienince, calls showXGrid() and showYGrid()*/
  void showGrid(bool show) { showXGrid(show); showYGrid(show); };

  void resetZoom();

  /*! See WGraphFrame::setZoomEnabled()*/
  void setZoomEnabled(bool zoom) { drawingFrame->setZoomEnabled(zoom); }
  /*! See WGraphFrame::setAutoDisableZoom()*/
  void setAutoDisableZoom(bool autoDis) { drawingFrame->setAutoDisableZoom(autoDis); }
  
  /*! See WGraphFrame::setPanEnabled()*/
  void setFramePanEnabled(bool pan) { drawingFrame->setPanEnabled(pan); }
  /*! See WGraphAxis::setPanEnabled()*/
  void setAxisPanEnabled(bool pan) {
    xGraphAxis->setPanEnabled(pan); 
    yGraphAxis->setPanEnabled(pan); 
  }

  /*! Added for convienince, calls setXAxisAutoScale() and setYAxisAutoScale()*/
  void setAutoScale(bool autoScale) { 
    xAxis()->setAutoScale(autoScale); yAxis()->setAutoScale(autoScale); 
  }
  
  /*! See WGraphAxis::setDataRange()*/
  void setXDataRange(WRange r) { xGraphAxis->setDataRange(r); }
  /*! See WGraphAxis::setDataRange()*/
  void setYDataRange(WRange r) { yGraphAxis->setDataRange(r); }
  
  /*! See WGraphAxis::setVisRange()*/
  void setXVisRange(WRange r) { xGraphAxis->setVisRange(r); }
  /*! See WGraphAxis::setVisRange()*/
  void setYVisRange(WRange r) { yGraphAxis->setVisRange(r); }

  /*! See WGraphFrame::doRedraw()*/
  void doRedraw() { drawingFrame->doRedraw(); }

  /*! Updates the graph. Calling updateGraph() many times in a row
    will generate a single update.*/
  virtual void updateGraph() { drawingFrame->update(); }

signals:

  /*! This signal forwards the corresponding signals of the two axes.
   \sa WGraphAxis::visRangeChanged()*/
  void visRangeChanged();
  /*! Forwards the corresponding signal of the horizontal axis. \sa
    WGraphAxis::visRangeChanged(WRange)*/
  void xVisRangeChanged(WRange);
  /*! Forwards the corresponding signal of the vertical axis. \sa
    WGraphAxis::visRangeChanged(WRange)*/
  void yVisRangeChanged(WRange);
  /*! Forwards the corresponding signal of the drawing frame. \sa
    WGraphFrame::mouseEvent()*/
  void mouseEvent(QEvent::Type type, QPoint pos, int button, int state);

  /*! Forwards the corresponding signal of the drawing frame. \sa
    WGraphFrame::aboutToDraw()*/
  void aboutToDraw();

  /*! Forwards the corresponding signal of the drawing frame. \sa
    WGraphFrame::resized()*/
  void frameResized();
  /*! Forwards the corresponding signal of the drawing frame. \sa
    WGraphFrame::zoomed()*/
  void zoomed();
  void zoomReset();

 protected:
  
  /*!  Virtual function that draws the contents of the drawing frame. 

  The QPainter is already open when you get it, and you must leave it
  open. Painter transformations are switched off on entry. If you
  transform the painter, remember to take the frame into account and
  reset the transformation before returning.

  This function is reimplemented by subclasses that draw something
  inside the frame. It should draw only inside p.window(). The
  default function does nothing.

  For some situations, graphs do not need complete redraw, but only
  small changes. If, however, \em redraw = true, the whole conent
  should be redrawn, because, for example, the drawing frame was
  resized, or the visible range of one or both axes changed.*/
  virtual void drawContent(QPainter &p, bool redraw)=0;

  WGraphXAxis *xGraphAxis;
  WGraphYAxis *yGraphAxis;
  
  WGraphFrame *frame() const { return drawingFrame; }
  
  QToolButton *zoomButton;

  QPopupMenu *showGridPopup;
  int horizGridID, vertGridID;
  
 protected slots:

  void prepShowGridPopup();
  void toggleShowXGrid() { showXGrid(!isXGridVisible()); }
  void toggleShowYGrid() { showYGrid(!isYGridVisible()); }
  
 private:
  
  WGraphFrame   *drawingFrame;
};

#endif// WGRAPHWIDGET_HPP
