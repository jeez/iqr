#ifndef CLSFEPARAMRELAIS_H
#define CLSFEPARAMRELAIS_H

#include <iostream>
#include <list>
#include <qwidget.h>

#include "ClsParamTrade.h"
#include <qobject.h>

using namespace std;


class ClsQHarborImpl;

class ClsFEParamRelais  : public QObject {
    Q_OBJECT
public:

    
    static ClsFEParamRelais* Instance();
    static void initializeParamRelais(QWidget* _parent = NULL);

    void showHarbor();
     void cleanHarbor();
    list<ClsParamTrade> getParamTrades();
    int setParameterByItemID(string strType, string strItemID, string strParamID, double fValue);

public slots:
    void slotItemDeleted(int iType, string strID );
    void slotItemChanged(int iType, string strID );
    void slotSimulationRunning(bool);
    void applyConfig(string strFilename="");


private:
    QWidget* parent;
    ClsFEParamRelais (QWidget* _parent = NULL);
    static ClsFEParamRelais* _instanceParamRelais;

    ClsQHarborImpl* clsQHarborImpl;
    
};


#endif


//// Local Variables:
//// mode: c++
//// End:
