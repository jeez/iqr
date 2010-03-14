#include "wGraphTrace.h"
#include "wGraphLegend.h"

#include <QtGui/qcheckbox.h>  
#include <QtGui/qradiobutton.h>
#include <QtGui/qlayout.h>

//#include "wGraphTrace.moc"

WGraphTrace::WGraphTrace(WGraphLegend *l, const QString &n, 
			 QPen p, int length) 
    : QObject(), name(n), pen(p), legend(l) {
  
    resize(length);
    visible=true;
  
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
  
    if (legend) {
    	legend->hide();
//zzz	legend->constPolish();
    
	checkButton = new QCheckBox(name,NULL /*legend->checkGroup*/);
	legend->addCheckBox(checkButton);

	radioButton = new QRadioButton(name, NULL /*legend->radioGroup*/);
	legend->addRadioButton(radioButton);

    
	QPalette p=checkButton->palette();
	p.setColor(QPalette::Normal,QPalette::Foreground,pen.color());
	p.setColor(QPalette::Inactive,QPalette::Foreground,pen.color());
	checkButton->setPalette(p);
	radioButton->setPalette(p);
    
	checkButton->setChecked(isVisible());

/*THIS NEEDS FIXING
	if (legend->radioGroup->id(radioButton)==0) {
	    radioButton->setChecked(true);
	}
*/    

	connect(checkButton,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
	connect(radioButton,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    
	connect(legend->exclButton,SIGNAL(toggled(bool)), this,SLOT(toggleExclusive(bool)));
    
	legend->show();
    
    } else {
	checkButton=0; 
	radioButton=0;
    }
}

WGraphTrace::~WGraphTrace() {
    if (legend) {
	legend->hide();
	if (checkButton) {
	    checkButton->close(/*zzz true*/);
	}
	if (radioButton) {
	    radioButton->close(/*zzz true*/);
	}
	legend->layout->invalidate();
	legend->show();
    }
}

void WGraphTrace::toggleExclusive(bool on) {
    if (on){
	setVisible(radioButton->isChecked());
    } else{
	setVisible(checkButton->isChecked());
    }
}

void WGraphTrace::setVisible(bool vis) { 
  
    if (legend->isExclusive()) {
	radioButton->blockSignals(true);
	radioButton->setChecked(vis);
	radioButton->blockSignals(false);
    } else {
	checkButton->blockSignals(true);
	checkButton->setChecked(vis);
	checkButton->blockSignals(false);
    }
  
    visible=vis; 
  
    emit needRefresh(); 
}

const WRange &WGraphTrace::checkRange() {
    dataRange.resetForRangeCheck();
    for (QVector<float>::Iterator it=data.begin(); it!=data.end(); ++it) {
	dataRange.adjust(*it);
    }
    return dataRange;
}

void WGraphTrace::writeXML(QDomElement &elem) {
    QDomElement trace=elem.ownerDocument().createElement("trace");
    elem.appendChild(trace);
    trace.setAttribute("name",getName());
    trace.setAttribute("nonexclVisible",checkButton->isChecked()?"true":"false");
    trace.setAttribute("exclVisible",radioButton->isChecked()?"true":"false");
}

void WGraphTrace::readXML(QDomElement &elem) {
    QDomNodeList traceList=elem.elementsByTagName("trace");
    for (unsigned int i=0; i<traceList.count(); i++) {
	QDomElement trace=traceList.item(i).toElement();
	if (trace.attribute("name")==getName()) {
	    checkButton->setChecked(trace.attribute("nonexclVisible")=="true");
	    radioButton->setChecked(trace.attribute("exclVisible")=="true");
	    return;
	}      
    }
}
