/****************************************************************************
 ** $Filename: stringWidget.cpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 **
 ** $CreateDate: Thu Nov 15 13:01:03 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>

#include <QLayout>
#include <QToolTip>
#include <QLabel>


#include "stringParameter.hpp"
#include "stringWidget.hpp"
#include "qstringConversions.hpp"

//#define DEBUG_CLSSTRINGWIDGET
#ifdef DEBUG_CLSSTRINGWIDGET
static const bool bDebugStringWidget = true;
#else
static const bool bDebugStringWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsStringWidget::ClsStringWidget(ClsStringParameter &_parameter, 
					QWidget *_pqwgtParent, 
					const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName) {
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.

    QHBoxLayout *pqlayLayout = new QHBoxLayout(this);
    
    QLabel *pqlabWidgetLabel = new QLabel(this);
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    pqlayLayout->addWidget(pqlabWidgetLabel);
//    pqlayLayout->setAlignment(pqlabWidgetLabel, Qt::AlignHCenter ); 

    pqledValueWidget = new QLineEdit(string2QString(_parameter.getValue()), this);
    connect(pqledValueWidget, SIGNAL(textChanged(const QString &)), this, SLOT(setValueChanged()));
    pqlayLayout->addWidget(pqledValueWidget);
    pqlayLayout->setStretchFactor ( pqledValueWidget, 2 );
    setToolTip(_parameter.getDescription().c_str());

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsStringWidget::~ClsStringWidget() {
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::~ClsStringWidget"
	     << endl;
    }
}

string iqrfe::ClsStringWidget::getValue() const {
    return QString2string(pqledValueWidget->text());
}

string iqrfe::ClsStringWidget::getValueAsString() const {
    return QString2string(pqledValueWidget->text());
}

void iqrfe::ClsStringWidget::setValue(string _strValue) {
    pqledValueWidget->setText(string2QString(_strValue));
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::setValue: " << getName()
	     << " value set to " << _strValue << endl;
    }
}

void iqrfe::ClsStringWidget::setValueFromString(string _strValue) {
    pqledValueWidget->setText(string2QString(_strValue));
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::setValue: "  << getName()
	     << " value set to " << _strValue << endl;
    }
}

void iqrfe::ClsStringWidget::setValueChanged() {
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::setValueChanged: " << endl;
    }
    
    bValueChanged = true;
    emit changed();
    
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
