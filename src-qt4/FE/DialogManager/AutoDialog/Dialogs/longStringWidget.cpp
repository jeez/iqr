/****************************************************************************
 ** $Filename: longStringWidget.cpp
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
#include <QLabel>
#include <QToolTip>


#include "stringParameter.hpp"
#include "longStringWidget.hpp"
#include "qstringConversions.hpp"

#ifdef DEBUG_CLSLONGSTRINGWIDGET
static const bool bDebugLongStringWidget = true;
#else
static const bool bDebugLongStringWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsLongStringWidget::ClsLongStringWidget(ClsStringParameter &_parameter, 
						QWidget *_pqwgtParent, 
						const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName) {
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.

    QVBoxLayout *pqlayLayout = new QVBoxLayout(this);

   

    QLabel *pqlabWidgetLabel = new QLabel(this);
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    pqlayLayout->addWidget(pqlabWidgetLabel);
    pqlayLayout->setAlignment(pqlabWidgetLabel, Qt::AlignHCenter ); 

    pqmledValueWidget = new QTextEdit (this);
    pqmledValueWidget->setText(string2QString(_parameter.getValue()));
    connect(pqmledValueWidget, SIGNAL(textChanged()), this, SLOT(setValueChanged()));
    pqlayLayout->addWidget(pqmledValueWidget);
    setToolTip(_parameter.getDescription().c_str());

    setFixedHeight(sizeHint().height()/2);
    
}

iqrfe::ClsLongStringWidget::~ClsLongStringWidget() {
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::~ClsLongStringWidget" << endl;
    }
}

string iqrfe::ClsLongStringWidget::getValue() const {
    return pqmledValueWidget->toPlainText().toStdString();
}

string iqrfe::ClsLongStringWidget::getValueAsString() const {
    return pqmledValueWidget->toPlainText().toStdString();
}

void iqrfe::ClsLongStringWidget::setValue(string _strValue) {
    pqmledValueWidget->setText(string2QString(_strValue));
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void iqrfe::ClsLongStringWidget::setValueFromString(string _strValue) {
    pqmledValueWidget->setText(string2QString(_strValue));
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void iqrfe::ClsLongStringWidget::setValueChanged() {
    bValueChanged = true;
    emit changed();
}



//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

