#include <iostream>
#include <stdio.h>
//#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <list>
#include <fstream>
#include <popt.h>
#include <stdio.h>

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#endif

using namespace std;


void usage(poptContext optCon, int exitcode, char *error, char *addl) {
    poptPrintUsage(optCon, stderr, 0);
    if (error) fprintf(stderr, "%s: %s0", error, addl);
    exit(exitcode);
}

int main(int argc, const char *argv[] ) {

/* 
   assumptions:
   - 1. column: time
   2. column: supervised
   - data has predefined group size
*/


/* cmdl parsing */

     char    c;            /* used for argument parsing */
     int     i = 0;        /* used for tracking options */
     char    *pcaInFileName;
     int iGroupWidth = 64;
     int iGroupHeight = 48;

     int     j;
     char    buf[BUFSIZ+1];
     poptContext optCon;   /* context for parsing command-line options */
 
     struct poptOption optionsTable[] = {
 	{ "width",    'w', POPT_ARG_INT, &iGroupWidth, 0, "input width", "WIDHT" },
 	{ "height",    'h', POPT_ARG_INT, &iGroupHeight, 0, "input height", "HEIGHT" },
 	POPT_AUTOHELP
 	{ NULL, 0, 0, NULL, 0 }
     };
 
     optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
     poptSetOtherOptionHelp(optCon, "<filename>");
 
     if (argc < 2) {
 	poptPrintUsage(optCon, stderr, 0);
 	return(-1);
     }
 
 
     /* Now do options processing, get pcaInFileName */
     while ((c = poptGetNextOpt(optCon)) >= 0) {
 	switch (c) {
 	case 'c':
 	    buf[i++] = 'c';
 	    break;
 	case 'h':
 	    buf[i++] = 'h';
 	    break;
 	case 's':
 	    buf[i++] = 's';
 	    break;
 	case 'n':
 	    buf[i++] = 'n';
 	    break;
 	}
     }
     pcaInFileName = (char*)poptGetArg(optCon);
     if((pcaInFileName == NULL) || !(poptPeekArg(optCon) == NULL)){
 	usage(optCon, 1, "Specify a single port", ".e.g., /dev/cua0");
     }
 
     if (c < -1) {
 	/* an error occurred during option processing */
 	fprintf(stderr, "%s: %s\n",
 		poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
 		poptStrerror(c));
 	return 1;
     }
 
     /* Print out options, pcaInFileName chosen */
     printf("Options  chosen: ");
     for(j = 0; j < i ; j++){
	 printf("-%c ", buf[j]);
     }


     if(iGroupWidth) {
	 printf("-w %d ", iGroupWidth);
     }

     if(iGroupHeight) {
	 printf("-h %d ", iGroupHeight);
     }

     printf("\nPcaInFileName chosen: %s\n", pcaInFileName);
 
     poptFreeContext(optCon);
//     sleep(2);
/* ------------ */


/* parameters */
    string strDataInFileName = pcaInFileName;
    string strDataOutFileName = "unsup.dat";
    string strMPEGFileName = "output.mpeg";
    

/* -------- */

    int iTime;
    double fUnsupervised;
    vector<double> vectVideoData;
    
    vectVideoData.resize(iGroupWidth*iGroupHeight);
    
    IplImage* iplimgGroup = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 1);
    IplImage* iplimgGroupColor = cvCreateImage (cvSize (iGroupWidth,iGroupHeight),IPL_DEPTH_8U, 3);

/*
    int iOutWidth = 352;
    int iOutHeight = 288;
*/

    int iOutWidth = 176;
    int iOutHeight = 144;

    IplImage* iplImgVideoFrameOut = cvCreateImage (cvSize (iOutWidth,iOutHeight),IPL_DEPTH_8U, 3);

    CvVideoWriter* writer = cvCreateVideoWriter( strMPEGFileName.c_str(), 
						 CV_FOURCC('P','I','M','1'),
						 25, 
						 cvSize(iOutWidth, iOutHeight), 
						 1 );

    

    ifstream ifstr;
    ifstr.open(strDataInFileName.c_str(), ios::in);

    ofstream ofstr;
    ofstr.open(strDataOutFileName.c_str(), ios::out); 
    if(ifstr.fail()) {
	ifstr.clear(ios::failbit);
	cerr << "error openng file" << endl;
	return -1;
    }

    std::stringstream ssResponse;

    char tmp;
    int iCounter = 0;

    string strComment = "#";
    string strLine = "";

    while (! ifstr.eof() ) {
	getline (ifstr,strLine);

 	if(!strComment.compare(strLine.substr(0,1))){
	    
	} else {
	    if(iCounter>0){ /* this first line after the comment is the column header */
		ssResponse.str(strLine);
//	    cout << "BUFFER: " << ssResponse.str() << endl;

		ssResponse >> iTime >> tmp;
		ssResponse >> fUnsupervised >> tmp;
//		cout << "iTime: " << iTime << endl;
//		cout << "fUnsupervised: " << fUnsupervised << endl;
	
		
		if(iTime>=0){
		    ofstr << fUnsupervised << endl;
		    
		    for(unsigned int ii=0;ii<vectVideoData.size()-1;ii++){
			if(ssResponse.good()){
			    ssResponse >> vectVideoData[ii] >> tmp;
			}  
		    }
		    
		    
		    for (int x=0;x<iGroupWidth;x++) {
			for (int y=0;y<iGroupHeight;y++) {
			    iplimgGroup->imageData[x+y*iGroupWidth] =  (unsigned char)(vectVideoData[x+y*iGroupWidth]*255.); 
			}
		    }
		    cvCvtColor( iplimgGroup, iplimgGroupColor, CV_GRAY2BGR);
		    cvResize( iplimgGroupColor, iplImgVideoFrameOut);
		    cvWriteFrame(writer, iplImgVideoFrameOut);
		    
		}
		
		iTime = -99;
		fUnsupervised = -99;
	    }

	    if((iCounter%10)==0){
		cout  << iCounter << " ";
		cout.flush();
 	    }
 	    iCounter++;
//	    if(iCounter>30){
//		break;
//	    }
 	}
	strLine = "";
    }
    cout << endl << "iTime: " << iTime << endl;
    cout << "counter: " << iCounter << endl << "-----------------" << endl;
    ifstr.close();
    ofstr.close();

    cvReleaseVideoWriter(&writer);

    return 0;
}


