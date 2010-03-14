#include <qsocket.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpalette.h>
#include <qapplication.h>


class IpcSocket : public QSocket {
    Q_OBJECT

public:
    IpcSocket( QObject *parent) : QSocket( parent )     {
	packetSize = 0;
	connect( this, SIGNAL(readyRead()), SLOT(read()) );
    }

signals:
    void receivedText( const QString& );

private slots:
    void read() {
	Q_ULONG bytesAvail = bytesAvailable();
	QCString s;
	s.resize( bytesAvail );
	readBlock( s.data(), bytesAvail  );
	emit receivedText( s );
    }

private:
    Q_UINT32 packetSize;
};
