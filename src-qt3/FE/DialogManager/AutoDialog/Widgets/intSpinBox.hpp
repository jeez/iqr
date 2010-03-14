/****************************************************************************
 ** $Filename: intSpinBox.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Fri Aug 10 18:15:46 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: this class extends the normal Qt spin box.
 **
 **               The value is guaranteed to be within the range.
 **
 *****************************************************************************/

#ifndef INTSPINBOX_HPP
#define INTSPINBOX_HPP

#include <qspinbox.h>

namespace iqrfe {

    class ClsIntSpinBox : public QSpinBox 
    {	
	Q_OBJECT
	
    public:	
	ClsIntSpinBox(QWidget *_pqwgtParent = 0, 
		      const char *_pcName = 0);
	ClsIntSpinBox(int _iMin,
		      int _iMax,
		      int _iStep = DEFAULT_LINE_STEP,
		      QWidget *_pqwgtParent = 0,
		      const char *pcName = 0);
	
	QString getRangeAsString() const;
	
	// Constants are public members of class, access from outside
	// using ClsIntSpinBox::DEFAULT_XXX.
	static const int DEFAULT_MINIMUM   = 0;
	static const int DEFAULT_MAXIMUM   = 10;
	
	static const int DEFAULT_LINE_STEP = 1;
	static const int DEFAULT_PAGE_STEP = 10;
	
    public slots:
        // Wrapper slots for setting minimum and maximum values.
        void setMinimum(int _iMin); //{setMinValue(_iMin);}
	void setMaximum(int _iMax); //{setMaxValue(_iMax);}
	
    protected:	
	// rangeChange() overriden to remap value after range changes.
	virtual void rangeChange();
	
	virtual void keyPressEvent(QKeyEvent *_e);	
    };
};

#endif
