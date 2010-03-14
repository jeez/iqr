/****************************************************************************
 ** $Filename: intSpinBox.cpp
 ** $Header$
 **
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
#include <QtGui/qpushbutton.h>
#include <QtGui/qtooltip.h>

#include "intSpinBox.hpp"

#ifdef DEBUG_CLSINTSPINBOX
static const bool bDebugIntSpinBox = true;
#else
static const bool bDebugIntSpinBox = false;
#endif

iqrfe::ClsIntSpinBox::ClsIntSpinBox(QWidget *_pqwgtParent) : QSpinBox(_pqwgtParent) {
    setRange(DEFAULT_MINIMUM, DEFAULT_MAXIMUM);
    setToolTip(getRangeAsString());
}

iqrfe::ClsIntSpinBox::ClsIntSpinBox(int _iMin, int _iMax, int _iStep,
				    QWidget *_pqwgtParent) : QSpinBox(_pqwgtParent) {
    setRange(_iMin, _iMax);
    setToolTip(getRangeAsString());
}

/** 
 * Ensure that the value is within the correct range.
 *
 * This function is called automatically after setRange().  It checks
 * whether the current value is within the new range, and adjusts the
 * value if necessary.  The enable state of the two buttons is also
 * checked.
 */
void iqrfe::ClsIntSpinBox::rangeChange() {
    if (bDebugIntSpinBox) {
	qDebug("ClsIntSpinBox::rangeChange: %s, " " checking whether value is within range", objectName());
	qDebug(" value is %d, range is %d,%d", value(), minimum(), maximum());
    }

    if (value() <= minimum()) {	
	setValue(minimum());

	if (bDebugIntSpinBox) {
	    qDebug("ClsIntSpinBox::rangeChange: %s, value set to minimum, %d", objectName(), value());
	}

    }

    if (value() >= maximum()) {
	setValue(maximum());
	if (bDebugIntSpinBox) {
	    qDebug("ClsIntSpinBox::rangeChange: %s value set to maximum, %d", objectName(), value());
	}
    } 

    setToolTip(getRangeAsString());
}

QString iqrfe::ClsIntSpinBox::getRangeAsString() const {
    return QString(tr("%1:%2").arg(minimum()).arg(maximum()));
}


