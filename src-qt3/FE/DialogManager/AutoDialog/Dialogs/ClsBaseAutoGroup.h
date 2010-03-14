/****************************************************************************
 ** $Filename: ClsBaseAutoGroup.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri May 30 19:16:21 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSBASEAUTOGROUP_H
#define CLSBASEAUTOGROUP_H    /*+ To stop multiple inclusions. +*/

#include <string>

//--#include "dialog.hpp"
#include <qgroupbox.h> 

using namespace std;

namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;

    class ClsBaseAutoGroup : public QGroupBox {
	Q_OBJECT
	
    public:
	
	ClsBaseAutoGroup(ClsItem &_item, QWidget *_pqwgtParent = NULL, const char *_pcName = 0);
	virtual ~ClsBaseAutoGroup(){};

	virtual void subItemChanged();


protected slots:
    virtual void apply();
	
private slots:
// 	void reset();
	virtual void closeEvent ( QCloseEvent * /*e*/ ){};
	
signals:
	void autoGroupClosed(string, string, unsigned int);
	void changed();
	void sigSubItemChanged();


protected:
	ClsItem &item;
	string strName;	
	ClsAutoWidget *pAutoWidget;

	static const int DIALOG_BORDER    =  5;
	static const int WIDGET_V_STRETCH = 10;
	static const int BUTTON_V_STRETCH =  0;


private:


	
    };
};

#endif /* CLSBASEAUTOGROUP_H */


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

