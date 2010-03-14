#include "v4l2Device.hpp"
#include <libv4l2.h>
#include <fcntl.h>
#include <sys/ioctl.h>  // ioctl macros
#include <cstring>      // memset
#include <cstdlib>      // calloc
#include <cassert>      // calloc
#include <sys/mman.h>   // mmap
#include <sstream>      // stringstream
#include <iostream>     // remove once you're done
#include <errno.h>      // remove once you're done
using namespace std;

V4L2Device::V4L2Device(const string& device_name){
    fpFrame = NULL;
    cpRawFrame = NULL;
    iFD = -1; sBuffers = NULL; iNBuffers = 0;

    if((iFD = v4l2_open(device_name.c_str(), O_RDWR /*| O_NONBLOCK*/)) < 0 || 
       (v4l2_fd_open(iFD, V4L2_ENABLE_ENUM_FMT_EMULATION) < 0) ){
        strErrorMessage = "Not a v4l2 device";
        return;
    } 
}

bool V4L2Device::isValid(){
    return (0 <= iFD);
}

string V4L2Device::info(){
    return listInputs();
}

bool V4L2Device::setParameters(int input, int standard, int fps, int iWidth,
                               int iHeight, int brightness, int saturation,
                               int contrast, bool autogain){
    if(!selectInput(input) || !selectStandard(standard) ||
       !selectFPS(fps) || !selectResolution(iWidth, iHeight)) return false;

    this->iWidth = iWidth;
    this->iHeight = iHeight;
    // If controls fail to work, don't abort
    if(!setControl(V4L2_CID_BRIGHTNESS, brightness)){};
    if(!setControl(V4L2_CID_SATURATION, saturation)){};
    if(!setControl(V4L2_CID_CONTRAST, contrast)){};
    if(!setControl(V4L2_CID_AUTOGAIN, autogain)){};

    fpFrame = new float[iWidth*iHeight*sizeof(float)*3];
    cpRawFrame = new uchar[iWidth*iHeight*sizeof(uchar)*3];

    return true;
}

bool V4L2Device::startCapturing(){
    if(!init()){ // If init() fails, close() takes care of cleaning the mess
        close();
        return false;
    }

    return true;
}

void V4L2Device::stopCapturing(){
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_ioctl(iFD, VIDIOC_STREAMOFF, &type);
}

uchar* V4L2Device::bgrFrame(){
    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    while(-1 == v4l2_ioctl(iFD, VIDIOC_DQBUF, &buf)){
        if(EAGAIN != errno){
            cout<<"DQBUF failed"<<endl;
            cout<<"errno "<<errno<<endl;
            return NULL;
        }
    }

    assert((int)buf.index < iNBuffers);  // Sanity check

    memcpy(cpRawFrame, sBuffers[buf.index].start, iWidth*iHeight*sizeof(uchar)*3);

    assert(-1 != v4l2_ioctl(iFD, VIDIOC_QBUF, &buf));

    return cpRawFrame;
}

V4L2Device::~V4L2Device(){
    close();
}

// private:

bool V4L2Device::selectInput(int input){
    stringstream ss_err;
    if (-1 == v4l2_ioctl(iFD, VIDIOC_S_INPUT, &input)){
        ss_err << "Incorrect Input ("<<input<<"). "
            << "Use \"info window\" to list available inputs";
        strErrorMessage = ss_err.str();
        return false;
    }
    return true;
}

bool V4L2Device::selectStandard(int standard){
    v4l2_std_id std_id = standard;

    string standards = listStandards();
    // if no standards available, don't try to set one
    if(standards.rfind("None") != standards.size()){ return true; }

    if(-1 == v4l2_ioctl(iFD, VIDIOC_S_STD, &std_id)){
        strErrorMessage = "Could not set standard.";
        return false;
    }

    return true;
}

string V4L2Device::listInputs(){
    stringstream avail_in;
    struct v4l2_input input;

    avail_in<<"inputs ([Index, Name]):\n\n";
    input.index = 0;
    while (v4l2_ioctl(iFD, VIDIOC_ENUMINPUT, &input) >= 0) {
        avail_in<<"["<<input.index<<", "<<input.name<<"]"<<endl;
        v4l2_ioctl(iFD, VIDIOC_S_INPUT, &input); // set input
        avail_in<<listStandards()<<endl;
        ++input.index;
    }
    return avail_in.str();
}

string V4L2Device::listStandards(){
    stringstream avail_std;
    v4l2_standard std;

    avail_std<<"    standards ([Index, Name, Max FPS]):\n\t";
    std.index = 0;
    while(-1 != v4l2_ioctl(iFD, VIDIOC_ENUMSTD, &std)){
        avail_std<<"["<<std.index<<", "<<std.name<<", "<<
            std.frameperiod.denominator/std.frameperiod.numerator<<"] ";
        ++std.index;
        if(0 == std.index%5){avail_std<<endl<<"\t";}
    }
    if(!std.index){ avail_std<<"None"<<endl;}
    avail_std<<endl;
    return avail_std.str();
}

