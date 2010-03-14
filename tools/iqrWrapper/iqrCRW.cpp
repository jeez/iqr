/****************************************************************************
** Form implementation generated from reading ui file 'iqrCRW.ui'
**
** Created: Wed Feb 16 01:51:53 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.1   edited Nov 24 13:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "iqrCRW.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a iqrCRW as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The wizard will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal wizard.
 */
iqrCRW::iqrCRW( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QWizard( parent, name, modal, fl )
{
    if ( !name )
	setName( "iqr_crash_report" );

    WizardPage = new QWidget( this, "WizardPage" );
    WizardPageLayout = new QHBoxLayout( WizardPage, 11, 6, "WizardPageLayout"); 

    textLabel4 = new QLabel( WizardPage, "textLabel4" );
    WizardPageLayout->addWidget( textLabel4 );
    addPage( WizardPage, QString("") );

    WizardPage_2 = new QWidget( this, "WizardPage_2" );
    WizardPageLayout_2 = new QVBoxLayout( WizardPage_2, 11, 6, "WizardPageLayout_2"); 

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 

    textLabel2 = new QLabel( WizardPage_2, "textLabel2" );
    layout19->addWidget( textLabel2 );
    spacer1 = new QSpacerItem( 92, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( spacer1 );

    leSendeEmail = new QLineEdit( WizardPage_2, "leSendeEmail" );
    leSendeEmail->setMinimumSize( QSize( 250, 0 ) );
    layout19->addWidget( leSendeEmail );
    WizardPageLayout_2->addLayout( layout19 );

    layout18 = new QVBoxLayout( 0, 0, 6, "layout18"); 

    textLabel3 = new QLabel( WizardPage_2, "textLabel3" );
    layout18->addWidget( textLabel3 );

    teCrashDescription = new QTextEdit( WizardPage_2, "teCrashDescription" );
    layout18->addWidget( teCrashDescription );
    WizardPageLayout_2->addLayout( layout18 );
    addPage( WizardPage_2, QString("") );

    WizardPage_3 = new QWidget( this, "WizardPage_3" );
    WizardPageLayout_3 = new QVBoxLayout( WizardPage_3, 11, 6, "WizardPageLayout_3"); 

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    textLabel5 = new QLabel( WizardPage_3, "textLabel5" );
    layout11->addWidget( textLabel5 );
    spacer2 = new QSpacerItem( 38, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer2 );

    textLabel7 = new QLabel( WizardPage_3, "textLabel7" );
    textLabel7->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    textLabel7->setFrameShape( QLabel::LineEditPanel );
    textLabel7->setFrameShadow( QLabel::Sunken );
    layout11->addWidget( textLabel7 );
    WizardPageLayout_3->addLayout( layout11 );

    layout17 = new QVBoxLayout( 0, 0, 6, "layout17"); 

    textLabel6 = new QLabel( WizardPage_3, "textLabel6" );
    textLabel6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel6->sizePolicy().hasHeightForWidth() ) );
    textLabel6->setMinimumSize( QSize( 0, 20 ) );
    layout17->addWidget( textLabel6 );

    teMessageBody = new QTextEdit( WizardPage_3, "teMessageBody" );
    layout17->addWidget( teMessageBody );
    WizardPageLayout_3->addLayout( layout17 );

    sendStatus = new QLabel( WizardPage_3, "sendStatus" );
    WizardPageLayout_3->addWidget( sendStatus );
    addPage( WizardPage_3, QString("") );
    languageChange();
    resize( QSize(495, 524).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
iqrCRW::~iqrCRW()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void iqrCRW::languageChange()
{
    setCaption( tr( "Form2" ) );
    textLabel4->setText( tr( "<font size=\"+2\"><tt><b>iqr</b></tt> seems to have crashed.<br>\n"
"Would you like to submit a report of the crash?</font>" ) );
    setTitle( WizardPage, tr( "iqr has crashed" ) );
    textLabel2->setText( tr( "Your email address:" ) );
    textLabel3->setText( tr( "Please describe what you did before <tt><b>iqr</b></tt> crashed:" ) );
    setTitle( WizardPage_2, tr( "iqr crash details" ) );
    textLabel5->setText( tr( "You report will be sent to:" ) );
    textLabel7->setText( tr( "<Ulysses Bernardet> ulysses@ini.phys.ethz.ch" ) );
    textLabel6->setText( tr( "The report contains the following information:" ) );
    sendStatus->setText( QString::null );
    setTitle( WizardPage_3, tr( "Send report" ) );
}

