#include "wSpacePlot.h"

#include <QColor>
#include <QToolTip>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>

#include <stdio.h>
#include <iostream>

#include "wMath.h"

//#include "wSpacePlot.moc"

WSpacePlot::WSpacePlot(QSize size, bool dg, 
		       QWidget* parent, QWidget *colorBarParent,
		       WGraphXAxis::Position xAxisPos,
		       WGraphYAxis::Position yAxisPos,
		       int border)
    : WGraphWidget(parent,xAxisPos,yAxisPos,border),
      paintColor(Qt::red), dataRange(0.0, 1.0),
      recRects(0),
      colorBar(0), startCell(0,0), currCell(0,0), 
      selecting(true), dragEnabled(false), drawGrid(dg), 
      autoScale(true), multiSelect(true) {
    
    if (colorBarParent) {
	colorBar = new WColorBar(colorBarParent);
	connect(this,SIGNAL(colorChanged(QColor)),
		colorBar,SLOT(setColor(QColor)));
	connect(colorBar,SIGNAL(rangeSelected(WRange)),
		this,SLOT(selectRange(WRange)));
	connect(colorBar,SIGNAL(yVisRangeChanged(WRange)),
		this,SLOT(setDataRange(WRange)));
	colorBar->show();
    } 
    
    setCursor(Qt::CrossCursor);

    if (drawGrid) {
	QPalette palette;
	palette.setColor(this->backgroundRole(), Qt::white);
	this->setPalette(palette);
    }    
    setFocusPolicy(Qt::TabFocus);
    
    xAxis()->setBaseTick(1.0);
    yAxis()->setBaseTick(1.0);
    
    showGrid(false);
    
    setSize(size);
    
    autoPanTimer = new QTimer(this);
    labelClearTimer = new QTimer(this);
    
    qrbSelection = new QRubberBand(QRubberBand::Rectangle, this);

    connect(frame(),SIGNAL(aboutToDraw()),this,SLOT(checkDataRange()));
    connect(autoPanTimer,SIGNAL(timeout()),this,SLOT(autoPan()));
//--    connect(labelClearTimer,SIGNAL(timeout()),this,SLOT(clearCellLabel()));
    connect(this,SIGNAL(frameResized()),SLOT(calcGeometry()));
    connect(this,SIGNAL(visRangeChanged()),SLOT(calcGeometry()));
}

WSpacePlot::~WSpacePlot() {
    delete[] recRects;
}

void WSpacePlot::writeXML(QDomElement &elem) {
    WGraphWidget::writeXML(elem);
    if (!colorBar){
	elem.setAttribute("autoScale",autoScale?"true":"false");
    }
    elem.setAttribute("multiSelect",multiSelect?"true":"false");
    elem.setAttribute("min",dataRange.min());
    elem.setAttribute("max",dataRange.max());
}

void WSpacePlot::readXML(QDomElement &elem) {
    WGraphWidget::readXML(elem);
    autoScale=elem.attribute("autoScale","true")=="true";
    multiSelect=elem.attribute("multiSelect")=="true";
    dataRange=WRange(elem.attribute("min").toFloat(), elem.attribute("max").toFloat());
}

void WSpacePlot::calcGeometry() {

    QVector<SpaceCell>::Iterator it=cellData.begin();

    for(int y=0; y < viewSize.height(); y++)
	for (int x=0; x < viewSize.width(); x++) {
	    it->rect=QRect(mapToViewCoords(x-0.5,y+0.5), mapToViewCoords(x+0.5,y-0.5)).normalized();
	    it->rect.setSize(it->rect.size()-QSize(1,1));
	    it->visible=it->rect.intersects(QRect(QPoint(0,0), getFrameSize()));
	    ++it;
	}
}


void WSpacePlot::setSize(QSize size) {

    viewSize=size;
    
    cellData.resize(viewSize.width()*viewSize.height());
    clearCells();
  
    setXDataRange(WRange(-0.5,viewSize.width()-0.5));
    setYDataRange(WRange(-0.5,viewSize.height()-0.5));

    calcGeometry();
//--    clearCellLabel();

    currCell=startCell=QPoint(0,0);
}

void WSpacePlot::setCellLabel(QPoint pos, QPoint cell) {
//    std::cout << "WSpacePlot::setCellLabel(QPoint pos, QPoint cell)" << std::endl;

    QString cellLabelString;
    cellLabelString.sprintf("%d:%d",cell.x(),cell.y());
    QToolTip::showText(pos + mapToGlobal(QPoint(0,0)), cellLabelString);

    qrbSelection->setGeometry(QRect( pos, QSize()));
    qrbSelection->show();
}

