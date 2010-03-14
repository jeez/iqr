/****************************************************************************
 ** $Filename: optionsWidget.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:40:21 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef OPTIONSWIDGET_HPP
#define OPTIONSWIDGET_HPP

#include <QtGui/qcombobox.h>
// #include <qlabel.h>
// #include <qpushbutton.h>
// #include <qstringlist.h>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsOptionsParameter;
}

namespace iqrfe {
    
    using iqrcommon::ClsOptionsParameter;

    class ClsOptionsWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:	
	ClsOptionsWidget(ClsOptionsParameter &_parameter, 
			 QWidget *_pqwgtParent, 
			 const char *_pcName = 0);
 	~ClsOptionsWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(int _iSelected);
	void setValueFromString(string _strValue);
	
    private slots:
        void setValueChanged();

    private:
	QComboBox   *pqcmbValueWidget;
    };
};

#endif
