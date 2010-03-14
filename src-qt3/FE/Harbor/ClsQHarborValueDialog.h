/****************************************************************************
** Form interface generated from reading ui file 'ClsQHarborValueDialog.ui'
**
** Created: Thu Dec 8 11:39:31 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CLSQHARBORVALUEDIALOG_H
#define CLSQHARBORVALUEDIALOG_H

#include <string>
#include <iostream>
#include <qvariant.h>
#include <qdialog.h>

#include <doubleSpinBox.hpp>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

using namespace std;
using namespace iqrfe;


class ClsQHarborValueDialog : public QDialog {
    Q_OBJECT

public:
    ClsQHarborValueDialog( QWidget* parent, 
			   int _iItemType, string _strItemID, 
			   int _iIndex,
			   string _strParamName, string _strParamLabel, 
			   double _fMin, double _fMax, double _fValue);
	


    ~ClsQHarborValueDialog();

    QLabel* qlblParamName;
    ClsDoubleSpinBox *clsDoubleSpinBox;
    QPushButton* buttonCancel;
    QPushButton* buttonOk;

    signals:
    void sigChangeValue(int, string, int, string, double);

protected:
    QVBoxLayout* ClsQHarborValueDialogLayout;
    QHBoxLayout* layout5;
    QSpacerItem* spacer5;
    QHBoxLayout* layout6;
    QSpacerItem* Horizontal_Spacing2;
    int iItemType;
    string strItemID;
    int iIndex;
    string strParamName;


protected slots:
    virtual void languageChange();
    void slotApply();


};

#endif // CLSQHARBORVALUEDIALOG_H
