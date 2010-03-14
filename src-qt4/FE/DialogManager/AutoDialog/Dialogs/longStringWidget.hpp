/****************************************************************************
 ** $Filename: longStringWidget.hpp
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

#ifndef LONGSTRINGWIDGET_HPP
#define LONGSTRINGWIDGET_HPP

//#include <qmultilineedit.h>
#include <QtGui/qtextedit.h>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsStringParameter;
}

namespace iqrfe {
    
    using iqrcommon::ClsStringParameter;

    class ClsLongStringWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:
	ClsLongStringWidget(ClsStringParameter &_parameter, 
			    QWidget *_pqwgtParent, 
			    const char *_pcName = 0);
	~ClsLongStringWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);
	
    private slots:
        void setValueChanged();

    private:
//	QMultiLineEdit  *pqmledValueWidget;
	QTextEdit *pqmledValueWidget;
    };
};

#endif
