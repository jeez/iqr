/****************************************************************************
 ** $Filename: ClsListItems.h
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sun Jun  1 12:36:54 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSLISTITEMS_H
#define CLSLISTITEMS_H    /*+ To stop multiple inclusions. +*/

#include <iostream>
#include <qlistbox.h> 
#include <vector>
#include <list>

#include <map>
#include "iqrUtils.h"
#include "ClsHyperLists.h"

using namespace std;


//--typedef vector<pair<int, int> > tListOfPoints;
typedef vector<tiPoint> tVectorOfPoints;

class ClsListItemList : public QListBoxText  {

public:
//    ClsListItemList(QListBox * listbox, vector<pair<int, int> > _lst) : QListBoxText ( listbox ), lst(_lst) {
    ClsListItemList(QListBox * listbox, tVectorOfPoints _lst) : QListBoxText ( listbox ), lst(_lst) {
	string str;
	
//	vector<pair<int, int> >::iterator it;
	vector<tiPoint>::iterator it;
	for(it=lst.begin();it!=lst.end();it++){
//	    cout << "x: " << (*it).first << ", y: " << (*it).second << endl;
	    str.append("(");
	    str.append(iqrUtils::int2string((*it).first));
	    str.append(",");
	    str.append(iqrUtils::int2string((*it).second));
	    str.append(")");
	}

	setText(str.c_str());
    };
    
    tVectorOfPoints getVector() { return lst; };
    
private:
    tVectorOfPoints lst;
};


class ClsListItemTuples : public QListBoxText  {

public:
    ClsListItemTuples(QListBox * vectorbox, tVectorOfPoints _lstSource, tVectorOfPoints _lstTarget) : 
	QListBoxText ( vectorbox ), lstSource(_lstSource), lstTarget(_lstTarget) {
	string strSources, strTargets, strTitle;
//	 cout << "lstSource.size(): " << lstSource.size() << endl;
//	 cout << "lstTarget.size(): " << lstTarget.size() << endl;
	 
	unsigned int ii = 0;
	tVectorOfPoints::iterator itSource;
	for(itSource=lstSource.begin();itSource!=lstSource.end();itSource++){
	    ii++;
//	    cout << "(*itSource).first: " << (*itSource).first << endl;
	    
//	    strSources.append("(");
  	    strSources.append(iqrUtils::int2string((*itSource).first));
  	    strSources.append(",");
  	    strSources.append(iqrUtils::int2string((*itSource).second));
//  	    strSources.append(")");
	    
	    if(ii<lstSource.size()){
		strSources.append("|");
	    }
  	}
	
	ii = 0;
  	tVectorOfPoints::iterator itTarget;
  	for(itTarget=lstTarget.begin();itTarget!=lstTarget.end();itTarget++){
	    ii++;
//	    strTargets.append("(");
  	    strTargets.append(iqrUtils::int2string((*itTarget).first));
  	    strTargets.append(",");
  	    strTargets.append(iqrUtils::int2string((*itTarget).second));
//  	    strTargets.append(")");
	    if(ii<lstTarget.size()){
		strTargets.append("|");
	    }
  	}

	strTitle.append(strSources);
	strTitle.append(":");
	strTitle.append(strTargets);
	
  	setText(strTitle.c_str());
    };

    tVectorOfPoints getVectorSource() { return lstSource; };
    tVectorOfPoints getVectorTarget() { return lstTarget; };

    
private:
    tVectorOfPoints lstSource;
    tVectorOfPoints lstTarget;
    
};





#endif 









//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j8"
//// End:

