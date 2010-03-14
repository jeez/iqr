#include "ClsQBaseStateVariableDisplay.h"

ClsQBaseStateVariableDisplay::ClsQBaseStateVariableDisplay(ClsFEDataClient *_clsFEDataClient, 
							   string _strID, QWidget *_parent) :
    QFrame(_parent, 0, Qt::WDestructiveClose), 
    parent(_parent), clsFEDataClient(_clsFEDataClient), strID(_strID){

    setAcceptDrops(TRUE);
    this->setFrameStyle(Box | Raised);
    this->setLineWidth ( 1 );

    
    lblCaption = new QLabel(this);
    QPalette palette;
    palette.setColor(lblCaption->backgroundRole(), QColor(255, 255, 205));
    lblCaption->setPalette(palette);



};

bool ClsQBaseStateVariableDisplay::isLive() { 
    return qchkbxLiveData->isChecked ();
};
