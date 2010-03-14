#include <iostream>

#include <QLayout>
#include <QToolTip>
#include <QLabel>
#include <QColorDialog>

#include "colorParameter.hpp"
#include "colorWidget.hpp"

//#define DEBUG_CLSCOLORWIDGET
#ifdef DEBUG_CLSCOLORWIDGET
static const bool bDebugColorWidget = true;
#else
static const bool bDebugColorWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsColorWidget::ClsColorWidget(ClsColorParameter &_parameter, 
					QWidget *_pqwgtParent, 
					const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName) {

    QHBoxLayout *pqlayLayout = new QHBoxLayout(this);
    

    QLabel *pqlabWidgetLabel = new QLabel(this);
    pqlabWidgetLabel->setText(_parameter.getLabel().c_str());
    pqlayLayout->addWidget(pqlabWidgetLabel);
    pqlayLayout->setStretchFactor ( pqlabWidgetLabel, 2 );


    qpbColor = new QPushButton(this);
    qpbColor->setEnabled(false);
    qpbColor->setFlat(true);
    qpbColor->setAutoFillBackground(true);   
    pqlayLayout->addWidget(qpbColor);


    qpb = new QPushButton("select", this);
    connect(qpb, SIGNAL(clicked (bool)), this, SLOT(setValueChanged()));
    pqlayLayout->addWidget(qpb);
    setColor(_parameter.getValue().c_str());

    setToolTip(_parameter.getDescription().c_str());
    setFixedHeight(sizeHint().height());
}


void iqrfe::ClsColorWidget::setColor(string str){
//    cout << "ClsColorWidget::setColor(string str)" << endl;
    if(qpbColor!=NULL){
	QPalette palette;
	palette.setColor(qpbColor->backgroundRole(), QColor(str.c_str()));
	qpbColor->setPalette(palette);    
    }
}

iqrfe::ClsColorWidget::~ClsColorWidget() {
    if (bDebugColorWidget) {
	cout << "ClsColorWidget::~ClsColorWidget"
	     << endl;
    }
}

string iqrfe::ClsColorWidget::getValue() const {
    return qpbColor->palette().color(qpbColor->backgroundRole()).name().toStdString();
}

string iqrfe::ClsColorWidget::getValueAsString() const {
    return qpbColor->palette().color(qpbColor->backgroundRole()).name().toStdString();
}

void iqrfe::ClsColorWidget::setValue(string _strValue) {
    if (bDebugColorWidget) {
	cout << "ClsColorWidget::setValue: " << getName()
	     << " value set to " << _strValue << endl;
    }
    setColor(_strValue);
}

void iqrfe::ClsColorWidget::setValueFromString(string _strValue) {
    if (bDebugColorWidget) {
	cout << "ClsColorWidget::setValue: "  << getName()
	     << " value set to " << _strValue << endl;
    }
    setColor(_strValue);
}

void iqrfe::ClsColorWidget::setValueChanged() {
    if (bDebugColorWidget) {
	cout << "ClsColorWidget::setValueChanged: " << endl;
    }

   QColor c = QColorDialog::getColor ( QColor(getValueAsString().c_str()), this );
   setColor(c.name().toStdString());
    
    bValueChanged = true;
    emit changed();
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k"
//// End:
