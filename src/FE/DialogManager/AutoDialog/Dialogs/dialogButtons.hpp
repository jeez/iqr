/****************************************************************************
 ** $Filename: dialogButtons.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Tue Jun 19 15:30:31 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: definition of the dialog buttons for modal and 
 **               modeless dialogs.  These are reused for all dialogs
 **               but implemented only a single time to simplify label
 **               changes.
 **
 *****************************************************************************/

#ifndef DIALOGBUTTONS_HPP
#define DIALOGBUTTONS_HPP

#include <qpushbutton.h>

namespace iqrfe {

    class ClsModalButtons : public QWidget
    {
	Q_OBJECT
	
    public:
	ClsModalButtons(QWidget *_pqwgtParent, const char *_pcName = "");
	
    signals:
	void ok();
	void cancel();

    private:
	QPushButton *pqpbOK, *pqpbCancel;
    };

    class ClsModelessButtons : public QWidget
    {
	Q_OBJECT

    public:
	ClsModelessButtons(QWidget *_pqwgtParent, const char *_pcName);
	
    public slots:
        void enableApply();
	void disableApply();
	void enableUndo();
	void disableUndo();
	void applyPressed();

    signals:
        void apply();
	void undo();
	void close();

    protected:
	// The apply button should be disabled when the dialog is opened.
	virtual void showEvent(QShowEvent *);
	
    private:
	QPushButton *pqpbApply, *pqpbUndo, *pqpbClose;
    };

};

#endif
