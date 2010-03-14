#include "memberWidget.h"

#include <iostream>


iqrfe::ClsMemberWidget::ClsMemberWidget(QWidget *parent, string _strName, bool _bModal, string _strValue, list<string> lstValues) : 
    QGroupBox(parent, _strName.c_str()), strName(_strName), bModal(_bModal), strValue(_strValue) {

    bValueChanged = false;
    

    QGridLayout *pqlayLayout = new QGridLayout(this, 3, 3, 5, 0, strName.c_str());

    setTitle(strName.c_str());

    QLabel *qlblText0 = new QLabel(this,  "current_type");
    qlblText0->setText("current type:");

    qlblValue = new QLabel(this,  strName.c_str());
    qlblValue->setText(strValue.c_str());

    qpb = new QPushButton("edit",  this, "edit");

    qpb->setToggleButton(true);
    connect(qpb, SIGNAL(clicked()), this, SLOT(buttonClick()));

    
    QLabel *qlblText1 = new QLabel(this,  "set_type");
    qlblText1->setText("set type:");

    qcombo = new QComboBox(false, this, "combo box");
    
    if(strValue.size()<=0){
	qcombo->insertItem(QString(""));
	qpb->setEnabled(false);
    }

    list<string>::iterator it;
    for (it = lstValues.begin(); it != lstValues.end(); ++it) {
	qcombo->insertItem((*it).c_str());
    }

    qcombo->setCurrentText(strValue.c_str());

    connect(qcombo, SIGNAL(activated(const QString &)), this, SLOT(setValueChanged(const QString &)));

//    pqlayLayout->addMultiCellWidget(qlblText0, 0, 0, 0, 2);

    pqlayLayout->addWidget(qlblText0, 1, 0);

    pqlayLayout->addWidget(qlblValue, 1, 1);
    pqlayLayout->addWidget(qpb,       1, 2);
    
    pqlayLayout->addWidget(qlblText1, 2, 0);
    pqlayLayout->addMultiCellWidget(qcombo,    2, 2,  1, 2);
};
	

void iqrfe::ClsMemberWidget::buttonClick() {
//    cout << "ClsMemberWidget::buttonClick()" << endl;

    if(qlblValue->text().length()>0){

	if(qpb->state()==QButton::On){
//	    cout << "show Member" << endl;
	    toggleEdit(false);
	    emit toggleMemberEdit(strName, qlblValue->text().latin1(), ClsMemberWidget::SHOW);
	}
	else {
//	    cout << "hide Member" << endl;
	    toggleEdit(true);
	    emit toggleMemberEdit(strName, qlblValue->text().latin1(), ClsMemberWidget::HIDE);
	}
    } else {
	qpb->setOn(false);
    }
}


void iqrfe::ClsMemberWidget::toggleEdit(bool b) {
    qcombo->setEnabled(b);
    if(b){
	qpb->setText("edit");
	qpb->setOn(false);
    } 
    else {
	qpb->setText("hide");
	qpb->setOn(true);
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
//    cout << "ClsMemberWidget::setValueChanged(QString)" << qstrValue.latin1() << endl;
    strValue = qstrValue.latin1();
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
