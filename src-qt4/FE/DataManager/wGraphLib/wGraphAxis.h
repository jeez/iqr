#ifndef WGRAPHAXIS_HPP
#define WGRAPHAXIS_HPP

#include <cmath> 
#include <iostream>

#include <qwidget.h>

#include <QVector>
#include <QAction>
#include <QWidgetAction>

#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMenu>

#include "wGraphFrame.h"

#include "wRange.h"

class WRangeEdit;

class WGraphAxisLabel : public QWidget {
  Q_OBJECT
 public:
  enum Orientation { horizontal, vertical };
  
  WGraphAxisLabel(QWidget *parent, WGraphAxisLabel::Orientation orient, const QString& label);
  
  void setLabel(const QString& l) { 
    label=l; updateGeometry(); update(); 
  }
  void setMagnitude(int m) { 
    if (magnitude==m) return; magnitude=m; 
    updateGeometry(); update();
  }
  
  QSize sizeHint() const;

 protected:
  
  QString text() const; 
  
  void paintEvent(QPaintEvent *);
  
  QString label;
  Orientation orientation;
  int magnitude;
};

/*! The WGraphAxis class is the abstract base class for WGraphXAxis
  and WGraphYAxis used in WGraphWidget. These classes have two
  functionalities. First, they display the tick marks at the side of a
  WGraphFrame, second they are responsible for all transformations
  between data and view coordinates.

  The tick marks displayed by the axis are multiples of {0.1, 0.2,
  0.25, 0.5} x 10^n, where n is an integer. This behavior can be
  modulated by specifying a base tick mark which specifies a lower
  limit (e.g. setting the base tick to 1 will result in integers
  only).
  
  Each axis object has two ranges associated to it, the data range,
  which specifies the maximal extent of the data in the direction of
  the axis, and the visible range which specifies which portion of the
  data range is visible. The dynamic adjustment of the visible range
  can be controlled with setAutoScale and setExpandOnly.*/
class WGraphAxis : public QWidget {
  Q_OBJECT
  friend class WGraphFrame;
 public:
  /*! Initializes an axis object. 
    \param parent: parent widget (see Qt)
    \param frame: the frame to which this axis belongs to 
  */
  WGraphAxis(QWidget *parent, WGraphFrame *frame);
  
  /*! Returns the array of tick mark positions in the view coordinates
    defined by the frame to which this axis belongs to. */
  QVector<int> &getTickArray() { return tickPosArray; };

  /*! Returns the visible range for this axis*/
  WRange getVisRange() const { return visRange; };
  /*! Returns the data range for this axis*/
  WRange getDataRange() const { return dataRange; };
  
  /*! This function transforms val in data coordinates to view
    coordinates. */
  virtual int mapToViewCoords(float val)=0;
  /*! This function transforms val in view coordinates to data
    coordinates. */
  virtual float mapFromViewCoords(int val)=0;
  /*! This function differs from the above only by what arguments it
    takes.  for horizontal/vertical axes, either the x/y component are
    are used for the transformation respectively.*/
  virtual float mapFromViewCoords(QPoint p)=0;

  /*! Returns true if this axis does auto scaling, i.e. the visible
    range is always adjusted to fit the data range \sa setAutoScale()*/
  bool isAutoScale() const { return autoScale; }
  /*! Returns true if this axis is only expanding its visible
    range if auto scaling is enabled. */
  bool isExpandOnly() const { return expandOnly; }
  
  /*! Returns a pointer to the axis' popup menu.*/
  QMenu *getPopupMenu() { return popupMenu; }
  
  /*! Writes all the parameters descriving this axis to the
    QDomElement \em e.*/
  virtual void writeXML(QDomElement &e);
  /*! Reads all the parameters from the QDomElement \em e.*/
  virtual void readXML(QDomElement &e);
  
  WGraphAxisLabel *getLabel() { return label; }
    
 public slots:

  /*! Sets the direction of the axis. */
  void setDirection(int d) { dir=d>0?1:-1; }
  
  /*! If set to true, the user can pan this axis at the GUI level. 
    The visible range, however, always needs to be within the data range. */
  void setPanEnabled(bool pan) { panEnabled=pan; }
  /*! Enabling auto scaling means that the visible range is always the
    same as the data range, i.e. changing the data range changes the
    visible range accordingly.*/
  void setAutoScale(bool b);
  /*! If set to true, this axis will only expand its visible range if
    auto scaling is enbaled. This has no effect if auto scaling is
    disabled.*/
  void setExpandOnly(bool b);
  /*! Sets the visible range of this axis to \em r. Nothing happens if auto
    scaling is enabled. \sa isAutoScale() */
  void setVisRange(WRange r);
  /*! Sets the data range of this axis ro \em r. If auto scaling is
    enabled, the visible range will be adjusted accordingly. */
  void setDataRange(WRange r);
  /*! Sets the base tick of this axis to \em base. */
  void setBaseTick(float base) { baseTick=base; update(); }
  