void WSpacePlot::selectRectTmp(const QRect &rect, bool select) {
//    std::cout << "WSpacePlot::selectRectTmp(const QRect &rect, bool select)" << std::endl;
    QRect r=rect.normalized();

    qrbSelection->setGeometry(r);

    if (select){
	for (int x=r.left(); x<=r.right(); x++){
	    for (int y=r.top(); y<=r.bottom(); y++){
		cellData[x+y*viewSize.width()].attr|=tmpSelected;
	    }
	}
    } else {
	for (int x=r.left(); x<=r.right(); x++) {
	    for (int y=r.top(); y<=r.bottom(); y++){
		cellData[x+y*viewSize.width()].attr&=normal|marked;
	    }
	}
    }
}

void WSpacePlot::clearTmpSelected() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin();
	 it!=cellData.end();++it)
	if (it->attr & tmpSelected) it->attr&=~tmpSelected;
}

void WSpacePlot::acceptTmpSelected() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it){
	if (it->attr&tmpSelected) {
	    if (it->attr&normal) { 
		it->attr|=selected; 
	    } else {
		it->attr=0;
	    }
	}
    }
    emit selectionChanged();
}

void WSpacePlot::clearSelection() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it){
	it->attr&=normal|marked;
    }
    emit selectionChanged();
}

void WSpacePlot::invertSelection() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it){
	if (it->attr&normal){
	    if (it->attr&selected){ 
		it->attr&=~selected; 
	    } else {
		it->attr|=selected;
	    }
	}
    }
    emit selectionChanged();
}

int WSpacePlot::countSelectedCells() {
    int count=0;
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it){
	if ((it->attr & selected) && (it->attr & normal)) { 
	    count++;
	}
    }
    return count;
}

QVector<int> &WSpacePlot::getSelecIndexList() {

    selecIndexList.resize(countSelectedCells());
    int listIndex=0;
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it){
	if ((it->attr & selected) && (it->attr & normal)) {
	    selecIndexList[listIndex++]=it-cellData.begin();
	}
    }
    return selecIndexList;
}

void WSpacePlot::setSelecIndexList(QVector<int> &list) {

    clearSelection();
    for (int i=0; i<list.count(); i++) {
	cellData[list[i]].attr|=selected;
    }
   
    selecIndexList=list;
}

void WSpacePlot::setMultiSelect(bool b) {
    multiSelect=b;
    clearSelection();
    emit selectionChanged();
}

void WSpacePlot::selectRange(WRange r) {
    for (QVector<SpaceCell>::Iterator it=cellData.begin();
	 it!=cellData.end();++it)
	if (it->attr&normal && r.contains(it->data)){
	    it->attr|=selected;
	} else {
	    it->attr&=~selected;
	}
    updateGraph();
    emit selectionChanged();
}

void WSpacePlot::markSelected() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it)
	if (it->attr & selected) {
	    it->attr|=marked;
	} else {
	    it->attr&=~marked;
	}
    updateGraph();
}

void WSpacePlot::exchgMarkedSelected() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it) {
    
	if (it->attr & selected && !(it->attr & marked)) {
	    it->attr&=~selected;
	    it->attr|=marked;
	} else if (!(it->attr & selected) && (it->attr & marked)) {
	    it->attr&=~marked;
	    it->attr|=selected;
	}
    }
    updateGraph();
    emit selectionChanged();
}

void WSpacePlot::clearMarked() {
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end();++it) it->attr&=~marked;
    updateGraph();
}

void WSpacePlot::mousePressEvent(QMouseEvent *e) {
//    std::cout << "WSpacePlot::mousePressEvent(QMouseEvent *e)" << std::endl;
    if (e->buttons() & Qt::LeftButton) {
	QPoint pos=frame()->mapFrom(this,e->pos())-frame()->contentsRect().topLeft();
	currCursorPos=pos;
	float x,y;
	mapFromViewCoords(pos,x,y);
	QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()), clampi(int(rint(y)),0,viewSize.height()));
	int index=cell.x()+cell.y()*viewSize.width();
	if (multiSelect) {
	    if (e->modifiers() & Qt::ShiftModifier) {
		clearTmpSelected();
		selectRectTmp(QRect(startCell,cell),selecting);
	    } else if (e->modifiers() & Qt::ControlModifier) {
		startCell=cell;
		acceptTmpSelected();
		selecting=!(cellData[index].attr & selected);
		selectRectTmp(QRect(startCell,cell),selecting);
	    } else {
		startCell=cell;
		if ((cellData[index].attr & selected) && (cellData[index].attr & normal))
		    dragEnabled=true;
		else {
		    clearSelection();
		    selecting=true;
		    selectRectTmp(QRect(startCell,cell),selecting);
		}
		e->ignore();
	    }
	}
	else {
	    clearSelection();
	    selectRectTmp(QRect(cell,cell),true);
	    emit selectionChanged();
	    e->ignore();
	}
	setCellLabel(pos,cell);
	currCell=cell;
    } else {
	e->ignore();
    }
}

