#include "memberWidget.h"

#include <iostream>


iqrfe::ClsMemberWidget::ClsMemberWidget(QWidget *parent, string _strName, bool _bModal, string _strValue, list<string> lstValues) : 
    QGroupBox(parent), strName(_strName), bModal(_bModal), strValue(_strValue) {

    bValueChanged = false;
    

    QGridLayout *pqlayLayout = new QGridLayout(this);
    pqlayLayout->setSpacing(0);

    setTitle(strName.c_str());

    QLabel *qlblText0 = new QLabel(this);
    qlblText0->setText("current type:");

    qlblValue = new QLabel(this);
    qlblValue->setText(strValue.c_str());

    qpb = new QPushButton("edit",  this);

    qpb->setCheckable(true); //ZZZ
    connect(qpb, SIGNAL(clicked(bool)), this, SLOT(buttonClick(bool)));

    
    QLabel *qlblText1 = new QLabel(this);
    qlblText1->setText("set type:");

    qcombo = new QComboBox(this);
    
    if(strValue.size()<=0){
	qcombo->addItem(QString(""));
	qpb->setEnabled(false);
    }

    list<string>::iterator it;
    for (it = lstValues.begin(); it != lstValues.end(); ++it) {
	qcombo->addItem((*it).c_str());
    }

    int iIndex = qcombo->findText(strValue.c_str());
    if(iIndex>0){
	qcombo->setCurrentIndex(iIndex);
    }


    connect(qcombo, SIGNAL(activated(const QString &)), this, SLOT(setValueChanged(const QString &)));

    pqlayLayout->addWidget(qlblText0, 1, 0);
    pqlayLayout->addWidget(qlblValue, 1, 1);
    pqlayLayout->addWidget(qpb,       1, 2);
    pqlayLayout->addWidget(qlblText1, 2, 0);
    pqlayLayout->addWidget(qcombo, 2, 1,  1, 2); 

};
	

void iqrfe::ClsMemberWidget::buttonClick(bool bChecked) {
//    cout << "ClsMemberWidget::buttonClick()" << endl;

    if(qlblValue->text().length()>0){

	if(bChecked){ //ZZZ
//	    cout << "show Member" << endl;
	    toggleEdit(false);
	    emit toggleMemberEdit(strName, qlblValue->text().toStdString(), ClsMemberWidget::SHOW);
	}
	else {
//	    cout << "hide Member" << endl;
	    toggleEdit(true);
	    emit toggleMemberEdit(strName, qlblValue->text().toStdString(), ClsMemberWidget::HIDE);
	}
    } else {
	qpb->setChecked(false);
    }
}

void iqrfe::ClsMemberWidget::toggleEdit(bool b) {
    qcombo->setEnabled(b);
    if(b){
	qpb->setText("edit");
	qpb->setChecked(false);
    } 
    else {
	qpb->setText("hide");
	qpb->setChecked(true);
    }
}

void iqrfe::ClsMemberWidget::slotSelectorEnabled(bool b) {
//    cout << "iqrfe::ClsMemberWidget::slotSelectorEnabled(bool b)" << endl;
    qcombo->setEnabled(b);
}

void iqrfe::ClsMemberWidget::slotEditEnabled(bool b) {
    /* this is a bit obscure, but we have to make sure, we 
       don not enable the button if the label is empty */
    if(b && qlblValue->text().length()>0){
	qpb->setEnabled(true);
    }
    else {
	qpb->setEnabled(false);
    }
}

void iqrfe::ClsMemberWidget::setValueChanged(const QString & qstrValue) {
//    cout << "ClsMemberWidget::setValueChanged(QString)" << qstrValue.toStdString() << endl;
    strValue = qstrValue.toStdString();
    bValueChanged = true;

/*
    cout << "strValue.size(): " << strValue.size() << endl;
    cout << "qlblValue->text().length(): " << qlblValue->text().length() << endl;
*/

    if(qlblValue->text().length()>0){
	qpb->setEnabled(true);
    } else {
	qpb->setEnabled(false);
    }

    emit sigChanged();
}

void iqrfe::ClsMemberWidget::setValue(string _strValue) {
//    cout << "ClsMemberWidget::setValue(string _strValue)" << endl;
    strValue = _strValue;
    qlblValue->setText(strValue.c_str());    

    if(strValue.size()>0){
	qpb->setEnabled(true);
    } else {
	qpb->setEnabled(false);
    }
}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
