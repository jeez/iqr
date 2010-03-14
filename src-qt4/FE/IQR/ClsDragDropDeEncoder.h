#ifndef CLSDRAGDROPDEENCODER_H
#define CLSDRAGDROPDEENCODER_H

#include <iostream>
#include <string>

#include <qregexp.h>

#include <iqrUtils.h>

struct dropInfo {
    int ItemType;
    string ItemID;
    string Params;
    string Range;
};

class ClsDragDropDeEncoder {

public:
    static string encode(int ItemType, string ItemID, string Params, string Range){
	string str = iqrUtils::int2string(ItemType)  + "|" + ItemID + "|" + Params + "|" + Range + "|";
	return str;
    };

    static dropInfo decode(string str){
	dropInfo di;
	QRegExp rx( "(.*)(\\|)(.*)(\\|)(.*)(\\|)(.*)(\\|)(.*)" );
	int pos = rx.indexIn(QString(str.c_str())); //ZZZ
	if ( pos > -1 ) {
	    di.ItemType = iqrUtils::string2int(rx.cap(1).toStdString());
	    di.ItemID = rx.cap(3).toStdString();
	    di.Params = rx.cap(5).toStdString();
	    di.Range = rx.cap(7).toStdString();
	};
/*
	cout << "di.ItemType: " << di.ItemType << endl; 
	cout << "di.ItemID: " << di.ItemID << endl; 
	cout << "di.Params : " << di.Params << endl;  
	cout << "di.Range  : " << di.Range << endl;    
*/
	return di;
    }

private:
    ClsDragDropDeEncoder(){};
};

#endif




//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/IQR/ && make -k "
//// End:
