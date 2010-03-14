/****************************************************************************
 ** $Filename: ClsInfoDiagramIcon.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 *****************************************************************************/


#ifndef CLSINFODIAGRAMICON_H
#define CLSINFODIAGRAMICON_H    /*+ To stop multiple inclusions. +*/

#include <string>
#include "../Helper/iqrUtils.h"

#define DEFAULT_LOCATION 10
#define DEFAULT_SIZE 50

// 05/02/02 [jmb] Placed system file parser classes into namespace
// iqrcommon.
namespace iqrcommon {
    
    class ClsInfoDiagramIcon
    {
    public:
	ClsInfoDiagramIcon(){ 
	    iX= iY = DEFAULT_LOCATION;
	    iWidth = iHeight = DEFAULT_SIZE;
	};
	
	
	ClsInfoDiagramIcon(int _iX, int _iY, int _iWidth, int _iHeight ): iWidth(_iWidth), iHeight(_iHeight) { 
	    iX = (_iX >= 0 ? _iX : DEFAULT_LOCATION);
	    iY = (_iY >= 0 ? _iY : DEFAULT_LOCATION);
	};
	
	ClsInfoDiagramIcon(int _iX, int _iY) { 
	    iX = (_iX >= 0 ? _iX : DEFAULT_LOCATION);
	    iY = (_iY >= 0 ? _iY : DEFAULT_LOCATION);
	    iWidth = iHeight = DEFAULT_SIZE;
	};     
	
	int getX(){ return iX; };
	int getY(){ return iY; };
	int getWidth(){ return iWidth; };
	int getHeight(){ return iHeight; };

	string getXAsString(){ return iqrUtils::int2string(iX); };
	string getYAsString(){ return iqrUtils::int2string(iY); };
	string getWidthAsString(){ return iqrUtils::int2string(iWidth); };
	string getHeightAsString(){ return iqrUtils::int2string(iHeight); };

	
	void setX(string strX){ 
	    iX = iqrUtils::string2int(strX); 
	    iX = (iX >= 0 ? iX : DEFAULT_LOCATION);
	};

	void setY(string strY){ 
	    iY = iqrUtils::string2int(strY); 
	    iY = (iY >= 0 ? iY : DEFAULT_LOCATION);
	};



	void setWidth(string strWidth){ iWidth = iqrUtils::string2int(strWidth); };
	void setHeight(string strHeight){ iHeight = iqrUtils::string2int(strHeight); };

	void setX(int _iX){ 
	    iX = (_iX >= 0 ? _iX : DEFAULT_LOCATION);
	};
	
	void setY(int _iY){ 
	    iY = (_iY >= 0 ? _iY : DEFAULT_LOCATION);
	};

	void setWidth(int _iWidth){ iWidth = _iWidth; };
	void setHeight(int _iHeight){ iHeight = _iHeight; };	

	
    private:
	int iX, iY;
	int iWidth, iHeight;
	
	friend ostream &operator<<(ostream &stream, 
				   ClsInfoDiagramIcon clsInfoDiagramIcon);	
    };
}

#endif /* CLSINFODIAGRAMICON_H */

/// Local Variables: 
/// mode: c++
/// End: 

