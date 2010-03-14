#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <list>

#include "ClsHyperLists.h"

using namespace std;

typedef vector<tiPoint> tListOfPoints;

class ClsTuplesImporter {

public:
    ClsTuplesImporter(){};
    static list<pair<tListOfPoints, tListOfPoints> > parseFile(string strFileName){
	list<pair<tListOfPoints, tListOfPoints> > lstTuples;
	ifstream ifstr; 
	ifstr.open(strFileName.c_str(), ios::in);
	while(!ifstr.eof()){
	    string strLine;	
	    ifstr >> strLine;

	    if(strLine.length()>0){
//		cout << "--------------------\ninput: " << strLine << endl;

		string::size_type pos = strLine.find_first_of(":");
		string strSources = strLine.substr(0, pos );
		string strTargets = strLine.substr(pos+1, pos - strLine.length());


		tListOfPoints lstSource;
		tListOfPoints lstTarget;

		vector<string>::iterator iter;
		char tmp;
		int iX, iY;

//		cout << "sources" << endl;
		vector<string> vecSourceTokens;
		tokenize(strSources, vecSourceTokens, "|" );
		for(iter = vecSourceTokens.begin();iter!=vecSourceTokens.end();iter++){
//	cout<< (*iter) << endl;
		    iX = iY = 0;
		    std::stringstream ssTemp;
		    ssTemp.str ((*iter));
		    ssTemp >> iX >> tmp >> iY;
//		    cout << "\tiX, iY: " << iX << ", " << iY << endl;

		    tiPoint pairTemp;
		    pairTemp.first = iX;
		    pairTemp.second = iY;

		    lstSource.push_back(pairTemp);	
		}

//		cout << "targets" << endl;
		vector<string> vecTargetTokens;
		tokenize(strTargets, vecTargetTokens, "|" );
		for(iter = vecTargetTokens.begin();iter!=vecTargetTokens.end();iter++){
//	cout<< (*iter) << endl;
		    iX = iY = 0;
		    std::stringstream ssTemp;
		    ssTemp.str ((*iter));
		    ssTemp >> iX >> tmp >> iY;
//		    cout << "\tiX, iY: " << iX << ", " << iY << endl;
		
		    tiPoint pairTemp;
		    pairTemp.first = iX;
		    pairTemp.second = iY;

		    lstTarget.push_back(pairTemp);
		}
		pair<tListOfPoints, tListOfPoints> pairT(lstSource, lstTarget);
		lstTuples.push_back(pairT);

	    }
	}

	return lstTuples;
    }
    
private:
    static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos     = str.find_first_of(delimiters, lastPos);
	
	while (string::npos != pos || string::npos != lastPos) {
	    tokens.push_back(str.substr(lastPos, pos - lastPos));
	    lastPos = str.find_first_not_of(delimiters, pos);
	    pos = str.find_first_of(delimiters, lastPos);
	}
    }
};


