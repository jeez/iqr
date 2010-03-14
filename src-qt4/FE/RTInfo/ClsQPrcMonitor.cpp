/****************************************************************************
 ** $Filename: ClsQPrcMonitor.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Tue Sep 11 14:31:25 2001
 **
 ** $Keywords:
 ** $Description: the overly complicated way of handling the labels and icons (i.e. putting them in a
 ** list in a map) has it's reason in the fact, that at some points the gridlayout does not allow adding
 ** of things to it anymore...
 **
 *****************************************************************************/

#include <ClsFESystemManager.h>
#include "ClsQPrcMonitor.h"
#include <ClsQThermo.h>
#include "ClsFEProcess.h"

namespace {
#include <prc_alive.xpm>
#include <prc_launched.xpm>
#include <prc_ready.xpm>
#include <prc_sfp.xpm>
#include <prc_mt.xpm>
}

#include <qmessagebox.h> 

#define NR_STATES 5


/** 
 * Constructor
 */
ClsQPrcMonitor::ClsQPrcMonitor(QWidget *parent, const char * /* name=0 */, Qt::WindowFlags /* f=0 */) : 
//     QWidget ( parent, name,  WStyle_Title |WStyle_NoBorder ) {
     QDialog ( parent) {


     QPixmap qpixmapPrc_alive = QPixmap (prc_alive);  

     
     qpixmapPrc_launched = QPixmap (prc_launched);
     qpixmapPrc_ready = QPixmap (prc_ready);  
     qpixmapPrc_sfp = QPixmap (prc_sfp);   
     qpixmapPrc_mt = QPixmap (prc_mt);    

     QPalette palette;
     palette.setColor(this->backgroundRole(), QColor(0,0,0));
     this->setPalette(palette);


     qgltPrcs = new QGridLayout( this, 1, 8, 1 );

     QLabel *l;

     l = new QLabel(" PrcID ", this, "PrcID", 0); 
     l->setFixedHeight(l->sizeHint().height());
     qgltPrcs->addWidget(l,0,0);

     l = new QLabel("port ", this, "port", 0); 
     l->setFixedHeight(l->sizeHint().height());
     qgltPrcs->addWidget(l,0,1);

     l = new QLabel("A", this, "A", 0); l->setAlignment(AlignCenter);
     QToolTip::add( l, "Prc alive" );
     qgltPrcs->addWidget(l,0,2);

     l = new QLabel("P", this, "P", 0); l->setAlignment(AlignCenter);
     QToolTip::add( l, "Prc systemfile parsed" );
     qgltPrcs->addWidget(l,0,3);

     l = new QLabel("B", this, "B", 0); l->setAlignment(AlignCenter);
     QToolTip::add( l, "Prc system buildt" );
     qgltPrcs->addWidget(l,0,4);

     l = new QLabel("C", this, "C", 0); l->setAlignment(AlignCenter);
     QToolTip::add( l, "Prc connected" );
     qgltPrcs->addWidget(l,0,5);

     l = new QLabel("R", this, "R", 0); l->setAlignment(AlignCenter);
     QToolTip::add( l, "Prc ready" );
     qgltPrcs->addWidget(l,0,6);

     l = new QLabel(" speed ", this, "speed", 0);
     l->setFixedHeight(l->sizeHint().height());
     qgltPrcs->addWidget(l,0,NR_STATES+2);
};
     

/** 
 * Add a Prc to monitor.
 * @param strPrcID ID of the Prcs to add.
 */