bool V4L2Device::selectFPS(int fps){
    /*
       if(fps > 0){
       strErrorMessage = "Frames per second still not available";
       return false;
       }
       */
    return true;
}

bool V4L2Device::selectResolution(int iWidth, int iHeight){
    struct v4l2_format fmt;
    stringstream ss_err;

    memset(&fmt, 0, sizeof(fmt));

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = iWidth;
    fmt.fmt.pix.height = iHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if(v4l2_ioctl(iFD, VIDIOC_S_FMT, &fmt) < 0){
        ss_err << "V4L2_PIX_FMT_BGR24 or resolution ("<<iWidth<<", "<<iHeight
            <<") not supported";
        strErrorMessage = ss_err.str();
        return false;
    }

    if((int)fmt.fmt.pix.width != iWidth || (int)fmt.fmt.pix.height != iHeight){
        ss_err <<"Resolution ("<<iWidth<<", "<<iHeight<<") not supported"
            <<endl<<"closest reported by driver is ("<<fmt.fmt.pix.width<<", "
            <<fmt.fmt.pix.height<<")";
        strErrorMessage = ss_err.str();
        return false;
    }

    return true;
}

bool V4L2Device::setControl(int identifier, int value){
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;

    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = identifier;

    if(v4l2_ioctl(iFD, VIDIOC_QUERYCTRL, &queryctrl) < 0){
        strErrorMessage = "Could not query control";
        return false;
    }

    memset(&control, 0, sizeof(control));
    control.id = identifier;

    double min, max, step; // possible values in range [min, max]
    int range;

    switch(queryctrl.type){
      case V4L2_CTRL_TYPE_INTEGER:
        min = queryctrl.minimum;
        max = queryctrl.maximum;
        step = queryctrl.step;
        range = (max-min)/step;
        control.value = min + (range * value)/100;
        break;
      case V4L2_CTRL_TYPE_BOOLEAN:
        control.value = (value > 0);
        break;
      default:
        break;
    }

    if(v4l2_ioctl(iFD, VIDIOC_S_CTRL, &control) < 0){
        strErrorMessage = "Could not set control";
        return false;
    }

    return true;
}


bool V4L2Device::init(){
    struct v4l2_requestbuffers req;

    memset(&req, 0, sizeof(req));

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if(-1 == v4l2_ioctl(iFD, VIDIOC_REQBUFS, &req)){
        strErrorMessage = "mmap not provided";
        return false;
    }

    if(req.count < 2) {
        strErrorMessage = "insufficient buffer memory for mmap";
        return false;
    }

    iNBuffers = req.count;

    if( !(sBuffers = (Buffer*) calloc(iNBuffers, sizeof (*sBuffers))) ){
        strErrorMessage = "insufficient memory for buffer allocation";
        return false;
    }

    for(int i = 0; i < iNBuffers; ++i) sBuffers[i].start = MAP_FAILED;

    // Check status of sBuffers
    for(int buffer_i = 0; buffer_i < iNBuffers; ++buffer_i) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = buffer_i;

        if (-1 == v4l2_ioctl(iFD, VIDIOC_QUERYBUF, &buf)){
            strErrorMessage = "VIDIOC_QUERYBUF failed, unable to capture";
            return false;
        }

        sBuffers[buffer_i].length = buf.length;
        sBuffers[buffer_i].start = v4l2_mmap(NULL, buf.length,
                                            PROT_READ | PROT_WRITE,
                                            MAP_SHARED, iFD, buf.m.offset);

        if (MAP_FAILED == sBuffers[buffer_i].start){
            strErrorMessage = "mmap failed, unable to capture";
            return false;
        }
    }

    for (int i = 0; i < iNBuffers; ++i) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == v4l2_ioctl(iFD, VIDIOC_QBUF, &buf)){
            strErrorMessage = "could not queue buffer";
            return false;
        }
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == v4l2_ioctl(iFD, VIDIOC_STREAMON, &type)){
        strErrorMessage = "could not start streaming";
        return false;
    }

    return true;
}

void V4L2Device::close(){
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));


    if(sBuffers){        // Allocated sBuffers
        for(int i = 0; i < iNBuffers; ++i){
            if(MAP_FAILED != sBuffers[i].start){
                v4l2_munmap(sBuffers[i].start, sBuffers[i].length);
                sBuffers[i].start = MAP_FAILED;
            }
        }
        free(sBuffers);
        iNBuffers = 0;
        sBuffers = NULL;
    }

    if(0 <= iFD){
        req.count               = 0;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;
        v4l2_ioctl(iFD, VIDIOC_REQBUFS, &req); // Free allocated driver memory 

        v4l2_close(iFD);
        iFD = -1;
    }

    if(fpFrame){ 
        delete [] fpFrame;
        fpFrame = NULL;
    }

    if(cpRawFrame){ 
        delete [] cpRawFrame;
        cpRawFrame = NULL;
    }
}
