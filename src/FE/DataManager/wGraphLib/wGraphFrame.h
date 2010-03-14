#ifndef WGRAPHFRAME_HPP
#define WGRAPHFRAME_HPP

#include <qframe.h>
#include <qpixmap.h>
#include <qdom.h>
#include <qcursor.h>
#include <qpen.h>

class WGraphWidget;

/*! The WGraphFrame class is the drawing area of a WGraphWidget.*/
class WGraphFrame : public QFrame {
  Q_OBJECT
 public:
  /*! Constructs a WGraphFrame with \em parent as its WGraphWidget.*/
  WGraphFrame(WGraphWidget *parent);
  
  void setCursor(const QCursor &c) {
    defaultCursor=c;
    QWidget::setCursor(c);
  }
  void unsetCursor() { QWidget::setCursor(defaultCursor); }

  /*! Returns true if zooming at the GUI level is enabled.*/
  bool isZoomEnabled() const { return zoomEnabled; }

  /*! Returns true if the grid along the horizontal direction is visible.*/
  bool isXGridVisible() const { return xGridVisible; }
  /*! Returns true if the grid along the vertical direction is visible.*/
  bool isYGridVisible() const { return yGridVisible; }

  void setLineWidth(int );
  void setMidLineWidth(int );
  
  /*! If \em autoDis = true, zooming will be automatically disabled
    after each zooming.*/
  void setAutoDisableZoom(bool autoDis) { autoDisableZoom=autoDis; }

  /*! Enables paning at the GUI level, if \em pan = true. The mouse
    cursors shape above the drawing area will be changed accordingly.

    NOTE: zooming has priority.
  */
  void setPanEnabled(bool pan);

  /*! Shows the grid along the horizontal direction, if \em show = true,
    hides it otherwise.*/
  void showXGrid(bool show) { xGridVisible=show; update(); };
  /*! Shows the grid along the verical direction, if \em show = true,
    hides it otherwise.*/
  void showYGrid(bool show) { yGridVisible=show; update(); };

  /*! Sets the mouse cursor which will show up over the frame, when
    zooming is enabled.*/
  void setZoomCursor(QCursor c) { zoomCursor=c; };
  /*! Sets the mouse cursor which will show up over the frame, when
    paning is enabled.*/
  void setPanCursor(QCursor c) { panCursor=c; };
  
  void writeXML(QDomElement &);
  void readXML(QDomElement &);

public slots:

  /*! Call this function to force a redraw, i.e. in the next call to
    drawContent() redraw=TURE will be set in any case*/
  void doRedraw() { needsRedraw=true; update(); }

  /*! Enables zooming at the GUI level, if \em zoom = true. The mouse
    cursors shape above the drawing area will be changed accordingly.*/
  void setZoomEnabled(bool zoom);

signals:

  /*! This signal is emitted just before the contents of WGraphFrame
    are drawn.*/
  void aboutToDraw();
  /*! This signal is emitted when this WGraphFrame is resized.*/
  void resized();
  /*! This signal is emitted when zooming was applied at the GUI level.*/
  void zoomed();
  void zoomReset();
  
 protected:
  
  virtual void resizeEvent(QResizeEvent *);
  virtual void paintEvent(QPaintEvent *);

  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  
 private:

  void drawGrid(QPainter &);
  
  bool zoomEnabled, autoDisableZoom;
  bool panEnabled;
  bool xGridVisible, yGridVisible;

  QPoint zoomStart;
  QRect zoomRect, oldZoomRect;

  float panStartX, panStartY;

  QCursor defaultCursor;
  QCursor zoomCursor, panCursor;

  bool needsRedraw;

  QPixmap paintPixmap;
  WGraphWidget *parent;

  QPen zoomRectPen, gridPen;
};


#endif//WGRAPHFRAME_HPP
