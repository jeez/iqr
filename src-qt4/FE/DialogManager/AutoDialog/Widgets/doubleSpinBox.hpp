/****************************************************************************
 ** $Filename: doubleSpinBox.hpp
 ** $Header$
 **
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

#include <QDoubleSpinBox>

namespace iqrfe {

    class ClsDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT
	
    public:
	ClsDoubleSpinBox(QWidget *_pqwgtParent = 0);
	ClsDoubleSpinBox(double _dMin, 
			 double _dMax, 
			 int _iPrecision = DEFAULT_PRECISION, 
			 double _dStep = DEFAULT_LINE_STEP,
			 QWidget *_pqwgtParent = 0);
	
	double minValue() const;
	double maxValue() const;
	int    precision() const {return iPrecision;}

	void setMinValue(double _dMin);
	void setMaxValue(double _dMax);
	void setRange(double _dMin, double _dMax);
	void setPrecision(int _iPrecision);	

	QString getRangeAsString() const;
	
	static const double DEFAULT_MINIMUM(){ return -1000.0;};
	static const double DEFAULT_MAXIMUM(){ return  1000.0;};
    
	static const int    DEFAULT_PRECISION = 4;
	
	static const int    DEFAULT_LINE_STEP = 1;

    protected:
	int     mapTextToValue(bool *_pbOK);
 
    private:	
	int    iPrecision;
	QDoubleValidator *qdoublevalidator;
    };
};

#endif