void ClsQPrcMonitor::addPrc( string strPrcID) {
     int iRow = qgltPrcs->numRows();
     qgltPrcs->addWidget(new QLabel(strPrcID.c_str(), this, "prc", 0), iRow, 0);
   
     pair<string, int > pairTemp(strPrcID, iRow);
     mapPrcs.insert(pairTemp);

     QLabel * lbl;
     
     vector<QWidget*>  vectorLights;
     for(int ii=1; ii<=NR_STATES; ii++){
	  lbl = new QLabel(this);
	  QPalette palette;
	  palette.setBrush(lbl->backgroundRole(), QBrush(qpixmapPrc_mt));
	  lbl->setPalette(palette);

	  lbl->setFixedSize (14,14);
	  qgltPrcs->addWidget(lbl, iRow, ii+1);
	  vectorLights.push_back(lbl);
     }

     lbl = new QLabel(this);
     lbl->setText("--");
     qgltPrcs->addWidget(lbl, iRow, 1);
     vectorLights.push_back(lbl);

//         QDial *qdialSpeed = new QDial ( 0, 1000, 100, 0, this, "speed" );
//         qdialSpeed->setFixedWidth(25);
//         qdialSpeed->setFixedHeight(25);
//         qgltPrcs->addWidget(qdialSpeed, iRow, 6);
//         vectorLights.push_back(qdialSpeed);

 
//**     QwtThermo *qwtThermo = new QwtThermo (this , "speed");
     ClsQThermo *qwtThermo = new ClsQThermo (this , "speed");
//**     qwtThermo->setOrientation ( QwtThermo::Horizontal, QwtThermo::None);
     qwtThermo->setOrientation ( ClsQThermo::LEFT2RIGHT);
     qwtThermo->setRange (0., 3000.);
 

     qgltPrcs->addWidget(qwtThermo, iRow, NR_STATES + 2);
     vectorLights.push_back(qwtThermo);

     pair<string, vector<QWidget*> > pairTemp2(strPrcID, vectorLights);
     mapPrcsLights.insert(pairTemp2);
};     

/** 
 * Remove a Prc form the monitor (EXPERIMENTAL: not tested)
 * @param strPrcID ID of the Prc.
 */
void ClsQPrcMonitor::removePrc( string strPrcID) {
     if(mapPrcsLights.find(strPrcID) != mapPrcsLights.end())
	  mapPrcsLights.erase(strPrcID);
     
     if(mapPrcs.find(strPrcID) != mapPrcs.end())
	  mapPrcs.erase(strPrcID);
}



void ClsQPrcMonitor::slotSystemChanged(){
    cout << "ClsQPrcMonitor::SystemChanged()" << endl;
    
//+++    map<string, ClsInfoPrc> mapProcesses =  ClsFESystemManager::Instance()->getMapProcesses();
//+++    map<string, ClsInfoPrc>::iterator mapIteratorProcesses;
//+++    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
//+++	string strProcessID = mapIteratorProcesses->second.getPrcID();
//+++	addPrc(strProcessID);  
//+++    }
//+++

    map<string, ClsFEProcess*> mapProcesses =  ClsFESystemManager::Instance()->getMapFEProcesses();
    map<string, ClsFEProcess*>::iterator mapIteratorProcesses;
    for(mapIteratorProcesses = mapProcesses.begin(); mapIteratorProcesses!=mapProcesses.end(); ++mapIteratorProcesses){
	string strProcessID = mapIteratorProcesses->second->getProcessID();
	addPrc(strProcessID);  
    }

};

/** 
 * Set the portnumber.
 * @param strPrcID ID of the Prc.
 * @param iPort Portnumber of the Prc.
 */
void ClsQPrcMonitor::slotSetPrcPort( string strPrcID, int iPort) {

     if(mapPrcsLights.find(strPrcID) != mapPrcsLights.end()){
//	  QLabel *lbl = (QLabel *)mapPrcsLights.find(strPrcID)->second[4];
	  QLabel *lbl = (QLabel *)mapPrcsLights.find(strPrcID)->second[NR_STATES];
	  lbl->setText(iqrUtils::int2string(iPort).c_str());
     }
     
};


/** 
 * Set the time difference between the updates.
 * @param strPrcID ID of the Prc.
 * @param iDT Time difference between two updates in uSecs.
 */
void ClsQPrcMonitor::setPrcTimeDiffUSec( string strPrcID, int iDT) {
     if(mapPrcsLights.find(strPrcID) != mapPrcsLights.end()){
	  int iSpeed = (int)(1000000.0 / ((double)iDT > 0 ? (double)iDT : 1000.));

//  	  QDial *qdialSpeed = (QDial *)mapPrcsLights.find(strPrcID)->second[5];
//  	  qdialSpeed->setValue(iSpeed);

//**	  QwtThermo *qwtThermo = (QwtThermo *)mapPrcsLights.find(strPrcID)->second[NR_STATES + 1];
	  ClsQThermo *qwtThermo = (ClsQThermo *)mapPrcsLights.find(strPrcID)->second[NR_STATES + 1];
	  int iCol = (int)(130.0 / qwtThermo->maxValue() * iSpeed);
	  QColor c;
	  c.setHsv( iCol, 255, 255 ); 
	  qwtThermo->setValue((double)iSpeed);
	  qwtThermo->setFillColor ( c);

     }




};


