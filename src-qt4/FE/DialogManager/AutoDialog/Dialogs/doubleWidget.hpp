/****************************************************************************
 ** $Filename: doubleWidget.hpp
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Thu Nov 15 13:11:04 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef DOUBLEWIDGET_HPP
#define DOUBLEWIDGET_HPP

#include <string>

#include <QShowEvent>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsDoubleParameter;
}

namespace iqrfe {

    using std::string;
    using iqrcommon::ClsDoubleParameter;
    
    class ClsDoubleSpinBox;

    // The Qt moc requires the qualified name of the base class for
    // correct creation of SIGNAL/SLOT connections at runtime.
    class ClsDoubleWidget : public iqrfe::ClsParameterWidget
    {
	Q_OBJECT
	
    public:	
	ClsDoubleWidget(ClsDoubleParameter &_parameter, 
			QWidget *_pqwgtParent, 
			const char *_pcName = 0);
	~ClsDoubleWidget();
	
	double getValue() const;
	string getValueAsString() const;

	void setValue(double _dValue);
	void setValueFromString(string _strValue);

	string getMinSetter() const {return strMinSetter;}
	string getMaxSetter() const {return strMaxSetter;}
	
	void connectWidget(const ParameterWidgetMap &_mapWidgets);
	
    signals:
        void valueChanged(double);
	
    protected:	
	void showEvent(QShowEvent *_e);
	
    private slots:
        void setValueChanged();

    private:
	void connectToMinSetter(const ParameterWidgetMap &_mapWidgets);
	void connectToMaxSetter(const ParameterWidgetMap &_mapWidgets);
	
	string strMinSetter, strMaxSetter;	

	ClsDoubleSpinBox *pValueWidget;
    };
};
    
#endif


