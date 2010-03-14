#include <kdialog.h>
#include <klocale.h>

/****************************************************************************
** Form implementation generated from reading ui file '/home/ulysses/Code/iqr421_Redesign/FE/Harbor/ClsQHarbor.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "/home/ulysses/Code/iqr421_Redesign/FE/Harbor/ClsQHarbor.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a ClsQHarbor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ClsQHarbor::ClsQHarbor( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ClsQHarbor" );
    ClsQHarborLayout = new QVBoxLayout( this, 0, 0, "ClsQHarborLayout"); 

    qtableEntries = new QTable( this, "qtableEntries" );
    qtableEntries->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 1, 1, qtableEntries->sizePolicy().hasHeightForWidth() ) );
    qtableEntries->setResizePolicy( QTable::Manual );
    qtableEntries->setNumRows( 0 );
    qtableEntries->setNumCols( 9 );
    qtableEntries->setReadOnly( TRUE );
    ClsQHarborLayout->addWidget( qtableEntries );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    pbDeleteRow = new QPushButton( this, "pbDeleteRow" );
    pbDeleteRow->setDefault( FALSE );
    layout3->addWidget( pbDeleteRow );

    pbRefresh = new QPushButton( this, "pbRefresh" );
    layout3->addWidget( pbRefresh );
    spacer1 = new QSpacerItem( 175, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1 );

    pbLoadConfig = new QPushButton( this, "pbLoadConfig" );
    layout3->addWidget( pbLoadConfig );

    pbSaveConfig = new QPushButton( this, "pbSaveConfig" );
    layout3->addWidget( pbSaveConfig );
    ClsQHarborLayout->addLayout( layout3 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    pbLoadParamSet = new QPushButton( this, "pbLoadParamSet" );
    layout2->addWidget( pbLoadParamSet );

    pbSaveParamSet = new QPushButton( this, "pbSaveParamSet" );
    layout2->addWidget( pbSaveParamSet );
    spacer1_2 = new QSpacerItem( 330, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer1_2 );
    ClsQHarborLayout->addLayout( layout2 );
    languageChange();
    resize( QSize(636, 451).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ClsQHarbor::~ClsQHarbor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ClsQHarbor::languageChange()
{
    setCaption( tr2i18n( "iqr Harbor" ) );
    pbDeleteRow->setText( tr2i18n( "&Delete Row(s)" ) );
    pbDeleteRow->setAccel( QKeySequence( tr2i18n( "Alt+D" ) ) );
    pbRefresh->setText( tr2i18n( "Refresh" ) );
    pbLoadConfig->setText( tr2i18n( "&Load Config" ) );
    pbLoadConfig->setAccel( QKeySequence( tr2i18n( "Alt+L" ) ) );
    pbSaveConfig->setText( tr2i18n( "&Save Config" ) );
    pbSaveConfig->setAccel( QKeySequence( tr2i18n( "Alt+S" ) ) );
    pbLoadParamSet->setText( tr2i18n( "Load Parameters" ) );
    pbSaveParamSet->setText( tr2i18n( "Save Parameters" ) );
}

