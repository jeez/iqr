#include <iostream>

#include <QToolTip>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLayout>
#include <QBitmap>
#include <QMessageBox> 

#include "ClsQGroupStateManip.h"

#include "ClsQSAList.h"
#include <ClsFESystemManager.h>
#include <ClsFEConnection.h>
#include <ClsFEGroup.h>
#include <ClsTopologyRect.h>
#include <ClsTopologySparse.h>
#include <ClsTopologyHex.h>

#include "dialogButtons.hpp"

#include "ClsQStateArrayViewRect.h"
#include "ClsQStateArrayViewSparse.h"
#include "ClsQStateArrayViewHex.h"

#include "ClsGroupManipPattern.h"

#include "pencil_12x12.xpm"
#include "eraser_12x12.xpm"

#include "cursors/pencil_cursor.xbm"
#include "cursors/pencil_cursor_mask.xbm"

#include "cursors/eraser_cursor.xbm"
#include "cursors/eraser_cursor_mask.xbm"

//#define DEBUG_CLSQGROUPSTATEMANIP

#define DEFAULTVALUE 1.0

ClsQGroupStateManip::ClsQGroupStateManip ( const char * _name = 0,string _strGroupID = ""):
    QFrame( 0, _name, Qt::WDestructiveClose), strGroupID(_strGroupID) {

    bApplied = false;
    clsQStateArrayView = NULL;
    iInterval = 1;
    iLoops = 1;
    iStepSize = 1;


    string strGroupName = ClsFESystemManager::Instance()->getGroupName(strGroupID).c_str();
    string strTitle = "State Manipulation Panel for \"" + strGroupName + "\"";
    this->setCaption(strTitle.c_str());


    QBitmap qbmEraser(  eraser_cursor_width,  eraser_cursor_height,  eraser_cursor_bits, TRUE );
    QBitmap qbmEraserMask(  eraser_cursor_mask_width,  eraser_cursor_mask_height,  eraser_cursor_mask_bits, TRUE );
    qcursorEraser = new QCursor( qbmEraser, qbmEraserMask,0 ,0 ); 

    QBitmap qbmPencil(  pencil_cursor_width,  pencil_cursor_height,  pencil_cursor_bits, TRUE );
    QBitmap qbmPencilMask(  pencil_cursor_mask_width,  pencil_cursor_mask_height,  pencil_cursor_mask_bits, TRUE );
    qcursorPencil = new QCursor( qbmPencil, qbmPencilMask, 0, 0 ); 

    QSplitter *qsplitter = new QSplitter(this);
    QFrame *qfmLeftPane = new QFrame(qsplitter);

    QBoxLayout * layoutMain = new QHBoxLayout( this);
    layoutMain->setResizeMode (QLayout::Fixed);
    layoutMain->addWidget(qsplitter);



    QBoxLayout * layoutLeftPane = new QVBoxLayout( qfmLeftPane, 5, -1, "mainL");

    qlblCaption = new QLabel(qfmLeftPane);
    qlblCaption->setText(strGroupName.c_str());

    layoutLeftPane->addWidget(qlblCaption);

    qfmStateArray = new QFrame(qfmLeftPane);;
    QHBoxLayout *qlayoutQfm = new QHBoxLayout( qfmStateArray);
    qlayoutQfm->setAutoAdd ( true);

    createStateArray(strGroupID);
    layoutLeftPane->addWidget(qfmStateArray, 0, Qt::AlignHCenter);
    qfmStateArray->show();
    clsQStateArrayView->show();
    clsQStateArrayView->setValue(DEFAULTVALUE);

    QHBoxLayout *qlayoutGradient = new QHBoxLayout( layoutLeftPane);

    QString qstr;

    QLabel* qlblMin = new QLabel(qfmLeftPane);
    qstr.setNum(fMinVal());

    qlblMin->setText(qstr);
    qlayoutGradient->addWidget(qlblMin, 0, Qt::AlignRight);

    qlblGradientPixmap = new QLabel(qfmLeftPane);;
    qlayoutGradient->addWidget(qlblGradientPixmap, 1, Qt::AlignHCenter);

    qstr.setNum(fMaxVal());
    QLabel* qlblMax = new QLabel(qfmLeftPane);
    qlblMax->setText(qstr);
    qlayoutGradient->addWidget(qlblMax);

    int iImgWidth = clsQStateArrayView->width() - qlblMin->minimumWidth() - qlblMax->minimumWidth() - 30;
    int iImgHeight = 13;
    qlblGradientPixmap->setFixedSize(iImgWidth,iImgHeight);
    qlblGradientPixmap->setPixmap(clsQStateArrayView->getGradientPixmap(iImgWidth, iImgHeight));

/* -------------------------------- */
    qgrpbxTools = new QGroupBox( );

    QLabel *lblValue = new QLabel();
    lblValue->setText("Value:");

    qdblspnbx = new QDoubleSpinBox( qgrpbxTools );
    qdblspnbx->setMinimum(fMinVal());
    qdblspnbx->setMaximum(fMaxVal());
    qdblspnbx->setDecimals(3);
    qdblspnbx->setSingleStep ( 0.01);
    qdblspnbx->setValue(DEFAULTVALUE);
    connect(qdblspnbx, SIGNAL(valueChanged(double)), this, SLOT(slotChangeValue(double)));

    QPushButton* qpbtnPen = new QPushButton (QIcon(QPixmap(pencil)), "");
    qpbtnPen->setToggleButton ( true);
    qpbtnPen->setFlat(true);
    qpbtnPen->setChecked(true);

    slotSelectTool(TOOL_PENCIL);


    QPushButton* qpbtnEraser = new QPushButton (QIcon(QPixmap(eraser)), "");
    qpbtnEraser->setToggleButton ( true);
    qpbtnEraser->setFlat(true);

    QHBoxLayout *qlayoutTools = new QHBoxLayout;
    qlayoutTools->addWidget(lblValue);
    qlayoutTools->addWidget(qdblspnbx);
    qlayoutTools->addWidget(qpbtnPen);
    qlayoutTools->addWidget(qpbtnEraser);
    qgrpbxTools->setLayout(qlayoutTools);


    qbtngrpTools = new QButtonGroup();
    connect(qbtngrpTools, SIGNAL(buttonClicked(int)), this, SLOT(slotSelectTool(int)));
    qbtngrpTools->addButton(qpbtnPen, TOOL_PENCIL);
    qbtngrpTools->addButton(qpbtnEraser, TOOL_ERASER);
    layoutLeftPane->addWidget(qgrpbxTools);

/* ------------------------------------ */

    QHBoxLayout *layout2 = new QHBoxLayout( layoutLeftPane);

    QPushButton *qpbtnClear = new QPushButton ("Clear", qfmLeftPane);
    connect(qpbtnClear, SIGNAL(clicked()), this, SLOT(slotClear()));
    layout2->addWidget(qpbtnClear, Qt::AlignTop);

    QPushButton *qpbtnAdd = new QPushButton ("Add", qfmLeftPane);
    connect(qpbtnAdd, SIGNAL(clicked()), this, SLOT(slotAdd()));
    layout2->addWidget(qpbtnAdd, Qt::AlignTop);

    QPushButton *qpbtnReplace = new QPushButton ("Replace", qfmLeftPane);
    connect(qpbtnReplace, SIGNAL(clicked()), this, SLOT(slotReplace()));
    layout2->addWidget(qpbtnReplace, Qt::AlignTop);

/* ------------------------------------ */
    QGroupBox *qgrpbxMode = new QGroupBox("Mode", qfmLeftPane);
    QRadioButton *qrbClamp = new QRadioButton ("Clamp", qgrpbxMode );
    qrbClamp->setChecked( TRUE );
    slotSetMode(ClsGroupManipPattern::MODE_CLAMP);

    QRadioButton *qrbAddPattern = new QRadioButton ("Add", qgrpbxMode );
    QRadioButton *qrbMultiplyPattern = new QRadioButton ("Multiply", qgrpbxMode );

    QHBoxLayout *qlayoutMode = new QHBoxLayout;
    qlayoutMode->addWidget(qrbClamp, ClsGroupManipPattern::MODE_CLAMP);
    qlayoutMode->addWidget(qrbAddPattern, ClsGroupManipPattern::MODE_ADD);
    qlayoutMode->addWidget(qrbMultiplyPattern, ClsGroupManipPattern::MODE_MULTIPLY);
    qgrpbxMode->setLayout(qlayoutMode);

    QButtonGroup *qbtngrpMode = new QButtonGroup();
    connect(qbtngrpMode, SIGNAL(buttonClicked(int)), SLOT(slotSetMode(int)) );
    qbtngrpMode->addButton(qrbClamp, ClsGroupManipPattern::MODE_CLAMP);
    qbtngrpMode->addButton(qrbAddPattern, ClsGroupManipPattern::MODE_ADD);
    qbtngrpMode->addButton(qrbMultiplyPattern, ClsGroupManipPattern::MODE_MULTIPLY);
    layoutLeftPane->addWidget(qgrpbxMode);
/* ------------------------------------ */

/* ------------------------------------ */
    QGroupBox *qgrpbxPlayBack = new QGroupBox("Play Back", qfmLeftPane);

    QRadioButton *qrbPersist = new QRadioButton( "For ever" );
    qrbPersist->setChecked( TRUE );
    slotSetPlayback(ClsGroupManipPattern::PLAYBACK_LOOP);

    QRadioButton *qrbIterations = new QRadioButton( "Times");


    QButtonGroup *qbtngrpPlayBack = new QButtonGroup();
    connect(qbtngrpPlayBack, SIGNAL(buttonClicked(int)), SLOT(slotSetPlayback(int)) );
    qbtngrpPlayBack->addButton(qrbPersist, ClsGroupManipPattern::PLAYBACK_LOOP);
    qbtngrpPlayBack->addButton(qrbIterations, ClsGroupManipPattern::PLAYBACK_ITERATIONS);


    qspnbxIterations = new QSpinBox( );
    qspnbxIterations->setMinimum(1);
    qspnbxIterations->setMaximum(INT_MAX);
    qspnbxIterations->setMaximumWidth(50);
    connect(qspnbxIterations, SIGNAL(valueChanged(int)), this, SLOT(slotSetLoops(int)));


    QLabel *qlblInterval = new QLabel();
    qlblInterval->setText("Interval");

    qspnbxInterval = new QSpinBox( );
    qspnbxInterval->setMinimum(1);
    qspnbxInterval->setMaximum(INT_MAX);
    qspnbxInterval->setMaximumWidth(50);
    connect(qspnbxInterval, SIGNAL(valueChanged(int)), this, SLOT(slotSetInterval(int)));


    QLabel *qlblStepSize = new QLabel();
    qlblStepSize->setText("StepSize");

    qspnbxStepSize = new QSpinBox(); 
    qspnbxStepSize->setMinimum(1);
    qspnbxStepSize->setMaximum(INT_MAX);
    qspnbxStepSize->setMaximumWidth(50);
    connect(qspnbxStepSize, SIGNAL(valueChanged(int)), this, SLOT(slotSetInterval(int)));


    QGridLayout* qglayoutPlayBack = new QGridLayout ();
    qglayoutPlayBack->addWidget(qrbPersist, 1,1);
    qglayoutPlayBack->addWidget(qrbIterations, 1, 2);
    qglayoutPlayBack->addWidget(qspnbxIterations, 1, 3);


    qglayoutPlayBack->addWidget(qlblInterval, 2, 2);
    qglayoutPlayBack->addWidget(qspnbxInterval, 2, 3);
    qglayoutPlayBack->addWidget(qlblStepSize, 3,2);
    qglayoutPlayBack->addWidget(qspnbxStepSize, 3, 3);
    qgrpbxPlayBack->setLayout(qglayoutPlayBack);
    


    layoutLeftPane->addWidget(qgrpbxPlayBack);
/* ------------------------------------ */

    QHBoxLayout *qlayoutCmdButtons = new QHBoxLayout( layoutLeftPane, 5);

    QPushButton *qpbtnApply = new QPushButton ("Send", qfmLeftPane);
    connect(qpbtnApply, SIGNAL(clicked()), this, SLOT(slotApply()));

    qpbtnRevoke = new QPushButton ("Revoke", qfmLeftPane);
    qpbtnRevoke->setEnabled(false);
    connect(qpbtnRevoke, SIGNAL(clicked()), this, SLOT(slotRevoke()));

    QPushButton *qpbtnClose = new QPushButton ("Close", qfmLeftPane);
    connect(qpbtnClose, SIGNAL(clicked()), this, SLOT(close()));

    qlayoutCmdButtons->addWidget(qpbtnApply);
    qlayoutCmdButtons->addWidget(qpbtnRevoke);
    qlayoutCmdButtons->addWidget(qpbtnClose);



    QToolTip::add(qpbtnPen, "Pencil");
    QToolTip::add(qpbtnEraser, "Eraser");
    QToolTip::add(qpbtnClear, "Clear");
    QToolTip::add(qpbtnAdd, "Add");

    QToolTip::add(qrbClamp, "Repace Value");

    QToolTip::add(qrbAddPattern, "Add Values");
    QToolTip::add(qrbMultiplyPattern, "Mutliply with Values");

    QToolTip::add(qrbPersist, "Apply for ever");
    QToolTip::add(qspnbxIterations, "Apply for selected timesteps");
    QToolTip::add(qspnbxInterval, "Apply every X timestep");
    QToolTip::add(qspnbxStepSize, "Apply step by X");





    clsQSAList = new ClsQSAList(qsplitter, this);
    connect(clsQSAList, SIGNAL(sigChangeMatrix(vector <vector<double> >)), this, SLOT(slotMatrixChanged(vector <vector<double> >)));

    clsQSAList->show();
    qsplitter->setResizeMode(clsQSAList, QSplitter::FollowSizeHint);
};




