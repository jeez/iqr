#include "wGraphAxis.h"

#include <qpainter.h>
#include <qbitmap.h>
#include <qpopupmenu.h>
#include <qsimplerichtext.h>

#include "wRangeEdit.h"

#include <algorithm>

//#include "wGraphAxis.moc"

using std::max;
using std::min;

WGraphAxis::WGraphAxis(QWidget *parent, WGraphFrame *frame) 
  : QWidget(parent,"",WRepaintNoErase | WResizeNoErase),
    dir(1), baseTick(0), panEnabled(true), autoScale(true),
    expandOnly(false), panCursor(pointingHandCursor), 
    parentFrame(frame) {
  
  QFont f=font();
  f.setPointSize(int(rint((0.8*f.pointSize()))));
  setFont(f);
  
  popupMenu = new QPopupMenu(this);
  autoScaleID=popupMenu->insertItem("auto scale",
				    this,SLOT(toggleAutoScale()));
  
  //   popupMenu->setCheckable(autoScaleID);
  
  expandOnlyID=popupMenu->insertItem("expand only",
				     this,SLOT(toggleExpandOnly()));
    
  rangeEditID=popupMenu->insertItem(rangeEdit = new WRangeEdit(popupMenu));
  connect(this,SIGNAL(visRangeChanged(WRange)),
	  rangeEdit,SLOT(setRange(WRange)));
  connect(rangeEdit,SIGNAL(rangeChanged(WRange)),
	  this,SLOT(setVisRange(WRange)));    
  connect(popupMenu,SIGNAL(aboutToShow()),this,SLOT(prepPopup()));
}

void WGraphAxis::setAutoScale(bool b) {
  if (autoScale=b) setDataRange(dataRange);
}

void WGraphAxis::setExpandOnly(bool b) {
  if (!(expandOnly=b)) setDataRange(dataRange);
}

void WGraphAxis::setVisRange(WRange r) {  
  
  if (!r.valid() || visRange==r) return;
  
  visRange=r;
  
  emit visRangeChanged();
  emit visRangeChanged(visRange);
  
  calcTickArrays();
  update();
  parentFrame->update();
}

void WGraphAxis::setDataRange(WRange r) {
  if (!r.valid()) return;
  
  dataRange=r;

  if (autoScale) 
    if (!expandOnly) setVisRange(dataRange);
    else
      setVisRange(WRange(min(visRange.min(),dataRange.min()),
			 max(visRange.max(),dataRange.max())));
}

void WGraphAxis::mousePressEvent(QMouseEvent *e) {
  if (e->button()==LeftButton) {
    if (panEnabled) {
      setCursor(panCursor);
      panStart=mapFromViewCoords(e->pos());
    }
  } else if (e->button()==RightButton) 
    popupMenu->popup(e->globalPos(),1);
  else e->ignore();
}

void WGraphAxis::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button()==LeftButton)
    unsetCursor();
  else e->ignore();
}

void WGraphAxis::mouseMoveEvent(QMouseEvent *e) {
  if (panEnabled && (e->state()==LeftButton)) {
    visRange+=panStart-mapFromViewCoords(e->pos());
    if (visRange.adjustToFit(dataRange)) 
      setCursor(forbiddenCursor);
    else {
      setCursor(panCursor);
    
      emit visRangeChanged();
      emit visRangeChanged(visRange);
      update();
      parentFrame->update();
    }
  } else e->ignore();
}

void WGraphAxis::calcTicks(int maxCountTicks) {
  
  const float tickStepList[] = {0.5, 1.0, 2.0, 2.5, 5.0};

  tickStep=visRange.interval()/maxCountTicks;

  if (tickStep>baseTick) {
    
    ordOfMag=pow(10.0,rint(log10(tickStep)));
    
    tickStep/=ordOfMag;
    
    int bestFit=0;
    float maxFit=FLT_MAX;
    
    for (int i=0; i<5; i++)
      if (((tickStepList[i]-tickStep)>=0) &&
	  ((tickStepList[i]-tickStep)<maxFit) &&
	  ((baseTick==0) || (ordOfMag*tickStepList[i]/baseTick==
			     rint(ordOfMag*tickStepList[i]/baseTick)))) {
	bestFit=i;
	maxFit=tickStepList[i];
      }
    
    tickStep=tickStepList[bestFit]*ordOfMag;
    
  } else tickStep=baseTick;
  
  minTick  = ceil(visRange.min()/tickStep)*tickStep;
  countTicks = int(floor(visRange.max()/tickStep)-
		   ceil(visRange.min()/tickStep))+1;

  ordOfMag=max(fabs(visRange.min()),
	       fabs(visRange.max()));
  
  ordOfMag=pow(10.0,rint(log10(ordOfMag)));

  if (!label->isVisible() || fabs(rint(log10(ordOfMag)))<=3) ordOfMag=1;
}

