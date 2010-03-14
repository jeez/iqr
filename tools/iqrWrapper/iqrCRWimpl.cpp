#include <qframe.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <qapplication.h>

#include "iqrCRWimpl.h"
#include "smtp.h"

iqrCRWimpl::iqrCRWimpl(QString _qstrSubject, QString _qstrCoutSumm, QWidget*, const char*):
    qstrSubject(_qstrSubject), qstrCoutSumm(_qstrCoutSumm) {

    
    setFinishEnabled( WizardPage_3, TRUE );
    connect( finishButton() , SIGNAL(clicked()), SLOT(sendReport()) );
    connect( cancelButton() , SIGNAL(clicked()), qApp, SLOT(quit()) );
    finishButton()->setText("&Send");

}


void iqrCRWimpl::showPage(QWidget *page){
//    cout << "qrCRWimpl::showPage(QWidget * page)" << endl;
    if(page == WizardPage_3){
	qstrMessageBody.append("Subject: " + qstrSubject);
	qstrMessageBody.append("\n");
	qstrMessageBody.append("Description: " + teCrashDescription->text());
	qstrMessageBody.append("\n");
	qstrMessageBody.append("Dump: " + qstrCoutSumm);
	teMessageBody->setText(qstrMessageBody);
    }
    QWizard::showPage(page);
}

void iqrCRWimpl::sendReport(){
    QString qstrSenderEmail = leSendeEmail->text();
    QString qstrReceiverEmail = "ulysses@ini.phys.ethz.ch";

    Smtp *smtp = new Smtp(qstrSenderEmail, qstrReceiverEmail, qstrSubject, qstrMessageBody );
   connect( smtp, SIGNAL(status(const QString &)), SLOT(smtpStatus(const QString &)) );
    connect( smtp, SIGNAL(smtpError(const QString &)), SLOT(slotSendError(const QString &)) );
    connect( smtp, SIGNAL(messageSent()), SLOT(slotMessageSent()) );
}


void iqrCRWimpl::smtpStatus(const QString &qstr){
    cout << "iqrCRWimpl::smtpStatus(const QString &qstr)" << endl;
    cout << qstr << endl;
}




void iqrCRWimpl::slotSendError( const QString &qstr){
    cout << "iqrCRWimpl::slotSendError( const QString &qstr)" << endl;
    cout << qstr << endl;

    QMessageBox::warning( this,
			  tr( "iqr crash reporter" ),
			  tr( "Unexpected reply from SMTP server:\n\n" ) +
			  qstr );

    qApp->quit();
}

void iqrCRWimpl::slotMessageSent(){
    cout << "iqrCRWimpl::slotMessageSent()" << endl;
    qApp->quit();
}

