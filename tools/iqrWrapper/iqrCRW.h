/****************************************************************************
** Form interface generated from reading ui file 'iqrCRW.ui'
**
** Created: Wed Feb 16 01:51:27 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.1   edited Nov 24 13:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IQR_CRASH_REPORT_H
#define IQR_CRASH_REPORT_H

#include <qvariant.h>
#include <qwizard.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QWidget;
class QLabel;
class QLineEdit;
class QTextEdit;

class iqrCRW : public QWizard
{
    Q_OBJECT

public:
    iqrCRW( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~iqrCRW();

    QWidget* WizardPage;
    QLabel* textLabel4;
    QWidget* WizardPage_2;
    QLabel* textLabel2;
    QLineEdit* leSendeEmail;
    QLabel* textLabel3;
    QTextEdit* teCrashDescription;
    QWidget* WizardPage_3;
    QLabel* textLabel5;
    QLabel* textLabel7;
    QLabel* textLabel6;
    QTextEdit* teMessageBody;
    QLabel* sendStatus;

protected:
    QHBoxLayout* WizardPageLayout;
    QVBoxLayout* WizardPageLayout_2;
    QHBoxLayout* layout19;
    QSpacerItem* spacer1;
    QVBoxLayout* layout18;
    QVBoxLayout* WizardPageLayout_3;
    QHBoxLayout* layout11;
    QSpacerItem* spacer2;
    QVBoxLayout* layout17;

protected slots:
    virtual void languageChange();

};

#endif // IQR_CRASH_REPORT_H
