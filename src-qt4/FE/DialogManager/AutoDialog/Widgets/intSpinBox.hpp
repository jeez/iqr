/****************************************************************************
 ** $Filename: intSpinBox.hpp
 ** $Header$
 **
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

#include <QtGui/qspinbox.h>
#include <QtGui/QKeyEvent>

namespace iqrfe {

    class ClsIntSpinBox : public QSpinBox {	
	Q_OBJECT
	
    public:	
	ClsIntSpinBox(QWidget *_pqwgtParent = 0);
	ClsIntSpinBox(int _iMin,
		      int _iMax,
		      int _iStep = DEFAULT_LINE_STEP,
		      QWidget *_pqwgtParent = 0);
	
	QString getRangeAsString() const;
	static const int DEFAULT_MINIMUM   = 0;
	static const int DEFAULT_MAXIMUM   = 10;
	
	static const int DEFAULT_LINE_STEP = 1;
	
    public slots:
	
    protected:	
	virtual void rangeChange();
    };
};

#endif
