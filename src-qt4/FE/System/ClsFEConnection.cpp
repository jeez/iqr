#include <iostream>
#include "ClsFEConnection.h"

#include "ClsFESystemManager.h"
#include "ClsFEGroup.h"
#include "ClsPatterns.h"
#include "ClsSubPopulations.h"
#include "ClsFunctions.h"

#include "ClsArbAll.h"
#include "ClsArbEllipse.h"
#include "ClsArbEllipseWindow.h"
#include "ClsArbRandom.h"
#include "ClsArbRect.h"
#include "ClsArbRectWindow.h"

#include "optionsParameter.hpp"
#include "stringParameter.hpp"

#include "tagLibrary.hpp"

//#define DEBUG_CLSFECONNECTION

ClsFEConnection::ClsFEConnection( ) {
    addStringParameter(ClsTagLibrary::NotesTag(), "Notes",
		       "",
		       true, true,
		       "",
		       "Notes");
};


ClsFEConnection::ClsFEConnection( string _strConnectionID,
				  string _strConnectionName,
				  string _strConnectionSourceID,
				  string _strConnectionTargetID ) {

    strConnectionID = _strConnectionID;
    strConnectionName = _strConnectionName;
    strConnectionTargetID = _strConnectionTargetID;
    strConnectionSourceID = _strConnectionSourceID;


    setParameter(ClsTagLibrary::NameTag(), _strConnectionName);


    addStringParameter(ClsTagLibrary::NotesTag(), "Notes",
		       "",
		       true, true,
		       "",
		       "Notes");
};


string ClsFEConnection::getNotes() { return getParameter(ClsTagLibrary::NotesTag())->getValueAsString(); };
void ClsFEConnection::setNotes(string _str){ setParameter(ClsTagLibrary::NotesTag(),  _str); };

void ClsFEConnection::init() {
    calculateArbPattern();
    initSynapses();
}

void ClsFEConnection::calculateArbPattern() {
#ifdef DEBUG_CLSFECONNECTION
    cout << "ClsFEConnection::calculateArbPattern()" << endl;
#endif

    if(strConnectionSourceID.size()<=0 || strConnectionTargetID.size()<=0){
//	cerr << "ClsFEConnection::calculateArbPattern()::strConnectionSourceID.size()<=0 || strConnectionTargetID.size()<=0" << endl;
	return;
    }

    /* I guess the best is to fix the boundaries for regions etc here 
     as calculateArbPattern() is called every time the group size changes */


    ClsBaseTopology *clsBaseTopologySource = 
	ClsFESystemManager::Instance()->getFEGroup(strConnectionSourceID)->getTopology();
    ClsBaseTopology *clsBaseTopologyTarget = 
	ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getTopology();

    if(dynamic_cast<ClsPatternMapped*>(clsPattern) || dynamic_cast<ClsPatternForeach*>(clsPattern)){
	checkNfixPatternBoundaries(clsBaseTopologySource, clsPattern->getSourcePopulation());
	checkNfixPatternBoundaries(clsBaseTopologyTarget, clsPattern->getTargetPopulation());
    }
    else if(dynamic_cast<ClsPatternTuples*>(clsPattern)){
	checkNfixPatternBoundaries(clsBaseTopologySource, clsBaseTopologyTarget, clsPattern);
    }

    ListPointTriples listPointTriples;

    /* 2.) find out which pattern type we have: */
    if(dynamic_cast<ClsPatternMapped*>(clsPattern)){
	string strTypeTemp = dynamic_cast<ClsPatternMapped*>(clsPattern)->getParameter(ClsTagLibrary::ConnectionPatternMappedType())->getValueAsString();

	bool bAll = false;

	if(!strTypeTemp.compare(ClsTagLibrary::ConnectionPatternMappedTypeAll())) {
	    bAll = true;
	}

	ClsBaseSubPopulation *clsBaseSubPopulationSource = clsPattern->getSourcePopulation();
	ClsBaseSubPopulation *clsBaseSubPopulationTarget = clsPattern->getTargetPopulation();

	int iSourceWidth = -1;
	int iSourceHeight = -1;
	int iSourceXOffSet = 0;
	int iSourceYOffSet = 0;
	if(dynamic_cast<ClsAll*>(clsBaseSubPopulationSource)){
	    iSourceWidth =  clsBaseTopologySource->nrCellsHorizontal();
	    iSourceHeight = clsBaseTopologySource->nrCellsVertical();
	}
	else if(dynamic_cast<ClsRegion*>(clsBaseSubPopulationSource)){
	    ClsRegion* clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulationSource);
	    iSourceWidth = clsRegion->getWidth();
	    iSourceHeight = clsRegion->getHeight();
	    iSourceXOffSet = clsRegion->getXStart() -1;
	    iSourceYOffSet = clsRegion->getYStart() -1;
	}

	int iTargetWidth = -1;
	int iTargetHeight = -1;
	int iTargetXOffSet = 0;
	int iTargetYOffSet = 0;
	if(dynamic_cast<ClsAll*>(clsBaseSubPopulationTarget)){
	    iTargetWidth =  clsBaseTopologyTarget->nrCellsHorizontal();
	    iTargetHeight = clsBaseTopologyTarget->nrCellsVertical();
	}
	else if(dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget)){
	    ClsRegion* clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulationTarget);
	    iTargetWidth = clsRegion->getWidth();
	    iTargetHeight = clsRegion->getHeight();
	    iTargetXOffSet = clsRegion->getXStart() -1;
	    iTargetYOffSet = clsRegion->getYStart() -1;
	}

	double fAreaWidth;
	double fAreaHeight;
	if((iSourceWidth*iSourceHeight) > (iTargetWidth*iTargetHeight)){
//	    cout << "-----------------------" << endl;
//	    cout << "Source larger than target" << endl;
	    fAreaWidth = (double)iSourceWidth / (double)iTargetWidth;
	    fAreaHeight = (double)iSourceHeight / (double)iTargetHeight;


//	    cout << "iSourceWidth,  iTargetWidth : " <<  iSourceWidth  << ", " << iTargetWidth  << endl;
//	    cout << "iSourceHeight, iTargetHeight: " <<  iSourceHeight << ", " << iTargetHeight << endl;

//	    cout << "fAreaWidth, fAreaHeight: " << fAreaWidth << ", " <<  fAreaHeight << endl;

	    tListOfPairs tListOfPairsTarget = clsBaseTopologyTarget->getPointsForSubPopulation(clsBaseSubPopulationTarget);
//	    cout << "getting list for target" << endl;
//	    cout << "tListOfPairsTarget.size(): " << tListOfPairsTarget.size() << endl;
	    tListOfPairs::iterator itLOPTarget;
//	    cout << "-----------------------" << endl;

	    int iXStart = (*tListOfPairsTarget.begin()).first;
	    int iYStart = (*tListOfPairsTarget.begin()).second;
	    for(itLOPTarget=tListOfPairsTarget.begin();itLOPTarget!=tListOfPairsTarget.end();itLOPTarget++){
		int iXTarget = (*itLOPTarget).first;
		int iYTarget = (*itLOPTarget).second;

//		cout << "iXTarget, iYTarget: " << iXTarget << ", " << iYTarget << endl;
//		cout << "iXStart, iYStart: " << iXStart << ", " << iYStart << endl;

		double fX = iXTarget - iXStart +1;
		double fY = iYTarget - iYStart +1;
		double fXCenterProjected = ((fX - .5) * fAreaWidth)  + .5 + iSourceXOffSet;
		double fYCenterProjected = ((fY - .5) * fAreaHeight) + .5 + iSourceYOffSet;

//		cout << "fXCenterProjected, fYCenterProjected: " << fXCenterProjected << ", " << fYCenterProjected << endl;

		if(bAll){
		    tListOfPairs tListOfPairsSource;
		    tListOfPairsSource = clsBaseTopologySource->getPoints4Rect( fXCenterProjected, fYCenterProjected, lrint(fAreaWidth), lrint(fAreaHeight));
		    tListOfPairs::iterator it;
		    for(it=tListOfPairsSource.begin();it!=tListOfPairsSource.end();it++){
			PointTriple pointTriple((*it), (*itLOPTarget), 0.);
			listPointTriples.push_back(pointTriple);
		    }
		}
		else {
		    tfPoint PointSource(fXCenterProjected, fYCenterProjected);
		    tfPoint PointTarget(iXTarget, iYTarget);
		    PointTriple pointTriple(PointSource, PointTarget, 0.);
		    listPointTriples.push_back(pointTriple);
		}
	    }
	}
	else {
//	    cout << "target larger than source" << endl;
	    fAreaWidth = (double)iTargetWidth / (double)iSourceWidth;
	    fAreaHeight = (double)iTargetHeight / (double)iSourceHeight;

	    tListOfPairs tListOfPairsSource = clsBaseTopologySource->getPointsForSubPopulation(clsBaseSubPopulationSource);
//	    cout << "getting list for source" << endl;

	    tListOfPairs::iterator itLOPSource;
	    int iXStart = (*tListOfPairsSource.begin()).first;
	    int iYStart = (*tListOfPairsSource.begin()).second;
	    for(itLOPSource=tListOfPairsSource.begin();itLOPSource!=tListOfPairsSource.end();itLOPSource++){
		int iXSource = (*itLOPSource).first;
		int iYSource = (*itLOPSource).second;
//		cout << "(" << iXSource << "," << iYSource << "), ";

		double fX = iXSource - iXStart +1;
		double fY = iYSource - iYStart +1;
		double fXCenterProjected = ((fX - .5) * fAreaWidth) + .5  + iTargetXOffSet;
		double fYCenterProjected = ((fY - .5) * fAreaHeight) + .5 + iTargetYOffSet;

		if(bAll){
		    tListOfPairs tListOfPairsTarget;
		    tListOfPairsTarget = clsBaseTopologyTarget->getPoints4Rect( fXCenterProjected, fYCenterProjected, lrint(fAreaWidth), lrint(fAreaHeight));
		    tListOfPairs::iterator it;
		    for(it=tListOfPairsTarget.begin();it!=tListOfPairsTarget.end();it++){
			PointTriple pointTriple((*itLOPSource), (*it), 0.);
			listPointTriples.push_back(pointTriple);
		    }
		}
		else {
		    tfPoint PointSource(iXSource, iYSource);
		    tfPoint PointTarget(fXCenterProjected, fYCenterProjected);
		    PointTriple pointTriple(PointSource, PointTarget, 0.);
		    listPointTriples.push_back(pointTriple);
		}
	    }
	}
