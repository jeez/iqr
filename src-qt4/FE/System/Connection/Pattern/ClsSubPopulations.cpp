/****************************************************************************
 ** $Filename: ClsSubPopulations.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Jun 26 17:35:08 2003
 **
 *****************************************************************************/


#include "ClsSubPopulations.h"

#include "item.hpp"
#include "iqrUtils.h"
#include "intParameter.hpp"

//#define DEBUG_CLSSUBPOPULATIONS

ClsBaseSubPopulation::ClsBaseSubPopulation(){};
ClsBaseSubPopulation::~ClsBaseSubPopulation(){
    strType = "";
};

// ClsBaseSubPopulation& ClsBaseSubPopulation::operator=(const ClsBaseSubPopulation& r) {  /* copy operator */
//     cout << "ClsBaseSubPopulation::operator=(const ClsBaseSubPopulation& r)" << endl;
//     return *this;
// };

string ClsBaseSubPopulation::Type() {
    return strType;
};

void ClsBaseSubPopulation::print(){
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsBaseSubPopulation::print()" << endl;
#endif                         	
};

void ClsBaseSubPopulation::setParameter(ClsSysFileNode &/*_clsSysFileNode*/) {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsBaseSubPopulation::setParameter(ClsSysFileNode &_clsSysFileNode)" << endl;
#endif                             
}


/* ------------------------------------------------- */
    
ClsRegion::ClsRegion()  {
    strType = ClsTagLibrary::SelectorRegion();
    xStart = yStart = width = height =0;

/*
  addIntParameter(ClsTagLibrary::RegionXStart(), "xStart",
  "Description", 
  "Properties");

  addIntParameter(ClsTagLibrary::RegionYStart(), "yStart",
  "Description", 
  "Properties");

  addIntParameter(ClsTagLibrary::RegionWidth(), "width",
  "Description", 
  "Properties");

  addIntParameter(ClsTagLibrary::RegionHeight(), "height",
  "Description", 
  "Properties");
*/

};
    

/*
  ClsRegion::ClsRegion(ClsRegion& r) { // copy constructor 
  cout << "ClsRegion::ClsRegion(ClsRegion& r)" << endl;
      
  xStart = r.xStart;
  yStart = r.yStart;
  width = r.width;
  height = r.height;
	
  }; 
*/

ClsRegion& ClsRegion::operator=(const ClsRegion& r) {  /* copy operator */
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsRegion::operator=(const ClsRegion& r)" << endl;
#endif                           
/*  
    ParameterList parameterList = r.getListParameters();
    cout << "1" << endl;
    
    while (parameterList.size()) {
    cout << "2" << endl;
    string strParamName = parameterList.front()->getName();
    cout << "3" << endl;
    string strParamValue = parameterList.front()->getValueAsString(); //--
    cout << "4" << endl;
    cout << "\tstrParamName: " << strParamName << ": " << strParamValue << endl;
    cout << "5" << endl;

    setParameter(parameterList.front()->getName(),parameterList.front()->getValueAsString()); //--
    cout << "6" << endl;

    parameterList.pop_front();
    cout << "7" << endl;
    }
*/
/*
  cout << __FILE__ << ":" << "r.xStart :" << r.xStart << endl;
  cout << __FILE__ << ":" << "r.yStart :" << r.yStart << endl;
  cout << __FILE__ << ":" << "r.width  :" << r.width  << endl;
  cout << __FILE__ << ":" << "r.height :" << r.height << endl;
*/  
  
    xStart = r.xStart;
    yStart = r.yStart;
    width = r.width;
    height = r.height;

    return *this;
};
    
    
void ClsRegion::print(){
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsRegion::print()" << endl;
#endif                         
    cout << "\t\txStart: " << xStart << ",  yStart: " <<  yStart << endl;
    cout << "\t\twidth: " << width << ", height: " << height << endl;
};

