#ifndef CMDBUILDIMPL_H
#define CMDBUILDIMPL_H


#include <qlistview.h>
#include <qdom.h>

#include "CmdBuild.h"
#include "ClsSystemDB.h"

class CmdBuildImpl : public CmdBuild {
    Q_OBJECT

public:
    CmdBuildImpl(QWidget *parent = 0, const char *name = 0 );
    ~CmdBuildImpl();

private slots:
void slotComboCmd(const QString &);
    void slotComboIdType(const QString&);
    void slotComboItemType(const QString&);
    void slotComboItemIdentifier(const QString&);
    void slotGenerate();
    void slotOpenFile();
private:
    ClsSystemDB *clsSystemDB;
};

#endif 
