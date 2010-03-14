/****************************************************************************
 ** $Filename: intSpinBox.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Aug 10 18:20:17 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <iostream>
#include <qpushbutton.h>
#include <qtooltip.h>

#include "intSpinBox.hpp"

#ifdef DEBUG_CLSINTSPINBOX
static const bool bDebugIntSpinBox = true;
#else
static const bool bDebugIntSpinBox = false;
#endif

iqrfe::ClsIntSpinBox::ClsIntSpinBox(QWidget *_pqwgtParent, const char *_pcName) 
    : QSpinBox(_pqwgtParent, _pcName)
{
    setRange(DEFAULT_MINIMUM, DEFAULT_MAXIMUM);
    setSteps(DEFAULT_LINE_STEP, DEFAULT_PAGE_STEP);
}

iqrfe::ClsIntSpinBox::ClsIntSpinBox(int _iMin, int _iMax, int _iStep,
			     QWidget *_pqwgtParent, const char *_pcName)
	: QSpinBox(_pqwgtParent, _pcName)
{
    setRange(_iMin, _iMax);
    setSteps(_iStep, DEFAULT_PAGE_STEP);
}


void
iqrfe::ClsIntSpinBox::setMinimum(int _iMin) 
{
    if (bDebugIntSpinBox) {
	qDebug("ClsIntSpinBox::setMinimum: %s, %d", name(), _iMin);
    }

    setMinValue(_iMin);
}

void
iqrfe::ClsIntSpinBox::setMaximum(int _iMax)
{
    if (bDebugIntSpinBox) {
	qDebug("ClsIntSpinBox::setMaximum: %s, %d", name(), _iMax);
    }

    setMaxValue(_iMax);
}


/** 
 * Ensure that the value is within the correct range.
 *
 * This function is called automatically after setRange().  It checks
 * whether the current value is within the new range, and adjusts the
 * value if necessary.  The enable state of the two buttons is also
 * checked.
 */
void
iqrfe::ClsIntSpinBox::rangeChange()
{
    if (bDebugIntSpinBox) {
	qDebug("ClsIntSpinBox::rangeChange: %s, "
	       " checking whether value is within range", name());
	qDebug("                           "
	       " value is %d, range is %d,%d", value(), minValue(), maxValue());
    }

    if (value() <= minValue()) {	
	setValue(minValue());

	if (bDebugIntSpinBox) {
	    qDebug("ClsIntSpinBox::rangeChange: %s, "
		   " value set to minimum, %d", name(), value());
	}

// doesn't work in qt3.2	downButton()->setEnabled(false);
    } else {
// doesn't work in qt3.2	downButton()->setEnabled(true);
    }

    if (value() >= maxValue()) {
	setValue(maxValue());

	if (bDebugIntSpinBox) {
	    qDebug("ClsIntSpinBox::rangeChange: %s"
		   " value set to maximum, %d", name(), value());
	}
	
// doesn't work in qt3.2	upButton()->setEnabled(false);
    } else {
// doesn't work in qt3.2	upButton()->setEnabled(true);
    }

    // Display the range in a tooltip.
    QToolTip::add(this, getRangeAsString());
}

QString
iqrfe::ClsIntSpinBox::getRangeAsString() const
{
    return QString(tr("%1:%2").arg(minValue()).arg(maxValue()));
}


/** 
 * Handle key presses.
 *
 * This function overrides the default function, and includes page
 * steps up and down when Page Up and Page Down are pressed
 * respectively.
 *
 * @param *_e Details of the key pressed.
 */
void
iqrfe::ClsIntSpinBox::keyPressEvent(QKeyEvent *_e)
{
    if (bDebugIntSpinBox) {
	std::cout << "ClsIntSpinBox::keyPressEvent (QKeyEvent *k):" << _e->key() << std::endl;
    }
    switch (_e->key()) {

    case Key_Up:
	stepUp();
	break;

    case Key_Down:
	stepDown();
	break;

    case Key_PageUp:
	for (int i = 0; i < pageStep(); i++) {
	    stepUp();
	}
	break;

    case Key_PageDown:
	for (int i = 0; i < pageStep(); i++) {
	    stepDown();
	}
	break;
/* Call ignore to let the parent widget handle this keypress.
   --> bad idea! Will close the entire dialog with out activating "apply"
   default:
   _e->ignore();
*/
    }
}
