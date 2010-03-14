/****************************************************************************
 ** $Filename: doubleWidget.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:14:31 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <cmath>
#include <iostream>

#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>

#include "doubleParameter.hpp"
#include "doubleSpinBox.hpp"
#include "doubleWidget.hpp"
#include "qstringConversions.hpp"
#include "stringConversions.hpp"

#ifdef DEBUG_CLSDOUBLEWIDGET
static const bool bDebugDoubleWidget = true;
#else
static const bool bDebugDoubleWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsDoubleWidget::ClsDoubleWidget(ClsDoubleParameter &_parameter, 
					QWidget *_pqwgtParent, 
					const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false, 
			 _pqwgtParent,
			 _pcName),
      strMinSetter(_parameter.getMinSetter()),
      strMaxSetter(_parameter.getMaxSetter())
{
    // Widget can be initialized directly from parent parameter.  It
    // is assumed that the data in the parent is valid; no range
    // checking is performed here.
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this, 
					       0, 
					       SMALL_VSPACE, 
					       QCString(_pcName)+" layout");
    
    QLabel *pqlabWidgetLabel = new QLabel(this, QCString(_pcName)+" label");
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    
    pValueWidget = new ClsDoubleSpinBox(_parameter.getMinimum(), 
					_parameter.getMaximum(),
					_parameter.getPrecision(),
					1/pow(10.0, _parameter.getPrecision()),
					this,  
					QCString(_pcName)+" spin box");
    // Set initial value.
    pValueWidget->setValue(_parameter.getValue());
    pValueWidget->setWrapping(false);

    connect(pValueWidget, SIGNAL(valueChanged(double)),
	    this, SLOT(setValueChanged()));
    // Emit each value change for setting the min or max values in
    // other widgets.
    connect(pValueWidget, SIGNAL(valueChanged(double)), 
	    this, SIGNAL(valueChanged(double)));
    // Let the parent widget know that the value has been changed.
    connect(pValueWidget, SIGNAL(valueChanged(double)), 
	    this, SIGNAL(changed()));

    // @@@ 26/08/01 The magic numbers must be replaced here.
    pqlayLayout->addWidget(pqlabWidgetLabel, 8);
    pqlayLayout->addWidget(pValueWidget, 2);

    QToolTip::add((QWidget *)pqlabWidgetLabel, 
		  string2QString(_parameter.getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsDoubleWidget::~ClsDoubleWidget()
{
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::~ClsDoubleWidget"
	     << endl;
    }
}

/** 
 * Get the current value as a double.
 *
 * @return Current value in the spin box.
 */
double
iqrfe::ClsDoubleWidget::getValue() const 
{
    return pValueWidget->value();
}


/** 
 * Get the current value as a string.
 *
 * Using a string provides a general interface for all parameter
 * widget types at the expense of performance.
 *
 * @return Current value in the spin box, translated into a standard
 * library string.
 */
string
iqrfe::ClsDoubleWidget::getValueAsString() const
{
    return double2string(pValueWidget->value());
}

/** 
 * Set the value.
 * 
 * @param _dValue New value for the spin box.
 */
void
iqrfe::ClsDoubleWidget::setValue(double _dValue)
{
    pValueWidget->setValue(_dValue);
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _dValue
	     << endl;
    }
}

/** 
 * Set the value from a string.
 * 
 * Using a string provides a general interface for all parameter
 * widget types at the expense of performance.
 *
 * @param _strValue New value for the spin box as a standard library
 * string.
 */
void
iqrfe::ClsDoubleWidget::setValueFromString(string _strValue)
{
    double dValue = string2double(_strValue);
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::setValueFromString: "
	     << getName()
	     << " value set to "
	     << dValue
	     << endl;
    }
    pValueWidget->setValue(dValue);
}

/** 
 * Connect this widget.
 *
 * This (virtual) function is used by the auto widget to connect each
 * parameter widget to others which control its behaviour.  In the
 * base class, the widget knows which other widget enables/disables
 * it.  In this class, connections are made to other widgets which
 * set the minimum and maximum values.
 *
 * @param &_mapWidgets Map of other parameter widgets.
 */
void
iqrfe::ClsDoubleWidget::connectWidget(const ParameterWidgetMap &_mapWidgets)
{
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::connectWidget"
	     << endl;
    }

    connectToEnabler(_mapWidgets);
    connectToMinSetter(_mapWidgets);
    connectToMaxSetter(_mapWidgets);
}

