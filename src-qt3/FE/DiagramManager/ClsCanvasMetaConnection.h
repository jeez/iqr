#ifndef CLSCANVASMETACONNECTION_H
#define CLSCANVASMETACONNECTION_H

#include <string>
#include "ClsCanvasConnection.h"

class ClsCanvasMetaConnection  {

public:
    ClsCanvasMetaConnection( string _strID) : strID(_strID) {
	clsCanvasConnectionIP = NULL;
	clsCanvasConnectionPhantomStart = NULL;
	clsCanvasConnectionPhantomEnd = NULL;

    };

    ~ClsCanvasMetaConnection() {
//	cout << "ClsCanvasMetaConnection::~ClsCanvasMetaConnection" << endl;

//	if(clsCanvasConnectionIP!=NULL){
//	    cout << "ClsCanvasMetaConnection::delete clsCanvasConnectionIP" << endl;
//	    delete clsCanvasConnectionIP;
//	    clsCanvasConnectionIP = NULL;
//	}
	if(clsCanvasConnectionPhantomStart!=NULL){
//	    cout << "ClsCanvasMetaConnection::delete clsCanvasConnectionPhantomStart" << endl;
	    delete clsCanvasConnectionPhantomStart;
	    clsCanvasConnectionPhantomStart = NULL;
	}

	if(clsCanvasConnectionPhantomEnd!=NULL){
//	    cout << "ClsCanvasMetaConnection::delete clsCanvasConnectionPhantomEnd" << endl;
	    delete clsCanvasConnectionPhantomEnd;
	    clsCanvasConnectionPhantomEnd = NULL;
	}
    }


    void setSelected(bool b){
//	cout << "ClsCanvasMetaConnection::setSelected(bool b)" << endl;
	
	if(clsCanvasConnectionPhantomStart!=NULL){
	    if( clsCanvasConnectionPhantomStart->isSelected() != b){
		clsCanvasConnectionPhantomStart->setSelected(b);
	    }
	}
	
	if(clsCanvasConnectionPhantomEnd!=NULL){
	    if( clsCanvasConnectionPhantomEnd->isSelected() != b){
		clsCanvasConnectionPhantomEnd->setSelected(b);
	    }
	    
	}
    }


    void addIPConnection(ClsCanvasConnection* c) { clsCanvasConnectionIP = c; };
    void addPhantomConnectionStart(ClsCanvasConnection* c) { clsCanvasConnectionPhantomStart = c; };
    void addPhantomConnectionEnd(ClsCanvasConnection* c) { clsCanvasConnectionPhantomEnd = c; };

    string getID(){
	    return strID;
	};


    string getTargetID( ) { return strTargetID; };
    void setTargetID(string _strTargetID){
	strTargetID = _strTargetID;
	if(clsCanvasConnectionIP!=NULL){
	    if(strTargetID.compare(clsCanvasConnectionIP->getTargetID())) { /* this checking is necessary to avoid recursion:
									       the parent calls the child, which calls the parent, etc */
		clsCanvasConnectionIP->setTargetID(strTargetID);
	    }
	}

	if(clsCanvasConnectionPhantomStart!=NULL){
	    if(strTargetID.compare(clsCanvasConnectionPhantomStart->getTargetID())) {
		clsCanvasConnectionPhantomStart->setTargetID(strTargetID);
	    }
	}

	if(clsCanvasConnectionPhantomEnd!=NULL){
	    if(strTargetID.compare(clsCanvasConnectionPhantomEnd->getTargetID())) {
		clsCanvasConnectionPhantomEnd->setTargetID(strTargetID);
	    }
	}
    };

    string getSourceID( ) { return strSourceID; };
    void setSourceID(string _strSourceID){
//	cout << "ClsCanvasMetaConnection::setSourceID(string _strSourceID)"<< endl;
//	cout << "ClsCanvasMetaConnection::"<< endl;

	strSourceID = _strSourceID;
	if(clsCanvasConnectionIP!=NULL){
	    if(strSourceID.compare(clsCanvasConnectionIP->getSourceID())) {
		clsCanvasConnectionIP->setSourceID(strSourceID);
	    }
	}

	if(clsCanvasConnectionPhantomStart!=NULL){
	    if(strSourceID.compare(clsCanvasConnectionPhantomStart->getSourceID())) {
		clsCanvasConnectionPhantomStart->setSourceID(strSourceID);
	    }
	}

	if(clsCanvasConnectionPhantomEnd!=NULL){
	    if(strSourceID.compare(clsCanvasConnectionPhantomEnd->getSourceID())) {
		clsCanvasConnectionPhantomEnd->setSourceID(strSourceID);
	    }
	}
    };


    void setConnectionType(int iConnectionType){
	if(clsCanvasConnectionIP!=NULL){
	    if(iConnectionType != clsCanvasConnectionIP->getConnectionType()) {
		clsCanvasConnectionIP->setConnectionType(iConnectionType);
	    }
	}

	if(clsCanvasConnectionPhantomStart!=NULL){
	    if(iConnectionType != clsCanvasConnectionPhantomStart->getConnectionType()) {
		clsCanvasConnectionPhantomStart->setConnectionType(iConnectionType);
	    }
	}

	if(clsCanvasConnectionPhantomEnd!=NULL){
	    if(iConnectionType != clsCanvasConnectionPhantomEnd->getConnectionType()) {
		clsCanvasConnectionPhantomEnd->setConnectionType(iConnectionType);
	    }
	}
    };


    string getPhantomStartID() {
	if(clsCanvasConnectionPhantomStart!=NULL){
	    return clsCanvasConnectionPhantomStart->getID();
	}
	else {
	    return NULL;
	}

    };

    string getPhantomEndID() {
	if(clsCanvasConnectionPhantomEnd!=NULL){
	    return clsCanvasConnectionPhantomEnd->getID();
	}
	else {
	    return NULL;
	}

    };

    ClsCanvasConnection* getIPConnection(){
	return clsCanvasConnectionIP;
    }

    ClsCanvasConnection* getPhantomStartConnection(){
	return clsCanvasConnectionPhantomStart;
    }

    ClsCanvasConnection* getPhantomEndConnection(){
	return clsCanvasConnectionPhantomEnd;
    }

private:
    string strID;
    string strSourceID, strTargetID;

    ClsCanvasConnection* clsCanvasConnectionIP;
    ClsCanvasConnection* clsCanvasConnectionPhantomStart;
    ClsCanvasConnection* clsCanvasConnectionPhantomEnd;
};


#endif


//// Local Variables:
//// mode: c++
//// End:
