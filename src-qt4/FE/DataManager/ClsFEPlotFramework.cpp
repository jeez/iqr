/****************************************************************************
 ** $Filename: ClsFEPlotFramework.cpp
 ** $Id$
 **
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


//zzz#include <qobjectlist.h>


#include <ClsFEPlotFramework.h>
#include <ClsQDivider.h>
#include <ClsFEDataSink.h>
#include <ClsQNeuronStateVariableDisplay.h> 




ClsFEPlotFramework::ClsFEPlotFramework() : QWidget(0) {
//    cout << "ClsFEPlotFramework::ClsFEPlotFramework()" << endl;
    clsQBaseStateVariableDisplay = NULL;

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

//--    cout << "caption:\t" << this->caption() << endl;
//--    cout << "position:\t" << this->pos().x() << ", " << this->pos().y() << endl;
//--    cout << "size:\t\t" << this->size().width() << ", " << this->size().height() << endl;
    


    QObjectList qObjectList = clsQBaseStateVariableDisplay->queryList( "ClsQNeuronStateVariableDisplay" );

//zzz    QObjectListIt iter( *qObjectList );
    const QList<QObject *>::iterator it;

    QObject *tmp;

    ClsQNeuronStateVariableDisplay* clsQNeuronStateVariableDisplay;
//zzz    while( (tmp = iter.current()) ) {
//zzz	++iter;
//zzz	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
//zzz	    list<string> lst = clsQNeuronStateVariableDisplay->getSelectedStates();
//zzz	    list<string>::iterator it;
//zzz	}
//zzz    }
//zzz
    for(it=qObjectList.begin();it!=qObjectList.end();it++ ) {
	QObject *tmp = *it;
	if((clsQNeuronStateVariableDisplay = dynamic_cast<ClsQNeuronStateVariableDisplay*>(tmp))){
	    list<string> lst = clsQNeuronStateVariableDisplay->getSelectedStates();
	    list<string>::iterator it;
	}
   }



};



//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