//	printTupleList(lstPLP);
    }
    else if(dynamic_cast<ClsPatternForeach*>(clsPattern)){
//	cout << "ClsPatternForeach" << endl;
	ClsBaseSubPopulation *clsBaseSubPopulationSource = clsPattern->getSourcePopulation();
	ClsBaseSubPopulation *clsBaseSubPopulationTarget = clsPattern->getTargetPopulation();

	tListOfPairs tListOfPairsSource = clsBaseTopologySource->getPointsForSubPopulation(clsBaseSubPopulationSource);
	tListOfPairs tListOfPairsTarget = clsBaseTopologyTarget->getPointsForSubPopulation(clsBaseSubPopulationTarget);

/*
	cout << "(";
	tListOfPairs::iterator itLOPSource;
	for(itLOPSource=tListOfPairsSource.begin();itLOPSource!=tListOfPairsSource.end();itLOPSource++){
	    int iXSource = (*itLOPSource).first;
	    int iYSource = (*itLOPSource).second;
	    cout << "(" << iXSource << "," << iYSource << "), ";
	}
	cout << ")<->(";
	tListOfPairs::iterator itLOPTarget;

	for(itLOPTarget=tListOfPairsTarget.begin();itLOPTarget!=tListOfPairsTarget.end();itLOPTarget++){
	    int iXTarget = (*itLOPTarget).first;
	    int iYTarget = (*itLOPTarget).second;
	    cout << "(" << iXTarget << "," << iYTarget << "), ";
	}
	cout << ")" << endl;
*/
	listPointTriples = expandXXX(tListOfPairsSource, tListOfPairsTarget);
    }

    else if(dynamic_cast<ClsPatternTuples*>(clsPattern)){
//	cout << "ClsPatternTuples" << endl;
	if(clsPattern->getTuples()!=NULL){
	    list<pair<tListOfPairs, tListOfPairs> > lstPLP = clsPattern->getTuples()->getData();
//	    cout << "===========" << endl;
//	printTupleList(lstPLP);
	    
	    listIndexQuadruples.clear();
	    list<pair<tListOfPairs, tListOfPairs> >::iterator it;
	    for(it=lstPLP.begin(); it!=lstPLP.end(); it++){
		ListPointTriples listPointTriplesLocal;
		listPointTriplesLocal = expandXXX((*it).first, (*it).second);
		listPointTriples.merge(listPointTriplesLocal);
	    }
	}
    }

    listIndexQuadruples.clear();


//    listIndexQuadruples = PointTriples2IndexQuadruples(listPointTriples, clsBaseTopologySource, clsBaseTopologyTarget );
//    cout << "listIndexQuadruples:" << endl << listIndexQuadruples << endl;


    /*
      at this point we have a list (listPointTriples) which containt all the links cell to cell as specified
      in the pattern. Next step is to expand this list to include link defined in the arborization + calculate
      the topographic distances
    */


    listIndexQuadruples = calculateArborization(listPointTriples, clsBaseTopologySource, clsBaseTopologyTarget );
//    cout << "listIndexQuadruples:" << endl << listIndexQuadruples << endl;


/* 
   distances...

   - createMask()....
   - calculate delays: 
      connection needs function to know how to calculate delays form distances

      for pre and post
      	    // We need a delay for the model on the presynaptic output.
	    based on delay function (gaussian etc...)
	    with max delay ---> qadruple list
	    pNeuronPreOutput->setLengthHistory(2);  // We need 2 timesteps

*/

/* delay: */
    
    if(clsDelayFunction!=NULL){  
	clsDelayFunction->Calculate(listIndexQuadruples);
    } else {
	cerr << __FILE__ << "::clsDelayFunction==NULL" << endl;
    }
	
    // ### jmb 9.5.2005 Added dendritic attenuation
    if(clsAttenuationFunction!=NULL){  
	clsAttenuationFunction->Calculate(listIndexQuadruples);
    } else {
	cerr << __FILE__ << "::clsAttenuationFunction==NULL" << endl;
    }    


//    cout << listIndexQuadruples << endl;
}