/** 
 * Set the state of the Prc. This will define which lights will be on.
 * @param strPrcID ID of the Prc.
 * @param iState State of the Prcs as defined in ClsInfoPrc.h.
 */
void ClsQPrcMonitor::setPrcState( string strPrcID, int iState) {

     if(mapPrcsLights.find(strPrcID) != mapPrcsLights.end()){
	  QPixmap iconPrcState;

	  /* with this turned on, only one light can be on at one time */
	  for(int ii=0; ii<NR_STATES; ii++){
	       QLabel *lbl = (QLabel *)mapPrcsLights.find(strPrcID)->second[ii];
	       QPalette palette;
	       palette.setBrush(lbl->backgroundRole(), QBrush(qpixmapPrc_mt));
	       lbl->setPalette(palette);


	  }
	  /* --- */

//	      PRC_DIED             0 
//	      PRC_LAUNCHED         1 *
//	      PRC_ALIVE            2 
//	      PRC_SYSTEMFILEPARSED 3
//	      PRC_SYSTEMBUILDT     4
//	      PRC_CONNECTED2PRCS   5 *
//	      PRC_READY            6 *


	  if(mapPrcs.find(strPrcID) != mapPrcs.end()){
	       int ilblIndex = 0;
//	       int iRow = mapPrcs.find(strPrcID)->second;


	       /*
    	       QMessageBox::critical( this, "STATE",
				      iqrUtils::int2string(iState).c_str(),
  				      QMessageBox::Ok, QMessageBox::Escape );
	       */


	       switch (iState){
	       case iqrcommon::ClsInfoPrc::PRC_DIED: // 0
		    iconPrcState = qpixmapPrc_mt ;
		    break;
	       case iqrcommon::ClsInfoPrc::PRC_ALIVE:
		    iconPrcState = qpixmapPrc_alive ;
		    ilblIndex = 0;
		    break;
	       case iqrcommon::ClsInfoPrc::PRC_SYSTEMFILEPARSED:
		    iconPrcState = qpixmapPrc_sfp ;
		    ilblIndex = 1;
		    break;
	       case iqrcommon::ClsInfoPrc::PRC_SYSTEMBUILDT:
		    iconPrcState = qpixmapPrc_alive ;
		    ilblIndex = 2;
		    break;
	       case iqrcommon::ClsInfoPrc::PRC_CONNECTED2PRCS:
		    iconPrcState = qpixmapPrc_alive ;
		    ilblIndex = 3;
		    break;
	       case iqrcommon::ClsInfoPrc::PRC_READY:
		    iconPrcState = qpixmapPrc_ready ;
		    ilblIndex = 4;
		    break;
	       default:
		    iconPrcState = qpixmapPrc_mt ;
		    break;
	       }
	  
	       QLabel *lbl = (QLabel *)mapPrcsLights.find(strPrcID)->second[ilblIndex];
	       lbl->setBackgroundPixmap(iconPrcState);
	  } 
	  update();
     }
     
};

/** 
 * Reset the monitor.
 */
void ClsQPrcMonitor::reset() {
     map<string, vector<QWidget*> >::iterator mapIteratorPrcs;
     for(mapIteratorPrcs = mapPrcsLights.begin(); mapIteratorPrcs!=mapPrcsLights.end(); ++mapIteratorPrcs){
	  for(int ii=0; ii<NR_STATES; ii++){
	       QLabel *lbl = (QLabel *)mapIteratorPrcs->second[ii];
	       lbl->setBackgroundPixmap(qpixmapPrc_mt);
	  }
	  ClsQThermo *qwtThermo = (ClsQThermo *)mapIteratorPrcs->second[5];
//**	  QwtThermo *qwtThermo = (QwtThermo *)mapIteratorPrcs->second[5];
	  qwtThermo->setValue(0.);
     }
     update();
};

/// Local Variables: 
/// mode: c++
/// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SimControl/ && make -k"
/// End: 


     
