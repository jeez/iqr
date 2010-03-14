#ifndef PATHWIDGET_HPP
#define PATHWIDGET_HPP

#include <QLineEdit>
#include <QPushButton>

#include "parameterWidget.hpp"

namespace iqrcommon {
    class ClsPathParameter;
}

namespace iqrfe {
    using iqrcommon::ClsPathParameter;

    class ClsPathWidget : public iqrfe::ClsParameterWidget {
	Q_OBJECT
	
    public:
	ClsPathWidget(ClsPathParameter &_parameter, 
			QWidget *_pqwgtParent, 
			const char *_pcName = 0);
	~ClsPathWidget();

	string getValue() const;
	string getValueAsString() const;
	void setValue(string _strValue);
	void setValueFromString(string _strValue);
	
    private slots:
        void setValueChanged();

    private:
	QLineEdit  *pqledValueWidget;
	QPushButton* qpb;
    };
};

#endif
