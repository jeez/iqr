#ifndef WCOLORBAR_HPP
#define WCOLORBAR_HPP

#include "wGraphWidget.h"

class WColorBar : public WGraphWidget {
  Q_OBJECT
 public:
  WColorBar(QWidget *); 
  
  bool isAutoScale() { return yAxis()->isAutoScale(); }
  
public slots:
	     
  void setRangeSelectEnabled(bool b) {  
    if (rangeSelectEnabled==b) return;
    rangeSelectEnabled=b;
    if (rangeSelectEnabled) setCursor(SplitHCursor); else unsetCursor();
  }
  
  void setColor(QColor c) { color=c; updateGraph(); }
  void setAutoScale(bool a) { yAxis()->setAutoScale(a); }
  
 signals:
  
  void rangeSelected(WRange);

 protected:

  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  
  virtual void drawContent(QPainter & , bool);

  bool rangeSelectEnabled, selected;

  int selecStartPos, selecEndPos;
  
  bool rangeSelecting;

  QColor color;
};

#endif// WCOLORBAR_HPP
