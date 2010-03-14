/****************************************************************************
 ** $Filename: optionsWidget.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:06:33 2001
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

#include "optionsParameter.hpp"
#include "optionsWidget.hpp"
#include "qstringConversions.hpp"

#ifdef DEBUG_CLSOPTIONSWIDGET
static const bool bDebugOptionsWidget = true;
#else
static const bool bDebugOptionsWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsOptionsWidget::ClsOptionsWidget(ClsOptionsParameter &_parameter, 
					  QWidget *_pqwgtParent, 
					  const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName)
{
    if (bDebugOptionsWidget) {
	cout << "ClsOptionsWidget::ClsOptionsWidget: "
	     << _parameter.getName()
	     << endl;
    }
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this, 0, SMALL_VSPACE, 
					        QCString(_pcName)+" layout");
    
    QLabel *pqlabWidgetLabel = new QLabel(this,  QCString(_pcName)+" label");
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    
    pqcmbValueWidget = new QComboBox(!_parameter.isReadOnly(),
				     this, QCString(_pcName)+" combo box");
    // Add options.
    OptionsList::const_iterator itOption;
    for (itOption = _parameter.getListOptions().begin();
	 itOption != _parameter.getListOptions().end();
	 ++itOption) {
	pqcmbValueWidget->insertItem(string2QString(*itOption));
	if (bDebugOptionsWidget) {
	    cout << "    Added option "
		 << *itOption
		 << endl;
	}
    }
    pqcmbValueWidget->setCurrentItem(_parameter.getSelected());

    if (!_parameter.isReadOnly()) {
	// For non read-only widgets, allow user typing to change
	// selection.
	connect(pqcmbValueWidget, SIGNAL(textChanged(const QString &)), 
		this, SLOT(setValueChanged()));
    }

    connect(pqcmbValueWidget, SIGNAL(activated(int)), 
	    this, SLOT(setValueChanged()));
    
    // How can the magic numbers be replaced here?  The dialog must
    // look nice.
    pqlayLayout->addWidget(pqlabWidgetLabel/*, stretch*/);
    pqlayLayout->addWidget(pqcmbValueWidget/*, stretch*/);

    QToolTip::add((QWidget *)pqlabWidgetLabel, 
		  string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsOptionsWidget::~ClsOptionsWidget()
{
    if (bDebugOptionsWidget) {
	cout << "ClsOptionsWidget::~ClsOptionsWidget"
	     << endl;
    }
}

string
iqrfe::ClsOptionsWidget::getValue() const 
{
    return QString2string(pqcmbValueWidget->currentText());
}

string
iqrfe::ClsOptionsWidget::getValueAsString() const 
{
    return QString2string(pqcmbValueWidget->currentText());
}

void
iqrfe::ClsOptionsWidget::setValue(int _iValue)
{
    if (_iValue >= 0 && _iValue < pqcmbValueWidget->count()) {
	pqcmbValueWidget->setCurrentItem(_iValue);
	if (bDebugOptionsWidget) {
	    cout << "ClsOptionsWidget::setValue: "
		 << getName()
		 << " index set to "
		 << _iValue
		 << ", value now "
		 << pqcmbValueWidget->currentText().latin1()
		 << endl;
	}
    } else {
	if (bDebugOptionsWidget) {
	    cout << "ClsOptionsWidget::setValue: "
		 << getName()
		 << ": invalid index "
		 << _iValue
		 << endl;
	}
    }
}

void
iqrfe::ClsOptionsWidget::setValueFromString(string _strValue)
{
    int  iOption = 0;
    bool bOptionFound = false;
    QString qstrValue = string2QString(_strValue);
    // Scan combo box options for the supplied string.
    while (!bOptionFound && iOption < pqcmbValueWidget->count()) {
	if (pqcmbValueWidget->text(iOption) == qstrValue) {
	    // Found the matching option.
	    pqcmbValueWidget->setCurrentItem(iOption);
	    bOptionFound = true;
	    if (bDebugOptionsWidget) {
		cout << "ClsOptionsWidget::setValueFromString: "
		     << getName()
		     << " value set to "
		     << _strValue
		     << ", index now "
		     << iOption
		     << endl;
	    }
	} else {
	    ++iOption;
	}
    }
    if (!bOptionFound) {
	pqcmbValueWidget->insertItem(qstrValue);
	pqcmbValueWidget->setCurrentItem(pqcmbValueWidget->count() - 1);
	if (bDebugOptionsWidget) {
	    cout << "ClsOptionsWidget::setValueFromString: "
		 << getName()
		 << " added option "
		 << _strValue
		 << endl;
	}
    }
}

void 
iqrfe::ClsOptionsWidget::setValueChanged()
{
    bValueChanged = true;
    emit changed();
}
