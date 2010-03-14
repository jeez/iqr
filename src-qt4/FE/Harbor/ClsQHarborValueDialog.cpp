/****************************************************************************
** Form implementation generated from reading ui file 'ClsQHarborValueDialog.ui'
**
** Created: Thu Dec 8 11:39:44 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ClsQHarborValueDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

//#include "moc_ClsQHarborValueDialog.cxx"

/*
 *  Constructs a ClsQHarborValueDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */


ClsQHarborValueDialog::ClsQHarborValueDialog( QWidget* parent, 
					      int _iItemType, string _strItemID, 
					      int _iIndex,
					      string _strParamName, string _strParamLabel, 
					      double _fMin, double _fMax, double _fValue)
    
    
    : QDialog( parent /*ZZZ, "ClsQHarborValueDialog", true, Qt::WType_Dialog*/ ), iItemType(_iItemType), strItemID(_strItemID), iIndex(_iIndex), strParamName(_strParamName)  {
    
    setWindowTitle( tr( "Harbor:change value-" ) );
    
    
    setSizeGripEnabled( FALSE );
    setModal( TRUE );
    ClsQHarborValueDialogLayout = new QVBoxLayout( this /*ZZZ, 11, 6, "ClsQHarborValueDialogLayout"*/); 

    layout5 = new QHBoxLayout( /*ZZZ0, 0, 6, "layout5"*/); 

    qlblParamName = new QLabel( this /*, "qlblParamName" */);
    qlblParamName->setText(_strParamLabel.c_str());
    layout5->addWidget( qlblParamName );
    spacer5 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( spacer5 );

    clsDoubleSpinBox = new  ClsDoubleSpinBox(_fMin, //double _dMin,
					     _fMax, //double _dMax,
					     5, //int _iPrecision = DEFAULT_PRECISION,
					     .01, //double _dStep = DEFAULT_LINE_STEP,
					     this /*,*/ //QWidget *_pqwgtParent = 0,
					     /*"clsDoubleSpinBox"*/); //const char *pcName = 0);
    clsDoubleSpinBox->setValue(_fValue);

    layout5->addWidget( clsDoubleSpinBox );
    ClsQHarborValueDialogLayout->addLayout( layout5 );

    layout6 = new QHBoxLayout(/*ZZZ 0, 0, 6, "layout6"*/); 

    buttonCancel = new QPushButton( this /*, "buttonCancel" */);
    buttonCancel->setAutoDefault( TRUE );
    layout6->addWidget( buttonCancel );
    Horizontal_Spacing2 = new QSpacerItem( 100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this /*, "buttonOk" */);
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    layout6->addWidget( buttonOk );
    ClsQHarborValueDialogLayout->addLayout( layout6 );
    languageChange();
    resize( QSize(212, 86).expandedTo(minimumSizeHint()) );
//ZZZ    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( slotApply() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    setTabOrder( buttonOk, buttonCancel );

}

/*
 *  Destroys the object and frees any allocated resources
 */
ClsQHarborValueDialog::~ClsQHarborValueDialog() {
    // no need to delete child widgets, Qt does it all for us
}


void ClsQHarborValueDialog::slotApply(){
//    cout << "ClsQHarborValueDialog::slotApply()" << endl;
    emit sigChangeValue(iItemType,  strItemID,  iIndex, strParamName,  clsDoubleSpinBox->value());
    accept();
}

void ClsQHarborValueDialog::languageChange() {
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setShortcut( QKeySequence( QString::null ) );
    buttonOk->setText( tr( "&Apply" ) );
    buttonOk->setShortcut( QKeySequence( tr( "Alt+A" ) ) );
}

