#include "wGraphWidget.h"

#include "wIcons.h"

#include <qlayout.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>

//#include "wGraphWidget.moc"

WGraphWidget::WGraphWidget(QWidget *parent,
			   WGraphXAxis::Position xAxisPos,
			   WGraphYAxis::Position yAxisPos,
			   int border) 
  : QWidget(parent,"",WRepaintNoErase | WPaintUnclipped) {
  
    QGridLayout *layout = new QGridLayout(this,5,5);
    
    drawingFrame = new WGraphFrame(this);
    layout->addWidget(drawingFrame,2,2);
    
    xGraphAxis = new WGraphXAxis(this,drawingFrame,xAxisPos);
    yGraphAxis = new WGraphYAxis(this,drawingFrame,yAxisPos);

    if (yAxisPos==WGraphYAxis::right) {
      layout->addWidget(yAxis(),2,3);
      layout->addWidget(yAxis()->getLabel(),2,4);
    } else {
      layout->addWidget(yAxis(),2,1);
      layout->addWidget(yAxis()->getLabel(),2,0);
    } 
    
    if (xAxisPos==WGraphXAxis::top) {
      layout->addWidget(xAxis(),1,2);
      layout->addWidget(xAxis()->getLabel(),0,2);
    } else { 
      layout->addWidget(xAxis(),3,2);
      layout->addWidget(xAxis()->getLabel(),4,2);
    }
    
    int width=xAxis()->fontMetrics().height()+xAxis()->fontMetrics().width("0")/2;

    QPixmap zoomIcon;
    zoomIcon.convertFromImage(qembed_findImage("zoom").
			      smoothScale(width,width)); 
    
    zoomButton = new QToolButton(this);
    
    layout->addMultiCellWidget(zoomButton,
			       xAxisPos==WGraphXAxis::top?0:3,xAxisPos==WGraphXAxis::top?1:4,
			       yAxisPos==WGraphYAxis::left?0:3,yAxisPos==WGraphYAxis::left?1:4,
			       (xAxisPos==WGraphXAxis::top?AlignTop:AlignBottom) |
			       (yAxisPos==WGraphYAxis::left?AlignLeft:AlignRight));
    
    zoomButton->setAutoRaise(true);
    zoomButton->setIconSet(QIconSet(zoomIcon));
    zoomButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
					  QSizePolicy::Fixed));
    zoomButton->setToggleButton(true);
    
    showGridPopup = new QPopupMenu(this);
    horizGridID=showGridPopup->insertItem("horizontal",this,
					  SLOT(toggleShowYGrid()));
    vertGridID=showGridPopup->insertItem("vertical",this,
					 SLOT(toggleShowXGrid()));
    showGridPopup->setCheckable(horizGridID);
    showGridPopup->setCheckable(vertGridID);
    
    connect(showGridPopup,SIGNAL(aboutToShow()),this,SLOT(prepShowGridPopup()));
    
    connect(drawingFrame,SIGNAL(resized()),SIGNAL(frameResized()));
    
    connect(drawingFrame,SIGNAL(zoomed()),SIGNAL(zoomed()));
    connect(drawingFrame,SIGNAL(zoomReset()),SIGNAL(zoomReset()));
    
    connect(drawingFrame,SIGNAL(aboutToDraw()),SIGNAL(aboutToDraw()));
    
    connect(xAxis(),SIGNAL(visRangeChanged()),SIGNAL(visRangeChanged()));
    connect(yAxis(),SIGNAL(visRangeChanged()),SIGNAL(visRangeChanged()));
    
    connect(xAxis(),SIGNAL(visRangeChanged(WRange)),SIGNAL(xVisRangeChanged(WRange)));
    connect(yAxis(),SIGNAL(visRangeChanged(WRange)),SIGNAL(yVisRangeChanged(WRange)));
    
    connect(zoomButton,SIGNAL(toggled(bool)),
	    drawingFrame,SLOT(setZoomEnabled(bool)));

    layout->setColStretch(2,1);
    layout->setRowStretch(2,1);
    
    layout->addColSpacing(1,border);
    layout->addColSpacing(3,border);
    layout->addRowSpacing(3,border);
    layout->addRowSpacing(1,border);
  }

void WGraphWidget::resetZoom() {
  setAutoScale(true);
  emit zoomReset();
}

QPoint WGraphWidget::mapToViewCoords(float x, float y) const {
  return QPoint(xAxis()->mapToViewCoords(x),
		yAxis()->mapToViewCoords(y));
}

void WGraphWidget::mapFromViewCoords(QPoint p, float &x, float &y) const {
  x=xAxis()->mapFromViewCoords(p.x());
  y=yAxis()->mapFromViewCoords(p.y());
}

void WGraphWidget::showXAxis(bool b) {
  xAxis()->setShown(b);
  xAxis()->getLabel()->setShown(b);
  zoomButton->setShown(xAxis()->isVisible() && yAxis()->isVisible());
}

void WGraphWidget::showYAxis(bool b) {
  yAxis()->setShown(b);
  yAxis()->getLabel()->setShown(b);
  zoomButton->setShown(xAxis()->isVisible() && yAxis()->isVisible());
}

void WGraphWidget::setXLabel(const QString &label) {
  xAxis()->getLabel()->setLabel(label);
}

void WGraphWidget::setYLabel(const QString &label) {
  yAxis()->getLabel()->setLabel(label);
}

void WGraphWidget::writeXML(QDomElement &elem) {
  xAxis()->writeXML(elem);
  yAxis()->writeXML(elem);
  drawingFrame->writeXML(elem);
}

void WGraphWidget::readXML(QDomElement &elem) {
  xAxis()->readXML(elem);
  yAxis()->readXML(elem);
  drawingFrame->readXML(elem);
  if (isZoomEnabled()) zoomButton->setOn(true);
}

void WGraphWidget::prepShowGridPopup() {
  showGridPopup->setItemChecked(horizGridID,isYGridVisible());
  showGridPopup->setItemChecked(vertGridID,isXGridVisible());
}
