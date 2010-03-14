#include "ClsFEGroup.h"

#include <neuronManager.hpp>

#include <tagLibrary.hpp>
#include <optionsParameter.hpp>
#include <stringParameter.hpp>

#include "ClsGroupManipPattern.h" 

ClsFEGroup::ClsFEGroup ( ){
    clsGroupManipPattern = NULL;
    qmutexThread = NULL;
};

ClsFEGroup::ClsFEGroup(string _strGroupID, string _strGroupName, string _strPrcID) :
    ClsBaseGroup(_strGroupID, _strGroupName, _strPrcID) {
//--    ClsCompGroup(_strGroupID, _strGroupName, _strPrcID) {

    clsGroupManipPattern = NULL;
    qmutexThread = NULL;

    addStringParameter(ClsTagLibrary::NotesTag(), /* _strName */
		       "Notes", /* _strLabel */
		       "", /* _strValue */
		       true,  /* _bEditable */
		       true, /* _bLong */
		       "", /* _strDescription */
		       "Notes" /* _strCategory */);

    addColorParameter(ClsTagLibrary::ColorTag(), /* _strName */
		       "Color", /* _strLabel */
		       "#FFFFFF", /* _strValue */
		       true,  /* _bEditable */
		       "Color on Diagram",
		       "Properties" /* _strCategory */);


}



string ClsFEGroup::getNotes() {
//    cout << "ClsFEGroup::getNotes()" << endl;
string strNote = (getParameter(ClsTagLibrary::NotesTag()))->getValueAsString();
    return strNote;
}

void ClsFEGroup::setNotes(string _strNotes){
    setParameter(ClsTagLibrary::NotesTag(),  _strNotes);
}

void ClsFEGroup::setColor( string _str) { setParameter(ClsTagLibrary::ColorTag(),  _str); };
string ClsFEGroup::getColor( ){
    return getParameter(ClsTagLibrary::ColorTag())->getValueAsString();
};


string ClsFEGroup::validate(){
//    cout << "ClsFEGroup::validate()" << endl;
    string strMessage = "";

    if(pNeuron==NULL){
	strMessage = getParameter(ClsTagLibrary::NameTag())->getValueAsString();
	strMessage.append("\n\tNo Neuron defined");
    }
    return strMessage;
}


ClsGroupManipPattern* ClsFEGroup::getGroupManipPattern(){
#ifdef DEBUG_CLSFEGROUP
    cout << "ClsFEGroup::getGroupManipPattern()" << endl;
#endif
    if(clsGroupManipPattern==NULL){
	clsGroupManipPattern = new ClsGroupManipPattern(getNumberOfNeurons());
    }
    return clsGroupManipPattern;
}

void ClsFEGroup::removeGroupManipPattern(){
#ifdef DEBUG_CLSFEGROUP
    cout << "ClsFEGroup::removeGroupManipPattern()" << endl;
#endif
    if(qmutexThread!=NULL){
	qmutexThread->lock();
    }

    if(clsGroupManipPattern!=NULL){
	delete clsGroupManipPattern;
	clsGroupManipPattern = NULL;
    }
    if(qmutexThread!=NULL){
	qmutexThread->unlock();
    }
}

void ClsFEGroup::setMutex(QMutex* _qmutexThread) {
    qmutexThread = _qmutexThread;
//    cout << "qmutexThread == NULL?: " << (qmutexThread==NULL ? "yes" : "no" ) << endl;
};

void ClsFEGroup::update() {
#ifdef DEBUG_CLSFEGROUP
    cerr << "ClsFEGroup::update()" << endl;
#endif
//    cout << "qmutexThread == NULL?: " << (qmutexThread==NULL ? "yes" : "no" ) << endl;

    qmutexThread->lock();
    
    ClsBaseGroup::update();

    if(clsGroupManipPattern!=NULL){
  	if( clsGroupManipPattern->bDoUpdate){
	    if( iRoundCounter%clsGroupManipPattern->getInterval() == 0){
		if( clsGroupManipPattern->getMode() == ClsGroupManipPattern::MODE_CLAMP  ){
		    pNeuron->getOutputState()->getStateArray()[0] = clsGroupManipPattern->getPatternBuffer()[0];
		} else {
		    if( clsGroupManipPattern->getMode() == ClsGroupManipPattern::MODE_ADD  ){
			pNeuron->getOutputState()->getStateArray()[0] += clsGroupManipPattern->getPatternBuffer()[0];
		    }
		    else if( clsGroupManipPattern->getMode() == ClsGroupManipPattern::MODE_MULTIPLY  ){
			pNeuron->getOutputState()->getStateArray()[0] *= clsGroupManipPattern->getPatternBuffer()[0];
		    }
		}
		clsGroupManipPattern->advance();
	    }
 	}
    }

    qmutexThread->unlock();

}


//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:


