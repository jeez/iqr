/****************************************************************************
 ** $Filename: threadModule.cpp
 **
 ** $Author: Ulysses Bernardet
 ** 
 *****************************************************************************/


#include "threadModule.hpp"

using namespace std;

void* iqrcommon::ClsThreadModule::run(void *arg){ 
//    cout << "ClsThreadModule::run()" << endl;

    ClsThreadModule *th = (ClsThreadModule*)arg;
    th->flag = 1;

    while(th->flag){
	th->update();
    }
    return NULL;
}

void iqrcommon::ClsThreadModule::start(){
//    cout << "ClsThreadModule::start()" << endl;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid, &attr, run, this);
}


void iqrcommon::ClsThreadModule::stop(){ 
//    cout << "ClsThreadModule::stop" << endl;
    flag = 0;
#ifndef _WINDOWS // this check does not work under windows....
    if(tid){
#endif
	pthread_join(tid, NULL);
#ifndef _WINDOWS
    }
#endif
};

void iqrcommon::ClsThreadModule::join(){
//    cout << "ClsThreadModule::join()" << endl;
#ifndef _WINDOWS // this check does not work under windows....
    if(tid){
#endif
	pthread_join(tid, NULL);
#ifndef _WINDOWS
    }
#endif
}