/**   
 * When the widget is shown explicitly, emit the value of the
 * parameter to initialize dependent widgets correctly.
 *
 * This function, which overrides the one inherited from
 * QWidget, is called automatically whenever the widget is
 * shown.
 *
 * @param *_e Details of the show event.  
 */
void 
iqrfe::ClsDoubleWidget::showEvent(QShowEvent *_e)
{
    if (!_e->spontaneous()) {
	if (bDebugDoubleWidget) {
	    cout << "ClsDoubleWidget::showEvent: "
		 << getName()
		 << " value is "
		 << pValueWidget->value()
		 << endl;
	}

	// Emit the current state.
	emit valueChanged(pValueWidget->value());
    }
}

void 
iqrfe::ClsDoubleWidget::setValueChanged()
{
    bValueChanged = true;

    // @@@ 29/12/01 For intelligent handling of the value, which can
    // show when the value has changed, introduce a reference value
    // and set bValueChanged according to the difference between the
    // current value and the reference value.  A new function
    // setReferenceValue(double) should be added, and set to the value
    // each time the value is applied by a parent or reset.
    
//     if (pclsValueWidget->value() != parameter.Value()) {
// 	bValueChanged = true;
//     } else {
// 	// The current selection in the widget matches the value in
// 	// the parameter object.  In effect, the parameter is
// 	// unchanged.
// 	bValueChanged = false;	
//     }

//     showChanged(widgetChanged());
}

/** 
 * Connects this widget to its min setter.
 *
 * The min setter widget controls the minimum value in this widget's
 * spin box.  It must be an double type which emits a
 * valueChanged(double) SIGNAL.
 *
 * The inherited function findWidget performs the search.
 *
 * @param &_mapWidgets Map of parameter widgets to search.  
 */
void 
iqrfe::ClsDoubleWidget::connectToMinSetter(const ParameterWidgetMap &_mapWidgets)
{
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::ConnectToMinSetter"
	     << endl;
    }

    // If parent parameter has non-empty enabler name, attempt to
    // connect this widget to the enabler widget.
    if (getMinSetter().empty()) {
	return;
    }

    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::ConnectToMinSetter: searching for widget "
	     << getMinSetter()
	     << endl;
    }

    ClsParameterWidget *pMinSetter;
    if ((pMinSetter = findWidget(_mapWidgets, getMinSetter())) != 0) {
	
	if (bDebugDoubleWidget) {
	    cout << "ClsDoubleWidget::ConnectToMinSetter: found min setter"
		 << endl;
	}	
	
	// If found, connect min setter's valueChanged(double) SIGNAL to the
	// setMinimum(double) SLOT of this widget's spin box.
	connect(pMinSetter, SIGNAL(valueChanged(double)),
		pValueWidget, SLOT(setMinimum(double)));
    }
}

/** 
 * Connects this widget to its max setter.
 *
 * The max setter widget controls the maximum value in this widget's
 * spin box.  It must be an double type which emits a
 * valueChanged(double) SIGNAL.
 *
 * The inherited function findWidget performs the search.
 *
 * @param &_mapWidgets Map of parameter widgets to search.  
 */
void 
iqrfe::ClsDoubleWidget::connectToMaxSetter(const ParameterWidgetMap &_mapWidgets)
{
    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::ConnectToMaxSetter"
	     << endl;
    }

    // If parent parameter has non-empty enabler name, attempt to
    // connect this widget to the enabler widget.
    if (getMaxSetter().empty()) {
	return;
    }

    if (bDebugDoubleWidget) {
	cout << "ClsDoubleWidget::ConnectToMaxSetter: searching for widget "
	     << getMaxSetter()
	     << endl;
    }

    ClsParameterWidget *pMaxSetter;
    if ((pMaxSetter = findWidget(_mapWidgets, getMaxSetter())) != 0) {
	
	if (bDebugDoubleWidget) {
	    cout << "ClsDoubleWidget::ConnectToMaxSetter: found max setter"
		 << endl;
	}	
	
	// If found, connect max setter's valueChanged(double) SIGNAL
	// to the setMaximum(double) SLOT of this widget's spin box.
	connect(pMaxSetter, SIGNAL(valueChanged(double)),
		pValueWidget, SLOT(setMaximum(double)));
    }
}
