#include "ClsQBaseStateVariableDisplay.h"

ClsQBaseStateVariableDisplay::ClsQBaseStateVariableDisplay(ClsFEDataClient *_clsFEDataClient, 
							   string _strID, QWidget *_parent) :
    QFrame(_parent, 0, WDestructiveClose), 
    parent(_parent), clsFEDataClient(_clsFEDataClient), strID(_strID){

    setAcceptDrops(TRUE);
    this->setFrameStyle(Box | Raised);
    this->setLineWidth ( 1 );

    
    lblCaption = new QLabel(this);
    lblCaption->setBackgroundColor ( QColor(255, 255, 205));



};

bool ClsQBaseStateVariableDisplay::isLive() { 
    return qchkbxLiveData->isChecked ();
};