void ClsFEConnection::initSynapses(){
/* from componentTest.cpp  2003/10/23 */
	
    pNeuronPreOutput = ClsFESystemManager::Instance()->getFEGroup(strConnectionSourceID)->getNeuron()->getOutputState();
    if (!pNeuronPreOutput) {
	cerr << "ERROR: no output state defined for presynaptic neuron" << endl;
	return;
    }
    pSynapseInput = pSynapse->getInputState();
    if (!pSynapseInput) {
	cerr << "ERROR: no input state defined for synapse" << endl;
	return;
    }
    pSynapseOutput = pSynapse->getOutputState();
    if (!pSynapseOutput) {
	cerr << "ERROR: no output state defined for synapse" << endl;
	return;
    }

    // Setup the feedback.  Associate remote state array pointer with
    // synapse state array pointer
    ClsNeuron* pNeuronPost = ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNeuron();
    XRefList fbInputs = pSynapse->getListFeedbackInputs();

    for (XRefList::iterator it = fbInputs.begin(); it != fbInputs.end(); ++it) {
//	cout << "Searching for postsynaptic state " << (*it)->getTarget()
//	     << " for feedback input " << (*it)->getName() << endl;
	
	ClsStateVariable *pNeuronState = pNeuronPost->getState((*it)->getTarget());
	ClsStateVariable *pSynapseInput = pSynapse->getState((*it)->getName());
	
	if (pNeuronState && pSynapseInput) {
//	    cout << "    found state " << pNeuronState->getLabel()
//		 << " for feedback input " << pSynapseInput->getLabel() << endl;

	    // Got both the state variable in the neuron and the input
	    // in the synapse, store the pointers
	    FeedbackInputType fb;
	    fb.pNeuronState  = pNeuronState;
	    fb.pSynapseInput = pSynapseInput;
	    fbStates.push_back(fb);
	} else {	
	    throw runtime_error(string("couldn't find feedback state")
				+(*it)->getName()+" for synapse");
	}
    }



    // Resize the postsynaptic input (connection output) array for the
    // postsynaptic group size.  Note that the size of this array is
    // equal to the number of neuron in the postsynaptic group,
    // withone entry per neuron, rather than the number of synapses.
    // The output of several synapses may be combined to obtain the
    // input for the neuron.

    unsigned int nNeuronsPost = ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNumberOfNeurons();
    unsigned int nNeuronsPre = ClsFESystemManager::Instance()->getFEGroup(strConnectionSourceID)->getNumberOfNeurons();

//    cout << "nNeuronsPost: " << nNeuronsPost << endl;
    connectionOut.setWidth(nNeuronsPost);
    // Reset values to save time during update, since we're sure that
    // unused entries are 0
    connectionOut[0] = 0.0;
    
    string strConnectionType = getParameter(ClsTagLibrary::ConnectionTypeTag())->getValueAsString();
    if(!strConnectionType.compare(ClsTagLibrary::ExcitatoryConnectionTag())){
	ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNeuron()->addExcitatoryInput(&connectionOut);
    }
    else if(!strConnectionType.compare(ClsTagLibrary::InhibitoryConnectionTag())){
	ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNeuron()->addInhibitoryInput(&connectionOut);
    }
    else if(!strConnectionType.compare(ClsTagLibrary::ModulatoryConnectionTag())){
	ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNeuron()->addModulatoryInput(&connectionOut);
    }
    else {
	ClsFESystemManager::Instance()->getFEGroup(strConnectionTargetID)->getNeuron()->addExcitatoryInput(&connectionOut);
    }
    
	
    // Initialize synapse state arrays to hold the correct number of
    // elements.
    const unsigned int nSynapses = listIndexQuadruples.size();
    try {
	pSynapse->resize(nSynapses);
    }
    catch (exception &ex) {
	// ERROR: couldn't create state arrays, can't go any further
	cerr << "Exception during synapse state array resize: "
	     << ex.what()
	     << endl;
	return;
    }
    synapseOutDelayed.setWidth(nSynapses);

    pSynapse->initialize(); // new 20080421 on request of Ivan


    // ### jmb 9.5.2005 Added missing connIn initialization
    //
    //////////////////////////////////////////
    //
    // INPUT FROM PRESYNAPTIC GROUP TO SYNAPSE
    //
    // Ensure that the presynaptic neuron's output state array has
    // enough history to handle the axonal delay.
    pNeuronPreOutput->setLengthHistory(axonalDelay+1);

//    cout << "Building masks for input from presynaptic group" << endl;
    // Input from presynaptic group to synapse. We need only 1 input
    // mask with 1 entry per synapse to specify the presynaptic cell
    // index.
    connIn.axonalDelay = axonalDelay;
    connIn.maskPre.resize(nSynapses);
    for (unsigned int i = 0; i < nSynapses; ++i) {
	// Only one neuron per synapse.
	Indices indices = listIndexQuadruples.getIndicesBySynapse(i);
	connIn.maskPre[i] = indices.neuronPre;
    }    
//    cout << "maskPre = " << connIn.maskPre << endl;
    //
    //////////////////////////////////////////

    ////////////////////////////////////////////
    //
    // OUTPUT FROM SYNAPSE TO POSTSYNAPTIC GROUP
    //
    // Ensure that the synapse's output state array has enough history
    // to handle the maximum delay.
    int maxDelay = listIndexQuadruples.getMaxDelay();

//    cout << "************maxDelay: " << maxDelay << endl;

    pSynapseOutput->setLengthHistory(maxDelay+1);

    // Find the delay which we need to handle.  The presynaptic
    // values from each delay are copied simultaneously using standard
    // valarray calls.
    list<int> delays = listIndexQuadruples.getDelays();
    int nDelays = delays.size();
//    cout << "Number of delays used = " << nDelays << endl << endl;

    // We need a synapse mask for each delay.
    synOut.reserve(nDelays);

    for (list<int>::iterator it = delays.begin(); it != delays.end(); ++it) {
	int delay = *it;
//	cout << "Searching for delay " << delay << "..."  << endl;
	vector<Indices> indices = listIndexQuadruples.getIndicesByDelay(delay);
	if (!indices.empty()) {
	    SynOutType so;

	    // Initialize the mask sizes to the correct size.
	    so.maskSyn.resize(indices.size());

	    // Store the delay which is used to select the valarray
	    // with the ring buffer to use.
	    so.dendriticDelay = delay;

	    // Transform index data into valarray masks
	    for (unsigned int i = 0; i < indices.size(); ++i) {
		so.maskSyn[i] = indices[i].synapse;
//		cout << "   synapse = " << so.maskSyn[i] << endl;
	    }
//	    cout << "Delay = " << delay << ", maskSyn = " << so.maskSyn << endl;
	    synOut.push_back(so);
	}	
//	cout << endl;
    }

    // [jmb 19.6.2004] This version only creates entries for
    // postsynaptic neurons that receive input.
    //
    // We might need one entry in connOut for each postsynaptic
    // neuron.  Which postsynaptic neurons receive input?
    list<int> lstIdsPost = listIndexQuadruples.getSeconds();
    connOut.reserve(lstIdsPost.size());

    for (list<int>::iterator it = lstIdsPost.begin(); it != lstIdsPost.end(); ++it) {
	ConnOutType co;
	co.idPost = *it;
// 	cout << "Building mask for postsynaptic neuron " << co.idPost << "..."  << endl;

	// Get arborization for postsynaptic neuron.
	vector<Indices> indices = listIndexQuadruples.getIndicesBySecond(co.idPost);

	// Initialize the mask to the correct size.
	co.maskSyn.resize(indices.size());

	// Transform index data into valarray mask
	for (unsigned int i = 0; i < indices.size(); ++i) {
	    co.maskSyn[i] = indices[i].synapse;
	} 
// 	cout << "Postsynaptic neuron = " << co.idPost << ", maskSyn = " << co.maskSyn << endl;
	connOut.push_back(co);
    }
//     cout << endl;	

    // ### jmb 9.5.2005 Added dendritic attenuation
    dendriticAttenuation.resize(nSynapses);
    
    for (unsigned int i = 0; i < nSynapses; ++i) {
	Indices indices = listIndexQuadruples.getIndicesBySynapse(i);
	dendriticAttenuation[i] = indices.attenuation;
    }    

//    cout << "dendriticAttenuation: " << dendriticAttenuation << endl;

     fMaxAttenuation = clsAttenuationFunction->max();

    //
    ////////////////////////////////////////////

    //////////////////////////////////////////////
    //
    // FEEDBACK FROM POSTSYNAPTIC GROUP TO SYNAPSE
    //
    connFb.reserve(nDelays);

//    cout << "Building masks for feedback from postsynaptic group" << endl;
    for (list<int>::iterator it = delays.begin(); it != delays.end(); ++it) {
	int delay = *it;
//	cout << "Searching for delay " << delay << "..."  << endl;
	vector<Indices> indices = listIndexQuadruples.getIndicesByDelay(delay);
	if (!indices.empty()) {
	    ConnFbType cf;

	    // Initialize the mask sizes to the correct size.
	    cf.maskSyn.resize(indices.size());
	    cf.maskPost.resize(indices.size());

	    // Store the delay which is used to select the valarray
	    // with the ring buffer to use.
	    cf.dendriticDelay = delay;

	    // Transform index data into valarray masks
	    for (unsigned int i = 0; i < indices.size(); ++i) {
		cf.maskSyn[i] = indices[i].synapse;
		cf.maskPost[i] = indices[i].neuronPost;
//		cout << "   post = " << cf.maskPost[i]
//		     << ",  synapse = " << cf.maskSyn[i]
//		     << endl;
	    }
//	    cout << "Delay = " << delay << ", maskPost = " << cf.maskPost 
//		 << ", maskSyn = " << cf.maskSyn << endl;
	    connFb.push_back(cf);
	}	
//	cout << endl;
    }
    //
    //////////////////////////////////////////////
    
    // Ensure that feedback state arrays have enough entries in ring
    // buffer for maximum delay
    for (unsigned int i = 0; i < fbStates.size(); ++i) {
	fbStates[i].pNeuronState->setLengthHistory(maxDelay+1);
    }

    // ### jmb 9.5.2005 Added missing pre2post and post2pre initialization
    //
    // Build mask sets for data extraction.  Masks might be empty.
    //
    // Use vector resize here, we don't need to use push_back because
    // we already know the number of elements (= number of presynaptic
    // neurons).
    pre2post.resize(nNeuronsPre);

//    cout << "Building pre2post..." << endl;
    for (unsigned int pre = 0; pre < nNeuronsPre; ++pre) {
        vector<Indices> indices = listIndexQuadruples.getIndicesByFirst(pre);
	// Length of index vector gives us the number of connections
	// the current neuron makes.
        int nConns = indices.size();
	pre2post[pre].maskSyn.resize(nConns);
	pre2post[pre].maskPost.resize(nConns);

//	cout << pre << " makes " << nConns << " connections" << endl;

	// Store connection data.
	pre2post[pre].pre = pre;
	for (int conn = 0; conn < nConns; ++conn) {
	    pre2post[pre].maskSyn[conn] = indices[conn].synapse;
	    pre2post[pre].maskPost[conn] = indices[conn].neuronPost;
//	    cout << pre << " " 
//		 << pre2post[pre].maskSyn[conn] << " " 
//		 << pre2post[pre].maskPost[conn] << endl;
	}
    }
//    cout << endl;

    // Use vector resize here, we don't need to use push_back because
    // we already know the number of elements (= number of
    // postsynaptic neurons).
    post2pre.resize(nNeuronsPost);

//    cout << "Building post2pre..." << endl;
    for (unsigned int post = 0; post < nNeuronsPost; ++post) {
        vector<Indices> indices = listIndexQuadruples.getIndicesBySecond(post);
	// Length of index vector gives us the number of connections
	// the current neuron makes.
        int nConns = indices.size();
	post2pre[post].maskSyn.resize(nConns);
	post2pre[post].maskPre.resize(nConns);

//	cout << post << " receives " << nConns << " connections" << endl;

	// Store connection data.
	post2pre[post].post = post;
	for (int conn = 0; conn < nConns; ++conn) {
	    post2pre[post].maskSyn[conn] = indices[conn].synapse;
	    post2pre[post].maskPre[conn] = indices[conn].neuronPre;
//	    cout << post << " " 
//		 << post2pre[post].maskSyn[conn] << " "
//		 << post2pre[post].maskPre[conn] << endl;
	}
    }    
//    cout << endl;
};

