#ifndef CLSQMINIGROUP_H
#define CLSQMINIGROUP_H

#include <list>

//#include <qlabel.h> 
//--#include <qcstring.h> 
#include <qimage.h> 
#include <qwmatrix.h> 
//#include <qwidget.h>

#include <iqrUtils.h> 
#include <ClsHyperLists.h>

#include "ClsTopologySparse.h"
#include "ClsTopologyHex.h"
#include "ClsTopologyRect.h"

using namespace std;


class ClsQMiniGroup {


public:


    static QPixmap getPixmap(ClsBaseTopology* clsBaseTopology, string strRange, int iMyWidth, int iMaxHeight) {
	QImage qimg;
	QPixmap qpm;


	int iGroupWidth, iGroupHeight;
	double fScaleFactor;

	if(dynamic_cast<ClsTopologySparse*>(clsBaseTopology)){
	    int iHMax = 0;
	    int iHMin = 99999;
	    int iVMax = 0;
	    int iVMin = 99999;

	    list<tiPoint > lstPoints = dynamic_cast<ClsTopologySparse*>(clsBaseTopology)->getList();
	    list<tiPoint >::iterator it;	
	    for(it=lstPoints.begin(); it!=lstPoints.end();it++){
		int iX = (*it).first;
		iHMax = (iX > iHMax ? iX : iHMax);
		iHMin = (iX < iHMin ? iX : iHMin);
		
		int iY = (*it).second;
		iVMax = (iY > iVMax ? iY : iVMax);
		iVMin = (iY < iVMin ? iY : iVMin);
	    }
	    iGroupWidth = iHMax + 1; 
	    iGroupHeight = iVMax + 1;
	    fScaleFactor = init(qimg, iGroupWidth, iGroupHeight, iMyWidth, iMaxHeight);	    
	    qimg.fill(BLACK);

	    if(strRange.size() > 0){
		for(it = lstPoints.begin(); it != lstPoints.end(); it++){
		    int ix = it->first;
		    int iy = it->second;
		    *(qimg.scanLine(iy) + ix) = ((ix + iy)%2 ? LIGHT_GRAY : MID_GRAY);
		}

		string::size_type pos;
		int iIndex;
		while((pos = strRange.find(";")) != string::npos){
		    iIndex = iqrUtils::string2int(strRange.substr(0, pos));

		    tiPoint p = clsBaseTopology->index2pos(iIndex);
		    int ix = p.first;
		    int iy = p.second;

//		    cout << __FILE__ << ": ix, iy: " << ix << ", " << iy << endl;
//		    tiPoint p(ix, iy);
		    if(std::find(lstPoints.begin(), lstPoints.end(), p)!=lstPoints.end()){
			*(qimg.scanLine(iy) + ix) = ((ix + iy)%2 ? SALMON : RED);
		    }
		
		    strRange.erase(0, pos+1);
		}
	    } else {
		for(it = lstPoints.begin(); it != lstPoints.end(); it++){
		    int ix = it->first;
		    int iy = it->second;
//		    cout << "ix: " << ix << endl;
//		    cout << "iy: " << iy << endl;
		
		    *(qimg.scanLine(iy) + ix) = ((ix + iy)%2 ? SALMON : RED);
		}
	    }
	
	    finalize(qimg, qpm, fScaleFactor);

	} else {
	    iGroupWidth = clsBaseTopology->nrCellsHorizontal();
	    iGroupHeight = clsBaseTopology->nrCellsVertical();
	    fScaleFactor = init(qimg, iGroupWidth, iGroupHeight, iMyWidth, iMaxHeight);
	    if(strRange.size() > 0){
		drawCheckerBoard(qimg, LIGHT_GRAY, MID_GRAY, iGroupWidth, iGroupHeight);
		string::size_type pos;
		int iIndex;
		while((pos = strRange.find(";")) != string::npos){
		    iIndex = iqrUtils::string2int(strRange.substr(0, pos));

		    /*
		    int iy = iGroupHeight - int(iIndex / iGroupWidth) -1;
		    int ix = iIndex - int(iIndex / iGroupWidth) * iGroupWidth;
		    */

		    tiPoint p = clsBaseTopology->index2pos(iIndex);
		    int ix = p.first -1;
		    int iy = p.second -1;

		    *(qimg.scanLine(iy) + ix) = ((ix + iy)%2 ? SALMON : RED);
		    strRange.erase(0, pos+1);
		}
	       
	    }
	    else {
		drawCheckerBoard(qimg, SALMON, RED, iGroupWidth, iGroupHeight);
	    }
	    finalize(qimg, qpm, fScaleFactor);

	}
	return qpm;
    };

  
private:
    
    enum COLOR {
	MID_GRAY = 0,
	LIGHT_GRAY =1,
	RED = 2,
	SALMON = 3,
	BLACK = 4
    };
	
	

    static double init(QImage &qimg, int iGroupWidth, int iGroupHeight, int iMyWidth, int iMaxHeight ){
	double fXScaleFactor = (double)iMyWidth / (double)iGroupWidth;
	double fYScaleFactor = (double)iMaxHeight / (double)iGroupHeight;
	double fScaleFactor = ( fXScaleFactor < fYScaleFactor ? fXScaleFactor : fYScaleFactor);

	qimg.create(iGroupWidth, iGroupHeight, 8, 5, QImage::IgnoreEndian); 

	qimg.setColor( MID_GRAY, qRgb(90,90,90) );
	qimg.setColor( LIGHT_GRAY, qRgb(160,160,160) );
	qimg.setColor( RED, qRgb(240,15,15) );
	qimg.setColor( SALMON, qRgb(250,110,110) );
	qimg.setColor( BLACK, qRgb(0,0,0) );

	return fScaleFactor;
    }


    static void finalize(QImage &qimg, QPixmap &qpm, double fScaleFactor){
	qpm.convertFromImage(qimg, QPixmap::Color);
	QWMatrix m;                
	m.scale(fScaleFactor, fScaleFactor);
	qpm = qpm.xForm( m ); 
//	this->setPixmap(qpm);

    }

    static void drawCheckerBoard(QImage &qimg, int iColor0, int iColor1, int iWidth, int iHeight){
	qimg.fill(iColor0);
	for (int ix = 0; ix<iWidth; ix++){
	    for (int iy = 0; iy<iHeight; iy++){
		*(qimg.scanLine(iy) + ix) = ((ix + iy)%2 ? iColor0 : iColor1);
	    }
	}
    }


};


#endif


//// Local Variables:
//// mode: c++
//// compile-command: "cd /home/ulysses/Code/iqr421_Redesign/FE/SystemGUI/ && make -k -j8"
//// End:
