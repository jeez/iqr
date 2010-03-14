/****************************************************************************
** Form implementation generated from reading ui file 'iqrAbout.ui'
**
** Created: Tue Feb 20 16:20:50 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.5   edited Aug 31 12:13 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "iqrAbout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "iqrAbout.ui.h"

/*
 *  Constructs a dlgAboutIqr as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
dlgAboutIqr::dlgAboutIqr( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "dlgAboutIqr" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    setModal( TRUE );
    dlgAboutIqrLayout = new QVBoxLayout( this, 0, 10, "dlgAboutIqrLayout"); 

    frame3 = new QFrame( this, "frame3" );
    frame3->setPaletteBackgroundColor( QColor( 0, 0, 184 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3->setMargin( 20 );
    frame3Layout = new QVBoxLayout( frame3, 0, 10, "frame3Layout"); 

    textLabel1 = new QLabel( frame3, "textLabel1" );
    frame3Layout->addWidget( textLabel1 );

    layout3 = new QHBoxLayout( 0, 0, 10, "layout3"); 
    spacer3_3 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer3_3 );

    qlblMovie = new QLabel( frame3, "qlblMovie" );
    layout3->addWidget( qlblMovie );
    spacer3_2_2 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer3_2_2 );

    qtxtVersion = new QTextBrowser( frame3, "qtxtVersion" );
    qtxtVersion->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)4, 0, 1, qtxtVersion->sizePolicy().hasHeightForWidth() ) );
    qtxtVersion->setMaximumSize( QSize( 130, 30 ) );
    qtxtVersion->setPaletteForegroundColor( QColor( 218, 218, 218 ) );
    qtxtVersion->setPaletteBackgroundColor( QColor( 0, 0, 184 ) );
    QFont qtxtVersion_font(  qtxtVersion->font() );
    qtxtVersion_font.setPointSize( 16 );
    qtxtVersion_font.setBold( TRUE );
    qtxtVersion->setFont( qtxtVersion_font ); 
    qtxtVersion->setFrameShape( QTextBrowser::NoFrame );
    qtxtVersion->setFrameShadow( QTextBrowser::Plain );
    qtxtVersion->setVScrollBarMode( QTextBrowser::AlwaysOff );
    qtxtVersion->setHScrollBarMode( QTextBrowser::AlwaysOff );
    qtxtVersion->setTextFormat( QTextBrowser::PlainText );
    layout3->addWidget( qtxtVersion );
    frame3Layout->addLayout( layout3 );

    textLabel1_2 = new QLabel( frame3, "textLabel1_2" );
    textLabel1_2->setPaletteForegroundColor( QColor( 218, 218, 218 ) );
    textLabel1_2->setPaletteBackgroundColor( QColor( 0, 0, 230 ) );
    textLabel1_2->setMargin( 3 );
    textLabel1_2->setAlignment( int( QLabel::AlignCenter ) );
    frame3Layout->addWidget( textLabel1_2 );
    dlgAboutIqrLayout->addWidget( frame3 );

    qtxtAbout = new QTextBrowser( this, "qtxtAbout" );
    qtxtAbout->setEnabled( TRUE );
    QFont qtxtAbout_font(  qtxtAbout->font() );
    qtxtAbout_font.setPointSize( 11 );
    qtxtAbout->setFont( qtxtAbout_font ); 
    qtxtAbout->setLineWidth( 0 );
    qtxtAbout->setTextFormat( QTextBrowser::RichText );
    dlgAboutIqrLayout->addWidget( qtxtAbout );

    layout3_2 = new QHBoxLayout( 0, 0, 10, "layout3_2"); 
    spacer1_2 = new QSpacerItem( 140, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3_2->addItem( spacer1_2 );

    pbtnClose = new QPushButton( this, "pbtnClose" );
    pbtnClose->setDefault( TRUE );
    layout3_2->addWidget( pbtnClose );
    spacer1 = new QSpacerItem( 140, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3_2->addItem( spacer1 );
    dlgAboutIqrLayout->addLayout( layout3_2 );
    languageChange();
    resize( QSize(343, 469).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pbtnClose, SIGNAL( clicked() ), this, SLOT( close() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dlgAboutIqr::~dlgAboutIqr()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dlgAboutIqr::languageChange()
{
    setCaption( tr( "about iqr" ) );
    textLabel1->setText( QString::null );
    qlblMovie->setText( tr( "textLabel2" ) );
    qtxtVersion->setText( QString::null );
    textLabel1_2->setText( tr( "www.iqr-sim.net" ) );
    qtxtAbout->setText( tr( "<b>Authors</b>\n"
"<ul>\n"
"<li>Ulysses Bernardet &lt;bernuly@gmail.com&gt;</li>\n"
"<li>Mark Blanchard</li>\n"
"<li>Paul F.M.J. Verschure</li>\n"
"</ul> \n"
"\n"
"<b>Thanks to</b>\n"
"<ul>\n"
"<li>Reto Wyss</li>\n"
"</ul>\n"
"\n"
"<b>Disclaimer</b><br>\n"
"The <tt>iqr</tt> software is provided to you on an \"as is\" basis. We cannot give you any promises, guarantees or warranties, either express or implied, including but not limited to the quality or its performance." ) );
    pbtnClose->setText( tr( "OK" ) );
}

