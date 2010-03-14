/****************************************************************************
 ** $Filename: dialog.hpp
 ** $Header$
 **
 ** Copyright (C) 2001 by Mark Blanchard
 ** $Author: Mark Blanchard
 **
 ** $CreateDate: Tue Jun 19 16:56:52 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Abstract base class for all dialogs.
 **
 **               This class defines common look'n'feel values and
 **               handles the IQR run state.
 **
 *****************************************************************************/

#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <qdialog.h>
#include <string>

using namespace std;

namespace iqrfe {

    class ClsDialog : public QDialog
    {
	Q_OBJECT

    public:
	ClsDialog(QWidget *_pqwgtParent, const char *_pcName, int _iType, string _strID, bool _bModal, WFlags f)
	    : QDialog(_pqwgtParent, _pcName, _bModal, f), iType(_iType), strID(_strID) {
	    setCaption(_pcName);
	}
	int getType() { return iType; }
	string getID() { return strID; }


//	~ClsDialog() {}

    protected:
	// This class is a placeholder for the dialog look-and-feel
	// values.
	static const int DIALOG_BORDER    =  5;
	static const int WIDGET_V_STRETCH = 10;
	static const int BUTTON_V_STRETCH =  0;
	int iType;
	string strID;

    };
};

#endif
