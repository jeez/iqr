#include <iostream>

#include "EPuck_Disp.h"
#include "ClsEPuckDisp.hpp"

#define Inherited QWidget


qtProtekData::qtProtekData ( QWidget* parent, const char* name ) : Inherited( parent, name, 0 ) {
	qtarch_btnStart = new QPushButton( this, "btnStart" );
//	qtarch_btnStart->setToggleButton(TRUE);
	qtarch_btnStart->setGeometry( 5, 60, 80, 30 );
	qtarch_btnStart->setMinimumSize( 0, 0 );
	qtarch_btnStart->setMaximumSize( 32767, 32767 );
	qtarch_btnStart->setFocusPolicy( QWidget::TabFocus );
	qtarch_btnStart->setBackgroundMode( QWidget::PaletteBackground );
//	qtarch_btnStart->setFontPropagation( QWidget::NoChildren );
//	qtarch_btnStart->setPalettePropagation( QWidget::NoChildren );
	qtarch_btnStart->setText( "Start" );
	qtarch_btnStart->setAutoRepeat( FALSE );
	qtarch_btnStart->setAutoResize( FALSE );
	connect(qtarch_btnStart, SIGNAL(clicked()), SLOT(DoEPuckera()) );

	QPushButton* qtarch_btnQuit;
	qtarch_btnQuit = new QPushButton( this, "btnQuit" );
	qtarch_btnQuit->setGeometry( 100, 60, 80, 30 );
	qtarch_btnQuit->setMinimumSize( 0, 0 );
	qtarch_btnQuit->setMaximumSize( 32767, 32767 );
	qtarch_btnQuit->setFocusPolicy( QWidget::TabFocus );
	qtarch_btnQuit->setBackgroundMode( QWidget::PaletteBackground );
//	qtarch_btnQuit->setFontPropagation( QWidget::SameFont );
//	qtarch_btnQuit->setPalettePropagation( QWidget::NoChildren );
	qtarch_btnQuit->setText( "Quit" );
	qtarch_btnQuit->setAutoRepeat( FALSE );
	qtarch_btnQuit->setAutoResize( FALSE );
	connect(qtarch_btnQuit, SIGNAL(clicked()), qApp, SLOT(quit()) );

	lblFilename = new QLabel( this, "lblDim" );
	lblFilename->setGeometry( 10, 25, 70, 30 );
	lblFilename->setMinimumSize( 0, 0 );
	lblFilename->setMaximumSize( 32767, 32767 );
	lblFilename->setFocusPolicy( QWidget::NoFocus );
	lblFilename->setBackgroundMode( QWidget::PaletteBackground );
//	lblFilename->setFontPropagation( QWidget::NoChildren );
//	lblFilename->setPalettePropagation( QWidget::NoChildren );
	lblFilename->setText( "Save to File:" );
	lblFilename->setAlignment( 289 );
	lblFilename->setMargin( -1 );

	leFilename = new QLineEdit( this, "lblDim" );
	leFilename->setGeometry( 90, 20, 160, 30 );
	leFilename->setMinimumSize( 0, 0 );
	leFilename->setMaximumSize( 32767, 32767 );

//	connect(clsEPuckDisp, SIGNAL(GraphClicked( int )), this, SLOT(GraphClicked( int )) );
	
	connect(qtarch_btnQuit, SIGNAL(clicked()), qApp, SLOT(quit()) );

	resize( 400,300 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );

	clsEPuckDisp = new ClsEPuckDisp( "EPuckera Display");

	clsEPuckDisp->setGeometry(0, 0, 250, 250);
	clsEPuckDisp->show();
};







void qtProtekData::DoEPuck() {
//     clsEPuckDisp->DrawStatus(strType, iTime, fValue);     


     int aiColl[8];
     float afIR[8];
     float afMotor[2];
     int iInhibit;
     
     int ii;
     
     for (ii = 0; ii<8 ; ii++){
	  aiColl[ii] = 0;
	  afIR[ii] = 0; 
	  if (ii <= 2) afMotor[ii] = 0;
     }

     
     aiColl[4] = 1; // can only be 0 or 1
     afIR[0] = 50; // ranges from 0 to 10
     afMotor[1] = 50; // ranges from 0 to 10
     iInhibit = 2;

     clsEPuckDisp->setCollision( aiColl );
     clsEPuckDisp->setIR( afIR );
     clsEPuckDisp->setMotor( afMotor );
     clsEPuckDisp->setInhibition( iInhibit );
     clsEPuckDisp->DrawStatus( );


     QString qstrFileName = "/disks/zebra3/ulysses/Code/EPuckera_Disp/test1.ps";

     clsEPuckDisp->setFileName ( qstrFileName );
     clsEPuckDisp->saveGraph();
    
}