void WGraphAxis::prepPopup() {
  popupMenu->setItemChecked(autoScaleID,isAutoScale());
  popupMenu->setItemChecked(expandOnlyID,expandOnly);
  popupMenu->setItemEnabled(expandOnlyID,isAutoScale());
  popupMenu->setItemEnabled(rangeEditID,!isAutoScale());
  
  rangeEdit->setEnabled(!isAutoScale());
}

void WGraphAxis::writeXML(QDomElement &elem) {
  elem.setAttribute("panEnabled",panEnabled?"true":"false");
  elem.setAttribute("autoScale",autoScale?"true":"false");
  elem.setAttribute("visible",isVisible()?"true":"false");
  elem.setAttribute("expandOnly",expandOnly?"true":"false");
  
  QDomElement vr=elem.ownerDocument().createElement("visRange");
  elem.appendChild(vr);
  vr.setAttribute("min",visRange.min());
  vr.setAttribute("max",visRange.max());
  QDomElement dr=elem.ownerDocument().createElement("dataRange");
  elem.appendChild(dr);
  dr.setAttribute("min",dataRange.min());
  dr.setAttribute("max",dataRange.max());
}

void WGraphAxis::readXML(QDomElement &elem) {
  panEnabled=elem.attribute("panEnabled")=="true";
  autoScale=elem.attribute("autoScale")=="true";
  if (elem.attribute("visible")=="true") show(); else hide();
  expandOnly=elem.attribute("expandOnly")=="true";
  
  QDomElement vr=elem.elementsByTagName("visRange").item(0).toElement();
  QDomElement dr=elem.elementsByTagName("dataRange").item(0).toElement();

  setDataRange(WRange(dr.attribute("min").toFloat(),
		      dr.attribute("max").toFloat()));
  setVisRange(WRange(vr.attribute("min").toFloat(),
		     vr.attribute("max").toFloat()));
}

WGraphXAxis::WGraphXAxis(QWidget *parent, WGraphFrame *frame, Position p) 
  : WGraphAxis(parent,frame), pos(p) { 

    label = new WGraphAxisLabel(parent,WGraphAxisLabel::horizontal,"");
    
    panCursor=sizeHorCursor; 
    setFixedHeight(fontMetrics().height()+fontMetrics().width("0"));
  }

void WGraphXAxis::calcTickArrays() {
  
  int width=fontMetrics().width("00");
  int counter=0;
  
  do {
    counter++;
    calcTicks(int(rint(size().width()/(3.0*width))));

    if (countTicks) {
      width=0;
      for (int i=0; i<countTicks; i++) {
	float val=(rint((minTick+i*tickStep)/tickStep)*tickStep);
	width+=fontMetrics().width(QString::number(val/ordOfMag));
      }
      width/=countTicks;
    }
  } while ((countTicks*3*width>size().width()) && (counter < 5));

  tickValArray.resize(countTicks);
  tickPosArray.resize(countTicks);

  for (int i=0; i<countTicks; i++) {

    float val=(rint((minTick+i*tickStep)/tickStep)*tickStep);
    if (val==0) val=0;
    
    tickValArray[i]=val;
    tickPosArray[i]=mapToViewCoords(val);
  }
  
  label->setMagnitude(int(rint(log10(ordOfMag))));
}


void WGraphXAxis::paintEvent(QPaintEvent *e) {

  erase(e->rect());
  
  QPainter p(this);
  
  p.setClipRect(e->rect());
  
  int tickLen=p.fontMetrics().width("0")/2;
  
  for (int i=0; i<countTicks; i++) {

    QRect textRect=QRect(QPoint(0,0),p.fontMetrics().
			 size(SingleLine,QString::number(tickValArray[i]/ordOfMag)));
    
    int x=parentFrame->contentsRect().x()+tickPosArray[i];
    
    textRect.moveBy(-textRect.center().x(),0);
    textRect.moveCenter(textRect.center()+QPoint(x,0));
    int dx;
    if ((dx=textRect.right()-size().width())>0) textRect.moveBy(-dx,0);
    if ((dx=textRect.left()-1)<0) textRect.moveBy(-dx,0);
    
    if (pos==top) {
      int height=size().height()-1;
      p.drawLine(x,height,x,height-tickLen);
      textRect.moveBy(0,height-p.fontMetrics().height()-2*tickLen);
    } else  {
      p.drawLine(x,0,x,tickLen);
      textRect.moveBy(0,2*tickLen);
    }
    
    p.drawText(textRect, AlignVCenter, 
	       QString::number(tickValArray[i]/ordOfMag));
  }
  label->setMagnitude(int(rint(log10(ordOfMag))));
}

