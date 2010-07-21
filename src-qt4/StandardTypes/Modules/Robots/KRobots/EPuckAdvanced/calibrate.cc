#include <iostream>
#include <vector>
#include <unistd.h>
#include "epuck.h"
using namespace std;

int main(void){
    Epuck epuck;

    if(!epuck.open("/dev/rfcomm0")){
        cout<<"Error opening serial port";
        return -1;
    }

    epuck.calibrateProximitySensors();
    epuck.close();
    return 0;
}
