/****************************************************************************
 ** $Filename: longStringWidget.cpp
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

#include <qlayout.h>
#include <qtooltip.h>
#include <qvgroupbox.h>

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
    
//    pqmledValueWidget = new QMultiLineEdit(pqvgbxWidgetBox, QCString(_pcName)+" multiline edit");
    pqmledValueWidget = new QTextEdit (pqvgbxWidgetBox, QCString(_pcName)+" text edit");

    pqmledValueWidget->setTextFormat(Qt::PlainText);
    pqmledValueWidget->setText(string2QString(_parameter.getValue()));

    connect(pqmledValueWidget, SIGNAL(textChanged()), this, SLOT(setValueChanged()));

    pqlayLayout->addWidget(pqvgbxWidgetBox);

    QToolTip::add((QWidget *)pqvgbxWidgetBox, 
		  string2QString(_parameter.getDescription()));

//    setFixedHeight(sizeHint().height());
}

iqrfe::ClsLongStringWidget::~ClsLongStringWidget()
{
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::~ClsLongStringWidget"
	     << endl;
    }
}

string
iqrfe::ClsLongStringWidget::getValue() const
{
    return QString2string(pqmledValueWidget->text());
}

string
iqrfe::ClsLongStringWidget::getValueAsString() const
{
    return QString2string(pqmledValueWidget->text());
}

void
iqrfe::ClsLongStringWidget::setValue(string _strValue)
{
    pqmledValueWidget->setText(string2QString(_strValue));
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void
iqrfe::ClsLongStringWidget::setValueFromString(string _strValue)
{
    pqmledValueWidget->setText(string2QString(_strValue));
    if (bDebugLongStringWidget) {
	cout << "ClsLongStringWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _strValue
	     << endl;
    }
}

void 
iqrfe::ClsLongStringWidget::setValueChanged()
{
    bValueChanged = true;
    emit changed();
//     if (pqledValueWidget->text() != clsParameter.Value()) {
// 	bValueChanged = true;	
//     } else {
// 	// The current selection in the widget matches the value in
// 	// the parameter object.  In effect, the parameter is
// 	// unchanged.
// 	bValueChanged = false;	
//     }

//     ShowChanged(WidgetChanged());
}
