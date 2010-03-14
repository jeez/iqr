cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)


SET(libSrc
    moduleVideo.cpp
    videoDeviceFactory.cpp
    videoDevice.cpp
    v4l2Device.cpp
)

INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/qtVideoDisp
  ${CMAKE_CURRENT_SOURCE_DIR} 
  /usr/include/iqr
  ${QT_INCLUDE_DIR}
)


IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    SET(LIBV4L /usr/lib64/libv4l2.so)
ELSE(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    SET(LIBV4L /usr/lib/libv4l2.so)
ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")


ADD_SUBDIRECTORY(qtVideoDisp)


ADD_LIBRARY(moduleVideo SHARED ${libSrc})


SET_TARGET_PROPERTIES(moduleVideo PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(moduleVideo 
 ${LIBV4L}
 m
 VideoDisp
)

INSTALL(TARGETS 
  moduleVideo
  LIBRARY DESTINATION lib/iqr/Modules/ 
  ARCHIVE DESTINATION lib/iqr/Modules/
  RUNTIME DESTINATION bin 
)
