/****************************************************************************
 ** $Filename: xRefWidget.cpp
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Fri Dec  5 18:58:12 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <iostream>

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>

#include "xRefWidget.hpp"
#include "qstringConversions.hpp"

#include "ClsFESystemManager.h"


#ifdef DEBUG_CLSXREFWIDGET
static const bool bDebugXRefWidget = true;
#else
static const bool bDebugXRefWidget = false;
#endif

using namespace std;
using namespace iqrcommon;

iqrfe::ClsXRefWidget::ClsXRefWidget(iqrcommon::ClsXRef* _xRef,
				    list<string> lstXRefValues,
				    string strTargetName,
				    QWidget *_pqwgtParent,
				    const char *_pcName)
    : QWidget(_pqwgtParent/*ZZZ, _pcName*/),
      xRef(_xRef),
      bValueChanged(false)

{
    if (bDebugXRefWidget) {
	cout << "ClsXRefWidget::ClsXRefWidget: "
	     << xRef->getName()
	     << endl;
    }
    // Widget initialized directly from parent parameter.  It is
    // assumed that the data in the parent is valid.
//ZZZ
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this/*ZZZ, 0, SMALL_VSPACE, QString(_pcName)+" layout"*/);

    QLabel *pqlabWidgetLabel = new QLabel(this /*ZZZ,  QString(_pcName)+" label"*/);
    pqlabWidgetLabel->setText(string2QString(xRef->getLabel()));

    pqcmbValueWidget = new QComboBox(this/*ZZZ, QString(_pcName)+" combo box"*/);


    if(strTargetName.size()<=0){
	pqcmbValueWidget->addItem(QString(""));
    }


    list<string>::iterator it;
    for (it = lstXRefValues.begin(); it != lstXRefValues.end(); ++it) {
	pqcmbValueWidget->addItem(string2QString(*it));
    }

    
    int iIndex = pqcmbValueWidget->findText(strTargetName.c_str());
    if(iIndex>0){
	pqcmbValueWidget->setCurrentIndex(iIndex);
    }

    connect(pqcmbValueWidget, SIGNAL(textChanged(const QString &)), this, SLOT(setValueChanged()));
    connect(pqcmbValueWidget, SIGNAL(activated(int)), this, SLOT(setValueChanged()));


    // How can the magic numbers be replaced here?  The dialog must
    // look nice.
    pqlayLayout->addWidget(pqlabWidgetLabel/*, stretch*/);
    pqlayLayout->addWidget(pqcmbValueWidget/*, stretch*/);

//    QToolTip::add((QWidget *)pqlabWidgetLabel, string2QString(xRef->getDescription()));

    setFixedHeight(sizeHint().height());
}

iqrfe::ClsXRefWidget::~ClsXRefWidget() {
    if (bDebugXRefWidget) {
	cout << "ClsXRefWidget::~ClsXRefWidget"
	     << endl;
    }
}

void iqrfe::ClsXRefWidget::setValueChanged() {
    if (bDebugXRefWidget) {
	cout << "ClsXRefWidget::setValueChanged()" << endl;
    }
    bValueChanged = true;
    emit changed();
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