void ClsQGroupStateManip::createStateArray(string _strGroupID) {


    ClsBaseTopology* clsBaseTopology = ClsFESystemManager::Instance()->getFEGroup(_strGroupID)->getTopology();
    clsQStateArrayView = NULL;
    if(dynamic_cast<ClsTopologyRect*>(clsBaseTopology)){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
	cout << "cast clsBaseTopologyGroup to ClsTopologyRect" << endl;
#endif                            
	clsQStateArrayView = new ClsQStateArrayViewRect(qfmStateArray, clsBaseTopology, "source", ClsBaseQStateArrayView::LIST);
    }
    else if(dynamic_cast<ClsTopologyHex*>(clsBaseTopology)){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
	cout << "cast clsBaseTopologyGroup to ClsTopologyHex" << endl;
#endif                            
//***	clsQStateArrayView = new ClsQStateArrayViewHex(qfmStateArray, clsBaseTopology, "source", ClsBaseQStateArrayView::LIST);
    }

    else if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
	cout << "cast clsBaseTopologyGroup to ClsTopologySparse" << endl;
#endif                            
//***	clsQStateArrayView = new ClsQStateArrayViewSparse(qfmStateArray, clsBaseTopology, "source", ClsBaseQStateArrayView::LIST);
    }

    if(clsQStateArrayView!=NULL){
    clsQStateArrayView->setColorMode(ClsBaseQStateArrayView::BLUE2RED);

    clsQStateArrayView->setMinValue(fMinVal());
    clsQStateArrayView->setMaxValue(fMaxVal());
    qfmStateArray->setMinimumSize (clsQStateArrayView->minimumSize());
    }

}



