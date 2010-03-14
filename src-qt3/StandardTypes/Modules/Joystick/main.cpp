#include <unistd.h>
#include "joystick.h"

int main(int argc, char *argv[]){
    Joystick* joystick = new Joystick("/dev/input/js0");
    ActorData ad;

    int iCounter = 0;
    while(iCounter >= 0){
	joystick->acquire(ad);

	cout << "servo   : " << ad.servo << endl;
	cout << "motorLH : " << ad.motorLH << endl;
	cout << "motorLV : " << ad.motorLV << endl;
	cout << "motorRH : " << ad.motorRH << endl;
	cout << "motorRV : " << ad.motorRV << endl;
	cout << "motorD  : " << ad.motorD << endl;
	cout << "reserved: " << ad.reserved << endl;
	cout << "++++++++++++++++++++++++++++++++" << endl;
	usleep(10e5);
    }

    
    return 0;
}


