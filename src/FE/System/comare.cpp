void ClsBaseConnection::update(){
//    cout << "ClsBaseConnection::update()" << endl;

    // Update all synapses
    pSynapse->advance();
    
    /////////////////////
    // CONNECTION::UPDATE

    StateArray &neuronPreOutput  = pNeuronPreOutput->getStateArray();
    StateArray &synapseInput     = pSynapseInput->getStateArray();
    StateArray &synapseOutput    = pSynapseOutput->getStateArray();
    
    // Connection copies presynaptic output into synapse
    synapseInput[0] = neuronPreOutput[connIn.axonalDelay][connIn.maskPre];
    
    // Copy feedback from postsynaptic neuron into synapse
    for (unsigned int i = 0; i < fbStates.size(); ++i) {
	StateArray &neuronFeedback = fbStates[i].pNeuronState->getStateArray();
	StateArray &synapseInput   = fbStates[i].pSynapseInput->getStateArray();
	cout << "Neuron_feedback " << fbStates[i].pNeuronState->getLabel()
	     << " " << neuronFeedback[0] << endl;
	
	for (unsigned int j = 0; j < connFb.size(); ++j) {
	    synapseInput[0][connFb[j].maskSyn] 
		= neuronFeedback[connFb[j].dendriticDelay][connFb[j].maskPost];
	}
	cout << "Feedback_input[" << fbStates[i].pSynapseInput->getName()
	     << "] " << synapseInput[0] << endl;
    }

    pSynapse->update();
    
/*    for (int i = 0; i < synapseOutput.getDepth(); ++i) {
	cout << "Synapse_output[" << i << "] " << synapseOutput[i] << endl;
    }
*/

    // Connection copies the data out of the synapse output array into
    // the connection out array, transforming from number of synapses
    // to number of postsynaptic neurons.
    //
    // synOut handles the dendritic delay
    for (unsigned int i = 0; i < synOut.size(); ++i) {
	synapseOutDelayed[0][synOut[i].maskSyn]
	    = synapseOutput[synOut[i].dendriticDelay][synOut[i].maskSyn];
    }
//    cout << "Synapse_output_delayed " << synapseOutDelayed[0] << endl;

    // connOut handles the summing for postsynaptic neurons    
    for (unsigned int i = 0; i < connOut.size(); ++i) {
/*	cout << "Output_to_cell[" << connOut[i].idPost << "]" 
	     << static_cast<valarray<double> >(synapseOutDelayed[0][connOut[i].maskSyn]) 
	     << endl; */
	connectionOut[0][connOut[i].idPost]
	    = static_cast<valarray<double> >(synapseOutDelayed[0][connOut[i].maskSyn]).sum();
    }

/*    cout << "Connection_output " << connectionOut[0] << endl; */

    // Write output of pre- and post-synaptic neurons and synapse into file.
/*    cout << "Connection_output   " << connectionOut[0] << endl; */
    // Postsynaptic neuron output is one timestep behind the input
    // buffer, as the neuron is updated before the synapse.
/*    if (pNeuronPostOutput) {
	cout << "Neuron_post_output "  << (pNeuronPostOutput->getStateArray())[0] << endl;
	}*/

    

    // CONNECTION::UPDATE
    /////////////////////

/*    
	for (unsigned int i = 0; i < neuronPreOutput.getDepth(); ++i) {
	    cout << "Neuron pre output[ " << i << "] " << neuronPreOutput[i] << endl;
	}

	cout << "Synapse input  " << synapseInput[0] << endl;
	cout << "Synapse output " << synapseOutput[0] << endl;
	cout << "Connection output   " << connectionOut[0] << endl;
*/





// 	// Postsynaptic neuron output is one timestep behind the input
// 	// buffer, as the neuron is updated before the synapse.
// 	if (pNeuronPostOutput) {
// 	    cout << "Neuron post output " 
// 		 << (pNeuronPostOutput->getStateArray())[0] 
// 		 << endl;
// 	}
//	cout << endl;

    
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
	cout << "Searching for postsynaptic state " << (*it)->getTarget()
	     << " for feedback input " << (*it)->getName() << endl;
	
	ClsStateVariable *pNeuronState = pNeuronPost->getState((*it)->getTarget());
	ClsStateVariable *pSynapseInput = pSynapse->getState((*it)->getName());
	
	if (pNeuronState && pSynapseInput) {
	    cout << "    found state " << pNeuronState->getLabel()
		 << " for feedback input " << pSynapseInput->getLabel() << endl;

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
//    cout << "nNeuronsPost: " << nNeuronsPost << endl;
    connectionOut.setWidth(nNeuronsPost);

    
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
		cout << "   synapse = " << so.maskSyn[i] << endl;
	    }
//	    cout << "Delay = " << delay << ", maskSyn = " << so.maskSyn << endl;
	    synOut.push_back(so);
	}	
