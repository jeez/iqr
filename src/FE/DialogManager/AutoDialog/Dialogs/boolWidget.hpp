/****************************************************************************
 ** $Filename: boolWidget.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:13:20 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef BOOLWIDGET_HPP
#define BOOLWIDGET_HPP

#include <qcheckbox.h>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsBoolParameter;
}

namespace iqrfe {
    
    using iqrcommon::ClsBoolParameter;

    class ClsBoolWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:
	ClsBoolWidget(ClsBoolParameter &_parameter, 
		      QWidget *_pqwgtParent, 
		      const char *_pcName = 0);
	~ClsBoolWidget();

	bool getValue() const;
	string getValueAsString() const;

	void setValue(bool _bValue);
	void setValueFromString(string _strValue);
	
    signals:
        void selected(bool _bIsSelected);

    public slots:
	// Override QWidget::setEnabled so that the current state of
	// this widget can be emitted correctly.
	void setEnabled(bool _bEnable);

    protected:
	void showEvent(QShowEvent *_e);

    private slots:
        void setValueChanged();

    private:
	QCheckBox *pqchkValueWidget;
    };
};

#endif
