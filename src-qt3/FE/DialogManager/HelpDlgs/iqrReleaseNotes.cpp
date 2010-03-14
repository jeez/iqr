/****************************************************************************
** Form implementation generated from reading ui file 'iqrReleaseNotes.ui'
**
** Created: Tue Feb 20 16:20:52 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.5   edited Aug 31 12:13 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "iqrReleaseNotes.h"

#include <qvariant.h>
#include <qtextbrowser.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "iqrReleaseNotes.ui.h"

/*
 *  Constructs a dlgReleaseNotes as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
dlgReleaseNotes::dlgReleaseNotes( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "dlgReleaseNotes" );
    dlgReleaseNotesLayout = new QVBoxLayout( this, 5, 10, "dlgReleaseNotesLayout"); 

    qtxtReleaseNotes = new QTextBrowser( this, "qtxtReleaseNotes" );
    qtxtReleaseNotes->setEnabled( TRUE );
    QFont qtxtReleaseNotes_font(  qtxtReleaseNotes->font() );
    qtxtReleaseNotes->setFont( qtxtReleaseNotes_font ); 
    qtxtReleaseNotes->setLineWidth( 0 );
    qtxtReleaseNotes->setTextFormat( QTextBrowser::AutoText );
    dlgReleaseNotesLayout->addWidget( qtxtReleaseNotes );

    layout3 = new QHBoxLayout( 0, 0, 10, "layout3"); 
    spacer1_2 = new QSpacerItem( 140, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1_2 );

    pbtnClose = new QPushButton( this, "pbtnClose" );
    pbtnClose->setDefault( TRUE );
    layout3->addWidget( pbtnClose );
    spacer1 = new QSpacerItem( 140, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1 );
    dlgReleaseNotesLayout->addLayout( layout3 );
    languageChange();
    resize( QSize(576, 419).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pbtnClose, SIGNAL( clicked() ), this, SLOT( close() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dlgReleaseNotes::~dlgReleaseNotes()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dlgReleaseNotes::languageChange()
{
    setCaption( tr( "iqr release notes" ) );
    qtxtReleaseNotes->setText( QString::null );
    pbtnClose->setText( tr( "OK" ) );
}

