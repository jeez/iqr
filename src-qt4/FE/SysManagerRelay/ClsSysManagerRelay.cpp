#include <qstring.h>

#include "ClsSysManagerRelay.h"
#include "ClsFESystemManager.h"


ClsSysManagerRelay*  ClsSysManagerRelay::_instanceParamRelais = NULL;

void ClsSysManagerRelay::initializeParamRelais(){
    _instanceParamRelais = new ClsSysManagerRelay();
}


ClsSysManagerRelay* ClsSysManagerRelay::Instance(){
    if(_instanceParamRelais == NULL){
	_instanceParamRelais = new ClsSysManagerRelay();
    } 
    return _instanceParamRelais;
}


ClsSysManagerRelay::ClsSysManagerRelay (){
}


list<ClsParamTrade> ClsSysManagerRelay::getParamTrades() {
    list<ClsParamTrade> lstParamTrades;

   return lstParamTrades;
}


int ClsSysManagerRelay::setParameterByItemID(string strType, string strItemID, string strParamID, double fValue){
    return -1;
};





// 	return ClsFESystemManager::Instance()->setParameterByItemID(ClsFESystemManager::ITEM_NEURON, strItemID, strParamID, qstrValue.toStdString());

//void ClsFEConnectionDiagram::createDiagram( ) {
//#ifdef DEBUG_CLSFECONNECTIONDIAGRAM
//    cout << "ClsFEConnectionDiagram::createDiagram( )" << endl;
//#endif
//
//    string strSourceID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionSourceID();
//    string strTargetID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionTargetID();
//
////    ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->calculateArbPattern();
//    listIndexQuadruple = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getQuadrupleList();
//    iDelayMax = listIndexQuadruple.getMaxDelay()+1;
//    fAttenuationMax = listIndexQuadruple.getMaxAttenuation();
////    cout << "fAttenuationMax: " << fAttenuationMax << endl;
//    fAttenuationMax += fAttenuationMax/10.;
//
//    appendSourceStateArray(strSourceID);
//    appendTargetStateArray(strTargetID);
//
//    createPre2Post();
//    createPost2Pre();
//
//};
//
//
//void ClsFEConnectionDiagram::createPre2Post(){
//
//    // Use vector resize here, we don't need to use push_back because we already
//    // know the number of elements (= number of presynaptic neurons).
//
//    string strSourceID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionSourceID();
//    unsigned int _nNeuronsPre = ClsFESystemManager::Instance()->getFEGroup(strSourceID)->getNumberOfNeurons();
//
//    pre2post.resize(_nNeuronsPre);
//
////    cout << "Building pre2post..." << endl;
//    for (unsigned int pre = 0; pre < _nNeuronsPre; ++pre) {
//        vector<Indices> indices = listIndexQuadruple.getIndicesByFirst(pre);
//	// Length of index vector gives us the number of connections
//	// the current neuron makes.
//        int nConns = indices.size();
//	pre2post[pre].maskSyn.resize(nConns);
//	pre2post[pre].maskPost.resize(nConns);
//
////	cout << pre << " makes " << nConns << " connections" << endl;
//
//	// Store connection data.
//	pre2post[pre].pre = pre;
//	for (int conn = 0; conn < nConns; ++conn) {
//	    pre2post[pre].maskSyn[conn] = indices[conn].synapse;
//	    pre2post[pre].maskPost[conn] = indices[conn].neuronPost;
///*
//	    cout << pre << " " 
//		 << pre2post[pre].maskSyn[conn] << " "
//		 << pre2post[pre].maskPost[conn] << endl;
//*/
//	}
//    }
//}
//
//
//
//
//void ClsFEConnectionDiagram::createPost2Pre(){
//
//    // Use vector resize here, we don't need to use push_back because we already
//    // know the number of elements (= number of postsynaptic neurons).
//
//    string strTargetID = ClsFESystemManager::Instance()->getFEConnection(strConnectionID)->getConnectionTargetID();
//    unsigned int _nNeuronsPost = ClsFESystemManager::Instance()->getFEGroup(strTargetID)->getNumberOfNeurons();
//    post2pre.resize(_nNeuronsPost);
//
////    cout << "Building post2pre..." << endl;
//    for (unsigned int post = 0; post < _nNeuronsPost; ++post) {
//        vector<Indices> indices = listIndexQuadruple.getIndicesBySecond(post);
//	// Length of index vector gives us the number of connections
//	// the current neuron makes.
//        int nConns = indices.size();
//	post2pre[post].maskSyn.resize(nConns);
//	post2pre[post].maskPre.resize(nConns);
//
////	cout << post << " receives " << nConns << " connections" << endl;
//
//	// Store connection data.
//	post2pre[post].post = post;
//	for (int conn = 0; conn < nConns; ++conn) {
//	    post2pre[post].maskSyn[conn] = indices[conn].synapse;
//	    post2pre[post].maskPre[conn] = indices[conn].neuronPre;
///*
//	    cout << post << " " 
//		 << post2pre[post].maskSyn[conn] << " "
//		 << post2pre[post].maskPre[conn] << endl;
//*/
//	}
//    }    
//}