ListIndexQuadruples ClsFEConnection::calculateArborization(const ListPointTriples& listPointTriples, ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget ){

//    ClsBaseArborization *clsArborization;

    ListIndexQuadruples listIndexQuadruplesTemp;



    string strDirection = clsArborization->getParameter(ClsTagLibrary::ArborizationDirectionTag())->getValueAsString();
    if(!strDirection.compare(ClsTagLibrary::ConnectionArborizationPFTag())){
//	cout << "Arb is PF" << endl;
	ListPointTriples::const_iterator it;
	for(it=listPointTriples.begin();it!=listPointTriples.end();it++){
	    double fXCenterSource = (*it).first.first; /* second is target */
	    double fXCenterTarget = (*it).second.first; /* second is target */
	    double fYCenterSource = (*it).first.second;
	    double fYCenterTarget = (*it).second.second;

	    int iIndexSource = clsBaseTopologySource->pos2index(lrint(fXCenterSource), lrint(fYCenterSource)); /* CAST?? OR LRINT?? */
	    list<tIndexDist> listIndexDist = clsBaseTopologyTarget->getCellsForArborization(fXCenterTarget, fYCenterTarget, clsArborization);
	    list<tIndexDist>::iterator it1;
	    for(it1=listIndexDist.begin(); it1!=listIndexDist.end(); it1++){
		IndexQuadruple indexQuadruple(iIndexSource, (*it1).first, (*it1).second, 0);
		listIndexQuadruplesTemp.push_back(indexQuadruple);
	    }
	}
    }
    /* assume RF */
    else {
//	cout << "Arb is RF" << endl;
	ListPointTriples::const_iterator it;
	for(it=listPointTriples.begin();it!=listPointTriples.end();it++){
	    double fXCenterSource = (*it).first.first; /* second is target */
	    double fXCenterTarget = (*it).second.first; /* second is target */
	    double fYCenterSource = (*it).first.second;
	    double fYCenterTarget = (*it).second.second;

//	    cout << "\t\tfXCenterSource fYCenterSource: " <<  fXCenterSource << ", " << fYCenterSource << endl;
//	    cout << "\t\tfXCenterTarget fYCenterTarget: " <<  fXCenterTarget << ", " << fYCenterTarget << endl;

	    int iIndexTarget = clsBaseTopologyTarget->pos2index(lrint(fXCenterTarget), lrint(fYCenterTarget)); /* CAST?? OR LRINT?? */
	    list<tIndexDist> listIndexDist = clsBaseTopologySource->getCellsForArborization(fXCenterSource, fYCenterSource, clsArborization);
	    list<tIndexDist>::iterator it1;
	    for(it1=listIndexDist.begin(); it1!=listIndexDist.end(); it1++){
		IndexQuadruple indexQuadruple((*it1).first, iIndexTarget, (*it1).second, 0);
		listIndexQuadruplesTemp.push_back(indexQuadruple);
	    }
	}
    }

    return listIndexQuadruplesTemp;
}