void ClsQGroupStateManip::slotCellClicked( int /* iX */, int /* iY */) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotCellClicked( int iX, int iY)" << endl;
#endif                            
}


void ClsQGroupStateManip::slotSelectTool(int iID) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSelectTool(int)" << endl;
#endif                            
    if(iID == ClsQGroupStateManip::TOOL_PENCIL){
	clsQStateArrayView->setValue(qdblspnbx->value());
	clsQStateArrayView->setCursor(*qcursorPencil);
    }
    else {
	clsQStateArrayView->setValue(0.);
	clsQStateArrayView->setCursor(*qcursorEraser);
    }
}


void ClsQGroupStateManip::slotClear() {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotClear()" << endl;
#endif                            
    clsQStateArrayView->clear();
}

void ClsQGroupStateManip::slotAdd() {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotAdd()" << endl;
#endif                            
    QPixmap qpm = clsQStateArrayView->getValuePixmap(clsQStateArrayView->width(), clsQStateArrayView->height());

    clsQSAList->insertSA(qpm, "", clsQStateArrayView->getMatrix() );
}

void ClsQGroupStateManip::slotReplace() {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotReplace()" << endl;
#endif                            
    QPixmap qpm = clsQStateArrayView->getValuePixmap(clsQStateArrayView->width(), clsQStateArrayView->height());

    clsQSAList->insertSA(qpm, "", clsQStateArrayView->getMatrix(), true );
}

