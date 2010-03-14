/****************************************************************************
 ** $Filename: labelWidget.cpp
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

#include <QLabel>
#include <QLayout>
#include <QToolTip>
#include <QGroupBox>
#include <QBoxLayout>

#include "stringParameter.hpp"
#include "labelWidget.hpp"
#include "qstringConversions.hpp"

#ifdef DEBUG_CLSLABELWIDGET
static const bool bDebugLabelWidget = true;
#else
static const bool bDebugLabelWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsLabelWidget::ClsLabelWidget(ClsStringParameter &_parameter, 
				      QWidget *_pqwgtParent, 
				      const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName) {
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
    QVBoxLayout *pqlayLayout = new QVBoxLayout(this);
    
    QGroupBox* qgrpbxWidgetBox = new QGroupBox (string2QString(_parameter.getLabel()), this) ;
    pqlabValueWidget = new QLabel(/*qgrpbxWidgetBox*/);
    pqlabValueWidget->setText(string2QString(_parameter.getValue()));

    QHBoxLayout *qlayoutWidgetBox = new QHBoxLayout();
    qlayoutWidgetBox->setSpacing ( 0 );
    qlayoutWidgetBox->addWidget(pqlabValueWidget);
    qgrpbxWidgetBox->setLayout(qlayoutWidgetBox);

    pqlayLayout->addWidget(qgrpbxWidgetBox);

    QToolTip::add((QWidget *)qgrpbxWidgetBox, string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsLabelWidget::~ClsLabelWidget() {
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::~ClsLabelWidget" << endl;
    }
}

string iqrfe::ClsLabelWidget::getValue() const {
    return QString2string(pqlabValueWidget->text());
}

string iqrfe::ClsLabelWidget::getValueAsString() const {
    return QString2string(pqlabValueWidget->text());
}

void iqrfe::ClsLabelWidget::setValue(string _strValue) {
    pqlabValueWidget->setText(string2QString(_strValue));
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void iqrfe::ClsLabelWidget::setValueFromString(string _strValue) {
    pqlabValueWidget->setText(string2QString(_strValue));
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::setValueFromString: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}
