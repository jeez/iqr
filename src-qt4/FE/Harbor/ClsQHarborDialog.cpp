#include <kdialog.h>
#include <klocale.h>

/****************************************************************************
** Form implementation generated from reading ui file '/home/ulysses/Code/iqr421_Redesign/FE/Harbor/ClsQHarborDialog.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "/home/ulysses/Code/iqr421_Redesign/FE/Harbor/ClsQHarborDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a ClsQHarborDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ClsQHarborDialog::ClsQHarborDialog( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ClsQHarborDialog" );
    ClsQHarborDialogLayout = new QVBoxLayout( this, 11, 6, "ClsQHarborDialogLayout"); 

    layout8 = new QGridLayout( 0, 1, 1, 0, 6, "layout8"); 

    textLabel6 = new QLabel( this, "textLabel6" );

    layout8->addWidget( textLabel6, 1, 0 );

    lblName = new QLabel( this, "lblName" );
    lblName->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, lblName->sizePolicy().hasHeightForWidth() ) );

    layout8->addWidget( lblName, 0, 2 );

    lblID = new QLabel( this, "lblID" );
    lblID->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, lblID->sizePolicy().hasHeightForWidth() ) );

    layout8->addWidget( lblID, 1, 2 );
    spacer3 = new QSpacerItem( 80, 21, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    layout8->addItem( spacer3, 1, 1 );

    textLabel2 = new QLabel( this, "textLabel2" );

    layout8->addWidget( textLabel2, 0, 0 );
    spacer2 = new QSpacerItem( 80, 21, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    layout8->addItem( spacer2, 0, 1 );
    ClsQHarborDialogLayout->addLayout( layout8 );

    gboxSubtype = new QGroupBox( this, "gboxSubtype" );
    gboxSubtype->setColumnLayout(0, Qt::Vertical );
    gboxSubtype->layout()->setSpacing( 6 );
    gboxSubtype->layout()->setMargin( 11 );
    gboxSubtypeLayout = new QHBoxLayout( gboxSubtype->layout() );
    gboxSubtypeLayout->setAlignment( Qt::AlignTop );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    textLabel1 = new QLabel( gboxSubtype, "textLabel1" );
    layout3->addWidget( textLabel1 );

    qcomboParamList = new QComboBox( FALSE, gboxSubtype, "qcomboParamList" );
    layout3->addWidget( qcomboParamList );
    gboxSubtypeLayout->addLayout( layout3 );
    ClsQHarborDialogLayout->addWidget( gboxSubtype );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
    spacer1 = new QSpacerItem( 121, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer1 );

    pbCancel = new QPushButton( this, "pbCancel" );
    layout4->addWidget( pbCancel );

    pbOK = new QPushButton( this, "pbOK" );
    pbOK->setDefault( TRUE );
    layout4->addWidget( pbOK );
    ClsQHarborDialogLayout->addLayout( layout4 );
    languageChange();
    resize( QSize(326, 173).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ClsQHarborDialog::~ClsQHarborDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ClsQHarborDialog::languageChange()
{
    setCaption( tr2i18n( "Form2" ) );
    textLabel6->setText( tr2i18n( "ID:" ) );
    lblName->setText( tr2i18n( "name" ) );
    lblID->setText( tr2i18n( "id" ) );
    textLabel2->setText( tr2i18n( "Name:" ) );
    gboxSubtype->setTitle( tr2i18n( "groupBox1" ) );
    textLabel1->setText( tr2i18n( "Parameter:" ) );
    pbCancel->setText( tr2i18n( "Cancel" ) );
    pbOK->setText( tr2i18n( "Ok" ) );
}