void ClsQGroupStateManip::slotGroupChanged() {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotGroupChanged()" << endl;
#endif                            
    qlblCaption->setText(ClsFESystemManager::Instance()->getGroupName(strGroupID).c_str());
    clsQStateArrayView->hide();
    createStateArray(strGroupID);
    clsQStateArrayView->show();

    if(bApplied){
	ClsFESystemManager::Instance()->getFEGroup(strGroupID)->removeGroupManipPattern();
	slotApply();
    }


}

void ClsQGroupStateManip::slotRevoke(){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotRevoke()" << endl;
#endif                            
    ClsFESystemManager::Instance()->getFEGroup(strGroupID)->removeGroupManipPattern();

    bApplied = false;
    qpbtnRevoke->setEnabled(false);

}


void ClsQGroupStateManip::slotApply(){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotApply()" << endl;
#endif                            
    /* check for the group width/height.... */

    int iPatternCount = clsQSAList->getCount();

    if(iPatternCount<1){
	QMessageBox::information( this, "IQR", "Pattern stack is empty\n"
				  "No patterns will be applied." );
    } else {
	bool bSparse = false;
	if(dynamic_cast<ClsTopologyHex*>(ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getTopology())){
	    bSparse = true;
	}
	
	ClsFESystemManager::Instance()->getFEGroup(strGroupID)->removeGroupManipPattern();

	ClsGroupManipPattern* clsGroupManipPattern = ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getGroupManipPattern();
	clsGroupManipPattern->setMode(iMode);
	clsGroupManipPattern->setPlayBack(iPlayBack);
	clsGroupManipPattern->setInterval(iInterval);
	clsGroupManipPattern->setStepSize(iStepSize);
	clsGroupManipPattern->setLoops(iLoops);
	clsGroupManipPattern->setDepth(clsQSAList->getCount());

	ClsBaseTopology* clsBaseTopology = ClsFESystemManager::Instance()->getFEGroup(strGroupID)->getTopology();
 
	for(int ii=0; ii<clsQSAList->getCount(); ii++){
	    vector <vector<double> > m = clsQSAList->getVectorAt(ii);
	    clsGroupManipPattern->insertPattern(ii, matrix2vector(clsBaseTopology, m));
	}
    }

    bApplied = true;
    qpbtnRevoke->setEnabled(true);

}

