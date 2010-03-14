#include <qstring.h>

#include "ClsFEParamRelais.h"
#include "ClsQHarborImpl.h"
#include "ClsFESystemManager.h"


ClsFEParamRelais*  ClsFEParamRelais::_instanceParamRelais = NULL;

void ClsFEParamRelais::initializeParamRelais(QWidget* _parent){
    _instanceParamRelais = new ClsFEParamRelais(_parent);
}


ClsFEParamRelais* ClsFEParamRelais::Instance(){
    if(_instanceParamRelais == NULL){
	_instanceParamRelais = new ClsFEParamRelais();
    } 
    return _instanceParamRelais;
}


ClsFEParamRelais::ClsFEParamRelais (QWidget* _parent) : parent(_parent) {
    clsQHarborImpl = NULL;
}


void ClsFEParamRelais::showHarbor() {
//    cout << "ClsFEParamRelais::showHarbor()" << endl;
    if(clsQHarborImpl == NULL){
	clsQHarborImpl = new ClsQHarborImpl(parent);
#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
	connect(ClsFESystemManager::Instance(), SIGNAL(sigItemChanged(int, string) ), clsQHarborImpl, SLOT(slotItemChanged(int, string) ));
	connect(ClsFESystemManager::Instance(), SIGNAL(sigItemDeleted(int, string) ), clsQHarborImpl, SLOT(slotItemDeleted(int, string) ));
#endif
	clsQHarborImpl->show();
    } else {
	clsQHarborImpl->show();
    }
 
}


list<ClsParamTrade> ClsFEParamRelais::getParamTrades() {
//    cout << "ClsFEParamRelais::getParamTrades()" << endl;
    list<ClsParamTrade> lstParamTrades;

    if(clsQHarborImpl != NULL){
	return clsQHarborImpl->getParamTrades();
    }

    return lstParamTrades;
}


int ClsFEParamRelais::setParameterByItemID(string strType, string strItemID, string strParamID, double fValue){

#ifndef _WINDOWS // cannot get this to work under minGW at the moment...
    QString qstrValue;
    qstrValue.setNum(fValue);
    if(!strType.compare("Group")){
	return ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_NEURON, strItemID, strParamID, qstrValue.toStdString());
    } 
    else if(!strType.compare("Connection")){
	return ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_SYNAPSE, strItemID, strParamID, qstrValue.toStdString());
    } 
#endif
    return -1;
};


void ClsFEParamRelais::cleanHarbor(){
//    cout << "ClsFEParamRelais::cleanHarbor()" << endl;
    if(clsQHarborImpl != NULL){
	clsQHarborImpl->clearTable();
    }

};

void ClsFEParamRelais::slotItemDeleted(int /*iType*/, string /*strID*/ ){
//    cout << "ClsFEParamRelais::slotItemDeleted(int iType, string strID )" << endl;
};

void ClsFEParamRelais::slotItemChanged(int /*iType*/, string /*strID*/ ){
//    cout << "ClsFEParamRelais::slotItemChanged(int iType, string strID )" << endl;
};


void ClsFEParamRelais::slotSimulationRunning(bool b){
    if(clsQHarborImpl != NULL){
	clsQHarborImpl->setSimulationRunning(b);
    }
}



void ClsFEParamRelais::applyConfig(string str){
//    cout << "ClsFEParamRelais::applyConfig(string str)" << endl;
    showHarbor();
    if(clsQHarborImpl != NULL){
	clsQHarborImpl->loadConfig(str);

    }
}