 signals:

  /*! This signal is emitted when the visible range of the axis
    changes. This can hapen either by zooming or paning at the GUI level,
    or by the explicit call of SetVisRange().*/
  void visRangeChanged(WRange visRange);
  /*! This signal differs from the above only by what arguments it takes. */
  void visRangeChanged();

 protected slots:

  void toggleAutoScale() { 
//      std::cout << "WGraphAxis::toggleAutoScale()" << std::endl;
      setAutoScale(!isAutoScale()); 
  }

  void toggleExpandOnly() { 
//      std::cout << "WGraphAxis::toggleExpandOnly()" << std::endl;
      expandOnly=!expandOnly; 
  }

  void prepPopup();

 protected:

  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

  void resizeEvent(QResizeEvent *) { calcTickArrays(); }
  
  void calcTicks(int );
  virtual void calcTickArrays()=0;
  
  float dir;

  int countTicks;
  float baseTick, minTick, tickStep, ordOfMag;
  
  bool  panEnabled, autoScale, expandOnly, visRangeRestricted;
  
  float panStart;

  WRange visRange, dataRange;

  QVector<int> tickPosArray;
  QVector<float> tickValArray;

  QCursor panCursor;

  QMenu *popupMenu;
  int autoScaleID, expandOnlyID, rangeEditID;

  WGraphAxisLabel *label;
		
  WRangeEdit *rangeEdit;

  WGraphFrame *parentFrame;

  QAction* qactAutoScale;
  QAction* qactExpandOnly;

  QWidgetAction* qwaRangeEdit;

};

/*! The WGraphXAxis is a horizontal axis, displayed either above or
  below a WGraphFrame. */
class WGraphXAxis : public WGraphAxis {
  Q_OBJECT 
public: 
  /*! This enum type defines the position of the axis:*/
  enum Position { 
    bottom, /*!< the tick marks are above the tick labels.*/
    top     /*!< the tick marks are below the tick labels.*/
  };
  
  /*! Constructs a horizontal axis. \em parent and \em frame are
    passed to WGraphAxis. \em pos defines the position of the axis. */
  WGraphXAxis(QWidget *parent, WGraphFrame *frame, Position p);
  
  virtual int mapToViewCoords(float val) {
    return int(rint((0.5+dir*(visRange.scaleValue(val)-0.5))*
		    parentFrame->contentsRect().width()));
  }
  
  virtual float mapFromViewCoords(int val) {
    return visRange.
      unscaleValue(0.5+dir*
		   (float(val)/parentFrame->contentsRect().width())-0.5);
  }
  
  float mapFromViewCoords(QPoint p) {
    return mapFromViewCoords(p.x());
  }

  void writeXML(QDomElement &elem) {
    QDomElement axis=elem.ownerDocument().createElement("xAxis");
    elem.appendChild(axis);
    WGraphAxis::writeXML(axis);
  }
  
  void readXML(QDomElement &elem) {
    QDomElement axis=elem.elementsByTagName("xAxis").item(0).toElement();
    WGraphAxis::readXML(axis);
  }
  
protected:

  void calcTickArrays();
  
  Position pos;

  void paintEvent(QPaintEvent *);

};

/*! The WGraphYAxis is a vertical axis, displayed either on the left or
  right side of a WGraphFrame. */
class WGraphYAxis : public WGraphAxis {
  Q_OBJECT
 public:
  /*! This enum type defines the position of the axis:*/
  enum Position {
    left, /*!< the tick marks are on the right side of the tick labels.*/
    right /*!< the tick marks are on the left side of the tick labels.*/
  };

  /*! Constructs a vertical axis. \em parent and \em frame are
    passed to WGraphAxis. \em pos defines the position of the axis. */
  WGraphYAxis(QWidget *parent, WGraphFrame *frame, Position p);

  virtual int mapToViewCoords(float val) {
    return int(rint((0.5-dir*(visRange.scaleValue(val)-0.5))*
		    parentFrame->contentsRect().height()));
  }
  
  virtual float mapFromViewCoords(int val) {
    return visRange.
      unscaleValue(0.5-dir*
		   (float(val)/parentFrame->contentsRect().height()-0.5));
  }
  
  float mapFromViewCoords(QPoint p) {
    return mapFromViewCoords(p.y());
  }

  void writeXML(QDomElement &elem) {
    QDomElement axis=elem.ownerDocument().createElement("yAxis");
    elem.appendChild(axis);
    WGraphAxis::writeXML(axis);
  }

  void readXML(QDomElement &elem) {
    QDomElement axis=elem.elementsByTagName("yAxis").item(0).toElement();
    WGraphAxis::readXML(axis);
  }

protected:

  void calcTickArrays();

  Position pos;

  void paintEvent(QPaintEvent *);
};

#endif // WGRAPHAXIS_HPP