ListPointTriples ClsFEConnection::expandXXX(const tListOfPairs &tListOfPairsSource , const tListOfPairs &tListOfPairsTarget){
    ListPointTriples listPointTriples;
    tListOfPairs::const_iterator itLOPSource;
    for(itLOPSource=tListOfPairsSource.begin();itLOPSource!=tListOfPairsSource.end();itLOPSource++){
	tListOfPairs::const_iterator itLOPTarget;
	for(itLOPTarget=tListOfPairsTarget.begin();itLOPTarget!=tListOfPairsTarget.end();itLOPTarget++){
	    PointTriple pointTriple((*itLOPSource), (*itLOPTarget), 0);
	    listPointTriples.push_back(pointTriple);
	}
    }
    return listPointTriples;
}

ListIndexQuadruples ClsFEConnection::PointTriples2IndexQuadruples(const ListPointTriples& listPointTriples, ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget ){
    ListIndexQuadruples listIndexQuadruples;
    ListPointTriples::const_iterator it;
    for(it=listPointTriples.begin();it!=listPointTriples.end();it++){
	int iXSource = lrint((*it).first.first);
	int iYSource = lrint((*it).first.second);
	int iIndexSource = clsBaseTopologySource->pos2index(iXSource, iYSource);

	int iXTarget = lrint((*it).second.first);
	int iYTarget = lrint((*it).second.second);
	int iIndexTarget = clsBaseTopologyTarget->pos2index(iXTarget, iYTarget);
	IndexQuadruple t(iIndexSource, iIndexTarget, 0.5, 0);
	listIndexQuadruples.push_back(t);
    }
    return listIndexQuadruples;
}


