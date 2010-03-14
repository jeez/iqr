/****************************************************************************
 ** $Filename: subtypedAutoDialog.hpp
 ** $Id: subtypedAutoDialog.hpp,v 1.1 2003/07/01 16:56:39 ulysses Exp ulysses $
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun May 18 18:50:23 2003
 ** $Date: 2003/07/01 16:56:39 $
 **
 ** $Log: subtypedAutoDialog.hpp,v $
 ** Revision 1.1  2003/07/01 16:56:39  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSSUBTYPEDAUTODIALOG_HPP
#define CLSSUBTYPEDAUTODIALOG_HPP

#include <qsplitter.h>
#include <qframe.h>

#include <string>
#include <map>
#include <list>

#include "dialog.hpp"

namespace iqrcommon {
    class ClsItem;
}

using namespace std;

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;
    class ClsBaseAutoGroup;
    class ClsMemberWidget;
    

    class ClsSubtypedAutoDialog : public iqrfe::ClsDialog
    {
	Q_OBJECT
	
    public:
	ClsSubtypedAutoDialog(ClsItem &_item, int _iType, string _strID, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0);

	~ClsSubtypedAutoDialog();

signals:
	void sigSubTypesChangable(bool);
	void sigApplied(int, string);
	void sigDialogClosed(string);

public slots:
     void slotSubTypesChangable(bool);

    private slots:
        void apply();
	void editItemMember(string strName, string strValue, unsigned int);
	void slotAutoGroupChanged();
	void reject ();

private:

	void closeEvent( QCloseEvent* e );
	ClsItem &item;
	ClsAutoWidget *pAutoWidget;


	QSplitter *qsplitter;
	QFrame *qframeMain;
	
	map<string, ClsBaseAutoGroup*> mapAutoGroups;
	map<string, ClsMemberWidget*> mapMemberWidgets;
	list<string> lstRuntimeDisable;
	
    };
};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:
