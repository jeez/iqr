#ifndef IQRCRWIMPL_H
#define IQRCRWIMPL_H

#include <iostream>
#include <qtimer.h>
#include "iqrCRW.h"

using namespace std;

class iqrCRWimpl : public iqrCRW {
    Q_OBJECT

public:
    iqrCRWimpl(QString _qstrSubject, QString _qstrCoutSumm, QWidget* parent = 0, const char* name = 0 );

private slots:
    void sendReport();
void smtpStatus(const QString &);
void slotSendError( const QString & );
void slotMessageSent();


private:
void showPage(QWidget * page);

QString qstrSubject, qstrCoutSumm;
QString qstrMessageBody;
};

#endif
