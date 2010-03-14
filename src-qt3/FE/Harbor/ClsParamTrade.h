#ifndef CLSPARAMTRADE_H
#define CLSPARAMTRADE_H

#include <string>
#include <iostream>

using namespace std;

struct ClsParamTrade {
    string Type;
    string Name;
    string ID;
    string Child;
    string Param;
    double Value;
    double Min;
    double Max;

/*
    ClsParamTrade(const ClsParamTrade& p){
	cout << "ClsParamTrade(const ClsParamTrade& p)" << endl;
	Type  = p.Type;
	Name  = p.Name; 
	ID    = p.ID;   
	Child = p.Child;
	Param = p.Param;
	Value = p.Value;
	Min   = p.Min;  
	Max   = p.Max;  
    }
*/
};

#endif








