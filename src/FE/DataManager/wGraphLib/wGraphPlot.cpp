#include <qradiobutton.h>
#include <qlayout.h> 

#include <stdio.h>  

#include "wGraphPlot.h"  
#include "wGraphLegend.h"

//#include "wGraphPlot.moc"

WGraphPlot::WGraphPlot(QWidget* parent, QWidget *lp,
		       WGraphXAxis::Position xAxisPos,
		       WGraphYAxis::Position yAxisPos,
		       int border)
  : WGraphWidget(parent,xAxisPos, yAxisPos,border),
    legendParent(lp) {
  
  if (legendParent) {
    legend = new WGraphLegend(legendParent);
  } else legend=0;
  
  setBackgroundColor(black);
  
  traces.setAutoDelete(true);
  setCursor(crossCursor);
}

WGraphPlot::~WGraphPlot() { traces.clear(); }

void WGraphPlot::writeXML(QDomElement &elem) {
  WGraphWidget::writeXML(elem);

  for (QListIterator<WGraphTrace> it(traces);
       it.current(); ++it)
    (*it)->writeXML(elem);
  
  if (legend) legend->writeXML(elem);
}

void WGraphPlot::readXML(QDomElement &elem) {
  WGraphWidget::readXML(elem);
  for (QListIterator<WGraphTrace> it(traces);
       it.current(); ++it)
    (*it)->readXML(elem);
  
  if (legend) legend->readXML(elem);
}

void WGraphPlot::checkRange() {
  
  if (!traces.isEmpty()) {
    WRange range;
    range.resetForRangeCheck();
    for (WGraphTrace *trace=traces.first();
	 trace !=0; trace=traces.next()) 
      if (trace->isVisible()) range|=trace->checkRange();
    range.expand(0.1);
    if (!getYDataRange().comp(range,0.05) && !range.isEmpty()) 
      setYDataRange(range);
  }
}

WGraphTrace *WGraphPlot::addTrace(const QString &name, QPen pen) {
  WGraphTrace *trace;
  traces.append(trace = new WGraphTrace(legend,name, pen, xData.count()));
  connect(trace,SIGNAL(needRefresh()),this,SLOT(doRedraw()));
  return trace;
}


void WGraphPlot::removeTraces() {

  // no clue why this is necessary, but it doesn't work otherwise
  traces.append(new WGraphTrace(legend,"dummy",black,0)); 
  traces.clear();
  if (legend) legend->toggleExclusive(false);
}

QString WGraphPlot::getVisibleTracesStr(int maxChars) const {

  QString str;
  int count=0;
  int countChars=0;
  
  for (QListIterator<WGraphTrace> it(traces);
       it.current(); ++it)
    if ((*it)->isVisible()) {
      count++;
      if (!str.isEmpty()) str+=",";
      str+="<font color=\""+(*it)->getPen().color().name()+"\">"+(*it)->getName()+"</font>";
      countChars+=(*it)->getName().length();
    }
  
  if (countChars>maxChars && count>1) str="...";
  
  return str;
}

void WGraphPlot::setTraceLength(unsigned int newLength) {
  
  if (xData.count()!=newLength) {
    for (QListIterator<WGraphTrace> it(traces);
	 it.current(); ++it)
      (*it)->resize(newLength);
    
    xData.resize(newLength);
    for (unsigned int i=0; i<xData.count(); i++)
      xData[i]=getXDataRange().unscaleValue(float(i)/(xData.count()-1));
  } else clear();
  updateGraph();
}

void WGraphPlot::clear() {
  for (QListIterator<WGraphTrace> it(traces);
       it.current(); ++it) (*it)->clear();
  setYDataRange(WRange(-0.5,0.5));
  updateGraph();
}

void WGraphPlot::setXDataRange(WRange r) { 
  WGraphWidget::setXDataRange(r);
  for (unsigned int i=0; i<xData.count(); i++)
    xData[i]=r.unscaleValue(float(i)/(xData.count()-1));
  updateGraph();
}


float WGraphPlot::dataPointsPerPixel() { 
  return float(traceLength()*getXVisRange().interval())/
    float(frame()->contentsRect().width()*getXDataRange().interval());
}