void WSpacePlot::mouseReleaseEvent(QMouseEvent *e) {
//    std::cout << "WSpacePlot::mouseReleaseEvent(QMouseEvent *e)" << std::endl;

    qrbSelection->hide();

    if (e->buttons()==Qt::LeftButton) {
	if (multiSelect) {
	    if (dragEnabled) {
		dragEnabled=false;
		clearSelection();
		selectRectTmp(QRect(startCell,startCell),true);
	    }
	    emit selectionChanged();
	}
	autoPanTimer->stop();
	labelClearTimer->setSingleShot(true);
	labelClearTimer->start(1000);
    } else { 
	e->ignore();
    }
}

void WSpacePlot::mouseMoveEvent(QMouseEvent *e) {
//    std::cout << "WSpacePlot::mouseMoveEvent(QMouseEvent *e)" << std::endl;
    if (e->buttons() & Qt::LeftButton) { 
	QPoint pos=frame()->mapFrom(this,e->pos())-frame()->contentsRect().topLeft();
	currCursorPos=pos;
    
	float x,y;
	mapFromViewCoords(pos,x,y);
	QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()), clampi(int(rint(y)),0,viewSize.height()));
    
	if (dragEnabled) {
	    dragEnabled=false;
//--	    clearCellLabel();
	    emit wouldDrag();
	} else {
	    if (!QRect(QPoint(0,0),getFrameSize()).contains(pos)) {
		if (!autoPanTimer->isActive()) {
		    autoPanTimer->start(1);
		}
	    } else {
		autoPanTimer->stop();
		if (cell!=currCell) {
		    if (multiSelect) {
			clearTmpSelected();
			selectRectTmp(QRect(startCell,cell),selecting);
		    } else {
			clearSelection();
			selectRectTmp(QRect(cell,cell),true);
			emit selectionChanged();
		    }
		}
		setCellLabel(pos,cell);
	    }
	}
    
	currCell=cell;

	updateGraph();
    
    } else e->ignore(); 
}

void WSpacePlot::keyPressEvent(QKeyEvent *e) {

    switch (e->key()) {
    case Qt::Key_Up: 
	if (currCell.y()<viewSize.height()-1) currCell+=QPoint(0,1);
	break;
    case Qt::Key_Down:
	if (currCell.y()>0) currCell-=QPoint(0,1);
	break;
    case Qt::Key_Right:
	if (currCell.x()<viewSize.width()-1) currCell+=QPoint(1,0);
	break;
    case Qt::Key_Left: 
	if (currCell.x()>0) currCell-=QPoint(1,0);
	break;
    case Qt::Key_End: currCell.setX(viewSize.width()-1); break;
    case Qt::Key_Home: currCell.setX(0); break;
    case Qt::Key_PageUp: currCell.setY(viewSize.height()-1); break;
    case Qt::Key_PageDown: currCell.setY(0); break;
    case Qt::Key_Space:
	if (multiSelect) {
	    if (!(e->modifiers() & Qt::ControlModifier)) clearSelection();
	    if (!(e->modifiers() & Qt::ShiftModifier)) startCell=currCell;
	    int index=currCell.x()+currCell.y()*viewSize.width();
	    selectRectTmp(QRect(startCell,currCell), !(cellData[index].attr & selected));
	    emit selectionChanged();
	}
	break;
    default: e->ignore(); return;
    }
  
    e->accept();
  
    if (!multiSelect) {
	clearSelection();
	selectRectTmp(QRect(currCell,currCell),true);
	emit selectionChanged();
    } 
  
    updateGraph();
}

