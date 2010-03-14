/****************************************************************************
 ** $Filename: autoGroupPatternForeach.hpp
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 30 19:10:29 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSAUTOGROUPPATTERNFOREACH_H
#define CLSAUTOGROUPPATTERNFOREACH_H

#include <string>
#include <map>

#include <QGroupBox> 
#include <QComboBox>
#include <QLayout>
#include <QStackedWidget>

#include "ClsBaseAutoGroup.h"
#include "dialog.hpp"


using namespace std;

class ClsPatternBaseSelector;
class ClsPatternForeach;
class ClsBaseTopology;

namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    class ClsModalButtons;
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;

    class ClsAutoGroupPatternForeach : public ClsBaseAutoGroup {
	Q_OBJECT
	
    public:
	     
	ClsAutoGroupPatternForeach(ClsItem& _itemConnection, ClsItem &_item, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0, string strValue=0, bool bShowList=true);
	~ClsAutoGroupPatternForeach();

    private slots:
        void apply();
	void closeEvent ( QCloseEvent * e );
	void sourceTypeChanged(const QString &);
	void targetTypeChanged(const QString &);
	
    private:
	ClsItem &itemConnection;
	
	ClsPatternForeach *clsPatternForeach;
	
	void setSourceSelector(string strType);
	void setTargetSelector(string strType);

	void setSelector(string strType, map<string, int> &_map, QStackedWidget *qws, ClsBaseTopology *clsBaseTopology);


	QWidget *pqwgtButtons;

	QGroupBox *qgrpxSource;
	QGroupBox *qgrpxTarget;
	
	QComboBox   *qcomboSourcePopulation;
	QComboBox   *qcomboTargetPopulation;

	map<string, int> mapSelectorSourceStack;
	map<string, int> mapSelectorTargetStack;

	QStackedWidget *qwidgetstackSourceSelectors;
	QStackedWidget *qwidgetstackTargetSelectors;

	QVBoxLayout* qlayoutSource;
	QVBoxLayout* qlayoutTarget;
    };
};

#endif
 
//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j2"
//// End:
