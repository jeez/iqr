#ifndef CLSQHARBORIMPL_H
#define CLSQHARBORIMPL_H

#include <iostream>
#include "ClsQHarbor.h"

#include "item.hpp"
#include "ClsParamTrade.h"

using namespace std;
using namespace iqrcommon;

class ClsQHarborImpl : public ClsQHarbor {
    Q_OBJECT

public:
    ClsQHarborImpl(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
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
    void slotCellDoubleClicked(int row, int col, int button, const QPoint & mousePos);

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



};


#endif


//// Local Variables:
//// mode: c++
//// End:


