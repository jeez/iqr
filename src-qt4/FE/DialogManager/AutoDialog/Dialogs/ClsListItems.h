/****************************************************************************
 ** $Filename: ClsListItems.h
 ** $Id$
 **
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
#define CLSLISTITEMS_H 

#include <iostream>


#include <QListWidgetItem>

#include <list>
#include <map>
#include "iqrUtils.h"
using namespace std;


typedef list<pair<int, int> > tListOfPairs;

class ClsListItemList : public QListWidgetItem  {

public:
//    ClsListItemList(QListWidget * listbox, list<pair<int, int> > _lst) : QListWidgetItem ( listbox ), lst(_lst) {
    ClsListItemList(QListWidget * listbox, tListOfPairs _lst) : QListWidgetItem ( listbox ), lst(_lst) {
	string str;
	
	list<pair<int, int> >::iterator it;
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
    
    tListOfPairs getList() { return lst; };
    
private:
    tListOfPairs lst;
};


class ClsListItemTuples : public QListWidgetItem  {

public:
    ClsListItemTuples(QListWidget * listbox, tListOfPairs _lstSource, tListOfPairs _lstTarget) : 
	QListWidgetItem ( listbox ), lstSource(_lstSource), lstTarget(_lstTarget) {
	string strSources, strTargets, strText;
//	 cout << "lstSource.size(): " << lstSource.size() << endl;
//	 cout << "lstTarget.size(): " << lstTarget.size() << endl;
	 
	unsigned int ii = 0;
	tListOfPairs::iterator itSource;
	for(itSource=lstSource.begin();itSource!=lstSource.end();itSource++){
	    ii++;
//	    cout << "(*itSource).first: " << (*itSource).first << endl;
  	    strSources.append(iqrUtils::int2string((*itSource).first));
  	    strSources.append(",");
  	    strSources.append(iqrUtils::int2string((*itSource).second));
	    
	    if(ii<lstSource.size()){
		strSources.append("|");
	    }
  	}
	
	ii = 0;
  	tListOfPairs::iterator itTarget;
  	for(itTarget=lstTarget.begin();itTarget!=lstTarget.end();itTarget++){
	    ii++;
  	    strTargets.append(iqrUtils::int2string((*itTarget).first));
  	    strTargets.append(",");
  	    strTargets.append(iqrUtils::int2string((*itTarget).second));
	    if(ii<lstTarget.size()){
		strTargets.append("|");
	    }
  	}

	strText.append(strSources);
	strText.append(":");
	strText.append(strTargets);
	
  	setText(strText.c_str());
    };

    tListOfPairs getListSource() { return lstSource; };
    tListOfPairs getListTarget() { return lstTarget; };

    
private:
    tListOfPairs lstSource;
    tListOfPairs lstTarget;
    
};





#endif /* CLSLISTITEMS_H */









//// Local Variables:
//// mode: c++
//// compile-command: "cd ../ && make -k -j4"
//// End:

