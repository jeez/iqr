/****************************************************************************
 ** $Filename: doubleSpinBox.cpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Feb  9 22:29:28 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: customized spin box for floating point values.
 **
 **               Uses QSpinBox as base class.
 **
 **               Precision variable specifies the number of decimal
 **               places.
 **
 *****************************************************************************/

#include <cmath>
#include <iostream>

#include <qpushbutton.h>
#include <qstring.h>
#include <qtooltip.h>
#include <qvalidator.h>

#include "doubleSpinBox.hpp"

#ifdef DEBUG_CLSDOUBLESPINBOX
static const bool bDebugDoubleSpinBox = true;
#else
static const bool bDebugDoubleSpinBox = false;
#endif

using namespace std;

iqrfe::ClsDoubleSpinBox::ClsDoubleSpinBox(QWidget *_pqwgtParent, 
					  const char *_pcName) 
    : QSpinBox(_pqwgtParent, 
	       _pcName),
      iPrecision(DEFAULT_PRECISION)
{
    // Calculate minimum and maximum for base spin box according to
    // precision (number of decimal places).
    dScale = pow(10.0, iPrecision);

    // Initialize base class object.
    QSpinBox::setRange(int(DEFAULT_MINIMUM() * dScale),
		       int(DEFAULT_MAXIMUM() * dScale));
    QSpinBox::setSteps(DEFAULT_LINE_STEP, DEFAULT_PAGE_STEP);

    QSpinBox::setValidator(new QDoubleValidator(DEFAULT_MINIMUM(), DEFAULT_MAXIMUM(), 
						iPrecision, this));

    // An intermediate slot is needed to edit the value as a double.
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(emitValue()));
}

iqrfe::ClsDoubleSpinBox::ClsDoubleSpinBox(double _dMin, 
					  double _dMax, 
					  int _iPrecision, 
					  double _dStep,
					  QWidget *_pqwgtParent, 
					  const char *_pcName)
    : QSpinBox(_pqwgtParent, 
	       _pcName),
      iPrecision(_iPrecision)
{
    // Calculate minimum and maximum for base spin box according to
    // precision (number of decimal places).
    dScale = pow(10.0, iPrecision);
    
    // Initialize base class object.
    QSpinBox::setRange(int(_dMin * dScale),  int(_dMax * dScale));
    QSpinBox::setSteps(int(_dStep * dScale), DEFAULT_PAGE_STEP);

    QSpinBox::setValidator(new QDoubleValidator(_dMin, _dMax, 
						iPrecision, this));

    // An intermediate slot is needed to edit the value as a double.
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(emitValue()));
}

// double 
// iqrfe::ClsDoubleSpinBox::computeDouble(int _i) const
// {
//     double d;
    
//     if (_i >= 0) {
// 	d = floor(double(_i)/dScale) + double(_i%int(dScale)) / dScale;
//     } else {
// 	d = ceil(double(_i)/dScale) + double(_i%int(dScale)) / dScale;
//     }

//     return d;
// }

double 
iqrfe::ClsDoubleSpinBox::minValue() const
{
    return double(QSpinBox::minValue()) / dScale;
}

double 
iqrfe::ClsDoubleSpinBox::maxValue() const
{
    return double(QSpinBox::maxValue()) / dScale;
}

double 
iqrfe::ClsDoubleSpinBox::value() const
{
    int iValue = QSpinBox::value();
    double dValue;

    if (iValue >=0) {
	dValue = floor(double(iValue)/dScale) + double(iValue%int(dScale))
	    / dScale;
    } else {
	dValue = ceil(double(iValue)/dScale) + double(iValue%int(dScale))
	    / dScale;
    }

    return dValue;
}

void 
iqrfe::ClsDoubleSpinBox::setMinValue(double _dMin)
{
    if (_dMin <= maxValue()) {
	QSpinBox::setMinValue(int(_dMin * dScale));
    }
}

void 
iqrfe::ClsDoubleSpinBox::setMaxValue(double _dMax)
{
    if (_dMax >= minValue()) {
	QSpinBox::setMaxValue(int(_dMax * dScale));
    }
}

void 
iqrfe::ClsDoubleSpinBox::setRange(double _dMin, double _dMax)
{
    if (_dMin <= _dMax) {
	QSpinBox::setRange(int(_dMin * dScale), int(_dMax * dScale));
    }
}

void 
iqrfe::ClsDoubleSpinBox::setPrecision(int _iPrecision)
{
    if (_iPrecision >= 0 && _iPrecision != iPrecision) {
	// Store values temporarily.
	double dMinimum = minValue();
	double dMaximum = maxValue();
	double dValue   = value();

	// Rescale minimum and maximum values.
	dScale = pow(10.0, _iPrecision);
	iPrecision = _iPrecision;
	
	// Restore values.
	setMinValue(dMinimum);
	setMaxValue(dMaximum);
	setValue(dValue);

	if (bDebugDoubleSpinBox) {
	    cout << "ClsDoubleSpinBox::setPrecision: "
		 << name()
		 << ": min set to "
		 << minValue()
		 << ", max set to "
		 << maxValue()
		 << ", value set to "
		 << value()
		 << endl;
	}
    }
}

void
iqrfe::ClsDoubleSpinBox::setLineStep(double _dLineStep)
{
    if (_dLineStep > 0 && _dLineStep <= maxValue() - minValue()) {
	QSpinBox::setLineStep(int(_dLineStep * dScale));
    }
}