valarray<double> ClsQGroupStateManip::matrix2vector(ClsBaseTopology* clsBaseTopology, vector <vector<double> > _m){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::matrix2vector(ClsBaseTopology* clsBaseTopology, vector <vector<double> > _m)" << endl;
#endif                            
    valarray<double> va(clsBaseTopology->Size());

    unsigned int iCellsX = clsBaseTopology->nrCellsHorizontal();
    unsigned int iCellsY = clsBaseTopology->nrCellsVertical();
    
    /* determin what is smaller: the group or the pattern */
    int iXBoundary = (iCellsX < (_m.size()-1)    ? iCellsX : (_m.size()-1));
    int iYBoundary = (iCellsY < (_m[0].size()-1) ? iCellsY : (_m[0].size()-1));

#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "iCellsX, _m.size(): " << iCellsX << ", " <<  _m.size() << endl;
    cout << "iCellsY, _m[0].size(): " << iCellsY  << ", " <<  _m[0].size() << endl; 
    cout << "clsBaseTopology->Size(): " << clsBaseTopology->Size() << endl;
    cout << "iXBoundary: " << iXBoundary << endl;
    cout << "iYBoundary: " << iYBoundary << endl;
#endif

    
    for(int iY=1; iY<=iYBoundary; iY++){
	for(int iX=1; iX<=iXBoundary; iX++){
	    int iIndex = clsBaseTopology->pos2index(iX, iY);
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
	    cout << "iX: " << iX << ", iY: " << iY << ", iIndex: " << iIndex << endl;
	    cout << "_m[iX][iY]; " << _m[iX][iY] << endl;
#endif
	    va[iIndex] = _m[iX][iY];
	}
    }    
    return va;
}


