#include <iostream>

#include <qobject.h>
#include <qprocess.h>
#include <qvbox.h>
#include <qtextview.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qdatetime.h>


#include <stdlib.h>

using namespace std;

class UicManager : public QVBox {
    Q_OBJECT

public:
    UicManager(QStringList qstringlstArgs);
    ~UicManager() {}

public slots:
    void readFromStdout();
    void readFromStderr();
    void appEnded();


private:
    QProcess *proc;

    QString qstrCout, qstrCerr;
    QString qstrCoutSumm, qstrCerrSumm;
};