void WSpacePlot::checkDataRange() {

    if (isAutoScale()) {
    
	WRange autoRange;
    
	autoRange.resetForRangeCheck();
    
	QVector<SpaceCell>::Iterator it;
	QVector<SpaceCell>::Iterator end=cellData.end();
    
	for (it=cellData.begin(); it!=end; ++it){
	    if ((it->attr & normal) && finite(it->data)){ 
		autoRange.adjust(it->data);
	    }
	}
	if (!autoRange.isEmpty() && dataRange!=autoRange) {
	    if (colorBar) {
		colorBar->setYDataRange(autoRange);
		dataRange=colorBar->getYVisRange();
	    } else dataRange = autoRange;
	}
    }
}

void WSpacePlot::drawContent(QPainter &p,bool) {
//std::cout << "WSpacePlot::drawContent(QPainter &p,bool)" << std::endl;

    QBrush brush;
    QColor color;
    int hue, selectHue; //s, v;
  
//ZZZ    paintColor.getHsv(hue,s,v);
//ZZZ    palette().highlight().color().getHsv(selectHue,s,v); //ZZZ FIX??
  
    QVector<SpaceCell>::Iterator it;
    QVector<SpaceCell>::Iterator end=cellData.end();

    if (drawGrid) {
	p.eraseRect(p.window());
	for (it=cellData.begin(); it!=end; ++it) {
	    if (it->attr & selected) {
		p.fillRect(it->rect,palette().highlight().color()); //ZZZ FIX?
		p.setPen(palette().highlight().color().light()); //ZZZ FIX?
	    } else p.setPen(palette().mid().color());
	    p.drawRect(it->rect);
	} 
    } else {   
	p.setBackgroundMode(Qt::OpaqueMode);
	QPalette palette;
	palette.setColor(backgroundRole(),  QColor(64,64,64));
	setPalette(palette);
	p.fillRect(p.window(),QBrush(Qt::black,Qt::Dense6Pattern));


	for (it=cellData.begin(); it!=end; ++it){
	    if (it->visible) {
		if (it->attr&normal) {
		    float val=clampf(dataRange.scaleValue(it->data));
		    if (finite(val)) {
			if (it->attr & selected) {
			    color.setHsv(selectHue,128,int((0.5+0.5*val)*255));
			} else {
			    color.setHsv(hue,255,int(val*255));
			}
			p.fillRect(it->rect,color);	
			if (it->attr&marked) {
			    if (it->attr&selected) {
				color.setHsv(hue,255,int(val*255));
			    } else {
				color.setHsv(selectHue,128,int((0.5+0.5*val)*255));	    
			    }
			    p.setPen(color);
			    p.drawRect(it->rect);
			}
		    } else {
			p.fillRect(it->rect,QBrush(paintColor,Qt::DiagCrossPattern));
		    }
		}
	    } 
	}
    }
  
    if (hasFocus() && QRect(QPoint(0,0),viewSize).contains(currCell)) {
//ZZZ	p.setRasterOp(XorROP);
	p.setCompositionMode(QPainter::CompositionMode_Xor);

	p.setPen(Qt::white);
	p.drawRect(cellData[currCell.x()+currCell.y()*viewSize.width()].rect);
//ZZZ	p.setRasterOp(CopyROP);
	p.setCompositionMode(QPainter::CompositionMode_Source /*zzz ???*/);
//	p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }
  
}

void WSpacePlot::clearCells(char attrMask) { 
    for (QVector<SpaceCell>::Iterator it=cellData.begin(); it!=cellData.end(); ++it) {
	it->setValue(0);
	it->attr&=attrMask; 
	it->attr|=normal;
    }
    emit selectionChanged();
}

void WSpacePlot::autoPan() {
    float x,y;
    mapFromViewCoords(currCursorPos,x,y);

    QPoint cell=QPoint(clampi(int(rint(x)),0,viewSize.width()), clampi(int(rint(y)),0,viewSize.height()));
  
    setCellLabel(currCursorPos,cell);

    float dx=getXVisRange()>x?getXVisRange().min()-x: (getXVisRange()<x?getXVisRange().max()-x:0);
    float dy=getYVisRange()>y?getYVisRange().min()-y: (getYVisRange()<y?getYVisRange().max()-y:0);

    if (dx) {
	WRange vrx=getXVisRange()-dx*0.05;
	vrx.adjustToFit(getXDataRange());
	setXVisRange(vrx);
    }
    
    if (dy) {
	WRange vry=getYVisRange()-dy*0.05;
	vry.adjustToFit(getYDataRange());
	setYVisRange(vry);
    }
    
    if (multiSelect) {
	clearTmpSelected();
	selectRectTmp(QRect(startCell,cell),selecting);
    } else {
	clearSelection();
	selectRectTmp(QRect(cell,cell),true);
	emit selectionChanged();
    }
}
