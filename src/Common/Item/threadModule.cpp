/****************************************************************************
 ** $Filename: threadModule.cpp
 ** $Id$
 **
 ** Copyright (C) 2004 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Sat Jan 10 01:50:18 2004
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
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
    if(tid){
	pthread_join(tid, NULL);
    }
};

void iqrcommon::ClsThreadModule::join(){
//    cout << "ClsThreadModule::join()" << endl;
    if(tid){
	pthread_join(tid, NULL);
    }
}































