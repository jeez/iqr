/****************************************************************************
 ** $Filename: intWidget.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:09:03 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef INTWIDGET_HPP
#define INTWIDGET_HPP

#include <string>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsIntParameter;
}

namespace iqrfe {
    
    using std::string;
    using iqrcommon::ClsIntParameter;

    class ClsIntSpinBox;

    // The Qt moc requires the qualified name of the base class for
    // correct creation of SIGNAL/SLOT connections at runtime.
    class ClsIntWidget : public iqrfe::ClsParameterWidget
    {	
	Q_OBJECT
	
    public:	
	ClsIntWidget(ClsIntParameter &_parameter, 
		     QWidget *_pqwgtParent, 
		     const char *_pcName = 0);
	~ClsIntWidget();
	
	int    getValue() const;
	string getValueAsString() const;

	void setValue(int _iValue);
	void setValueFromString(string _strValue);

	string getMinSetter() const {return strMinSetter;}
	string getMaxSetter() const {return strMaxSetter;}
	
	void connectWidget(const ParameterWidgetMap &_mapWidgets);
	
    signals:	
	void valueChanged(int);
	
    protected:	
	void showEvent(QShowEvent *_e);
	
    private slots:
        void setValueChanged();

    private:	
	void connectToMinSetter(const ParameterWidgetMap &_mapWidgets);
	void connectToMaxSetter(const ParameterWidgetMap &_mapWidgets);
	
	string strMinSetter, strMaxSetter;	

	ClsIntSpinBox *pValueWidget;	
    };
};

#endif


