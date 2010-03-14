/****************************************************************************
 ** $Filename: labelWidget.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
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

#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qvgroupbox.h>

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
			 _pcName)
{
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
    QVBoxLayout *pqlayLayout = new QVBoxLayout(this,
					       0,
					       SMALL_VSPACE, 
					       QCString(_pcName)+" layout");
    
    QVGroupBox *pqvgbxWidgetBox 
	= new QVGroupBox(string2QString(_parameter.getLabel()), 
			 this,
			 QCString(_pcName)+" frame");

    pqlabValueWidget = new QLabel(pqvgbxWidgetBox, QCString(_pcName)+" label");
    pqlabValueWidget->setText(string2QString(_parameter.getValue()));

    pqlayLayout->addWidget(pqvgbxWidgetBox);

    QToolTip::add((QWidget *)pqvgbxWidgetBox, 
		  string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsLabelWidget::~ClsLabelWidget()
{
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::~ClsLabelWidget"
	     << endl;
    }
}

string
iqrfe::ClsLabelWidget::getValue() const
{
    return QString2string(pqlabValueWidget->text());
}

string
iqrfe::ClsLabelWidget::getValueAsString() const
{
    return QString2string(pqlabValueWidget->text());
}

void
iqrfe::ClsLabelWidget::setValue(string _strValue)
{
    pqlabValueWidget->setText(string2QString(_strValue));
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void
iqrfe::ClsLabelWidget::setValueFromString(string _strValue)
{
    pqlabValueWidget->setText(string2QString(_strValue));
    if (bDebugLabelWidget) {
	cout << "ClsLabelWidget::setValueFromString: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}
