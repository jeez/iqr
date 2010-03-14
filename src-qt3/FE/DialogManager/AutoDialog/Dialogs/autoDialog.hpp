/****************************************************************************
 ** $Filename: autoDialog.hpp
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

#ifndef CLSAUTODIALOG_HPP
#define CLSAUTODIALOG_HPP

#include "dialog.hpp"

namespace iqrcommon {
    class ClsItem;
}

namespace iqrfe {
    
    using iqrcommon::ClsItem;
    
    class ClsAutoWidget;

    class ClsAutoDialog : public iqrfe::ClsDialog
    {
	Q_OBJECT
	
    public:
	
	ClsAutoDialog(ClsItem &_item, int _iType, string _strID, bool _bModal = true, QWidget *_pqwgtParent = NULL, const char *_pcName = 0);
	~ClsAutoDialog();
	
signals:
	void sigApplied(int, string);

    private slots:
        void apply();
// 	void reset();

    private:
	ClsItem &item;
	ClsAutoWidget *pAutoWidget;
    };
};

#endif
