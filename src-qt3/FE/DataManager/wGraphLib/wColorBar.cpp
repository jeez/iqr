#include "wColorBar.h"

#include "wMath.h"

#include <qpainter.h>
#include <qpopupmenu.h>

#include <algorithm>

//#include "wColorBar.moc"

using std::max;
using std::min;

WColorBar::WColorBar(QWidget *parent) 
  : WGraphWidget(parent,WGraphXAxis::bottom,WGraphYAxis::right),
    rangeSelecting(false) {
  
  showXAxis(false);
  setZoomEnabled(false);
  setAxisPanEnabled(false);
  
  setRangeSelectEnabled(true);
  yAxis()->setAutoScale(true);
  
  frame()->setFixedWidth(30);  
}

void WColorBar::drawContent(QPainter &p, bool) {
  
  int hue, selectHue, s, v;
  
  color.getHsv(hue,s,v);
  colorGroup().highlight().getHsv(selectHue,s,v);

  int left=p.window().left();
  int width=p.window().width();
  int top=p.window().top();
  
  int selecTop=-1, selecBottom=-1;
  
  if (rangeSelecting) {
    selecBottom=max(selecStartPos,selecEndPos);
    selecTop=min(selecStartPos,selecEndPos);
  }
  
  float step=float(p.window().height())/256;
  QColor paintColor;

  for (int i=0; i<256; i++) {

    float y=i*step;
    
    if (rint(y)>selecBottom || rint(y)<selecTop)
      paintColor.setHsv(hue,255,255-i);
    else
      paintColor.setHsv(selectHue,128,int(255-i*0.5));
    
    p.fillRect(left,top+int(rint(y)),width,int(rint(y+step)-rint(y)),
	       QBrush(paintColor));
  }
}

void WColorBar::mousePressEvent(QMouseEvent *e) {
  
  if (rangeSelectEnabled && e->button()==LeftButton) {
    
    QPoint pos=frame()->mapFrom(this,e->pos())-
      frame()->contentsRect().topLeft();
    
    if (frame()->hasMouse()) {
      
      rangeSelecting=true;
      
      selecEndPos=selecStartPos=pos.y();
      
      updateGraph();
    }
  } else e->ignore();
}

void WColorBar::mouseReleaseEvent(QMouseEvent *e) {

  if (rangeSelectEnabled && e->button()==LeftButton) {

    emit rangeSelected(WRange(yAxis()->mapFromViewCoords(selecStartPos),
			      yAxis()->mapFromViewCoords(selecEndPos)));
    
    rangeSelecting=false;
    updateGraph();
  } else e->ignore(); 
}

void WColorBar::mouseMoveEvent(QMouseEvent *e) {
  
  if (rangeSelecting && (e->state() & LeftButton)) {
    
    QPoint pos=frame()->mapFrom(this,e->pos())-
      frame()->contentsRect().topLeft();
    
    selecEndPos=pos.y();
    
    updateGraph();
  } else e->ignore();
}
