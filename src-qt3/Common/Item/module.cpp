/******************************************************************************************
 ** $Header$
 **
 ** Copyright (C) 2003 by Mark Blanchard
 ** $Author: Mark Blanchard
 **
 ** $CreateDate:
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *******************************************************************************************/

#include <iostream>
using namespace std;

#include "module.hpp"
#include "tagLibrary.hpp"
//#include "default-icon.h"
#include <ModuleDefaultIcon.h>

using namespace iqrcommon;


#ifdef DEBUG_CLSMODULE
static const bool bDebugModule = true;
#else
static const bool bDebugModule = false;
#endif

iqrcommon::ClsModule::ClsModule() : ClsItem() {
    if (bDebugModule) {
	cout << "ClsModule::ClsModule" << endl;
    }

    inputFromGroupXRefHolder.setName(ClsTagLibrary::ModuleInputFromGroup());
//    inputFromGroupXRefHolder.setCategory("Inputs from Groups");
    inputFromGroupXRefHolder.setCategory("Groups to Module");

    outputToGroupXRefHolder.setName(ClsTagLibrary::ModuleOutputToGroup());
//    outputToGroupXRefHolder.setCategory("Outputs to Groups");
    outputToGroupXRefHolder.setCategory("Module to Groups");
}

iqrcommon::ClsModule::~ClsModule() {
    if (bDebugModule) {
	cout << "ClsModule::~ClsModule" << endl;
    }
}

void 
iqrcommon::ClsModule::setProcessID(string str){
    strProcessID = str;
}

string 
iqrcommon::ClsModule::getProcessID(){
    return strProcessID;
}

moduleIcon 
iqrcommon::ClsModule::getIcon() {
//    moduleIcon mi(default_icon_png_data, default_icon_png_len, 2 ,2);
    moduleIcon mi(ModuleDefaultIcon_png_data, ModuleDefaultIcon_png_len, 2 ,2);
    return mi;
}

void
iqrcommon::ClsModule::initialize(){

/* there's a little problem with the call to the SystemManager from here:
   a lot is hanging off ClsFESystemManager.h which makes including it hairy.
   There are two ways to I can see:
   - wrap getFEGroup() in ClsFEProcess
   - do the resolving of name -> ID -> pointer in ClsFEProcess 
*/
    list<string> lstXRefHolders = getListOfXRefHolders();
    list<string>::iterator itXRefHolders;
    for(itXRefHolders=lstXRefHolders.begin(); itXRefHolders!=lstXRefHolders.end(); itXRefHolders++){
	XRefList xRefList = getXRefHolder(*itXRefHolders)->getListXRefs();
	XRefList::iterator itXRef;
	for(itXRef=xRefList.begin(); itXRef!=xRefList.end(); itXRef++){
	    string strXRefTarget = (*itXRef)->getTarget();
	}
    }

};

const XRefList& 
iqrcommon::ClsModule::getListInputsFromGroups() const {
    return inputFromGroupXRefHolder.getListXRefs();
};

const XRefList& 
iqrcommon::ClsModule::getListOutputsToGroups() const {
    return outputToGroupXRefHolder.getListXRefs();
};

/** 
 * This function is required to automate saving and GUI building.
 * It corresponds m.o.l ClsFEProcess::getListOfMembers() (that's the idea anyway).
 * @return Return a list with the types of XRefHolders used.
 */
list<string> 
iqrcommon::ClsModule::getListOfXRefHolders(){
    list<string> lst;
        lst.push_back(inputFromGroupXRefHolder.getName());
	lst.push_back(outputToGroupXRefHolder.getName());
    return lst;
}

const iqrcommon::ClsXRefHolder*
iqrcommon::ClsModule::getXRefHolder(string strName) const {
    if(!strName.compare(inputFromGroupXRefHolder.getName())){
	return &inputFromGroupXRefHolder;
    } else if(!strName.compare(outputToGroupXRefHolder.getName())){
	return &outputToGroupXRefHolder;
    }
    return NULL;
}

void 
iqrcommon::ClsModule::setXRefTargetByName(string strXRefHolderName, string strXRefName, string strTargetID){
//    cout << "ClsModule::setXRefTargetByName(string strXRefHolderName, string strXRefName, string strTargetID)" << endl;

/*
    cout << "strXRefHolderName: " << strXRefHolderName << endl;
    cout << "inputFromGroupXRefHolder.getName(): " << inputFromGroupXRefHolder.getName() << endl;
    cout << "outputToGroupXRefHolder.getName(): " << outputToGroupXRefHolder.getName() << endl;
*/

    if(!strXRefHolderName.compare(inputFromGroupXRefHolder.getName())){
	inputFromGroupXRefHolder.setXRefTargetByName(strXRefName, strTargetID);
    } else if(!strXRefHolderName.compare(outputToGroupXRefHolder.getName())){
	outputToGroupXRefHolder.setXRefTargetByName(strXRefName, strTargetID);
    }
}


iqrcommon::ClsXRef*
iqrcommon::ClsModule::getInputFromGroupRefByName(string _name) {
    return inputFromGroupXRefHolder.getXRefByName(_name);
}

iqrcommon::ClsXRef*
iqrcommon::ClsModule::getOutputToGroupRefByName(string _name) {
    return outputToGroupXRefHolder.getXRefByName(_name);
}

iqrcommon::ClsXRef*
iqrcommon::ClsModule::getInputFromGroupRefByLabel(string _label) {
    return inputFromGroupXRefHolder.getXRefByLabel(_label);
}

iqrcommon::ClsXRef*
iqrcommon::ClsModule::getOutputToGroupRefByLabel(string _label) {
    return outputToGroupXRefHolder.getXRefByLabel(_label);
}


void
iqrcommon::ClsModule::setInputFromGroupByName(string _name, string _input) {
    inputFromGroupXRefHolder.setXRefTargetByName(_name, _input);
}

void
iqrcommon::ClsModule::setOutputToGroupByName(string _name, string _input) {
    outputToGroupXRefHolder.setXRefTargetByName(_name, _input);
}

void
iqrcommon::ClsModule::setInputFromGroupByLabel(string _label, string _input) {
    inputFromGroupXRefHolder.setXRefTargetByLabel(_label, _input);
}

void
iqrcommon::ClsModule::setOutputToGroupByLabel(string _label, string _input) {
    outputToGroupXRefHolder.setXRefTargetByLabel(_label, _input);
}

iqrcommon::StateVariablePtr* 
iqrcommon::ClsModule::addInputFromGroup(string _name, string _label)
{
    inputFromGroupXRefHolder.addXRef(_name, _label);
    StateVariablePtr *pInputPtr = new StateVariablePtr(_name);
    inputFromGroupPtrList.push_back(pInputPtr);
    return pInputPtr;
}

iqrcommon::ClsStateVariable*
iqrcommon::ClsModule::addOutputToGroup(string _name, string _label)
{    
    outputToGroupXRefHolder.addXRef(_name, _label);
    return outputToGroupStateHolder.addStateVariable(_name, _label);
}

iqrcommon::ClsStateVariable*
iqrcommon::ClsModule::getState(string _name)
{
    return outputToGroupStateHolder.getState(_name);
}


void 
iqrcommon::ClsModule::resetOutputToGroupStateHolder(){
    outputToGroupStateHolder.resetSize(); 
}
