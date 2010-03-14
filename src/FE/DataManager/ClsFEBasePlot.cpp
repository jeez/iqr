/****************************************************************************
 ** $Filename: ClsFEBasePlot.cpp
 ** $Header: /home/ulysses/Code/iqr421_Redesign/FE/DataMgr/ClsFEBasePlot.cpp,v 1.1 2001/11/01 16:45:54 ulysses Exp ulysses $
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 **
 ** $CreateDate: Tue Oct 30 12:00:37 2001
 ** $Date: 2001/11/01 16:45:54 $
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#include <qobjectlist.h>


#include <ClsFEBasePlot.h>
#include <ClsQDivider.h>
#include <ClsFEDataSink.h>
#include <ClsQNeuronStateVariableDisplay.h> 




//$$$ClsFEBasePlot::ClsFEBasePlot() : QWidget(0) {
//$$$     this->setBackgroundColor(QColor(204,204,204));
//$$$     QHBoxLayout *boxlayoutBase = new QHBoxLayout(this, 0, 3);
//$$$
//$$$     boxlayoutPlot = new QHBoxLayout(boxlayoutBase,0,0);
//$$$//     boxlayoutBase->addLayout(boxlayoutPlot);
//$$$
//$$$
//$$$/* the divider inbetween the left and right pane */
//$$$     clsQDivider = new ClsQDivider(this);
//$$$     boxlayoutBase->addWidget(clsQDivider);
//$$$     connect(clsQDivider, SIGNAL(clicked(bool)), SLOT(slotHideControls ( bool )));
//$$$/* ----------------------- */
//$$$
//$$$     qframeControls = new QFrame(this);
//$$$     qframeControls->setFixedWidth(0);
//$$$     boxlayoutStateFrames = new QHBoxLayout(qframeControls,0,0);
//$$$     boxlayoutBase->addWidget(qframeControls, AlignLeft);
//$$$
//$$$
//$$$};
//$$$
//$$$void ClsFEBasePlot::slotHideControls ( bool bIsRight ) {
//$$$     if (bIsRight){
//$$$	  qframeControls->hide();
//$$$     } else{
//$$$	  qframeControls->show();
//$$$     }
//$$$};
//$$$
//$$$
//$$$void ClsFEBasePlot::print(){
//$$$    cout << "ClsFEBasePlot::print()" << endl;
//$$$
//$$$    cout << "caption:\t" << this->caption() << endl;
//$$$    cout << "position:\t" << this->pos().x() << ", " << this->pos().y() << endl;
//$$$    cout << "size:\t\t" << this->size().width() << ", " << this->size().height() << endl;
//$$$    
//$$$
//$$$
//$$$    const QObjectList *qObjectList = qframeControls->queryList( "ClsQNeuronStateVariableDisplay" );
//$$$
//$$$    QObjectListIt iter( *qObjectList );
//$$$    QObject *tmp;
//$$$
//$$$    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
//$$$    while( (tmp = iter.current()) ) {
//$$$	++iter;
//$$$	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
//$$$	    cout << "getGroupID():\t" << clsQNeuronStateVariableDisplay->getGroupID() << endl;
//$$$//	    cout << "getSelectedStatesAsString():\t" << clsQNeuronStateVariableDisplay->getSelectedStatesAsString() << endl;
//$$$
//$$$	    list<string> lst = clsQNeuronStateVariableDisplay->getSelectedStates();
//$$$	    list<string>::iterator it;
//$$$	    for(it=lst.begin();it!=lst.end();it++){
//$$$		cout << "\t\t" << (*it) << endl;
//$$$	    }
//$$$
//$$$
//$$$	    cout << "----------------" << endl;
//$$$	}
//$$$    }
//$$$
//$$$};
//$$$
//$$$
//$$$
//$$$ClsDataClientConfig ClsFEBasePlot::getConfig(){
//$$$    ClsDataClientConfig clsDataClientConfig(this->caption().latin1());
//$$$    clsDataClientConfig.setPosition(this->pos().x(), this->pos().y() );
//$$$    clsDataClientConfig.setGeometry(this->size().width(),  this->size().height());
//$$$
//$$$    const QObjectList *qObjectList = qframeControls->queryList( "ClsQNeuronStateVariableDisplay" );
//$$$    QObjectListIt iter( *qObjectList );
//$$$    QObject *tmp;
//$$$    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
//$$$    while( (tmp = iter.current()) ) {
//$$$	++iter;
//$$$	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
//$$$	    ClsStateVariableDisplayConfig clsStateVariableDisplayConfig = clsQNeuronStateVariableDisplay->getConfig();
//$$$	    clsDataClientConfig.addStateVariableDisplayConfig(clsStateVariableDisplayConfig);
//$$$	}
//$$$    }
//$$$
//$$$    return clsDataClientConfig;
//$$$}
//$$$
//$$$