//	cout << endl;
    }

    // Postsynaptic input doesn't include delays.  Any synaptic delay
    // should be modelled using axonal delay.

    // One output mask per postsynaptic neuron.  The mask collects the
    // data from all synapses for the neuron.
    // Which postsynaptic neurons receive input?
    list<int> lstIdsPost = listIndexQuadruples.getSeconds();

    connOut.reserve(lstIdsPost.size());

    for (list<int>::iterator it = lstIdsPost.begin(); it != lstIdsPost.end(); ++it) {

	ConnOutType co;
	co.idPost = *it;
//	cout << "Building mask for postsynaptic neuron " << co.idPost << "..."  << endl;

	// Get arborization for postsynaptic neuron.
	vector<Indices> indices = listIndexQuadruples.getIndicesBySecond(co.idPost);

	// Initialize the mask to the correct size.
	co.maskSyn.resize(indices.size());

	// Transform index data into valarray mask
	for (unsigned int i = 0; i < indices.size(); ++i) {
	    co.maskSyn[i] = indices[i].synapse;
	} 
//	cout << "     synapses = " << co.maskSyn << endl;
	connOut.push_back(co);
//	cout << endl;	
    }


    // We need one entry in connOut for each postsynaptic neuron.
    connOut.reserve(nNeuronsPost);

    cout << "Building masks for output to postsynaptic group" << endl;
    for (unsigned int i = 0; i < nNeuronsPost; ++i) {
	vector<Indices> indices = listIndexQuadruples.getIndicesBySecond(i);
	if (!indices.empty()) {
	    ConnOutType co;

	    // Initialize the mask sizes to the correct size.
	    co.maskSyn.resize(indices.size());
	    co.idPost = i;
	    for (unsigned int j = 0; j < indices.size(); ++j) {
		co.maskSyn[j] = indices[j].synapse;
		cout << "   synapse = " << co.maskSyn[j] << endl;
	    }
	    cout << "Postsynaptic neuron = " << co.idPost
		 << ", maskSyn = " << co.maskSyn << endl;
	    connOut.push_back(co);
	}	
	cout << endl;
    }
    //
    ////////////////////////////////////////////

    //////////////////////////////////////////////
    //
    // FEEDBACK FROM POSTSYNAPTIC GROUP TO SYNAPSE
    //
    connFb.reserve(nDelays);

    cout << "Building masks for feedback from postsynaptic group" << endl;
    for (list<int>::iterator it = delays.begin(); it != delays.end(); ++it) {
	int delay = *it;
	cout << "Searching for delay " << delay << "..."  << endl;
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
		cout << "   post = " << cf.maskPost[i]
		     << ",  synapse = " << cf.maskSyn[i]
		     << endl;
	    }
	    cout << "Delay = " << delay << ", maskPost = " << cf.maskPost 
		 << ", maskSyn = " << cf.maskSyn << endl;
	    connFb.push_back(cf);
	}	
	cout << endl;
    }
    //
    //////////////////////////////////////////////
    
    // Ensure that feedback state arrays have enough entries in ring
    // buffer for maximum delay
    for (unsigned int i = 0; i < fbStates.size(); ++i) {
	fbStates[i].pNeuronState->setLengthHistory(maxDelay+1);
    }
};
