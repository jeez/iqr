/****************************************************************************
 ** $Filename: xRefWidget.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
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

#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>

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
    : QWidget(_pqwgtParent, _pcName),
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
    QHBoxLayout *pqlayLayout = new QHBoxLayout(this, 0, SMALL_VSPACE, QCString(_pcName)+" layout");

    QLabel *pqlabWidgetLabel = new QLabel(this,  QCString(_pcName)+" label");
    pqlabWidgetLabel->setText(string2QString(xRef->getLabel()));

    pqcmbValueWidget = new QComboBox(false, this, QCString(_pcName)+" combo box");


    if(strTargetName.size()<=0){
	pqcmbValueWidget->insertItem(QString(""));
    }


    list<string>::iterator it;
    for (it = lstXRefValues.begin(); it != lstXRefValues.end(); ++it) {
	pqcmbValueWidget->insertItem(string2QString(*it));
    }

    pqcmbValueWidget->setCurrentText(strTargetName.c_str());


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