string ClsFEConnection::validate(){
//    cout << "ClsFEConnection::validate()" << endl;
    string strMessage = "";


    if(pSynapse==NULL){
//	strMessage = getParameter(ClsTagLibrary::NameTag())->getValueAsString(); //--
//	strMessage = "Connection " + strSourceGroupName + "->" + strTargetGroupName;
	strMessage.append("\n\tNo Synapse defined");
    }

    if(strConnectionSourceID.size()<=0){
//	strMessage = getParameter(ClsTagLibrary::NameTag())->getValueAsString(); //--
	strMessage.append("\n\tNo source defined");
    }

    if(strConnectionTargetID.size()<=0){
//	strMessage = getParameter(ClsTagLibrary::NameTag())->getValueAsString(); //--
	strMessage.append("\n\tNo target defined");
    }


    if(listIndexQuadruples.size()<=0){
	strMessage.append("\n\tNo synapses defined");
    }


    if(strMessage.size()>0){
	string strSourceGroupName = ClsFESystemManager::Instance()->getGroupName(strConnectionSourceID).c_str();
	string strTargetGroupName = ClsFESystemManager::Instance()->getGroupName(strConnectionTargetID).c_str();
	strMessage = "Connection \"" + getConnectionName() + "\"\n(\"" + strSourceGroupName + "\"->\"" + strTargetGroupName + "\")" + strMessage;

    }


    return strMessage;

}

