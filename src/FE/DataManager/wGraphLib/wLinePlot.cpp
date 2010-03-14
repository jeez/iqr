#include "wLinePlot.h"

#include <qpainter.h>

//#include "wLinePlot.moc"

WLinePlot::WLinePlot(QWidget* parent, QWidget *legendParent,
		     WGraphXAxis::Position xAxisPos,
		     WGraphYAxis::Position yAxisPos,
		     int border)
  : WGraphPlot(parent,legendParent,xAxisPos, yAxisPos,border),
    cursorPos(0), wrapAround(true) {
  
  connect(this,SIGNAL(aboutToDraw()),SLOT(checkRange()));
}

void WLinePlot::writeXML(QDomElement &elem) {
  WGraphPlot::writeXML(elem);
  elem.setAttribute("wrapAround",wrapAround?"true":"false");
}

void WLinePlot::readXML(QDomElement &elem) {
  WGraphPlot::readXML(elem);
  wrapAround=elem.attribute("wrapAround","true")=="true";
}

int WLinePlot::incCursorPos() { 
  if (xData.count()>0)
    return  cursorPos=(++cursorPos) % xData.count();
  else return cursorPos=0;
}

void WLinePlot::setTraceLength(unsigned int length) {
  WGraphPlot::setTraceLength(length);
  cursorPos%=length;
}

void WLinePlot::setCurrentValue(int trace, float value) {
  if ((trace>=0) && (trace<countTraces()))
    traces.at(trace)->getData()[cursorPos]=value;
  else
    qWarning("WLinePlot::setCurrentValue: index out of range");
}

void WLinePlot::drawContent(QPainter &p, bool completeRedraw) {

  completeRedraw=true;

  QArray<float>::Iterator xIt, yIt;
  
  p.eraseRect(p.window());
 
  if (!traces.isEmpty() && xData.count()>0) {
    
    if (wrapAround) {
      
      p.setPen(SolidLine);
      
      // cycle traces and draw them
      for (WGraphTrace *trace=traces.first();
	   trace !=0; trace=traces.next()) 
	if (trace->isVisible()) { 
	  
	  p.setPen(trace->getPen());
	  
	  xIt=xData.begin(); yIt=trace->getData().begin();
	  p.moveTo(mapToViewCoords(*xIt,*yIt));
	  for (++xIt, ++yIt; xIt<xData.end(); ++xIt, ++yIt)
	    p.lineTo(mapToViewCoords(*xIt,*yIt));
	}
      
      // drawing new cursor at newPos+1
      if (cursorPos<traceLength()-1) {
	int x1 = mapToViewCoords(xData[cursorPos],0).x();
	int x2 = mapToViewCoords(xData[cursorPos+1],0).x()+1;
	p.fillRect(x1,p.window().top(),x2-x1,p.window().height(),red);
      }
    } else {
      	
      float *x=xData.data(), *y;
      
      for (WGraphTrace *trace=traces.first();
	   trace !=0; trace=traces.next()) 
	if (trace->isVisible()) {
	  
	  y=trace->getData().data();
	  	  
	  p.setPen(trace->getPen());
	  p.moveTo(mapToViewCoords(x[0],y[(cursorPos+1) %
					  traceLength()]));
	  
	  for (int i=1; i<traceLength(); i++)
	    p.lineTo(mapToViewCoords(x[i],y[(i+cursorPos+1) % 
					    traceLength()]));
	}
    }
  }
}
