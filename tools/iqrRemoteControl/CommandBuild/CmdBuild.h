/****************************************************************************
** Form interface generated from reading ui file 'CmdBuild.ui'
**
** Created: Wed Feb 9 01:08:31 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CMDBUILD_H
#define CMDBUILD_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;

class CmdBuild : public QDialog
{
    Q_OBJECT

public:
    CmdBuild( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CmdBuild();

    QLabel* textLabel1_4;
    QComboBox* comboCmd;
    QComboBox* comboItemIdentifierType;
    QComboBox* comboItemIdentifier;
    QLineEdit* leValue;
    QLabel* textLabel1_2;
    QLabel* textLabel1_3;
    QComboBox* comboParamName;
    QComboBox* comboItemType;
    QLabel* textLabel1;
    QPushButton* cmdGenerate;
    QLineEdit* leCommand;
    QPushButton* pbOpenFile;
    QPushButton* quitButton;

protected:
    QVBoxLayout* CmdBuildLayout;
    QSpacerItem* spacer5;
    QHBoxLayout* layout9;
    QSpacerItem* spacer7;
    QGridLayout* layout4;
    QHBoxLayout* layout7;
    QSpacerItem* spacer6;
    QHBoxLayout* layout12;
    QSpacerItem* Spacer6;

protected slots:
    virtual void languageChange();

};

#endif // CMDBUILD_H
