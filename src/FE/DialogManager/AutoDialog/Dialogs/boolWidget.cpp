/****************************************************************************
 ** $Filename: boolWidget.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:17:00 2001
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

#include "boolParameter.hpp"
#include "boolWidget.hpp"
#include "qstringConversions.hpp"
#include "stringConversions.hpp"

#ifdef DEBUG_CLSBOOLWIDGET
static const bool bDebugBoolWidget = true;
#else
static const bool bDebugBoolWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsBoolWidget::ClsBoolWidget(ClsBoolParameter &_parameter, 
				    QWidget *_pqwgtParent, 
				    const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false, 
			 _pqwgtParent,
			 _pcName)
{
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this,
					       0,
					       SMALL_VSPACE, 
					       QCString(_pcName)+" layout");
    
    QLabel *pqlabWidgetLabel = new QLabel(this,  QCString(_pcName)+" label");
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    
    pqchkValueWidget = new QCheckBox(this, QCString(_pcName)+" check box");
    pqchkValueWidget->setChecked(_parameter.getValue());

    connect(pqchkValueWidget, SIGNAL(toggled(bool)), 
	    this, SLOT(setValueChanged()));
    // Emit the Selected signal to enable/disable dependent widgets.
    connect(pqchkValueWidget, SIGNAL(toggled(bool)),
	    this, SIGNAL(selected(bool)));    

    connect(pqchkValueWidget, SIGNAL(toggled(bool)), 
	    this, SIGNAL(changed()));


    // @@@ 26/08/01 The magic numbers must be replaced here.
    pqlayLayout->addWidget(pqlabWidgetLabel, 8);
    pqlayLayout->addWidget(pqchkValueWidget, 2);
    
    QToolTip::add((QWidget *)pqlabWidgetLabel, 
		  string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsBoolWidget::~ClsBoolWidget()
{
    if (bDebugBoolWidget) {
	cout << "ClsBoolWidget::~ClsBoolWidget"
	     << endl;
    }
}

bool 
iqrfe::ClsBoolWidget::getValue() const
{
    return pqchkValueWidget->isChecked();
}

string 
iqrfe::ClsBoolWidget::getValueAsString() const
{
    return bool2string(pqchkValueWidget->isChecked());
}

void
iqrfe::ClsBoolWidget::setValue(bool _bValue)
{
    pqchkValueWidget->setChecked(_bValue);
}

void
iqrfe::ClsBoolWidget::setValueFromString(string _strValue)
{
    pqchkValueWidget->setChecked(string2bool(_strValue));
}

/** 
 * Enable/disable this widget.
 *
 * PUBLIC SLOT
 *
 * This function overrides the default function in QWidget.  When a
 * bool widget is disabled, all dependent widgets should also be
 * disabled: this function uses the Selected SIGNAL to achieve this.
 * When the widget is enabled, the current state is emitted.
 *
 * @param _bEnable Enables the widget if true, disables it when false.
 */
void
iqrfe::ClsBoolWidget::setEnabled(bool _bEnable)
{
    QWidget::setEnabled(_bEnable);

    if (_bEnable) {
	if (bDebugBoolWidget) {
	    qDebug("ClsBoolWidget::setEnabled: %s: "
		   " widget enabled, %sabling all connected widgets",
		   name(), pqchkValueWidget->isChecked() ? "en" : "dis");
	}	

	// Emit the widget's current state in order to control
	// dependent widgets correctly.
	emit selected(pqchkValueWidget->isChecked());
    } else {
	if (bDebugBoolWidget) {
	    qDebug("ClsBoolWidget::setEnabled: %s:"
		   " widget disabled, disabling all connected widgets",
		   name());
	}	

	// Disable all dependent widgets.  If this widget isn't
	// enabled, they must all be disabled.
	emit selected(false);
    }
}

/**  
 * Indicate whether the value in the widget has changed by
 * changing the colors.
 *
 * PRIVATE SLOT
 *
 * This function is called automatically each time the state of the
 * value widget changes.  The value is compared with that in the
 * parent parameter: if the values are different, the colors are
 * changed; the colors revert to the default when the values are
 * equal.
 * 
 * The color change is handled by the base class.  
 */
void
iqrfe::ClsBoolWidget::setValueChanged()
{
    bValueChanged = true;

//     if (pqchkValueWidget->isChecked() != clsParameter.Value()) {

// 	bValueChanged = true;
	
//     } else {

// 	// The current selection in the widget matches the value in
// 	// the parameter object.  In effect, the parameter is
// 	// unchanged.
// 	bValueChanged = false;
	
//     }

//     showChanged(widgetChanged());
}

void 
iqrfe::ClsBoolWidget::showEvent(QShowEvent *_e)
{
    if (!_e->spontaneous()) {
	if (bDebugBoolWidget) {
	    qDebug("ClsBoolWidget::showEvent: %s", name());
	}

	if (isEnabled()) {	 
	    // Emit the current state, so that dependent widgets are
	    // enabled correctly.
	    emit selected(pqchkValueWidget->isChecked());
	} else {
	    /// The widget is disabled, disable all dependent widgets.
	    emit selected(false);
	}
    }
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