void ClsRegion::setParameter(ClsSysFileNode &_clsSysFileNode) {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsRegion::setParameter(ClsSysFileNode &_clsSysFileNode)" << endl;
#endif                             
    while (_clsSysFileNode.countParameters()){
	iqrcommon_old::ClsParameter clsParameterTemp = _clsSysFileNode.popParameter();
	string strParamName = clsParameterTemp.getName();
	string strParamValue = clsParameterTemp.getValueAsString(); //--
//	cout << "\t\tstrParamName: " << strParamName << endl; 
//	cout << "\t\tstrParamValue: " << strParamValue << endl;

	if(!strParamName.compare(ClsTagLibrary::RegionXStart())){
	    setXStart(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare(ClsTagLibrary::RegionYStart())){
	    setYStart(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare(ClsTagLibrary::RegionWidth())){
	    setWidth(iqrUtils::string2int(strParamValue));
	}
	else if(!strParamName.compare(ClsTagLibrary::RegionHeight())){
	    setHeight(iqrUtils::string2int(strParamValue));
	}
    }
};

    
int ClsRegion::getXStart() {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsRegion::getXStart()" << endl;
#endif                         

//--	string str = getParameter(ClsTagLibrary::RegionXStart())->getValueAsString();
//	int ii = iqrUtils::string2int(str);
//	return ii;
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::RegionXStart())->getValueAsString());
    return xStart;
	
};

int ClsRegion::getYStart() {
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::RegionYStart())->getValueAsString());
    return yStart;
};

int ClsRegion::getWidth() {
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::RegionWidth())->getValueAsString());
    return width;
};

int ClsRegion::getHeight() {
//--	return iqrUtils::string2int(getParameter(ClsTagLibrary::RegionHeight())->getValueAsString());
    return height;
};


void ClsRegion::setXStart(int i) {
//	 setParameter(ClsTagLibrary::RegionXStart(),iqrUtils::int2string(i));
    xStart = i;
};

void ClsRegion::setYStart(int i) {
//	 setParameter(ClsTagLibrary::RegionYStart(),iqrUtils::int2string(i));
    yStart = i;
};

void ClsRegion::setWidth(int i) {
//	 setParameter(ClsTagLibrary::RegionWidth(),iqrUtils::int2string(i));
    width = i;
	
};

void ClsRegion::setHeight(int i) {
//	 setParameter(ClsTagLibrary::RegionHeight(),iqrUtils::int2string(i));
    height = i;
};
    

/* ------------------------------------------------- */
ClsList::ClsList() {
    strType = ClsTagLibrary::SelectorList();
};
    
void ClsList::setData( list<pair<int, int> > _lst){
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsList::setData( list<pair<int, int> > _lst)" << endl;
#endif                         
    lst = _lst;
};
    
list<pair<int, int> > ClsList::getData() {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsList::getData()" << endl;
#endif                         
    return lst;
};

void ClsList::print() {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsList::print()" << endl;
#endif                         
    list<pair<int, int> >::iterator it;
    for(it=lst.begin(); it!=lst.end(); it++){
	cout << "\t\tx: " << (*it).first << ", y: " << (*it).second << endl;
    }

};

void ClsList::setParameter(ClsSysFileNode &_clsSysFileNode) {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsList::setParameter(ClsSysFileNode &_clsSysFileNode)" << endl;
#endif                         
    while (_clsSysFileNode.countNodes()){
 	ClsSysFileNode F1 = _clsSysFileNode.popNode(); 

//	string strNodeName = F1.getName();
//	cout << "\tNodeName: " << strNodeName << endl;

	int iX, iY = 0;
	while (F1.countParameters()){
	    iqrcommon_old::ClsParameter clsParameterTemp = F1.popParameter();
	    string strParamName = clsParameterTemp.getName();
	    string strParamValue = clsParameterTemp.getValueAsString(); //--
//	    cout << "\t\tstrParamName: " << strParamName << endl; 
//	    cout << "\t\tstrParamValue: " << strParamValue << endl;

	    if(!strParamName.compare(ClsTagLibrary::PointX())){
		iX = iqrUtils::string2int(strParamValue);
	    }
	    else if(!strParamName.compare(ClsTagLibrary::PointY())){
		iY = iqrUtils::string2int(strParamValue);
	    }
	}
	pair<int, int> pairTemp(iX, iY);;
	lst.push_back(pairTemp);
    }
}

