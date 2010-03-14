/****************************************************************************
 ** $Filename: ClsFEPlotFramework.cpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Nov  5 01:02:26 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#include <qobjectlist.h>


#include <ClsFEPlotFramework.h>
#include <ClsQDivider.h>
#include <ClsFEDataSink.h>
#include <ClsQNeuronStateVariableDisplay.h> 




ClsFEPlotFramework::ClsFEPlotFramework() : QWidget(0) {
//    cout << "ClsFEPlotFramework::ClsFEPlotFramework()" << endl;
    clsQBaseStateVariableDisplay = NULL;
//    this->setBackgroundColor(QColor(255,0,0));

    boxlayoutBase = new QGridLayout ( this, 1, 3, 0, 3, "boxlayoutBase");

/* the divider inbetween the left and right pane */
    clsQDivider = new ClsQDivider(this);
    boxlayoutBase->addWidget(clsQDivider, 0, 1);
    connect(clsQDivider, SIGNAL(clicked(bool)), SLOT(slotHideControls ( bool )));
/* ----------------------- */

};

void ClsFEPlotFramework::slotHideControls ( bool bIsRight ) {
    if (bIsRight && clsQBaseStateVariableDisplay!=NULL){
	clsQBaseStateVariableDisplay->hide();
    } else{
	clsQBaseStateVariableDisplay->show();
    }
};


void ClsFEPlotFramework::print(){
    cout << "ClsFEPlotFramework::print()" << endl;

    cout << "caption:\t" << this->caption() << endl;
    cout << "position:\t" << this->pos().x() << ", " << this->pos().y() << endl;
    cout << "size:\t\t" << this->size().width() << ", " << this->size().height() << endl;
    


    const QObjectList *qObjectList = clsQBaseStateVariableDisplay->queryList( "ClsQNeuronStateVariableDisplay" );

    QObjectListIt iter( *qObjectList );
    QObject *tmp;

    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
    while( (tmp = iter.current()) ) {
	++iter;
	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
//	    cout << "getGroupID():\t" << clsQNeuronStateVariableDisplay->getGroupID() << endl;
//	    cout << "getSelectedStatesAsString():\t" << clsQNeuronStateVariableDisplay->getSelectedStatesAsString() << endl;

	    list<string> lst = clsQNeuronStateVariableDisplay->getSelectedStates();
	    list<string>::iterator it;
	    for(it=lst.begin();it!=lst.end();it++){
//		cout << "\t\t" << (*it) << endl;
	    }
//	    cout << "----------------" << endl;
	}
    }

};



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
