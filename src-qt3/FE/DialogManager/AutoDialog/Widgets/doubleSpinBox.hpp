/****************************************************************************
 ** $Filename: doubleSpinBox.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Feb  9 22:19:25 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: customized spin box for floating point values.
 **
 **               The member function names from QSpinBox are kept.
 **               An added precision variable specifies the number of
 **               decimal places to use.
 **
 *****************************************************************************/

#ifndef DOUBLESPINBOX_HPP
#define DOUBLESPINBOX_HPP

#include <qspinbox.h>

namespace iqrfe {

    class ClsDoubleSpinBox : public QSpinBox 
    {
	Q_OBJECT
	
    public:
	ClsDoubleSpinBox(QWidget *_pqwgtParent = 0, 
			 const char *_pcName = 0);
	ClsDoubleSpinBox(double _dMin, 
			 double _dMax, 
			 int _iPrecision = DEFAULT_PRECISION, 
			 double _dStep = DEFAULT_LINE_STEP,
			 QWidget *_pqwgtParent = 0, 
			 const char *pcName = 0);
	
	// These functions have been named according to the
	// conventions used in the Qt widget library.
	double value() const;
	double minValue() const;
	double maxValue() const;
	int    precision() const {return iPrecision;}

	// setValue is a slot, defined below.
	void setMinValue(double _dMin);
	void setMaxValue(double _dMax);
	void setRange(double _dMin, double _dMax);
	void setPrecision(int _iPrecision);	
	void setLineStep(double _dLineStep);
	void setSteps(double _dLineStep, double _dPageStep);

	QString getRangeAsString() const;
	
	// Constants are public members of class, access from outside
	// using ClsDoubleSpinBox::DEFAULT_XXX.
	static const double DEFAULT_MINIMUM(){ return -1000.0;};
	static const double DEFAULT_MAXIMUM(){ return  1000.0;};
    
	static const int    DEFAULT_PRECISION = 4;
	
	static const int    DEFAULT_LINE_STEP = 1;
	static const int    DEFAULT_PAGE_STEP = 100;
    
    public slots:
        void setValue(double _dValue);
	// Wrapper slots for setting minimum and maximum values.
	void setMinimum(double _dMin);
	void setMaximum(double _dMax);

    signals:
        void valueChanged(double _dValue);

    protected:
        // Overridden base class member functions.
        QString mapValueToText(int _iValue);
	int     mapTextToValue(bool *_pbOK);
	void    rangeChange();
	
	void keyPressEvent(QKeyEvent *_e);
 
    private slots:
        void emitValue();

    private:	
	int    iPrecision;
	double dScale, dValue;
    };
};

#endif