void
iqrfe::ClsDoubleSpinBox::setSteps(double _dLineStep, double _dPageStep)
{
    if (_dLineStep > 0 && _dLineStep <= maxValue() - minValue() 
	&& _dPageStep > 0 && _dPageStep <= maxValue() - minValue()) {
	QSpinBox::setSteps(int(_dLineStep * dScale), int(_dPageStep * dScale));
    }
}

QString
iqrfe::ClsDoubleSpinBox::getRangeAsString() const
{
    return QString(tr("%1:%2")
		   .arg(minValue(), 0, 'f', precision())
		   .arg(maxValue(), 0, 'f', precision()));
}

void
iqrfe::ClsDoubleSpinBox::setValue(double _dValue)
{
    // Validator and range control check validity of value
    QSpinBox::setValue(int(rint(_dValue * dScale)));
}

void
iqrfe::ClsDoubleSpinBox::setMinimum(double _dMin) 
{
    if (bDebugDoubleSpinBox) {
	cout << "ClsDoubleSpinBox::setMinimum: "
	     << name()
	     << ": minimum set to "
	     << _dMin
	     << endl;
    }

    setMinValue(_dMin);
}

void
iqrfe::ClsDoubleSpinBox::setMaximum(double _dMax)
{
    if (bDebugDoubleSpinBox) {
	cout << "ClsDoubleSpinBox::setMaximum: "
	     << name()
	     << ": maximum set to "
	     << _dMax
	     << endl;
    }

    setMaxValue(_dMax);
}

QString
iqrfe::ClsDoubleSpinBox::mapValueToText(int _iValue)
{
    // Use member variable to store permanent copy of double value
    if (_iValue >=0) {
	dValue = floor(double(_iValue)/dScale) + double(_iValue%int(dScale))
	    / dScale;
    } else {
	dValue = ceil(double(_iValue)/dScale) + double(_iValue%int(dScale))
	    / dScale;
    }
    // @@@ 13/08/01 iPrecision can be omitted below.  This produces a
    // display where the number of decimal places shown is dependent
    // on the value.  However, sizeHint() will need to be overridden
    // in this case.
    return QString("%1").arg(dValue, 1, 'f', iPrecision);
}

int
iqrfe::ClsDoubleSpinBox::mapTextToValue(bool *_pbOK)
{
    return int(rint(text().toDouble(_pbOK) * dScale));
}

/** 
 * Ensure that the value is within the correct range.
 *
 * This function is called automatically after setRange().  It checks
 * whether the current value is within the new range, and adjusts the
 * value if necessary.  The enable state of the two buttons is also
 * checked.
 */
// 29/12/01 There is a rounding error when enabling the buttons.
void
iqrfe::ClsDoubleSpinBox::rangeChange()
{
    if (bDebugDoubleSpinBox) {
	cout << "ClsDoubleSpinBox::rangeChange: "
	     << name()
	     << ": checking whether value "
	     << value()
	     << " is within range "
	     << minValue()
	     << ":"
	     << maxValue()
	     << endl;
    }

    // @@@ 10/09/01 There is a fault in the test below such that it
    // fails when the values appear equal.  This is not serious, but
    // leaves the down arrow enabled incorrectly.
    if (value() <= minValue()) {	
	setValue(minValue());

	if (bDebugDoubleSpinBox) {
	    cout << "ClsDoubleSpinBox::rangeChange: "
		 << name()
		 << ": value set to minimum, "
		 << value()
		 << endl;
	}

// doesn't work in qt3.2	downButton()->setEnabled(false);
    } else {
// doesn't work in qt3.2	downButton()->setEnabled(true);
    }

    if (value() >= maxValue()) {
	setValue(maxValue());

	if (bDebugDoubleSpinBox) {
	    cout << "ClsDoubleSpinBox::rangeChange: "
		 << name()
		 << ": value set to maximum, "
		 << value()
		 << endl;
	}

// doesn't work in qt3.2	upButton()->setEnabled(false);
    } else {
// doesn't work in qt3.2	upButton()->setEnabled(true);
    }

    QToolTip::add(this, getRangeAsString());
}


void
iqrfe::ClsDoubleSpinBox::keyPressEvent(QKeyEvent *_e)
{
    if (bDebugDoubleSpinBox) {
	std::cout << "ClsDoubleSpinBox::keyPressEvent (QKeyEvent *k)" << std::endl;
    }
    switch (_e->key()) {
    case Key_Up:
	stepUp();
	break;
    case Key_Down:
	stepDown();
	break;
    case Key_PageUp:
	setValue(value()+pageStep());
	break;
    case Key_PageDown:
	setValue(value()-pageStep());
	break;
/* Call ignore to let the parent widget handle this keypress.
   --> bad idea! Will close the entire dialog with out activating "apply"
   default:
   _e->ignore();
*/
    }
}

/** 
 * Emits the value of the widget as a double.
 *
 * PRIVATE SLOT: This function is used to translate the standard
 * QSpinBox valueChanged(int) signal into valueChanged(double).
 */
void
iqrfe::ClsDoubleSpinBox::emitValue()
{
//     emit valueChanged(floor(double(_iValue)/dScale) + double(_iValue%int(dScale))
// 		      / dScale);
    emit valueChanged(value());
}
