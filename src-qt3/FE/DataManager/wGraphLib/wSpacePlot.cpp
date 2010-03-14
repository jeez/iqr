#include "wSpacePlot.h"

#include <qcolor.h>
#include <qtooltip.h>
#include <qtimer.h>
#include <qpainter.h>

#include <stdio.h>
#include "wMath.h"

//#include "wSpacePlot.moc"

WSpacePlot::WSpacePlot(QSize size, bool dg, 
		       QWidget* parent, QWidget *colorBarParent,
		       WGraphXAxis::Position xAxisPos,
		       WGraphYAxis::Position yAxisPos,
		       int border)
  : WGraphWidget(parent,xAxisPos,yAxisPos,border),
    paintColor(red), dataRange(0.0, 1.0),
    recRects(0), cellLabelRect(QRect()),
    colorBar(0), startCell(0,0), currCell(0,0), 
    selecting(true), dragEnabled(false), drawGrid(dg), 
    autoScale(true), multiSelect(true) {
    
    if (colorBarParent) {
      colorBar = new WColorBar(colorBarParent);
      connect(this,SIGNAL(colorChanged(QColor)),
	      colorBar,SLOT(setColor(QColor)));
      connect(colorBar,SIGNAL(rangeSelected(WRange)),
	      this,SLOT(selectRange(WRange)));
      connect(colorBar,SIGNAL(yVisRangeChanged(WRange)),
	      this,SLOT(setDataRange(WRange)));
      colorBar->show();
    } 
    
    setCursor(crossCursor);

    if (drawGrid) setBackgroundColor(white);
    
    setFocusPolicy(TabFocus);
    
    xAxis()->setBaseTick(1.0);
    yAxis()->setBaseTick(1.0);
    
    showGrid(false);
    
    setSize(size);
    
    autoPanTimer = new QTimer(this);
    labelClearTimer = new QTimer(this);
    
    connect(frame(),SIGNAL(aboutToDraw()),this,SLOT(checkDataRange()));
    connect(autoPanTimer,SIGNAL(timeout()),this,SLOT(autoPan()));
    connect(labelClearTimer,SIGNAL(timeout()),this,SLOT(clearCellLabel()));
    connect(this,SIGNAL(frameResized()),SLOT(calcGeometry()));
    connect(this,SIGNAL(visRangeChanged()),SLOT(calcGeometry()));
  }

WSpacePlot::~WSpacePlot() {
  delete[] recRects;
}

void WSpacePlot::writeXML(QDomElement &elem) {
  WGraphWidget::writeXML(elem);
  if (!colorBar)
    elem.setAttribute("autoScale",autoScale?"true":"false");
  elem.setAttribute("multiSelect",multiSelect?"true":"false");
  elem.setAttribute("min",dataRange.min());
  elem.setAttribute("max",dataRange.max());
}

void WSpacePlot::readXML(QDomElement &elem) {
  WGraphWidget::readXML(elem);
  autoScale=elem.attribute("autoScale","true")=="true";
  multiSelect=elem.attribute("multiSelect")=="true";
  dataRange=WRange(elem.attribute("min").toFloat(),
		   elem.attribute("max").toFloat());
}

void WSpacePlot::calcGeometry() {

  QArray<SpaceCell>::Iterator it=cellData.begin();

  for(int y=0; y < viewSize.height(); y++)
    for (int x=0; x < viewSize.width(); x++) {
      it->rect=QRect(mapToViewCoords(x-0.5,y+0.5),
		     mapToViewCoords(x+0.5,y-0.5)).normalize();

      it->rect.setSize(it->rect.size()-QSize(1,1));
      
      it->visible=it->rect.intersects(QRect(QPoint(0,0),
					    getFrameSize()));
      ++it;
    }
}


void WSpacePlot::setSize(QSize size) {

  viewSize=size;
    
  cellData.resize(viewSize.width()*viewSize.height());
  clearCells();
  
  setXDataRange(WRange(-0.5,viewSize.width()-0.5));
  setYDataRange(WRange(-0.5,viewSize.height()-0.5));

  calcGeometry();
  clearCellLabel();

  currCell=startCell=QPoint(0,0);
}

