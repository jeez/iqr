#ifndef COLORWIDGET_HPP
#define COLORWIDGET_HPP

#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsColorParameter;
}

namespace iqrfe {
    using iqrcommon::ClsColorParameter;

    class ClsColorWidget : public iqrfe::ClsParameterWidget {
	Q_OBJECT
	
    public:
	ClsColorWidget(ClsColorParameter &_parameter, 
			QWidget *_pqwgtParent, 
			const char *_pcName = 0);
	~ClsColorWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);
	
    private slots:
        void setValueChanged();

    private:
	QPushButton* qpb;
	QPushButton* qpbColor;

	void setColor(string str);
    };
};

#endif

/*
    setAutoFillBackground ( true);   
    QPalette palette;
    palette.setColor(backgroundRole(), QColor(127, 127, 127, 80));
    setPalette(palette);    
*/