void qtProtekData::GraphClicked( int iPassBack) {
  switch (iPassBack){
  case RESET:
    printf("qtProtekData: received reset form plot\n");
    iTime = 0;

    break;
  default:
    break;
  }
}



qtProtekData::~qtProtekData() {
}

//=================



int main( int argc, char **argv ) {   
     
  QApplication a( argc, argv );
  


// parsing cmdline
// syntax: [Collision 0-7], [IR 0-7], [Motor 0-1], [Inhibition], "Filename"
  
    if (argc > 1)
    {
	 if (argc == 6){
	      
	      argv++; QString qstrColl(argv[0]);
	      argv++; QString qstrIR(argv[0]);      
	      argv++; QString qstrMotor(argv[0]);      
	      argv++; QString qstrInhibition(argv[0]);      
	      argv++; QString qstrFileName(argv[0]);      
	      

	      int aiColl[8];   
	      float afIR[8];   
	      float afMotor[2];
	      int iInhibit;
	      int ifound;

	      int ii;
	      QString qstrT;

	      for (ii = 0; ii < 8; ii++){
		   qstrColl = qstrColl.simplifyWhiteSpace();
		   ifound = qstrColl.find ( " ", 0, false ) > 0 ? qstrColl.find ( " ", 0, false ): qstrColl.length();
		   qstrT =  qstrColl.left(ifound + 1);
		   qstrColl.remove(0, ifound + 1);

		   aiColl[ii] =  qstrT.toInt();
//		   printf("Coll: %i\n", qstrT.toInt());
	      }
	      for (ii = 0; ii < 8; ii++){
		   qstrIR = qstrIR.simplifyWhiteSpace();
		   ifound = qstrIR.find ( " ", 0, false ) > 0 ? qstrIR.find ( " ", 0, false ): qstrIR.length();
		   qstrT =  qstrIR.left(ifound + 1);
		   qstrIR.remove(0, ifound + 1);

		   afIR[ii] = qstrT.toInt();
//		   printf("%i, %i, IR: %i\n", ii, ifound, qstrT.toInt());
	      }

	      for (ii = 0; ii < 2; ii++){
		   qstrMotor = qstrMotor.simplifyWhiteSpace();
		   ifound = qstrMotor.find ( " ", 0, false ) > 0 ? qstrMotor.find ( " ", 0, false ): qstrMotor.length();
		   qstrT =  qstrMotor.left(ifound + 1);
		   qstrMotor.remove(0, ifound + 1);

		   afMotor[ii] = qstrT.toInt();
//		   printf("Motor: %i\n", qstrT.toInt());
	      }
	      
	      iInhibit = qstrInhibition.toInt();


  	      ClsEPuckDisp clsEPuckDisp("EPuck Display");

  	      clsEPuckDisp.setGeometry(0, 0, 250, 250);
	      
  	      clsEPuckDisp.setCollision( aiColl );
  	      clsEPuckDisp.setIR( afIR );
  	      clsEPuckDisp.setMotor( afMotor );
  	      clsEPuckDisp.setInhibition( iInhibit );
  	      clsEPuckDisp.DrawStatus( );
	      
  	      clsEPuckDisp.setFileName ( qstrFileName );
  	      clsEPuckDisp.saveGraph();
	      
  	      clsEPuckDisp.show();
  	      a.setMainWidget( &clsEPuckDisp );
	      
	      
	 } else {
	      printf("not enough or too many arguments\n");
	      exit(1);
	 }




    } else {
       qtProtekData ProtekDlg;
       ProtekDlg.setGeometry( 160, 390, 
			      280, //width
			      95 //height
	    );
       a.setMainWidget( &ProtekDlg );
       ProtekDlg.show();
       return a.exec();
    }
  
 



}

