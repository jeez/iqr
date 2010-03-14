#include <iostream>

#include <QLayout>
#include <QToolTip>
#include <QLabel>
#include <QFileDialog>

#include "pathParameter.hpp"
#include "pathWidget.hpp"

//#define DEBUG_CLSPATHWIDGET
#ifdef DEBUG_CLSPATHWIDGET
static const bool bDebugPathWidget = true;
#else
static const bool bDebugPathWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsPathWidget::ClsPathWidget(ClsPathParameter &_parameter, 
					QWidget *_pqwgtParent, 
					const char *_pcName) 
    : ClsParameterWidget(_parameter,
			 false,
			 _pqwgtParent, 
			 _pcName) {

    QHBoxLayout *pqlayLayout = new QHBoxLayout(this);
    

    QLabel *pqlabWidgetLabel = new QLabel(this);
    pqlabWidgetLabel->setText(_parameter.getLabel().c_str());
    pqlayLayout->addWidget(pqlabWidgetLabel);

    pqledValueWidget = new QLineEdit(_parameter.getValue().c_str(), this);
    pqledValueWidget->setReadOnly(true);
    pqlayLayout->addWidget(pqledValueWidget);
    pqlayLayout->setStretchFactor ( pqledValueWidget, 2 );


    qpb = new QPushButton("browse", this);
    connect(qpb, SIGNAL(clicked (bool)), this, SLOT(setValueChanged()));
    pqlayLayout->addWidget(qpb);

   

    setToolTip(_parameter.getDescription().c_str());
    setFixedHeight(sizeHint().height());
}

iqrfe::ClsPathWidget::~ClsPathWidget() {
    if (bDebugPathWidget) { cout << "ClsPathWidget::~ClsPathWidget" << endl;
    }
}

string iqrfe::ClsPathWidget::getValue() const {
    return pqledValueWidget->text().toStdString();
}

string iqrfe::ClsPathWidget::getValueAsString() const {
    return pqledValueWidget->text().toStdString();
}

void iqrfe::ClsPathWidget::setValue(string _strValue) {
/*    pqledValueWidget->setText(_strValue.c_str()); */
    if (bDebugPathWidget) { cout << "ClsPathWidget::setValue: " << getName() << " value set to " << _strValue << endl;
    }
}

void iqrfe::ClsPathWidget::setValueFromString(string _strValue) {
    pqledValueWidget->setText(_strValue.c_str());
    if (bDebugPathWidget) { cout << "ClsPathWidget::setValue: "  << getName() << " value set to " << _strValue << endl;
    }
}

void iqrfe::ClsPathWidget::setValueChanged() {
    if (bDebugPathWidget) { cout << "ClsPathWidget::setValueChanged: " << endl;
    }

    QString qstrValue = pqledValueWidget->text();
    QString qstrFileName = QFileDialog::getOpenFileName(
	this,
	"Choose File",
	qstrValue,
	"*"
	);

    setValueFromString(qstrFileName.toStdString());
    
    bValueChanged = true;
    emit changed();
    
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k"
//// End:

