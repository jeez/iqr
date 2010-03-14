/****************************************************************************
 ** $Filename: labelwidget.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:21:37 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef LABELWIDGET_HPP
#define LABELWIDGET_HPP

#include <qlabel.h>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsStringParameter;
}

namespace iqrfe {
    
    using iqrcommon::ClsStringParameter;

    class ClsLabelWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:
	ClsLabelWidget(ClsStringParameter &_parameter, 
		       QWidget *_pqwgtParent, 
		       const char *_pcName = 0);
	~ClsLabelWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);
	
    private:
	QLabel *pqlabValueWidget;
    };
};

#endif
