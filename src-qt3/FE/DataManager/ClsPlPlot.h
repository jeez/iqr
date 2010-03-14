#include <string>
#include <plplot/plstream.h>
#include <vector>

#include <ClsFEBasePlot.h>
#ifndef CLSPLPLOT_H
#define CLSPLPLOT_H


class ClsPlPlot : public ClsFEBasePlot {
    
public:
    
    ClsPlPlot(int _iXPoints, int _iYPoints, string _strTitle): iXPoints(_iXPoints), iYPoints(_iYPoints), strTitle(_strTitle) {
//	char caGeometry[] = "200x410+100+200"; // doesn't work....
	pls = new plstream(1, 1, "xwin");
//	pls->setopt("geometry", caGeometry);


//      char geometry[] = "500x410+100+200";
//      plsetopt("geometry", geometry);

//      plsdev("xwin");
//  //    plssub(1, 1);
//      plinit();



    setupPlot();

    };
    
    void setupPlot( ) {
//  	PLFLT xp=0.0; //Number of pixels, x. 
//  	PLFLT yp=0.0; //Number of pixels, y. 
//  	PLINT xleng=100; // Page length, x. 
//  	PLINT yleng=100; // Page length, y. 
//  	PLINT xoff=50; // Page offset, x. 
//  	PLINT yoff=50; // Page offset, y. 

//  	pls->spage(xp, yp, xleng, yleng, xoff, yoff);

	pls->adv(0);
	pls->vsta();
	pls->wind(-.5, (double)iXPoints-.5, -.5, (double)iYPoints-.5);
	pls->col0(7);
	pls->schr(0., 0.8);
	pls->lab("width", "height", strTitle.c_str());
	pls->schr(0., 0.3);
	pls->box("bcnt", 1.0, 0, "bcnvt", 1.0, 0);

	PLFLT plfltI[2], plfltH[2], plfltL[2], plfltS[2];
	plfltI[0] = 0.; plfltI[1] = 1.;
// Hue does not matter for zero saturation.
	plfltH[0] = 0.; plfltH[1] = 1.;
// Lightness varies through its full range.
	plfltL[0] = 0.; plfltL[1] = 1.;
// Saturation is zero for a gray scale.
	plfltS[0] = 0.; plfltS[1] = 0.;
// Note the first argument which specifies HLS space.
	pls->scmap1l(0, 2, plfltI, plfltH, plfltL, plfltS, NULL);
//----------------------------------
    };



    void plotData(double fValMin, double fValMax, vector<double> vectorData){
// doing the checker board -------------
	double fCol;
	int iIndex;
	for(int iy=0; iy< iYPoints; iy++){
	    for (int ix = 0; ix < iXPoints; ix++) {
		iIndex = (iy*iXPoints)+ix;
//		cout << "index: " << iIndex << " min: " << fValMin << ", Max: " << fValMax << ", val: " << vectorData[iIndex] << endl;
		fCol = ( vectorData[iIndex] - fValMin) / ((fValMax - fValMin)> 0 ? (fValMax - fValMin) : 0.00001);		
//	    fCol = vectorData[iIndex] / fValMax;		
		pls->col(fCol);
		pls->psty(0);
		drawBox(ix, iy);
	    }
	}
//-------------------------------------      

    };

private:
    void drawBox(PLFLT iXCenter, PLFLT iYCenter) {
	PLFLT x[4], y[4];

	x[0] = iXCenter - .5; y[0] = iYCenter - .5;
	x[1] = iXCenter - .5; y[1] = iYCenter + .5;
	x[2] = iXCenter + .5; y[2] = iYCenter + .5;
	x[3] = iXCenter + .5; y[3] = iYCenter - .5;

	pls->fill(4, x, y);
	pls->col0(7);
	pls->lsty(1);
	pls->line(4, x, y);
    };

    
    ~ClsPlPlot() {
	plend();
    };

    plstream *pls;
    int iXPoints, iYPoints;
    string strTitle;
    
    
};
    
    
#endif


//// Local Variables: 
//// mode: c++
//// End: 

