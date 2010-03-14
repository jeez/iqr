#include "wGraphFrame.h"
#include "wGraphWidget.h"

#include <QtGui/qpainter.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFrame>
#include <QPixmap>
#include <QMouseEvent>

//#include "wGraphFrame.moc"

WGraphFrame::WGraphFrame(WGraphWidget *p) 
    : QFrame(p,""/*, ZZZ Qt::WNoAutoErase|Qt::WResizeNoErase*/),
                   
      zoomEnabled(false), autoDisableZoom(false),
      panEnabled(false),
      xGridVisible(true), yGridVisible(true),
      parent(p),
      zoomRectPen(QColor(255,255,255),0,Qt::DotLine),
      gridPen(QColor(128,128,128),0,Qt::DotLine) {

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    
//ZZZ    paintPixmap.setOptimization(QPixmap::BestOptim);
  
    setZoomCursor(Qt::CrossCursor);
    setPanCursor(Qt::SizeAllCursor);
    setFrameStyle(QFrame::Sunken|QFrame::Panel);
    setLineWidth(2);
}

void WGraphFrame::setLineWidth(int w) { 
    QFrame::setLineWidth(w); setMinimumSize(2*frameWidth()+1, 2*frameWidth()+1); 
}
void WGraphFrame::setMidLineWidth(int w) { 
    QFrame::setMidLineWidth(w); setMinimumSize(2*frameWidth()+1, 2*frameWidth()+1); 
}

void WGraphFrame::setZoomEnabled(bool zoom) {
    zoomEnabled=zoom;
    if (zoom) {
	QWidget::setCursor(Qt::CrossCursor);
    } else if (panEnabled) {
	QWidget::setCursor(Qt::OpenHandCursor);
    } else {
	unsetCursor();
    }
}

void WGraphFrame::setPanEnabled(bool pan) { 
    panEnabled=pan; 
    if (!zoomEnabled){
	if (pan) {
	    QWidget::setCursor(Qt::OpenHandCursor);
	} else unsetCursor();
    }
};

void WGraphFrame::resizeEvent(QResizeEvent *) { 
    needsRedraw=true;
    emit resized();
};

void WGraphFrame::drawGrid(QPainter &p) {

    QVector<int>::Iterator it;

    QPen oldPen=p.pen();
    p.setPen(gridPen);
  
    if (xGridVisible && parent->xAxis()->isVisible()){
	for (it=parent->xAxis()->getTickArray().begin();
	     it!=parent->xAxis()->getTickArray().end(); ++it){
	    p.drawLine(*it, p.window().top(), *it, p.window().bottom());
	}
    }
    
    if (yGridVisible && parent->yAxis()->isVisible()){
	for (it=parent->yAxis()->getTickArray().begin();
	     it!=parent->yAxis()->getTickArray().end(); ++it){
	    p.drawLine(p.window().left(), *it, p.window().right(), *it);
	}
    }
    
    p.setPen(oldPen);
}

void WGraphFrame::paintEvent(QPaintEvent *e) {

    QPainter p;

    p.begin(this);
    drawFrame(&p);
    p.end();
  
    emit aboutToDraw();

/*ZZZ
  if (paintPixmap.size()!=contentsRect().size()){
  paintPixmap.resize(contentsRect().size());
  }
*/  

//ZZZ    if (p.begin(&paintPixmap,this)) {
    if (p.begin(this)) {

//ZZZ	p.setRasterOp(XorROP);
//zzz NOT NEEDED??	p.setCompositionMode(QPainter::CompositionMode_Xor);

	if (oldZoomRect.normalized().isValid()) {
	    p.setPen(zoomRectPen);
	    p.drawRect(oldZoomRect);
	}

//ZZZ	p.setRasterOp(CopyROP);
//zzz NOT NEEDED??	p.setCompositionMode(QPainter::CompositionMode_SourceOver);

	parent->drawContent(p, e->erased() || needsRedraw);

	needsRedraw=false;
	drawGrid(p);

//ZZZ	p.setRasterOp(XorROP);
	p.setCompositionMode(QPainter::CompositionMode_Xor);

	if (zoomRect.normalized().isValid()) {
	    p.setPen(zoomRectPen);
	    p.drawRect(zoomRect);
	}
	oldZoomRect=zoomRect;

//zzz this seems unneeded now...	p.drawPixmap (contentsRect(), paintPixmap, paintPixmap.rect());
	p.end();



//ZZZ	bitBlt(this,contentsRect().topLeft(),&paintPixmap);



    }
}