void WSpacePlot::setCellLabel(QPoint pos, QPoint cell) {
  
  pos+=QPoint(1,1);

  cellLabelString.sprintf("%d:%d",cell.x(),cell.y());
  
  QPainter p(this);
  cellLabelRect = p.boundingRect(pos.x()+2,pos.y()+2,80,20,
				 AlignLeft,cellLabelString);
  
  cellLabelRect.setSize(cellLabelRect.size()+QSize(4,4));
  
  int dx = (cellLabelRect.right() >= getFrameSize().width()) ? 
    -(cellLabelRect.right()-getFrameSize().width()+1) :
    (cellLabelRect.left() < 0) ? -cellLabelRect.left() : 0;
  int dy = (cellLabelRect.bottom() >= getFrameSize().height()) ? 
    -(cellLabelRect.bottom()-getFrameSize().height()+1) :
    (cellLabelRect.top() < 0) ? -cellLabelRect.top() : 0;

  cellLabelRect.moveBy(dx,dy);
  updateGraph();
}

void WSpacePlot::selectRectTmp(const QRect &rect, bool select) {
  QRect r=rect.normalize();
  if (select)
    for (int x=r.left(); x<=r.right(); x++)
      for (int y=r.top(); y<=r.bottom(); y++)
	cellData[x+y*viewSize.width()].attr|=tmpSelected;
  else
    for (int x=r.left(); x<=r.right(); x++)
      for (int y=r.top(); y<=r.bottom(); y++)
	cellData[x+y*viewSize.width()].attr&=normal|marked;
}

void WSpacePlot::clearTmpSelected() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if (it->attr & tmpSelected) it->attr&=~tmpSelected;
}

void WSpacePlot::acceptTmpSelected() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if (it->attr&tmpSelected) 
      if (it->attr&normal) it->attr|=selected; else it->attr=0;
  emit selectionChanged();
}

void WSpacePlot::clearSelection() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    it->attr&=normal|marked;
  emit selectionChanged();
}

void WSpacePlot::invertSelection() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if (it->attr&normal)
      if (it->attr&selected) it->attr&=~selected; else it->attr|=selected;
  emit selectionChanged();
}

int WSpacePlot::countSelectedCells() {
  int count=0;
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if ((it->attr & selected) && (it->attr & normal)) count++;
  return count;
}

QArray<int> &WSpacePlot::getSelecIndexList() {

  selecIndexList.resize(countSelectedCells());
  int listIndex=0;
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if ((it->attr & selected) && 
	(it->attr & normal)) 
      selecIndexList[listIndex++]=it-cellData.begin();
  return selecIndexList;
}

void WSpacePlot::setSelecIndexList(QArray<int> &list) {

  clearSelection();
  for (unsigned int i=0; i<list.count(); i++) 
    cellData[list[i]].attr|=selected;
   
  selecIndexList=list;
}

void WSpacePlot::setMultiSelect(bool b) {
  multiSelect=b;
  clearSelection();
  emit selectionChanged();
}

void WSpacePlot::selectRange(WRange r) {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if (it->attr&normal && r.contains(it->data)) it->attr|=selected;
    else it->attr&=~selected;
  updateGraph();
  emit selectionChanged();
}

void WSpacePlot::markSelected() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it)
    if (it->attr & selected) 
      it->attr|=marked;
    else it->attr&=~marked;
  updateGraph();
}

void WSpacePlot::exchgMarkedSelected() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it) {
    
    if (it->attr & selected && !(it->attr & marked)) {
      it->attr&=~selected;
      it->attr|=marked;
    } else if (!(it->attr & selected) && (it->attr & marked)) {
      it->attr&=~marked;
      it->attr|=selected;
    }
  }
  updateGraph();
  emit selectionChanged();
}

void WSpacePlot::clearMarked() {
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end();++it) it->attr&=~marked;
  updateGraph();
}

