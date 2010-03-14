#ifndef CLSQHARBORDIALOGIMPL_H
#define CLSQHARBORDIALOGIMPL_H

#include <iostream>
#include <map>

#include "ui_ClsQHarborDialog.h"
#include "ClsParamTrade.h"

using namespace std;
using namespace Ui;

class ClsQHarborDialogImpl : public QDialog, public Ui::ClsQHarborDialog {
    Q_OBJECT

public:
    ClsQHarborDialogImpl(string strItemType, string _strID, string strItemName, 
			 string strSubItemName, list<string> lstParams, int _iIndex,
			 QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 
	);


 private slots:
 void slotOK();
 void slotDoItemCanceled();
 

    signals:
 void sigDoItem(string, int);
 void sigDoItemCanceled(int);

private:
    int iItemType;
    string strID;

    string strItemName;
    string strSubItemName;
    int iIndex;


};


#endif