valarray<double> ClsQGroupStateManip::matrix2vector(bool bSparse, int iSize, vector <vector<double> > _m){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
				  
#endif                            
    valarray<double> va(iSize);
    int iCellCounter = 0;
    for(unsigned int ii=1; ii<_m.size(); ii++){
	vector<double> vaTemp = _m[ii];
	for(unsigned int i2=1; i2<vaTemp.size(); i2++){
	    if(bSparse){
		if(vaTemp[i2] > -99){
		    va[iCellCounter] = vaTemp[i2];
		    iCellCounter++;
		}
	    } else {
		va[iCellCounter] = vaTemp[i2];
		iCellCounter++;
	    }
	}
    }
    return va;
}


void ClsQGroupStateManip::closeEvent( QCloseEvent* ce ) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "iqrfe::ClsSubtypedAutoDialog::closeEvent( QCloseEvent* ce )" << endl;
#endif                            

    ClsFESystemManager::Instance()->getFEGroup(strGroupID)->removeGroupManipPattern();

    emit sigDiagramClosed(strGroupID);
    ce->accept();
}


void ClsQGroupStateManip::slotMatrixChanged(vector <vector<double> > v) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotMatrixChanged(vector <vector<double> > v)" << endl;
#endif                            
    clsQStateArrayView->setMatrix(v);

}


QPixmap ClsQGroupStateManip::getPixmap(vector <vector<double> > v) {
    clsQStateArrayView->setMatrix(v);
    QPixmap qpm = clsQStateArrayView->getValuePixmap(clsQStateArrayView->width(), clsQStateArrayView->height());

    return qpm;

}

void ClsQGroupStateManip::slotSetMode(int i) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSetMode(int)" << endl;
#endif                            
    iMode = i;
}

void ClsQGroupStateManip::slotSetPlayback(int i) {
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSetPlayback(int)" << endl;
#endif                            
    iPlayBack = i;
}


void ClsQGroupStateManip::slotSetInterval(int ii){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSetInterval(int i)" << endl;
#endif                            
    iInterval = ii;
}

void ClsQGroupStateManip::slotSetStepSize(int ii){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSetStepSize(int i)" << endl;
#endif                            
    iStepSize = ii;
}

void ClsQGroupStateManip::slotSetLoops(int ii){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotSetLoops(int i)" << endl;
#endif                            
    iLoops = ii;
}

void ClsQGroupStateManip::slotChangeValue(double dd){
#ifdef DEBUG_CLSQGROUPSTATEMANIP  
    cout << "ClsQGroupStateManip::slotChangeValue(double dd)" << endl;
#endif                            

    clsQStateArrayView->setValue(dd);
    clsQStateArrayView->setCursor(*qcursorPencil);
    if(qbtngrpTools->button (TOOL_PENCIL)!=NULL){
	qbtngrpTools->button (TOOL_PENCIL)->setChecked(true);
    }


}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../ && make -k "
//// End:
