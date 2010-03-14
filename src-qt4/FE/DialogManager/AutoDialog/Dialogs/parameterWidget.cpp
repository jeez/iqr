/****************************************************************************
 ** $Filename: parameterWidget.cpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Mon Feb  5 22:28:02 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Implementation of base class ClsParameterWidget.
 **
 *****************************************************************************/

#include <iostream>

// QApplication needed for access to default widget palette.
#include <QtGui/qapplication.h>


#include "parameter.hpp"
#include "parameterWidget.hpp"

#ifdef DEBUG_CLSPARAMETERWIDGET
static const bool bDebugParameterWidget = true;
#else
static const bool bDebugParameterWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsParameterWidget::ClsParameterWidget(ClsParameter &_parameter,
					      bool _bStretchable, 
					      QWidget *_pqwgtParent, 
					      const char *_pcName) 
    : QWidget(_pqwgtParent /*, _pcName*/),
      bValueChanged(false),
      strName(_parameter.getName()),
      strEnabler(_parameter.getEnabler()),
      bStretchable(_bStretchable)
{
    if (bDebugParameterWidget) {
	cout << "ClsParameterWidget::ClsParameterWidget: created widget " << _pcName << endl;
    }

    // Generate "changed" palette.
    // Indicate value has been changed via color change.
    pqpalChanged = new QPalette(qApp->palette());
    // Get default widget palette from application.
//ZZZ    *pqpalChanged = qApp->palette().copy();
    // Labels draw text in the foreground color.
    pqpalChanged->setColor(/*QColorGroup::Foreground*/ QPalette::WindowText, Qt::blue);
    // Value widgets draw text in the text color.
    pqpalChanged->setColor(QPalette::Text, Qt::red);
}

iqrfe::ClsParameterWidget::~ClsParameterWidget()
{
    // Delete "changed" palette.
    delete pqpalChanged;
}

/** 
 * Connect this widget.
 *
 * This (virtual) function is used by the auto widget to connect each
 * parameter widget to others which control its behaviour.  In this
 * base class, the widget knows which other widget enables/disables
 * it.
 *
 * @param &_lstWidgets List of other parameter widgets.  The enabling
 * widget should be stored within this list.
 */
void iqrfe::ClsParameterWidget::connectWidget(const ParameterWidgetMap &_mapWidgets) {
    connectToEnabler(_mapWidgets);
}

/** 
 * Apply the current value in the widget to the specified parameter.
 *
 * This function clears the value changed flag so that the same change
 * is not applied multiple times.
 *
 * @param *_pParameter Pointer to parameter object to which value
 * should be applied.  The type of the parameter object is not
 * identified, the change is passed as a string.  If a 0 pointer is
 * received, this function does nothing.
 */
void iqrfe::ClsParameterWidget::apply(ClsParameter *_pParameter) {
    if (_pParameter != 0) {
	// Using strings here allows a common interface for all
	// parameter types.
	_pParameter->setValueFromString(getValueAsString());
	if (bDebugParameterWidget) {
	    cout << "ClsParameterWidget::apply: "
		 << getName()
		 << " parameter set to "
		 << getValueAsString()
		 << endl;
	}
    }
    bValueChanged = false;
}

/**  
 * Change the color of the widget to show that its value has been
 * changed from that in the parent parameter.
 *
 * The color change is performed in this base class for consistency
 * across all widgets.
 *
 * @param _bChanged Selects either the changed palette (true) or the
 * normal palette (false).
 */
void iqrfe::ClsParameterWidget::showChanged(bool _bChanged) {
    if (_bChanged) {
	// Indicate widget has changed values using color of label.
	setPalette(*pqpalChanged);
    } else {
	// The current state of the widget matches the value in
	// the parameter object.  Reset the widget colors.	
//	unsetPalette(); //ZZZ
	setPalette(QApplication::palette());
    }
}

/** 
 * Connects this widget to its enabler.
 *
 * The enabler widget must be a boolean type which emits a
 * selected(bool) SIGNAL.
 *
 * @param &_mapWidgets Map of parameter widgets.
 */
void iqrfe::ClsParameterWidget::connectToEnabler(const ParameterWidgetMap &_mapWidgets) {
    // If parent parameter has non-empty enabler name, attempt to
    // connect this widget to the enabler widget.
    if (getEnabler().empty()) {
	if (bDebugParameterWidget) {
	    cout << "ClsParameterWidget::connectToEnabler: parameter widget "
		 << getName()
		 << " has no enabler"
		 << endl;
	}
	return;
    }

    ClsParameterWidget *pEnablerWidget = findWidget(_mapWidgets, getEnabler());
    if (pEnablerWidget != 0) {
// 	ClsBoolWidget *pEnabler = dynamic_cast<ClsBoolWidget*>(pEnablerWidget);

// 	if (pEnabler != 0) {
	if (bDebugParameterWidget) {
	    cout << "ClsParameterWidget::connectToEnabler: found enabler "
		 << getEnabler()
		 << " for parameter widget "
		 << getName()
		 << endl;
	}	
	
	// The enabler widget was found.  Connect its
	// selected(bool) SIGNAL to this widget's setEnabled(bool)
	// SLOT.
	connect(pEnablerWidget, SIGNAL(selected(bool)), 
		this, SLOT(setEnabled(bool)));
// 	setEnabled(pEnabler->getValue());
// 	} else {
// 	    if (bDebugParameterWidget) {
// 		cout << "WARNING: ClsParameterWidget::connectToEnabler: enabler "
// 		     << getEnabler()
// 		     << " not a bool widget"
// 		     << endl;
// 	    }
// 	}
    } else {
	if (bDebugParameterWidget) {
	    cout << "WARNING: ClsParameterWidget::connectToEnabler: enabler "
		 << getEnabler()
		 << " not found for parameter widget "
		 << getName()
		 << endl;
	}
    }
}

/** 
 * Find the widget named in the map provided.
 *
 * If the widget is found the function returns a pointer to it.  If
 * the widget is not found, 0 is returned.  The caller must check the
 * return type before use.
 *
 * @return Pointer to the widget, or 0 if the widget is not found.
 * @param &_mapWidgets Map of parameter widgets.
 * @param _strWidgetName Name of the desired widget.
 */
iqrfe::ClsParameterWidget* iqrfe::ClsParameterWidget::findWidget(const ParameterWidgetMap &_mapWidgets, string _strWidgetName) {
    ParameterWidgetMap::const_iterator itWidget;
    itWidget = _mapWidgets.find(_strWidgetName);
    
    if (itWidget == _mapWidgets.end()) {
	// Widget wasn't found, return 0.  The caller must check the
	// return value before use.
	return 0;
    }
    
    return itWidget->second;
}