WGraphYAxis::WGraphYAxis(QWidget *parent, WGraphFrame *frame, Position p) 
  : WGraphAxis(parent,frame), pos(p) { 
    
  label = new WGraphAxisLabel(parent,WGraphAxisLabel::vertical,"");
  
  panCursor=sizeHorCursor; 
}


void WGraphYAxis::calcTickArrays() {

  calcTicks(int(rint(size().height()/(3.0*fontMetrics().height()))));
  
  int minWidth=10;
  
  tickValArray.resize(countTicks);
  tickPosArray.resize(countTicks);
  
  for (int i=0; i<countTicks; i++) {
    
    float val=rint((minTick+i*tickStep)/tickStep)*tickStep;
    if (val==0) val=0;
    
    QRect textRect=QRect(QPoint(0,0),fontMetrics().
			 size(SingleLine,QString::number(val/ordOfMag)));
    if (textRect.width()>minWidth) minWidth=textRect.width();
    
    tickValArray[i]=val;
    tickPosArray[i]=mapToViewCoords(val);
  }
  
  minWidth+=3*fontMetrics().width("0")/2+1;
  
  if (minWidth>minimumWidth() ||
      minWidth<(minimumWidth()-fontMetrics().width("00"))) 
    setFixedWidth(minWidth);
  
  label->setMagnitude(int(rint(log10(ordOfMag))));
}

void WGraphYAxis::paintEvent(QPaintEvent *e) {

  erase(e->rect());

  QPainter p(this);

  p.setClipRect(e->rect());

  int tickLen=p.fontMetrics().width("0")/2;
  
  for (int i=0; i<countTicks; i++) {

    QRect textRect=QRect(QPoint(0,0),p.fontMetrics().
			 size(SingleLine,QString::number(tickValArray[i]/ordOfMag)));
    
    int y=parentFrame->contentsRect().y()+tickPosArray[i];
    
    textRect.moveBy(0,-textRect.center().y());
    textRect.moveCenter(textRect.center()+QPoint(0,y));
    int dy;
    if ((dy=textRect.bottom()-(size().height()))>0) textRect.moveBy(0,-dy);
    if ((dy=textRect.top())<0) textRect.moveBy(0,-dy);
    
    if (pos==left) {
      int width=size().width()-1;
      p.drawLine(width,y,width-tickLen,y);
      textRect.moveBy(width-textRect.width()-2*tickLen,0);
    } else { 
      p.drawLine(0,y,tickLen,y);
      textRect.moveBy(2*tickLen+1,0);
    }
    
    p.drawText(textRect, AlignVCenter, QString::number(tickValArray[i]/ordOfMag));
  }
}

WGraphAxisLabel::WGraphAxisLabel(QWidget *parent, Orientation o, 
				 const QString& l) 
  : QWidget(parent), label(l), orientation(o), magnitude(0) { 
  
  if (orientation==horizontal) 
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
			      QSizePolicy::Fixed,false));
  else
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
			      QSizePolicy::MinimumExpanding,true));
}

QString WGraphAxisLabel::text() const {

  QString unit;
  
  if (magnitude) {
    unit="10<sup>"+QString::number(magnitude)+"</sup>";
    if (!label.isEmpty()) unit=" ["+unit+"]";
    else unit="x "+unit;
  }
  return "<center>"+label+unit+"</center>";
}

QSize WGraphAxisLabel::sizeHint() const {
  
  if (!magnitude && label.isEmpty()) return QSize(0,0);
  
  QSimpleRichText rt(text(),font());

  if (orientation==horizontal) {
    rt.setWidth(width());
    return QSize(rt.widthUsed(),rt.height()+2);
  }  else {
    rt.setWidth(height());
    return QSize(rt.height()+2,rt.widthUsed());
  }
}

void WGraphAxisLabel::paintEvent(QPaintEvent *) {
  
  QSimpleRichText rt(text(),font());
  
  QPainter p(this);

  QRect win=p.window();
  
  if (orientation==horizontal) {
    rt.setWidth(p.window().width());
    rt.draw(&p,win.top(),win.left(),QRect(),colorGroup());
  } else {
    p.rotate(-90);
    p.translate(-win.height(),0);
    rt.setWidth(p.window().height());
    rt.draw(&p,win.top(),win.left(),QRect(),colorGroup());
  }
}
