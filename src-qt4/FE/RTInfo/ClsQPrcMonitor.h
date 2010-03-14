/****************************************************************************
 ** $Filename: ClsQPrcMonitor.h
 **
 ** $Author: Ulysses Bernardet
 **
 *****************************************************************************/


#ifndef CLSQPRCMONITOR_H
#define CLSQPRCMONITOR_H    /*+ To stop multiple inclusions. +*/


#include <string>
#include <map>
#include <vector>
#include <qpushbutton.h> 
#include <qwidget.h>
#include <qcolor.h> 
#include <qlayout.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qtooltip.h> 
#include <qdialog.h>


#include <ClsInfoPrc.h>


#include <iqrUtils.h>

using namespace std;

class ClsFESystemManager;

//class ClsQPrcMonitor: public QWidget  {
class ClsQPrcMonitor: public QDialog {
	
     Q_OBJECT
public:

     ClsQPrcMonitor(QWidget *parent, const char * name, Qt::WindowFlags f);
//BFS     void setSystemManagerPointer(ClsFESystemManager *_clsFESystemManager) {
//BFS	  clsFESystemManager = _clsFESystemManager; 
//BFS     };

     void addPrc( string strID);     
     void removePrc( string strPrcID);
     void setPrcTimeDiffUSec( string strPrcID, int iDT);

     void setPrcState( string strPrcID, int iPrcStatus );     
     void reset( );

public slots:
     void slotSystemChanged();
     void slotSetPrcPort( string strPrcID, int iPort);
    

private:
     QGridLayout *qgltPrcs;
     map<string, int> mapPrcs;
//     map<string, vector<QLabel*> > mapPrcsLights;
     map<string, vector<QWidget*> > mapPrcsLights;

     QPixmap qpixmapPrc_alive;
     QPixmap qpixmapPrc_launched;
     QPixmap qpixmapPrc_ready;
     QPixmap qpixmapPrc_sfp;
     QPixmap qpixmapPrc_mt;

//BFS      ClsFESystemManager *clsFESystemManager;

};


#endif /* CLSQPRCMONITOR_H */

/// Local Variables: 
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
/// End: 
