/****************************************************************************
 ** $Filename: ClsSelectorSparse.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Mon Jul  7 18:21:03 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSSELECTORSPARSE_H
#define CLSSELECTORSPARSE_H    /*+ To stop multiple inclusions. +*/

#include <list>
#include <string>
#include <map>

#include <qbutton.h> 
#include <qgroupbox.h> 
#include <qcombobox.h>
#include <qlayout.h>
#include <qtable.h> 


#include "ClsBaseAutoGroup.h"
#include "dialog.hpp"



using namespace std;



class ClsTopologySparse;


namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;

//    class ClsSelectorSparse : public QGroupBox {
    class ClsSelectorSparse : public ClsBaseAutoGroup {
	Q_OBJECT
	
    public:
	     
	ClsSelectorSparse(ClsItem &_item, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0, string strValue ="");
	~ClsSelectorSparse();

//	void subItemChanged();
	

    private slots:
        void apply();
// 	void reset();
	void tableChanged(int, int);
	
	void addRow();
	void deleteRow();
	int validateCell(int iR, int iC, bool bAllowEmpty);

/*
	void saveData();
	void loadData();
*/
	
	
	void closeEvent ( QCloseEvent * e );
	
    signals:
	

    private:

	ClsTopologySparse *clsTopologySparse;

	QTable *qtablePoints;
	QWidget *pqwgtButtons;

	QPushButton *qbtnAddRow;
	QPushButton *qbtnDeleteRow;

/*
	QPushButton *qbtnLoadPoint;
	QPushButton *qbtnSavePoint;
*/
	

    };
};

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

#endif /* CLSSELECTORSPARSE_H */
