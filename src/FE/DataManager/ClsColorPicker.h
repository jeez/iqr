/****************************************************************************
 ** $Filename: ClsColorPicker.h
 ** $Header$
 **
 ** Copyright (C) 2001 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Dec  5 12:48:30 2001
 ** $Date$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CLSCOLORPICKER_H
#define CLSCOLORPICKER_H    /*+ To stop multiple inclusions. +*/

#include <stack>
class ClsColorPicker {

public:

     ClsColorPicker():  iMin(0), iMax(360), iDiff(30){
     };
//       ClsColorPicker(int _iMin, int _iMax, int _iDiff) : iMin(_iMin), iMax(_iMax), iDiff(_iDiff) {
	  
//       };
     
       int getColor(){
//	    cout << "\nColorPicker's stackColors.size(): " << stackColors.size() << endl;
  	  int iColor;
  	  if(stackColors.size() == 1){
  	       iColor = stackColors.top() + iDiff;
  	       if(iColor>iMax) iColor = iMin;
  	       stackColors.top() = iColor;
  	  } else if(stackColors.size() == 0){
  	       iColor = iMin;
  	       stackColors.push(iColor);
  	  } else if(stackColors.size() > 1){
  	       iColor = stackColors.top();
  	       stackColors.pop();
  	  }
//	  cout << "ColorPicker's new Color: " << iColor << endl << endl;
	  return iColor;
       };

       void putColor(int _iColor){
//	    cout << "\nColorPicker's putColor: " << _iColor << endl << endl;
  	  stackColors.push(_iColor);
       };

       void reset(){
  	  while(!stackColors.empty()){
  	       stackColors.pop();
  	  }
       };
     

private:
     int iMin, iMax, iDiff;
     stack<int> stackColors;
};

#endif /* CLSCOLORPICKER_H */
