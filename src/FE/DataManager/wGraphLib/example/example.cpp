#include "example.hpp"

#include <stdlib.h>

#include <wGraphLegend.hpp>

#include <qlayout.h>
  
MainWin::MainWin() {

  timer = new QTimer(this);
  
  SpaceWidget *spaceWidget = new SpaceWidget();
  LineWidget  *lineWidget1 = new LineWidget();
  BarWidget   *barWidget = new BarWidget();
  
  connect(timer,SIGNAL(timeout()),spaceWidget,SLOT(update()));
  connect(timer,SIGNAL(timeout()),lineWidget1,SLOT(update()));
  connect(timer,SIGNAL(timeout()),barWidget,SLOT(update()));

  spaceWidget->show();
  lineWidget1->show();
  barWidget->show();

  timer->start(0);
}

SpaceWidget::SpaceWidget() {
  
  setCaption("Space Plot");

  plot = new WSpacePlot(QSize(25,25),FALSE,this,this);

  QHBoxLayout *boxLayout = new QHBoxLayout(this,3);

  plot->setColor(QColor(255,0,0));
  
  boxLayout->addWidget(plot,1);
  boxLayout->addWidget(plot->getColorBar());

  resize(400,400);

  xphase=yphase=0;
}

void SpaceWidget::update() {

  xphase+=0.1;
  yphase+=0.25;

  QArray<WSpacePlot::SpaceCell>::Iterator it;
  QArray<WSpacePlot::SpaceCell>::Iterator end=plot->getCellData().end();

  int x=0, y=0;

  for (it=plot->getCellData().begin(); it!=end;++it) {

    it->setValue(sin(x/float(plot->getSize().width()-1)*M_PI*3+xphase)+
		 cos(y/float(plot->getSize().height()-1)*M_PI*4+yphase));
    
    if (++x==plot->getSize().width()) { x=0; y++; }
  }  
  
  plot->updateGraph();
}


LineWidget::LineWidget() {
  
  setCaption("Line Plot");

  plot = new WLinePlot(this,this);

  QHBoxLayout *boxLayout = new QHBoxLayout(this,3);

  boxLayout->addWidget(plot);
  boxLayout->addWidget(plot->getLegend()); 
  
  plot->setXLabel("x-axis");
  plot->setYLabel("y-axis");
  
  plot->setTraceLength(100);
  plot->setWrapAround(false);
  
  plot->addTrace("sin",QColor(0,255,0));
  plot->addTrace("cos",QColor(0,0,255));
  plot->addTrace("sin+cos",QPen(QColor(255,0,0),2));
  
  resize(400,250);
}

void LineWidget::update() {

  sinPhase+=0.1651;
  cosPhase-=0.04;
  
  plot->setCurrentValue(0,sin(sinPhase));
  plot->setCurrentValue(1,cos(cosPhase));
  plot->setCurrentValue(2,sin(sinPhase)+cos(cosPhase));

  plot->incCursorPos();
  plot->updateGraph();  
}


BarWidget::BarWidget() {
  
  setCaption("Bar Plot");

  plot = new WBarPlot(this);

  QHBoxLayout *boxLayout = new QHBoxLayout(this,3);

  boxLayout->addWidget(plot);
  
  plot->setXLabel("x-axis");
  plot->setYLabel("y-axis");
  
  plot->setTraceLength(50);
  plot->setXDataRange(WRange(0,4*M_PI));
  
  plot->addTrace("noise",QColor(255,255,0));
  resize(400,250);
}


void BarWidget::update() {

  for (int t=0; t<50; t++) 
    plot->traceData(0)[t]=sin(t/49.0*4*M_PI+sinPhase)+
      0.2*(rand()/(RAND_MAX+1.0)*2-1);  
  
  plot->updateGraph();
}