void ClsFEConnection::checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget, ClsBasePattern *clsPattern){
#ifdef DEBUG_CLSFECONNECTION
    cout << "ClsFEConnection::checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopologySource, ClsBaseTopology *clsBaseTopologyTarget, ClsBasePattern *clsPattern)" << endl;
#endif

    ClsTuples* clsTuples = clsPattern->getTuples();
    if(clsTuples != NULL){
	int iXMaxSource = clsBaseTopologySource->nrCellsHorizontal();
	int iYMaxSource =  clsBaseTopologySource->nrCellsVertical();
	int iXMaxTarget = clsBaseTopologyTarget->nrCellsHorizontal();
	int iYMaxTarget =  clsBaseTopologyTarget->nrCellsVertical();

/*
	cout << "iXMaxSource: " << iXMaxSource << endl;
	cout << "iYMaxSource: " << iYMaxSource << endl;
	cout << "iXMaxTarget: " << iXMaxTarget << endl;
	cout << "iYMaxTarget: " << iYMaxTarget << endl;
*/


	list<pair<tListOfPairs, tListOfPairs> > lstValid;

	list<pair<tListOfPairs, tListOfPairs> > lst = clsTuples->getData();
	list<pair<tListOfPairs, tListOfPairs> >::iterator itTop;
	for(itTop=lst.begin(); itTop!=lst.end(); itTop++){
	    tListOfPairs::iterator itPre;
	    tListOfPairs lstPre = (*itTop).first;
	    tListOfPairs::iterator itPost;
	    tListOfPairs lstPost = (*itTop).second;
	    
	    tListOfPairs lstPreValid;
	    tListOfPairs lstPostValid;
	    

	    for(itPre=lstPre.begin(); itPre!=lstPre.end(); itPre++){
//		cout << "pre x: " << itPre->first << ", y: " << itPre->second << endl;
		if(itPre->first<= iXMaxSource && itPre->second<= iYMaxSource){
		    /* ADD */
		    pair<int, int> pairTemp = *itPre;
		    lstPreValid.push_back(pairTemp);
		} else {
		    cerr << "Connection with ID: " << strConnectionID << ": invalid pre synaptic tuple: " << " x: " << itPre->first << ", y: " << itPre->second << endl;
		}
	    }
	    for(itPost=lstPost.begin(); itPost!=lstPost.end(); itPost++){
//		cout << "post x: " << itPost->first << ", y: " << itPost->second << endl;
		if(itPost->first<= iXMaxTarget && itPost->second<= iYMaxTarget){
		    /* ADD */
		    pair<int, int> pairTemp = *itPost;
		    lstPostValid.push_back(pairTemp);
		} else {
		    cerr << "Connection with ID: " << strConnectionID << ": invalid post synaptic tuple: " << " x: " << itPost->first << ", y: " << itPost->second << endl;
		}
	    }

	    /* ADD LIST HERE, PROVIDED LISTS ARE NOT EMPTY */
	    if(lstPreValid.size() > 0 && lstPostValid.size() > 0){
		pair<tListOfPairs, tListOfPairs> pairTemp(lstPreValid, lstPostValid);
		lstValid.push_back(pairTemp);
	    }
	}
	clsTuples->setData(lstValid);
    }
    
/*


   void setTuples(ClsTuples* _clsSubPopulation);
    ClsTuples* getTuples();

*/

}


void ClsFEConnection::checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopology, ClsBaseSubPopulation *clsBaseSubPopulation){
#ifdef DEBUG_CLSFECONNECTION
    cout << "ClsFEConnection::checkNfixPatternBoundaries(ClsBaseTopology *clsBaseTopology, ClsBaseSubPopulation *clsBaseSubPopulation)" << endl;
#endif
    
    /* Warning:
       this is a bit confusing...
       only pattern mapped and foreach do have a sub-population! PatternTuples does not have a sub-population... 
    */
    
    if(dynamic_cast<ClsRegion*>(clsBaseSubPopulation)){
//	cout << "REGION" << endl;
	ClsRegion* clsRegion = dynamic_cast<ClsRegion*>(clsBaseSubPopulation);
    int iWidth = clsRegion->getWidth();
    int iHeight = clsRegion->getHeight();
    int iXOffSet = clsRegion->getXStart() -1;
    int iYOffSet = clsRegion->getYStart() -1;
    
    if((iXOffSet+iWidth)>clsBaseTopology->nrCellsHorizontal()){
//	    cout << "clsBaseTopology->nrCellsHorizontal(): " << clsBaseTopology->nrCellsHorizontal() << endl;
//	    cout << "iWidth: " << iWidth << endl;
	int iNewOffSet = clsBaseTopology->nrCellsHorizontal() - iWidth +1;
	if(iNewOffSet<1){
	    iNewOffSet = 1;
	    clsRegion->setWidth( clsBaseTopology->nrCellsHorizontal());
	}
	clsRegion->setXStart(iNewOffSet);
    }
    
    if((iYOffSet+iHeight)>clsBaseTopology->nrCellsVertical()){
	int iNewOffSet = clsBaseTopology->nrCellsVertical() - iHeight +1;
	if(iNewOffSet<1){
	    iNewOffSet = 1;
	    clsRegion->setHeight( clsBaseTopology->nrCellsVertical());
	}
	clsRegion->setYStart(iNewOffSet);
    }
}
else if(dynamic_cast<ClsList*>(clsBaseSubPopulation)){
//    cout << "LIST" << endl;
    ClsList* clsList = dynamic_cast<ClsList*>(clsBaseSubPopulation);
/* ?? to do: remove invalid points */

list<pair<int, int> > lstPoints = clsList->getData();

list<pair<int, int> > lstValidPoints;

int iXMax = clsBaseTopology->nrCellsHorizontal();
int iYMax =  clsBaseTopology->nrCellsVertical();

  list<pair<int, int> >::iterator it;
    for(it=lstPoints.begin(); it!=lstPoints.end(); it++){
//	cout << "\t\tx: " << (*it).first << ", y: " << (*it).second << endl;
	if((*it).first<=iXMax && (*it).second<=iYMax){
	    pair<int, int> pairTemp = *it;
	    lstValidPoints.push_back(pairTemp);
	} else {
	    cerr << "Connection with ID: " << strConnectionID << " :invalid point found:" << " x: " << (*it).first << ", y: " << (*it).second << endl;
	}
    }
clsList->setData(lstValidPoints);
}


}


//cout << __FILE__ << ":" << __LINE__ << endl;

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
