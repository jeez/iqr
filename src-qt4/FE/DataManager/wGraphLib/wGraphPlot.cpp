#include <QRadioButton>
#include <QLayout> 

#include <stdio.h>  

#include "wGraphPlot.h"  
#include "wGraphLegend.h"

//#include "wGraphPlot.moc"

WGraphPlot::WGraphPlot(QWidget* parent, QWidget *lp,
		       WGraphXAxis::Position xAxisPos,
		       WGraphYAxis::Position yAxisPos,
		       int border)
    : WGraphWidget(parent,xAxisPos, yAxisPos,border), legendParent(lp) {
  
    if (legendParent) {
	legend = new WGraphLegend(legendParent);
    } else {
	legend=0;
    }
  
    QPalette palette;
    palette.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(palette);
  
//ZZZ FIX    traces.setAutoDelete(true);
    setCursor(Qt::CrossCursor);
}

WGraphPlot::~WGraphPlot() { traces.clear(); }

void WGraphPlot::writeXML(QDomElement &elem) {
    WGraphWidget::writeXML(elem);

    QList<WGraphTrace*>::iterator it;
    for (it=traces.begin(); it!=traces.end(); it++) {
	(*it)->writeXML(elem);
    }
  
    if (legend) {
	legend->writeXML(elem);
    }
}

void WGraphPlot::readXML(QDomElement &elem) {
    WGraphWidget::readXML(elem);
    QList<WGraphTrace*>::iterator it;
    for (it=traces.begin(); it!=traces.end(); it++) {
	(*it)->readXML(elem);
    }
  
    if (legend) { 
	legend->readXML(elem);
    }
}

void WGraphPlot::checkRange() {
  
    if (!traces.isEmpty()) {
	WRange range;
	range.resetForRangeCheck();
	
	QList<WGraphTrace*>::iterator it;
	for (it=traces.begin(); it!=traces.end(); it++) {
//--	    WGraphTrace *trace= *it;
	    if ((*it)->isVisible()){ 
		range|=(*it)->checkRange();
	    }
	}
	range.expand(0.1);
	if (!getYDataRange().comp(range,0.05) && !range.isEmpty()) {
	    setYDataRange(range);
	}
    }
}

WGraphTrace *WGraphPlot::addTrace(const QString &name, QPen pen) {
    WGraphTrace *trace = new WGraphTrace(legend,name, pen, xData.count());
    traces.append(trace);
    connect(trace,SIGNAL(needRefresh()),this,SLOT(doRedraw()));
    return trace;
}


void WGraphPlot::removeTraces() {

    // no clue why this is necessary, but it doesn't work otherwise
//ZZZ    traces.append(new WGraphTrace(legend,"dummy",Qt::black,0)); 
    traces.clear();
    if (legend) legend->toggleExclusive(false);
}

QString WGraphPlot::getVisibleTracesStr(int maxChars) const {

    QString str;
    int count=0;
    int countChars=0;
  
//XXXX
//    QList<WGraphTrace>::iterator it;
//    for (it=traces.begin(); it!=traces.end(); it++) {
//	if ((*it)->isVisible()) {
//	    count++;
//	    if (!str.isEmpty()) str+=",";
//	    str+="<font color=\""+(*it)->getPen().color().name()+"\">"+(*it)->getName()+"</font>";
//	    countChars+=(*it)->getName().length();
//	}
//    }
  
    if (countChars>maxChars && count>1) { str="...";} 
  
    return str;
}

void WGraphPlot::setTraceLength(unsigned int newLength) {
  
     if (xData.count()!=newLength) {
	QList<WGraphTrace*>::iterator it;
	for (it=traces.begin(); it!=traces.end(); it++) {
	    (*it)->resize(newLength);
	}
    
	xData.resize(newLength);
	for (int i=0; i<xData.count(); i++){
	    xData[i]=getXDataRange().unscaleValue(float(i)/(xData.count()-1));
	}
    } else { 
	clear();
    }
    updateGraph();
}

void WGraphPlot::clear() {
    QList<WGraphTrace*>::iterator it;
    for (it=traces.begin(); it!=traces.end(); it++) {
	(*it)->clear();
    }
    setYDataRange(WRange(-0.5,0.5));
    updateGraph();
}

void WGraphPlot::setXDataRange(WRange r) { 
    WGraphWidget::setXDataRange(r);
    for (int i=0; i<xData.count(); i++){
	xData[i]=r.unscaleValue(float(i)/(xData.count()-1));
    }
    updateGraph();
}


float WGraphPlot::dataPointsPerPixel() { 
    return float(traceLength()*getXVisRange().interval())/
	float(frame()->contentsRect().width()*getXDataRange().interval());
}
