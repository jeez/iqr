#ifndef CLSQHARBORIMPL_H
#define CLSQHARBORIMPL_H

#include <iostream>
#include <QDragEnterEvent>

//--#include "ClsQHarbor.h"
#include "ui_ClsQHarbor.h"

#include "item.hpp"
#include "ClsParamTrade.h"

using namespace std;
using namespace iqrcommon;
using namespace Ui;

class ClsQHarborImpl : public QDialog, public Ui::ClsQHarbor {
    Q_OBJECT

public:
    ClsQHarborImpl(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
    list<ClsParamTrade> getParamTrades();

    void clearTable();
    void setSimulationRunning(bool);
    void loadConfig(string strConfigName);
    void loadParamSet(string strParamSetName);

public slots:
    void slotItemDeleted(int iType, string strID );
    void slotItemChanged(int iType, string strID );


 private slots:
 void slotDoItem(string,int);
    void slotDoItemCanceled(int);
    void slotDeleteRow();
    void slotSaveConfig();
    void slotLoadConfig();

    void slotSaveParamSet();
    void slotLoadParamSet();


    void slotRefresh();
    void slotChangeValue(int iItemType,  string strItemID,  int iIndex, string strParamName,  double fValue);



// clicked ( int row, int col, int button, const QPoint & mousePos )
    void slotCellDoubleClicked(int row, int col);

private:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    void doDialog(int iItemType, string strItemID, int iIndex);
    void doValueDialog(int iItemType, string strItemID, int iIndex, string strParamName, string strParamLabel, double fMin, double fMax, double fValue);

    void fillMinMaxValueField(ClsItem* clsItem, string strParamName, int iRow);
    bool checkForItem(string _strItemID, string _strParamName);
    bool setTableItemValue(string _strItemID, string _strParamName, string strValue);

    bool bSimulationRunning;
    ClsItem* clsItem;
    map<string, string> mapParams;


    void addHeaderItem(int iCol, string strLabel, bool bBold){
	QTableWidgetItem* item = new 	QTableWidgetItem(strLabel.c_str());
	QFont font = item->font();
	font.setBold(bBold);
	item->setFont(font);
	tableWidget->setHorizontalHeaderItem ( iCol, item );
    }

    void addItem(int iRow, int iCol, string strLabel){
//	cout << "ClsQHarborImpl::addItem(int iRow, int iCol, string strLabel)" << endl;
	QTableWidgetItem *newItem = new QTableWidgetItem(strLabel.c_str());
	tableWidget->setItem(iRow, iCol, newItem);
 
    }

    void changeItem(int iRow, int iCol, string strLabel){
//	cout << "ClsQHarborImpl::changeItem(int iRow, int iCol, string strLabel)" << endl;
//	cout << "\tiRow, iCol, strLabel: " << iRow << ", " << iCol << ", " << strLabel << endl;

	QTableWidgetItem *newItem = tableWidget->item(iRow, iCol);
	if(newItem!=NULL){
	    newItem->setText(strLabel.c_str());
	} else {
	    cerr << "ITEM NULL" << endl;
	}

    }


    string getItemValue(int iRow, int iCol){
//	cout << "getItemValue(int iRow, int iCol, string strLabel)" << endl;
	QTableWidgetItem *newItem = tableWidget->item(iRow, iCol);
	if(newItem!=NULL){
	    return newItem->text().toStdString();
	}
	return "";
    }



};


#endif


//// Local Variables:
//// mode: c++
//// End:


