#include "wGraphWidget.h"

#include <iostream>

#include <QPainter>
#include <QToolButton>
#include <QGridLayout>
#include <QPixmap>
#include <QMenu>

#include <zoom.xpm>

WGraphWidget::WGraphWidget(QWidget *parent,
			   WGraphXAxis::Position xAxisPos,
			   WGraphYAxis::Position yAxisPos,
			   int border) 
    : QWidget(parent) {
  
    setAttribute(Qt::WA_PaintUnclipped);
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins ( 0, 0, 0, 0 );
    
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
   
    zoomButton = new QToolButton(this);
    
    int iFromRow, iFromCol;
    if(xAxisPos==WGraphXAxis::top){
	iFromRow = 0;
    } else {
	iFromRow = 3;
    }

    if(yAxisPos==WGraphYAxis::left){
	iFromCol = 0;
    } else {
	iFromCol = 3;
    }

    layout->addWidget(zoomButton,
		      iFromRow, iFromCol,
		      2, 2,
		      (xAxisPos==WGraphXAxis::top?Qt::AlignTop:Qt::AlignBottom) |
		      (yAxisPos==WGraphYAxis::left?Qt::AlignLeft:Qt::AlignRight));
    
 
    
    zoomButton->setAutoRaise(true);
    zoomButton->setIcon(QPixmap(zoom));
    zoomButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
					  QSizePolicy::Fixed));
    zoomButton->setCheckable(true);
    showGridPopup = new QMenu(this);

//ZZZ    horizGridID=showGridPopup->insertItem("horizontal",this, SLOT(toggleShowYGrid()));
//ZZZ    vertGridID=showGridPopup->insertItem("vertical",this, SLOT(toggleShowXGrid()));
//ZZZ    showGridPopup->setCheckable(horizGridID);
//ZZZ    showGridPopup->setCheckable(vertGridID);
  

    qactShowYGrid = new QAction("horizontal", this);
    connect(qactShowYGrid, SIGNAL(triggered()), this, SLOT(toggleShowYGrid()));
    qactShowYGrid->setCheckable(true);
    showGridPopup->addAction(qactShowYGrid);

    qactShowXGrid = new QAction("vertical", this);
    connect(qactShowXGrid, SIGNAL(triggered()), this, SLOT(toggleShowXGrid()));
    qactShowXGrid->setCheckable(true);
    showGridPopup->addAction(qactShowXGrid);


    connect(showGridPopup,SIGNAL(aboutToShow()),this,SLOT(prepShowGridPopup()));
    
    connect(drawingFrame,SIGNAL(resized()),SIGNAL(frameResized()));
    
    connect(drawingFrame,SIGNAL(zoomed()),SIGNAL(zoomed()));
    connect(drawingFrame,SIGNAL(zoomReset()),SIGNAL(zoomReset()));
    
    connect(drawingFrame,SIGNAL(aboutToDraw()),SIGNAL(aboutToDraw()));
    
    connect(xAxis(),SIGNAL(visRangeChanged()),SIGNAL(visRangeChanged()));
    connect(yAxis(),SIGNAL(visRangeChanged()),SIGNAL(visRangeChanged()));
    
    connect(xAxis(),SIGNAL(visRangeChanged(WRange)),SIGNAL(xVisRangeChanged(WRange)));
    connect(yAxis(),SIGNAL(visRangeChanged(WRange)),SIGNAL(yVisRangeChanged(WRange)));
    
    connect(zoomButton,SIGNAL(toggled(bool)), drawingFrame,SLOT(setZoomEnabled(bool)));

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
  if (isZoomEnabled()){
//ZZZ      zoomButton->setOn(true);
      zoomButton->setChecked(true);
  }
}

void WGraphWidget::prepShowGridPopup() {
  qactShowYGrid->setChecked(isYGridVisible());
  qactShowXGrid->setChecked(isXGridVisible());
}
