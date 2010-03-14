#include "wBarPlot.h" 

#include <qradiobutton.h>
#include <qlayout.h> 
#include <qpainter.h>

//#include "wBarPlot.moc"

WBarPlot::WBarPlot(QWidget* parent, QWidget *legendParent,
		   WGraphXAxis::Position xAxisPos,
		   WGraphYAxis::Position yAxisPos,
		   int border)
  : WGraphPlot(parent,legendParent,xAxisPos, yAxisPos,border),
    barOrigin(0), originEnabled(true), barGap(1) {}

WBarPlot::~WBarPlot() {}

void WBarPlot::writeXML(QDomElement &elem) {
  WGraphWidget::writeXML(elem);
  elem.setAttribute("originEnabled",originEnabled?"true":"false");
  elem.setAttribute("barOrigin",barOrigin);
  elem.setAttribute("barGap",barGap);
}

void WBarPlot::readXML(QDomElement &elem) {
  WGraphWidget::readXML(elem);
  originEnabled=elem.attribute("originEnabled","true")=="true";
  barOrigin=elem.attribute("barOrigin","0").toFloat();
  barGap=elem.attribute("barGap","1").toInt();
}

void WBarPlot::checkRange() {
  WRange range;
  range.resetForRangeCheck();
  for (WGraphTrace *trace=traces.first();
       trace !=0; trace=traces.next()) 
    if (trace->isVisible()) range|=trace->checkRange();

  if (originEnabled) range.adjust(barOrigin);

  range.expand(0.1);
  blockSignals(true);
  if (!getYDataRange().comp(range,0.05) && !range.isEmpty()) 
    setYDataRange(range);
  blockSignals(false);
}

void WBarPlot::setXDataRange(WRange range) { 
  
  for (unsigned int i=0; i<xData.count(); i++)
    xData[i]=range.unscaleValue(float(i)/(xData.count()-1));  
  
  range.expand(0.5/traceLength());
  WGraphWidget::setXDataRange(range);
}

void WBarPlot::drawContent(QPainter &p, bool ) {

  QArray<float>::Iterator xIt, yIt;

  int leftGap, rightGap;

  if (dataPointsPerPixel()>0.25)
    leftGap=rightGap=0;
  else {
    leftGap=rightGap=barGap/2;
    leftGap+=barGap%2;
    rightGap++;
  }
  
  checkRange();
  
  if (!traces.isEmpty()) { 
    
    p.eraseRect(p.window());
      
    for (WGraphTrace *trace=traces.first();
	 trace !=0; trace=traces.next()) 
      if (trace->isVisible()) {
	p.setPen(trace->getPen());
	QRect bar;
	
	xIt=xData.begin();
	yIt=trace->getData().begin();

	QPoint p1, p2;
	p1=mapToViewCoords(*xIt-(*(xIt+1)-*xIt)/2,*yIt);
	
	for (++xIt, ++yIt; xIt!=xData.end(); ++xIt, ++yIt) {
	  
	  p2=mapToViewCoords(*xIt-(*xIt-*(xIt-1))/2,*yIt);
	  
	  bar.setCoords(p1.x(),p1.y(),p2.x(),p1.y());
	  
	  if (originEnabled) {
	    int y=mapToViewCoords(0,barOrigin).y();
	    if (y<bar.top()) bar.setTop(y);
	    else bar.setBottom(y);
	  } else bar.setBottom(p.window().bottom());
	  
	  bar.setRight(bar.right()-rightGap);
	  bar.setLeft(bar.left()+leftGap);
	  p.fillRect(bar,trace->getPen().color());
	  
	  p1=p2;
	}

	--xIt;
	--yIt;

	p2=mapToViewCoords(*xIt+(*xIt-*(xIt-1))/2,*yIt);

	bar.setCoords(p1.x(),p1.y(),p2.x(),p1.y());
	
	if (originEnabled) {
	  int y=mapToViewCoords(0,barOrigin).y();
	  if (y<bar.top()) bar.setTop(y);
	  else bar.setBottom(y);
	} else bar.setBottom(p.window().bottom());
	
	//	bar.setRight(bar.right()-1);
	bar.setRight(bar.right()-rightGap-1);
	bar.setLeft(bar.left()+leftGap);
	p.fillRect(bar,trace->getPen().color());
      }
  }
}
  