/* ------------------------------------------------- */
ClsAll::ClsAll() {
    strType = ClsTagLibrary::SelectorAll();
};

void ClsAll::print() {
    cout << "\t\tALL" << endl;
};


/* ------------------------------------------------- */
ClsTuples::ClsTuples() {
    strType = ClsTagLibrary::SelectorTuples();
    
};

// ClsTuples& ClsTuples::operator=(const ClsTuples& t) {  
//     cout << "ClsTuples::operator=(const ClsTuples& t)" << endl;
//     lstTuples = t.lstTuples;
//     return *this;
// };


void ClsTuples::addData(tListOfPairs lstSource, tListOfPairs lstTarget) {
    pair<tListOfPairs, tListOfPairs>  pT(lstSource, lstTarget);
    lstTuples.push_back(pT);
};
    
list<pair<tListOfPairs, tListOfPairs> > ClsTuples::getData() {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsTuples::getData()" << endl;
#endif                         
    return lstTuples;
};
    

void ClsTuples::setData(list<pair<tListOfPairs, tListOfPairs> > _lst){
    lstTuples = _lst;
}


    
void ClsTuples::print() {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "\tClsTuples::print()" << endl;
#endif                         
    cout << "\t\tTUPLES" << endl;
    cout << "\t\tlstTuples.size(): " << lstTuples.size() << endl;
    
};

void ClsTuples::setParameter(ClsSysFileNode &_clsSysFileNode) {
#ifdef DEBUG_CLSSUBPOPULATIONS 
    cout << "ClsTuples::setParameter(ClsSysFileNode &_clsSysFileNode)" << endl;
#endif                         
    tListOfPairs lstOPSource;
    tListOfPairs lstOPTarget;

    while (_clsSysFileNode.countNodes()){ /* SourceList, TargetList */
 	ClsSysFileNode F1 = _clsSysFileNode.popNode(); 
	
	string strNodeName = F1.getName();
//	cout << "\tNodeName: " << strNodeName << endl; 
	
	
	while (F1.countNodes()){ /* Point */
	    ClsSysFileNode  clsSysFileNodePoint= F1.popNode(); 
	    int iX, iY = 0;
	    while (clsSysFileNodePoint.countParameters()){
		iqrcommon_old::ClsParameter clsParameterTemp = clsSysFileNodePoint.popParameter();
		string strParamName = clsParameterTemp.getName();
		string strParamValue = clsParameterTemp.getValueAsString(); //--
//		cout << "\t\t\tstrParamName: " << strParamName << "\tstrParamValue: " << strParamValue << endl;
		if(!strParamName.compare(ClsTagLibrary::PointX())){
		    iX = iqrUtils::string2int(strParamValue);
		}
		else if(!strParamName.compare(ClsTagLibrary::PointY())){
		    iY = iqrUtils::string2int(strParamValue);
		}
	    }
	    
	    pair<int, int> pairTemp(iX, iY);;
	    if(!strNodeName.compare(ClsTagLibrary::SourceList())){
		lstOPSource.push_back(pairTemp);
//		cout << "!strNodeName.compare(ClsTagLibrary::SourceList())" << endl;
	    }
	    else if(!strNodeName.compare(ClsTagLibrary::TargetList())){
		lstOPTarget.push_back(pairTemp);
//		cout << "!strNodeName.compare(ClsTagLibrary::TargetList())" << endl;
	    }
	}
    }
    pair<tListOfPairs, tListOfPairs> pairLOP(lstOPSource, lstOPTarget);
    lstTuples.push_back(pairLOP);
};




//// Local Variables:
//// mode: c++
//// compile-command: "cd ../../../.. && make -k "
//// End:
