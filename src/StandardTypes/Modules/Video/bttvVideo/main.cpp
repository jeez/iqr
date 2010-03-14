#include <sys/time.h>

#include "moduleBttvVideo.hpp"

using namespace iqrcommon;

struct timeval tim0, tim1;
struct timezone tz;




int main(int argc, char *argv[]){
    moduleBttvVideo myUsbVideo;

    int iCounter;

    cout << "++++++++++++++++++++++++++++++++" << endl;


    int iInterval = 100;
    myUsbVideo.init();
    gettimeofday(&tim0, &tz);
    iCounter = 0;
    while(iCounter >= 0){
	myUsbVideo.update();
	if(iCounter%iInterval == 0){
	    gettimeofday(&tim1, &tz);
	    int iDeltaTime = (tim1.tv_sec-tim0.tv_sec);//+tim1.tv_usec-tim0.tv_usec;
	    gettimeofday(&tim0, &tz);
	    cout << "fps: " << (double)iInterval / (double)iDeltaTime << endl;
	}
	iCounter++;
    }
    myUsbVideo.cleanup();

    cout << "++++++++++++++++++++++++++++++++" << endl;

    return 0;
}
