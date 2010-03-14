/****************************************************************************
** Form implementation generated from reading ui file 'CmdBuild.ui'
**
** Created: Wed Feb 9 01:08:36 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "CmdBuild.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a CmdBuild as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CmdBuild::CmdBuild( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CmdBuild" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );
    CmdBuildLayout = new QVBoxLayout( this, 11, 6, "CmdBuildLayout"); 

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    textLabel1_4 = new QLabel( this, "textLabel1_4" );
    layout9->addWidget( textLabel1_4 );

    comboCmd = new QComboBox( FALSE, this, "comboCmd" );
    layout9->addWidget( comboCmd );
    spacer7 = new QSpacerItem( 51, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer7 );
    CmdBuildLayout->addLayout( layout9 );

    layout4 = new QGridLayout( 0, 1, 1, 0, 6, "layout4"); 

    comboItemIdentifierType = new QComboBox( FALSE, this, "comboItemIdentifierType" );

    layout4->addWidget( comboItemIdentifierType, 1, 0 );

    comboItemIdentifier = new QComboBox( FALSE, this, "comboItemIdentifier" );

    layout4->addWidget( comboItemIdentifier, 1, 1 );

    leValue = new QLineEdit( this, "leValue" );

    layout4->addWidget( leValue, 3, 1 );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );

    layout4->addWidget( textLabel1_2, 2, 0 );

    textLabel1_3 = new QLabel( this, "textLabel1_3" );

    layout4->addWidget( textLabel1_3, 0, 0 );

    comboParamName = new QComboBox( FALSE, this, "comboParamName" );

    layout4->addWidget( comboParamName, 2, 1 );

    comboItemType = new QComboBox( FALSE, this, "comboItemType" );

    layout4->addWidget( comboItemType, 0, 1 );

    textLabel1 = new QLabel( this, "textLabel1" );

    layout4->addWidget( textLabel1, 3, 0 );
    CmdBuildLayout->addLayout( layout4 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    cmdGenerate = new QPushButton( this, "cmdGenerate" );
    cmdGenerate->setDefault( TRUE );
    layout7->addWidget( cmdGenerate );
    spacer6 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7->addItem( spacer6 );
    CmdBuildLayout->addLayout( layout7 );

    leCommand = new QLineEdit( this, "leCommand" );
    CmdBuildLayout->addWidget( leCommand );
    spacer5 = new QSpacerItem( 31, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    CmdBuildLayout->addItem( spacer5 );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    pbOpenFile = new QPushButton( this, "pbOpenFile" );
    pbOpenFile->setDefault( FALSE );
    layout12->addWidget( pbOpenFile );
    Spacer6 = new QSpacerItem( 54, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout12->addItem( Spacer6 );

    quitButton = new QPushButton( this, "quitButton" );
    layout12->addWidget( quitButton );
    CmdBuildLayout->addLayout( layout12 );
    languageChange();
    resize( QSize(527, 279).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( quitButton, SIGNAL( clicked() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CmdBuild::~CmdBuild()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CmdBuild::languageChange()
{
    setCaption( tr( "iqr command builder" ) );
    textLabel1_4->setText( tr( "cmd:" ) );
    textLabel1_2->setText( tr( "paramID:" ) );
    textLabel1_3->setText( tr( "itemType:" ) );
    textLabel1->setText( tr( "value:" ) );
    cmdGenerate->setText( tr( "Generate Command" ) );
    cmdGenerate->setAccel( QKeySequence( QString::null ) );
    pbOpenFile->setText( tr( "Open &File..." ) );
    pbOpenFile->setAccel( QKeySequence( tr( "Alt+F" ) ) );
    quitButton->setText( tr( "&Quit" ) );
    quitButton->setAccel( QKeySequence( tr( "Alt+Q" ) ) );
}

