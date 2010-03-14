/****************************************************************************
 ** $Filename: stringWidget.cpp
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
			 _pcName)
{
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
    QVBoxLayout *pqlayLayout = new QVBoxLayout(this,
					       5,
					       SMALL_VSPACE, 
					       QCString(_pcName)+" layout");
    
    QVGroupBox *pqvgbxWidgetBox = new QVGroupBox(string2QString(_parameter.getLabel()), 
						 this,
						 "line edit frame");

    pqledValueWidget = new QLineEdit(string2QString(_parameter.getValue()),
				     pqvgbxWidgetBox, 
				     "string line edit");
    connect(pqledValueWidget, SIGNAL(textChanged(const QString &)), 
	    this, SLOT(setValueChanged()));

    pqlayLayout->addWidget(pqvgbxWidgetBox);

    QToolTip::add((QWidget *)pqvgbxWidgetBox, 
		  string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsStringWidget::~ClsStringWidget()
{
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::~ClsStringWidget"
	     << endl;
    }
}

string
iqrfe::ClsStringWidget::getValue() const
{
    return QString2string(pqledValueWidget->text());
}

string
iqrfe::ClsStringWidget::getValueAsString() const
{
    return QString2string(pqledValueWidget->text());
}

void
iqrfe::ClsStringWidget::setValue(string _strValue)
{
    pqledValueWidget->setText(string2QString(_strValue));
    if (bDebugStringWidget) {
	cout << "ClsStringWidget::setValue: " << getName()
	     << " value set to " << _strValue << endl;
    }
}

void
iqrfe::ClsStringWidget::setValueFromString(string _strValue)
{
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

