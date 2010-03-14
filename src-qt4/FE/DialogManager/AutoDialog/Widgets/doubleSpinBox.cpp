/****************************************************************************
 ** $Filename: doubleSpinBox.cpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Feb  9 22:29:28 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: customized spin box for floating point values.
 **
 **               Uses QDoubleSpinBox as base class.
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

iqrfe::ClsDoubleSpinBox::ClsDoubleSpinBox(QWidget *_pqwgtParent) : QDoubleSpinBox(_pqwgtParent), iPrecision(DEFAULT_PRECISION) {
    QDoubleSpinBox::setRange(DEFAULT_MINIMUM(), DEFAULT_MAXIMUM());

    setSingleStep ( 0.01);

    qdoublevalidator = new QDoubleValidator(DEFAULT_MINIMUM(), DEFAULT_MAXIMUM(), iPrecision, this);
    setToolTip(getRangeAsString());

}

iqrfe::ClsDoubleSpinBox::ClsDoubleSpinBox(double _dMin, 
					  double _dMax, 
					  int _iPrecision, 
					  double _dStep,
					  QWidget *_pqwgtParent ) : QDoubleSpinBox(_pqwgtParent), iPrecision(_iPrecision) {

    QDoubleSpinBox::setRange(_dMin,  _dMax);
    setSingleStep ( _dStep);

    QDoubleSpinBox::setDecimals(_iPrecision);
    qdoublevalidator = new QDoubleValidator(DEFAULT_MINIMUM(), DEFAULT_MAXIMUM(), iPrecision, this);
    setToolTip(getRangeAsString());

}


double iqrfe::ClsDoubleSpinBox::minValue() const {
    return QDoubleSpinBox::minimum();
}

double iqrfe::ClsDoubleSpinBox::maxValue() const {
    return QDoubleSpinBox::maximum();
}

void iqrfe::ClsDoubleSpinBox::setMinValue(double _dMin) {
    if (_dMin <= maxValue()) {
	QDoubleSpinBox::setMinimum(_dMin);
    }
}

void iqrfe::ClsDoubleSpinBox::setMaxValue(double _dMax) {
    if (_dMax >= minValue()) {
	QDoubleSpinBox::setMaximum(_dMax);
    }
}

void iqrfe::ClsDoubleSpinBox::setRange(double _dMin, double _dMax) {
    if (_dMin <= _dMax) {
	QDoubleSpinBox::setRange(_dMin, _dMax);
    }
}

void iqrfe::ClsDoubleSpinBox::setPrecision(int _iPrecision) {
    QDoubleSpinBox::setDecimals(_iPrecision);
}

QString iqrfe::ClsDoubleSpinBox::getRangeAsString() const {
    return QString(tr("%1:%2")
		   .arg(minValue(), 0, 'f', precision())
		   .arg(maxValue(), 0, 'f', precision()));
}

//--void iqrfe::ClsDoubleSpinBox::setMinimum(double _dMin) {
//--    if (bDebugDoubleSpinBox) {
//--	cout << "ClsDoubleSpinBox::setMinimum: "
//--	     << ": minimum set to "
//--	     << _dMin
//--	     << endl;
//--    }
//--    setMinValue(_dMin);
//--}
//--
//--void iqrfe::ClsDoubleSpinBox::setMaximum(double _dMax) {
//--    if (bDebugDoubleSpinBox) {
//--	cout << "ClsDoubleSpinBox::setMaximum: "
//--	     << ": maximum set to "
//--	     << _dMax
//--	     << endl;
//--    }
//--
//--    setMaxValue(_dMax);
//--}

int iqrfe::ClsDoubleSpinBox::mapTextToValue(bool *_pbOK) {
    return (int)(text().toDouble(_pbOK));
}

