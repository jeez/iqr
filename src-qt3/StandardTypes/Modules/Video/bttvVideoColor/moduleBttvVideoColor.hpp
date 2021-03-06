#ifndef BttvVIDEO_HPP
#define BttvVIDEO_HPP

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <inttypes.h>
#include <errno.h>

#include "v4l.h"
#include "types.h"

#ifdef IQRMODULE
#include <Common/Item/module.hpp>
#endif

#include <ClsBaseVideoItem-New.hpp>


extern int errno;
static V4LMMInfo v4lMMInfo;
static bits8     *captureBuf;


using namespace std;

namespace iqrcommon {

#ifdef IQRMODULE
    class moduleBttvVideoColor : public ClsBaseVideoItem {//, public ClsModule {
#else
    class moduleBttvVideoColor: public ClsBaseVideoItem  {
#endif

    public:

	moduleBttvVideoColor();
	~moduleBttvVideoColor();
	void init();
	void update();
	void cleanup();

#ifdef IQRMODULE
	moduleIcon getIcon();
#endif

    private:

#ifdef IQRMODULE  
	moduleBttvVideoColor(const moduleBttvVideoColor&);
	ClsStateVariable* clsStateVariableVideoOutputRed;
	ClsStateVariable* clsStateVariableVideoOutputGreen;
	ClsStateVariable* clsStateVariableVideoOutputBlue;
#endif

//--	char *pcGRAYBuffer_last;
//--	int iGRAYBufferSize;

	void openDevice(string strDeviceName);
	void closeDevice();

	void printDeviceInfo();
	void setPictureProperties();


	int captureFrame;
	V4LFormat  v4lFormat;
//	unsigned int iGrayPixel;
	unsigned int ir, ig, ib;
	unsigned long value;
	bits16 iPixel;

    };
}
#endif

