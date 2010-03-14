/****************************************************************************
 ** $Filename: ClsSelectorSparse.h
 ** $Id$
 **
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

//#include <QtGui/qbutton.h> 
#include <QtGui/qgroupbox.h> 
#include <QtGui/qcombobox.h>
#include <QtGui/qlayout.h>

#include <QtGui/QTableWidget> 

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

	QTableWidget *qtablePoints;
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
