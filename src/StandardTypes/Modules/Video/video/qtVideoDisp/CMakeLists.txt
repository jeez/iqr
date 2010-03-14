cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -fPIC) 

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)


SET (libSrc 
  ClsVideoDisp.cpp
)


SET(libMocHdr
  ClsVideoDisp.hpp
)

QT_WRAP_CPP(VideoDisp libSrc ${libMocHdr} )


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDE_DIR} 
)


ADD_LIBRARY(VideoDisp STATIC ${libSrc})

