/****************************************************************************
 ** $Filename: stringWidget.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:21:37 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef STRINGWIDGET_HPP
#define STRINGWIDGET_HPP

#include <QtGui/qlineedit.h>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsStringParameter;
}

namespace iqrfe {
    
    using iqrcommon::ClsStringParameter;

    class ClsStringWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:
	ClsStringWidget(ClsStringParameter &_parameter, 
			QWidget *_pqwgtParent, 
			const char *_pcName = 0);
	~ClsStringWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);
	
    private slots:
        void setValueChanged();

    private:
	QLineEdit  *pqledValueWidget;
    };
};

#endif
