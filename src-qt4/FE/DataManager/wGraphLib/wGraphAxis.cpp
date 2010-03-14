#include "wGraphAxis.h"

#include <QtGui/qpainter.h>
#include <QtGui/qbitmap.h>

#include <QTextDocument>
#include <QTextEdit>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QFontMetrics>

#include "wRangeEdit.h"

#include <algorithm>


using std::max;
using std::min;

WGraphAxis::WGraphAxis(QWidget *parent, WGraphFrame *frame) : QWidget(parent),
							      dir(1), baseTick(0), panEnabled(true), autoScale(true),
							      expandOnly(false), panCursor(Qt::PointingHandCursor), 
							      parentFrame(frame) {
    
    QFont f=font();
    f.setPointSize(int(rint((0.8*f.pointSize()))));
    setFont(f);
    
    popupMenu = new QMenu(this);
    
    
    qactAutoScale = new QAction("auto scale", this);
    qactAutoScale->setCheckable(true);
    qactAutoScale->setChecked(true);
    
    connect(qactAutoScale, SIGNAL(triggered()), this, SLOT(toggleAutoScale()));
    popupMenu->addAction(qactAutoScale);
    
    qactExpandOnly = new QAction("expand only", this);
    qactExpandOnly->setCheckable(true);
    
    connect(qactExpandOnly, SIGNAL(triggered()), this, SLOT(toggleExpandOnly()));
    popupMenu->addAction(qactExpandOnly);
    
    rangeEdit = new WRangeEdit(this);
    qwaRangeEdit = new QWidgetAction( this);
    qwaRangeEdit->setDefaultWidget ( rangeEdit);
    popupMenu->addAction(qwaRangeEdit);
							          
    connect(this,SIGNAL(visRangeChanged(WRange)), rangeEdit,SLOT(setRange(WRange))); 
    connect(rangeEdit,SIGNAL(rangeChanged(WRange)), this,SLOT(setVisRange(WRange)));    
    connect(popupMenu,SIGNAL(aboutToShow()),this,SLOT(prepPopup()));
							      }


void WGraphAxis::setAutoScale(bool b) {
    if (autoScale=b) {
	setDataRange(dataRange);
    }
}

void WGraphAxis::setExpandOnly(bool b) {
    if (!(expandOnly=b)) {
	setDataRange(dataRange);
    }
}

void WGraphAxis::setVisRange(WRange r) {  
  
    if (!r.valid() || visRange==r) {
	return;
    }

    visRange=r;
  
    emit visRangeChanged();
    emit visRangeChanged(visRange);
  
    calcTickArrays();
    update();
    parentFrame->update();
}

void WGraphAxis::setDataRange(WRange r) {
    if (!r.valid()) {
	return;
    }

    dataRange=r;

    if (autoScale) {
	if (!expandOnly) { 
	    setVisRange(dataRange);
	} else {
	    setVisRange(WRange(min(visRange.min(),dataRange.min()), max(visRange.max(),dataRange.max())));
	}
    }
}

void WGraphAxis::mousePressEvent(QMouseEvent *e) {
    if (e->button()==Qt::LeftButton) {
	if (panEnabled) {
	    setCursor(Qt::OpenHandCursor);
	    panStart=mapFromViewCoords(e->pos());
	}
    } else if (e->button()==Qt::RightButton) {
	popupMenu->popup(e->globalPos(),1);
    }
    else e->ignore();
}

void WGraphAxis::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button()==Qt::LeftButton){
	unsetCursor();
    } else {
	e->ignore();
    }
}

void WGraphAxis::mouseMoveEvent(QMouseEvent *e) {
    if (panEnabled && (e->buttons()&Qt::LeftButton)) { 
	visRange+=panStart-mapFromViewCoords(e->pos());
	if (visRange.adjustToFit(dataRange)) {
	    setCursor(Qt::ForbiddenCursor);
	}
	else {
	    setCursor(Qt::OpenHandCursor);
    
	    emit visRangeChanged();
	    emit visRangeChanged(visRange);
	    update();
	    parentFrame->update();
	}
    } else {
	e->ignore();
    }
}

void WGraphAxis::calcTicks(int maxCountTicks) {
  
    const float tickStepList[] = {0.5, 1.0, 2.0, 2.5, 5.0};

    tickStep=visRange.interval()/maxCountTicks;
    if (tickStep>baseTick) {
	ordOfMag=pow(10.0,rint(log10(tickStep)));
	tickStep/=ordOfMag;
	int bestFit=0;
	float maxFit=FLT_MAX;
    
	for (int i=0; i<5; i++){
	    if (((tickStepList[i]-tickStep)>=0) &&
		((tickStepList[i]-tickStep)<maxFit) &&
		((baseTick==0) || (ordOfMag*tickStepList[i]/baseTick==
				   rint(ordOfMag*tickStepList[i]/baseTick)))) {
		bestFit=i;
		maxFit=tickStepList[i];
	    }
	}
	tickStep=tickStepList[bestFit]*ordOfMag;
	
    } else {
	tickStep=baseTick;
    }
  
    minTick  = ceil(visRange.min()/tickStep)*tickStep;
    countTicks = int(floor(visRange.max()/tickStep)- ceil(visRange.min()/tickStep))+1;
    ordOfMag=max(fabs(visRange.min()), fabs(visRange.max()));
    ordOfMag=pow(10.0,rint(log10(ordOfMag)));

    if (!label->isVisible() || fabs(rint(log10(ordOfMag)))<=3) {
	ordOfMag=1;
    }
}

void WGraphAxis::prepPopup() {
    qactAutoScale->setChecked(isAutoScale());
    qactExpandOnly->setChecked(expandOnly);
    qactExpandOnly->setEnabled(isAutoScale());    
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

    setDataRange(WRange(dr.attribute("min").toFloat(), dr.attribute("max").toFloat()));
    setVisRange(WRange(vr.attribute("min").toFloat(), vr.attribute("max").toFloat()));
}

WGraphXAxis::WGraphXAxis(QWidget *parent, WGraphFrame *frame, Position p) 
    : WGraphAxis(parent,frame), pos(p) { 

    label = new WGraphAxisLabel(parent,WGraphAxisLabel::horizontal,QString(""));
    
    panCursor=Qt::SizeHorCursor; 
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
    QPainter p(this);
  
    p.setClipRect(e->rect());
  
    int tickLen=p.fontMetrics().width("0")/2;
  
    for (int i=0; i<countTicks; i++) {

	QRect textRect=QRect(QPoint(0,0),p.fontMetrics().size(Qt::TextSingleLine,QString::number(tickValArray[i]/ordOfMag)));
    	int x=parentFrame->contentsRect().x()+tickPosArray[i];
    
	textRect.translate(-textRect.center().x(),0);
	textRect.moveCenter(textRect.center()+QPoint(x,0));
	int dx;
	if ((dx=textRect.right()-size().width())>0) textRect.translate(-dx,0);
	if ((dx=textRect.left()-1)<0) textRect.translate(-dx,0);
    
	if (pos==top) {
	    int height=size().height()-1;
	    p.drawLine(x,height,x,height-tickLen);
	    textRect.translate(0,height-p.fontMetrics().height()-2*tickLen);
	} else  {
	    p.drawLine(x,0,x,tickLen);
	    textRect.translate(0,2*tickLen);
	}
    
	p.drawText(textRect, Qt::AlignVCenter, QString::number(tickValArray[i]/ordOfMag));
    }
    label->setMagnitude(int(rint(log10(ordOfMag))));
}

WGraphYAxis::WGraphYAxis(QWidget *parent, WGraphFrame *frame, Position p) 
    : WGraphAxis(parent,frame), pos(p) { 
    
    label = new WGraphAxisLabel(parent,WGraphAxisLabel::vertical,QString(""));
  
    panCursor=Qt::SizeHorCursor; 
}


void WGraphYAxis::calcTickArrays() {

    calcTicks(int(rint(size().height()/(3.0*fontMetrics().height()))));
  
    int minWidth=10;
  
    tickValArray.resize(countTicks);
    tickPosArray.resize(countTicks);
  
    for (int i=0; i<countTicks; i++) {
    
	float val=rint((minTick+i*tickStep)/tickStep)*tickStep;
	if (val==0) val=0;
    
	QRect textRect=QRect(QPoint(0,0),fontMetrics(). size(Qt::TextSingleLine, QString::number(val/ordOfMag)));
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
    QPainter p(this);

    p.setClipRect(e->rect());

    int tickLen=p.fontMetrics().width("0")/2;
  
    for (int i=0; i<countTicks; i++) {
	QRect textRect=QRect(QPoint(0,0),p.fontMetrics().size(Qt::TextSingleLine, QString::number(tickValArray[i]/ordOfMag)));
    
	int y=parentFrame->contentsRect().y()+tickPosArray[i];
    
	textRect.translate(0,-textRect.center().y());
	textRect.moveCenter(textRect.center()+QPoint(0,y));
	int dy;
	if ((dy=textRect.bottom()-(size().height()))>0) textRect.translate(0,-dy);
	if ((dy=textRect.top())<0) textRect.translate(0,-dy);
    
	if (pos==left) {
	    int width=size().width()-1;
	    p.drawLine(width,y,width-tickLen,y);
	    textRect.translate(width-textRect.width()-2*tickLen,0);
	} else { 
	    p.drawLine(0,y,tickLen,y);
	    textRect.translate(2*tickLen+1,0);
	}
    
	p.drawText(textRect,Qt::AlignVCenter, QString::number(tickValArray[i]/ordOfMag));
    }
}

WGraphAxisLabel::WGraphAxisLabel(QWidget *parent, WGraphAxisLabel::Orientation  o, const QString& l) : QWidget(parent), label(l), orientation(o), magnitude(0) { 

    if (orientation==horizontal) {
	setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    }
    else {
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    }


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
    if (!magnitude && label.isEmpty()){
	return QSize(0,0);
    }
    QFontMetrics qfm(font());
    if (orientation==horizontal) {
	return QSize(qfm.width(text()),qfm.height()+2);
    }  else {
	return QSize(qfm.height()+2,qfm.width(text()));
	
    }
}



void WGraphAxisLabel::paintEvent(QPaintEvent *) {
    QTextDocument rt;
    rt.setHtml(text());
    rt.setDefaultFont(font());

    QPainter p(this);
    QRect win=p.window();
  


    if (orientation==horizontal) {
	rt.setTextWidth(p.window().width());
	rt.drawContents (&p, QRectF(
			     QPointF(win.top(),win.left()),
			     QSizeF(width(), height())
			     ));
    } else {
	p.rotate(-90);
	p.translate(-win.height(),0);
	rt.setTextWidth(p.window().height());
	rt.drawContents (&p, QRectF(
			     QPointF(win.top(),win.left()),
			     QSizeF(height(), width())
			     ));
    }
}
