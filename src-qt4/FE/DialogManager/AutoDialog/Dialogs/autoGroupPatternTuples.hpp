/****************************************************************************
 ** $Filename: autoGroupPatternTuples.hpp
 ** $Id$
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jun  6 16:18:49 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSAUTOGROUPPATTERNTUPLES_H
#define CLSAUTOGROUPPATTERNTUPLES_H


#include <string>
#include <map>

#include <QtGui/qgroupbox.h> 
#include <QtGui/qcombobox.h>
#include <QtGui/qlayout.h>

#include "ClsBaseAutoGroup.h"
#include "dialog.hpp"



using namespace std;

//class ClsPatternBaseSelector;
class ClsPatternSelectorTuples;
class ClsPatternTuples;
//BFS class ClsBaseMediator;
class ClsBaseTopology;


namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;


    class ClsAutoGroupPatternTuples : public ClsBaseAutoGroup {
	Q_OBJECT
	
    public:
	
	ClsAutoGroupPatternTuples(ClsItem& _itemConnection, ClsItem &_item, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0, string strValue=0);

	~ClsAutoGroupPatternTuples();
	
    private slots:
        void setTuples();
        void apply();
// 	void reset();
	void closeEvent ( QCloseEvent * e );
	
    signals:

    private:

	ClsItem &itemConnection;
	ClsPatternTuples *clsPatternTuples;
	
	ClsPatternSelectorTuples *clsPatternSelector;
	QWidget *pqwgtButtons;
	
    };
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