void WSpacePlot::mousePressEvent(QMouseEvent *e) {
  
  if (e->button()==LeftButton) {
    
    QPoint pos=frame()->mapFrom(this,e->pos())-
      frame()->contentsRect().topLeft();
    
    currCursorPos=pos;
    
    float x,y;
    mapFromViewCoords(pos,x,y);
    QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()),
		       clampi(int(rint(y)),0,viewSize.height()));
    
    int index=cell.x()+cell.y()*viewSize.width();

    if (multiSelect)
      if (e->state() & ShiftButton) {
	clearTmpSelected();
	selectRectTmp(QRect(startCell,cell),selecting);
      } else if (e->state() & ControlButton) {
	startCell=cell;
	acceptTmpSelected();
	selecting=!(cellData[index].attr & selected);
	selectRectTmp(QRect(startCell,cell),selecting);
      } else {
	startCell=cell;
	if ((cellData[index].attr & selected) &&
	    (cellData[index].attr & normal))
	  dragEnabled=true;
	else {
	  clearSelection();
	  selecting=true;
	  selectRectTmp(QRect(startCell,cell),selecting);
	}
	e->ignore();
      }
    else {
      clearSelection();
      selectRectTmp(QRect(cell,cell),true);
      emit selectionChanged();
      e->ignore();
    }

    setCellLabel(pos,cell);
    updateGraph();

    currCell=cell;

  } else e->ignore();
}

void WSpacePlot::mouseReleaseEvent(QMouseEvent *e) {

  if (e->button()==LeftButton) {
    if (multiSelect) {
      if (dragEnabled) {
	dragEnabled=false;
	clearSelection();
	selectRectTmp(QRect(startCell,startCell),true);
      }
      emit selectionChanged();
    }
    autoPanTimer->stop();
    labelClearTimer->start(1000,true);
  } else e->ignore();
}

void WSpacePlot::mouseMoveEvent(QMouseEvent *e) {
  
  if (e->state() & LeftButton) {

    QPoint pos=frame()->mapFrom(this,e->pos())-
      frame()->contentsRect().topLeft();
   
    currCursorPos=pos;
    
    float x,y;
    mapFromViewCoords(pos,x,y);
    QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()),
		       clampi(int(rint(y)),0,viewSize.height()));
    
    if (dragEnabled) {
      dragEnabled=false;
      clearCellLabel();
      emit wouldDrag();
    } else {
      if (!QRect(QPoint(0,0),getFrameSize()).contains(pos)) {
	if (!autoPanTimer->isActive()) autoPanTimer->start(1);
      } else {
	autoPanTimer->stop();
	if (cell!=currCell) {
	  if (multiSelect) {
	    clearTmpSelected();
	    selectRectTmp(QRect(startCell,cell),selecting);
	  } else {
	    clearSelection();
	    selectRectTmp(QRect(cell,cell),true);
	    emit selectionChanged();
	  }
	}
	setCellLabel(pos,cell);
      }
    }
    
    currCell=cell;

    updateGraph();
    
  } else e->ignore(); 
}

void WSpacePlot::keyPressEvent(QKeyEvent *e) {

  switch (e->key()) {
  case Key_Up: 
    if (currCell.y()<viewSize.height()-1) currCell+=QPoint(0,1);
    break;
  case Key_Down:
    if (currCell.y()>0) currCell-=QPoint(0,1);
    break;
  case Key_Right:
    if (currCell.x()<viewSize.width()-1) currCell+=QPoint(1,0);
    break;
  case Key_Left: 
    if (currCell.x()>0) currCell-=QPoint(1,0);
    break;
  case Key_End: currCell.setX(viewSize.width()-1); break;
  case Key_Home: currCell.setX(0); break;
  case Key_PageUp: currCell.setY(viewSize.height()-1); break;
  case Key_PageDown: currCell.setY(0); break;
  case Key_Space:
    if (multiSelect) {
      if (!(e->state() & ControlButton)) clearSelection();
      if (!(e->state() & ShiftButton)) startCell=currCell;
      int index=currCell.x()+currCell.y()*viewSize.width();
      selectRectTmp(QRect(startCell,currCell),
		 !(cellData[index].attr & selected));
      emit selectionChanged();
    }
    break;
  default: e->ignore(); return;
  }
  
  e->accept();
  
  if (!multiSelect) {
    clearSelection();
    selectRectTmp(QRect(currCell,currCell),true);
    emit selectionChanged();
  } 
  
  updateGraph();
}

void WSpacePlot::checkDataRange() {

  if (isAutoScale()) {
    
    WRange autoRange;
    
    autoRange.resetForRangeCheck();
    
    QArray<SpaceCell>::Iterator it;
    QArray<SpaceCell>::Iterator end=cellData.end();
    
    for (it=cellData.begin(); it!=end; ++it)
      if ((it->attr & normal) && finite(it->data)) 
	autoRange.adjust(it->data);
    
    if (!autoRange.isEmpty() && dataRange!=autoRange) {
      if (colorBar) {
	colorBar->setYDataRange(autoRange);
	dataRange=colorBar->getYVisRange();
      } else dataRange = autoRange;
    }
  }
}

void WSpacePlot::drawContent(QPainter &p,bool) {

  QBrush brush;
  QColor color;
  int hue, selectHue, s, v;
  
  paintColor.getHsv(hue,s,v);
  colorGroup().highlight().getHsv(selectHue,s,v);
  
  QArray<SpaceCell>::Iterator it;
  QArray<SpaceCell>::Iterator end=cellData.end();

  if (drawGrid) {
    p.eraseRect(p.window());
    for (it=cellData.begin(); it!=end; ++it) {
      if (it->attr & selected) {
	p.fillRect(it->rect,colorGroup().highlight());
	p.setPen(colorGroup().highlight().light());
      } else p.setPen(colorGroup().mid());
      p.drawRect(it->rect);
    } 
  } else {   

    p.setBackgroundMode(OpaqueMode);
    p.setBackgroundColor(QColor(64,64,64));
    p.fillRect(p.window(),QBrush(black,Dense6Pattern));
    
    for (it=cellData.begin(); it!=end; ++it)
      if (it->visible) 
	if (it->attr&normal) {
	  
	  float val=clampf(dataRange.scaleValue(it->data));
	  
	  if (finite(val)) {
	    
	    if (it->attr & selected) 
	      color.setHsv(selectHue,128,int((0.5+0.5*val)*255));
	    else
	      color.setHsv(hue,255,int(val*255));
	    
	    p.fillRect(it->rect,color);	
	    
	    if (it->attr&marked) {
	      if (it->attr&selected) 
		color.setHsv(hue,255,int(val*255));
	      else 
		color.setHsv(selectHue,128,int((0.5+0.5*val)*255));	    
	      p.setPen(color);
	      p.drawRect(it->rect);
	    }
	  } else 
	    p.fillRect(it->rect,QBrush(paintColor,DiagCrossPattern));
	} 
  }
  
  if (hasFocus() && QRect(QPoint(0,0),viewSize).contains(currCell)) {
    p.setRasterOp(XorROP);
    p.setPen(white);
    p.drawRect(cellData[currCell.x()+currCell.y()*viewSize.width()].rect);
    p.setRasterOp(CopyROP);
  }
  
  if (!cellLabelRect.isEmpty()) {
    p.setBackgroundMode(TransparentMode);
    p.setPen(QToolTip::palette().active().text());
    p.fillRect(cellLabelRect,QToolTip::palette().active().background());
    p.drawRect(cellLabelRect);
    p.drawText(cellLabelRect,AlignCenter,cellLabelString);
  }
}

void WSpacePlot::clearCells(char attrMask) { 
  for (QArray<SpaceCell>::Iterator it=cellData.begin();
       it!=cellData.end(); ++it) {
    it->setValue(0);
    it->attr&=attrMask; 
    it->attr|=normal;
  }
  emit selectionChanged();
}

void WSpacePlot::autoPan() {

  float x,y;
  mapFromViewCoords(currCursorPos,x,y);

  QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()),
		     clampi(int(rint(y)),0,viewSize.height()));
  
  setCellLabel(currCursorPos,cell);

  float dx=getXVisRange()>x?getXVisRange().min()-x:
    (getXVisRange()<x?getXVisRange().max()-x:0);
  float dy=getYVisRange()>y?getYVisRange().min()-y:
    (getYVisRange()<y?getYVisRange().max()-y:0);

  if (dx) {
    WRange vrx=getXVisRange()-dx*0.05;
    vrx.adjustToFit(getXDataRange());
    setXVisRange(vrx);
  }

  if (dy) {
    WRange vry=getYVisRange()-dy*0.05;
    vry.adjustToFit(getYDataRange());
    setYVisRange(vry);
  }

  if (multiSelect) {
    clearTmpSelected();
    selectRectTmp(QRect(startCell,cell),selecting);
  } else {
    clearSelection();
    selectRectTmp(QRect(cell,cell),true);
    emit selectionChanged();
  }
}
