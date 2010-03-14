/****************************************************************************
 ** $Filename: threadModule.hpp
 ** $Id$
 **
 ** Copyright (C) 2004 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Fri Jan  9 23:58:08 2004
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef THREADMODULE_HPP
#define THREADMODULE_HPP

#include <stdio.h>
#include <pthread.h>

#include <qmutex.h> //new 

//#include <module.hpp>
#include <Common/Item/module.hpp>

using namespace std;


namespace iqrcommon {

    class ClsThreadModule : public ClsModule{
    public:
	~ClsThreadModule(){
//	    cout << "~ClsThreadModule" << endl;
	    cleanup();
	}

	void stop();
	void join();

	virtual void start();
	static void* run(void *arg);

	void setMutex(QMutex* _qmutexThread) {
	    qmutexThread = _qmutexThread;
	}


    protected:
	int flag;
	pthread_t tid;
	QMutex *qmutexThread;
    };
}

#endif
