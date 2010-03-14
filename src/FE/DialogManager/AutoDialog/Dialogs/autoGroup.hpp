/****************************************************************************
 ** $Filename: autoGroup.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Feb  6 00:25:26 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef CLSAUTOGROUP_HPP
#define CLSAUTOGROUP_HPP

#include <string>

#include "ClsBaseAutoGroup.h"
#include "dialog.hpp"
#include <qgroupbox.h> 

using namespace std;

namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;

//    class ClsAutoGroup : public QGroupBox {
    class ClsAutoGroup : public ClsBaseAutoGroup {
	Q_OBJECT
	
    public:
	
	ClsAutoGroup(ClsItem &_item, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0, string strValue=0);
	~ClsAutoGroup();
	
    private slots:
//        void apply();
// 	void reset();
	void closeEvent ( QCloseEvent * e );
	
    signals:
//	void autoGroupClosed(string, string, unsigned int);

    private:
	
    };
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