void WGraphFrame::mousePressEvent(QMouseEvent *e) {
    if (zoomEnabled && (e->button()==Qt::LeftButton)) {
	zoomStart=e->pos()-contentsRect().topLeft();
	zoomRect=QRect(zoomStart,QSize(0,0));
	update();
    } else {
	e->ignore();
	if (panEnabled && (e->button()==Qt::LeftButton)) {
	    parent->mapFromViewCoords(e->pos()-contentsRect().topLeft(), panStartX,panStartY);
	}
    }
}

void WGraphFrame::mouseReleaseEvent(QMouseEvent *e) {
    if (zoomEnabled && (e->button()==Qt::LeftButton)) {
	if (zoomRect.isNull()) {
	    parent->resetZoom();
	    emit zoomReset();
	} else {
	    zoomRect=zoomRect.normalized();
	    float x1,x2,y1,y2;
	    parent->mapFromViewCoords(zoomRect.bottomLeft(),x1,y1);
	    parent->mapFromViewCoords(zoomRect.topRight(),x2,y2);
      
	    parent->setAutoScale(false);

	    parent->setXVisRange(WRange(x1,x2));
	    parent->setYVisRange(WRange(y1,y2));
	    emit zoomed();
	    zoomRect=QRect();
	}
	update();
    } else if (panEnabled && (e->button()==Qt::LeftButton)) {
	QWidget::setCursor(Qt::OpenHandCursor);
    } else { 
	e->ignore();
    }
}

void WGraphFrame::mouseMoveEvent(QMouseEvent *e) {
    if (zoomEnabled && (e->buttons() & Qt::LeftButton)) {
	zoomRect=QRect(zoomStart,e->pos()-contentsRect().topLeft()).normalized();
	zoomRect.setTop((zoomRect.top()<0?0:zoomRect.top()));
	zoomRect.setLeft((zoomRect.left()<0?0:zoomRect.left()));
	zoomRect.setBottom((zoomRect.bottom()>(contentsRect().height()-1)? (contentsRect().height()-1):zoomRect.bottom()));
	zoomRect.setRight((zoomRect.right()>(contentsRect().width()-1)? (contentsRect().width()-1):zoomRect.right()));
    
	//     if (zoomRect.top()<0) zoomRect.setTop(0);
	//     if (zoomRect.bottom()>contentsRect().height()) zoomRect.setBottom()
    
	update();
    } else if (panEnabled && (e->buttons() & Qt::LeftButton)) {
	float x,y;
	parent->mapFromViewCoords(e->pos()-contentsRect().topLeft(),x,y);
	WRange rx=parent->getXVisRange()+panStartX-x;
	WRange ry=parent->getYVisRange()+panStartY-y;
	
	bool bx=rx.adjustToFit(parent->getXDataRange());
	bool by=ry.adjustToFit(parent->getYDataRange());
	
	if (bx && by) QWidget::setCursor(Qt::ForbiddenCursor);
	else if (bx) QWidget::setCursor(Qt::SizeVerCursor);
	else if (by) QWidget::setCursor(Qt::SizeHorCursor);
	else {
	    QWidget::setCursor(Qt::OpenHandCursor);    
	    parent->setXVisRange(rx);
	    parent->setYVisRange(ry);
	    update();
	}    
    } else e->ignore();
}

void WGraphFrame::writeXML(QDomElement &elem) {
    QDomElement frame=elem.ownerDocument().createElement("frame");
    elem.appendChild(frame);
  
    frame.setAttribute("zoomEnabled",zoomEnabled?"true":"false");
    frame.setAttribute("autoDisableZoom",autoDisableZoom?"true":"false");
    frame.setAttribute("panEnabled",panEnabled?"true":"false");
    frame.setAttribute("xGridVisible",xGridVisible?"true":"false");
    frame.setAttribute("yGridVisible",yGridVisible?"true":"false");
}

void WGraphFrame::readXML(QDomElement &elem) {
  
    QDomElement frame=elem.elementsByTagName("frame").item(0).toElement();
    if (frame.isNull()) {
	return;
    }

    zoomEnabled=frame.attribute("zoomEnabled")=="true";
    autoDisableZoom=frame.attribute("autoDisableZoom")=="true";
    panEnabled=frame.attribute("panEnabled")=="true";
    xGridVisible=frame.attribute("xGridVisible")=="true";
    yGridVisible=frame.attribute("yGridVisible")=="true";
    setZoomEnabled(zoomEnabled);
    setPanEnabled(panEnabled);
}
