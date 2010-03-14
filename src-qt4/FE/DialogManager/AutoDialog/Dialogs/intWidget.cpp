/****************************************************************************
 ** $Filename: intWidget.cpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:07:43 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtooltip.h>

#include "intParameter.hpp"
#include "intSpinBox.hpp"
#include "intWidget.hpp"
#include "qstringConversions.hpp"
#include "stringConversions.hpp"

#ifdef DEBUG_CLSINTWIDGET
static const bool bDebugIntWidget = true;
#else
static const bool bDebugIntWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsIntWidget::ClsIntWidget(ClsIntParameter &_parameter,
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
//ZZZ
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this/*,
					       0,
					       SMALL_VSPACE, 
					       QString(_pcName)+" layout"*/);
    
    QLabel *pqlabWidgetLabel = new QLabel(this /*,  QString(_pcName)+" label"*/);
    pqlabWidgetLabel->setText(string2QString(_parameter.getLabel()));
    
    pValueWidget = new ClsIntSpinBox(_parameter.getMinimum(), 
				     _parameter.getMaximum(), 
				     1, 
				     this /*, 
					    QString(_pcName)+" spin box"*/);
    // Set initial value.
    pValueWidget->setValue(_parameter.getValue());
    pValueWidget->setWrapping(false);                                   

    connect(pValueWidget, SIGNAL(valueChanged(int)), this, SLOT(setValueChanged()));
    // Emit each value change for setting the min or max values in
    // other widgets.
    connect(pValueWidget, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    // Let the parent widget know that the value has been changed.
    connect(pValueWidget, SIGNAL(valueChanged(int)), this, SIGNAL(changed()));

    // @@@ 26/08/01 The magic numbers must be replaced here.
    pqlayLayout->addWidget(pqlabWidgetLabel, 8);
    pqlayLayout->addWidget(pValueWidget, 2);

//ZZZ    QToolTip::add((QWidget *)pqlabWidgetLabel, 
//ZZZ		  string2QString(_parameter.getDescription()));

    pqlabWidgetLabel->setToolTip(_parameter.getDescription().c_str());

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsIntWidget::~ClsIntWidget()
{
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::~ClsIntWidget"
	     << endl;
    }
}

/** 
 * Get the current value as an integer.
 *
 * @return Current value in the spin box.
 */
int
iqrfe::ClsIntWidget::getValue() const 
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
iqrfe::ClsIntWidget::getValueAsString() const
{
    return int2string(pValueWidget->value());
}

/** 
 * Set the value.
 * 
 * @param _dValue New value for the spin box.
 */
void
iqrfe::ClsIntWidget::setValue(int _iValue)
{
    pValueWidget->setValue(_iValue);
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::setValue: "
	     << getName()
	     << " value set to "
	     << _iValue
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
iqrfe::ClsIntWidget::setValueFromString(string _strValue)
{
    int iValue = string2int(_strValue);
    pValueWidget->setValue(iValue);
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::setValueFromString: "
	     << getName()
	     << " value set to "
	     << iValue
	     << endl;
    }
}

/** 
 * Connect this widget.
 *
 * This (virtual) function is used by the auto widget to connect each
 * parameter widget to others which control its behaviour.  In the
 * base class, the widget knows which other widget enables/disables
 * it.  In this class, connections are made to other widgets which set
 * the minimum and maximum values.
 *
 * @param &_mapWidgets Map of other parameter widgets.
 */
void
iqrfe::ClsIntWidget::connectWidget(const ParameterWidgetMap &_mapWidgets)
{
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::connectWidget"
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
iqrfe::ClsIntWidget::showEvent(QShowEvent *_e)
{
    if (!_e->spontaneous()) {
	if (bDebugIntWidget) {
	    cout << "ClsIntWidget::showEvent: "
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
iqrfe::ClsIntWidget::setValueChanged()
{
    bValueChanged = true;
}

/** 
 * Connects this widget to its min setter.
 *
 * The min setter widget controls the minimum value in this widget's
 * spin box.  It must be an int type which emits a Value(int) SIGNAL.
 *
 * The inherited function findWidget performs the search.
 *
 * @param &_tlstWidgets List of parameter widgets to search.  
 */
void 
iqrfe::ClsIntWidget::connectToMinSetter(const ParameterWidgetMap &_mapWidgets)
{    
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::connectToMinSetter"
	     << endl;
    }

    // If parent parameter has non-empty enabler name, attempt to
    // connect this widget to the enabler widget.
    if (getMinSetter().empty()) {
	return;
    }

    if (bDebugIntWidget) {
	cout << "ClsIntWidget::connectToMinSetter: searching for widget "
	     << getMinSetter()
	     << endl;
    }

    ClsParameterWidget *pMinSetter;
    if ((pMinSetter = findWidget(_mapWidgets, getMinSetter())) != 0) {
	
	if (bDebugIntWidget) {
	    cout << "ClsIntWidget::connectToMinSetter: found min setter"
		 << endl;
	}	
	
	// If found, connect min setter's Value(int) SIGNAL to the
	// setMinimum(int) SLOT of this widget's spin box.
	connect(pMinSetter, SIGNAL(valueChanged(int)),
		pValueWidget, SLOT(setMinimum(int)));
    }    
}

/** 
 * Connects this widget to its max setter.
 *
 * The min setter widget controls the minimum value in this widget's
 * spin box.  It must be an int type which emits a Value(int) SIGNAL.
 *
 * The inherited function findWidget performs the search.
 *
 * @param &_tlstWidgets List of parameter widgets to search.  
 */
void 
iqrfe::ClsIntWidget::connectToMaxSetter(const ParameterWidgetMap &_mapWidgets)
{    
    if (bDebugIntWidget) {
	cout << "ClsIntWidget::connectToMaxSetter"
	     << endl;
    }

    // If parent parameter has non-empty enabler name, attempt to
    // connect this widget to the enabler widget.
    if (getMaxSetter().empty()) {
	return;
    }

    if (bDebugIntWidget) {
	cout << "ClsIntWidget::ConnectToMaxSetter: searching for widget "
	     << getMaxSetter()
	     << endl;
    }

    ClsParameterWidget *pMaxSetter;
    if ((pMaxSetter = findWidget(_mapWidgets, getMaxSetter())) != 0) {
	
	if (bDebugIntWidget) {
	    cout << "ClsIntWidget::ConnectToMaxSetter: found max setter"
		 << endl;
	}	
	
	// If found, connect max setter's Value(int) SIGNAL to the
	// setMaximum(int) SLOT of this widget's spin box.
	connect(pMaxSetter, SIGNAL(valueChanged(int)),
		pValueWidget, SLOT(setMaximum(int)));
    }
}
