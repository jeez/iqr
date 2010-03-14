#ifndef CMDBUILDIMPL_H
#define CMDBUILDIMPL_H


#include <q3listview.h>
#include <qdom.h>
#include <qtimer.h>
#include <qfileinfo.h>
#include <QDialog>


#include "ui_CmdBuild.h"
#include "ClsSystemDB.h"

using namespace Ui;

class CmdBuildImpl : public QDialog, public Ui::CmdBuild {
    Q_OBJECT

public:
    CmdBuildImpl(QWidget *parent = 0, const char *name = 0 );
    ~CmdBuildImpl();


signals:
    void sigCommand(QString qstr);

private slots:
void slotComboCmd(const QString &);
    void slotComboIdType(const QString&);
    void slotComboItemType(const QString&);
    void slotComboItemIdentifier(const QString&);
    void slotGenerate();
    void slotOpenFile(QString _qstrFilename="");
    void slotReloadFile();
    void slotCheckFile();
private:
    ClsSystemDB *clsSystemDB;
    QString qstrFilename;
    QTimer* qtimerFilewatch;
    QFileInfo qfinfoCurrentFile;

};

#endif 

//// Local Variables:
//// mode: c++
//// End:
