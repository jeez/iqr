#ifndef CLSFEDATABROADCASTER_H
#define CLSFEDATABROADCASTER_H

#include <map>

#include <QLabel>
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSpinBox>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

#include <idGenerator.hpp>
#include <ClsFEDataClient.h>
#include <ClsFEPlotFramework.h>
#include <ClsQNeuronStateVariableDisplay.h>

class ClsFEDataBroadcaster : public ClsFEPlotFramework,  public ClsFEDataClient  {
    Q_OBJECT

public:
    ClsFEDataBroadcaster(ClsFEDataManager *clsFEDataManager, QMutex *_qmutexSysGUI, string _strDataClientID);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, string strRange);
    string addStateVariableDisplay(ClsFEGroup* _clsFEGroup, list<string> lstSelectedStates, string strRange);
    void init();

    void setConfig(ClsDataClientConfig clsDataClientConfig){};
    ClsDataClientConfig getConfig();
    void groupChanged(string strGroupID);
    void groupDeleted(string strID);
    void close(){ QWidget::close();};



public slots:
    string addStateVariableDisplay(string, list<string>, string strRange);
    void removeStateVariableDisplay(string);
    void slotHideControls ( bool bIsRight );

private slots:
    void update();
    void broadcast(bool b);
    void toggleAllGroups(bool b);

private:

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent( QCloseEvent* );

    void DataSinkAdded(string strID, int iColor){};
    void DataSinkRemoved(string strID){};

    void addAllGroups();
    void removeAllGroups();

    QFrame* qframeStateVariableDisplays;
    QHBoxLayout *boxlayoutStateFrames;
    map<string, ClsQNeuronStateVariableDisplay*> mapStateVariableDisplays;

    int iPort;
    QUdpSocket* qs;
    QHostAddress qa;
    QPushButton *qpbControl;
    QSpinBox *qspinPort;
    QSpinBox *qspinInterval;
    QLineEdit *qleIPReceiver;

    QTimer *qtimer;
    QPushButton *qpbAddAll;

    bool bAllGroups;
    QLabel *qlblAllGroups;


};

#endif

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
