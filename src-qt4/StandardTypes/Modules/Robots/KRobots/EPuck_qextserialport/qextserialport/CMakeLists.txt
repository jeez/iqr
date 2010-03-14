SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

cmake_minimum_required(VERSION 2.6)

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG) 

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})




IF(UNIX)
  SET (libSrc 
    qextserialbase.cpp 
    qextserialport.cpp
    posix_qextserialport.cpp
  )
  ADD_DEFINITIONS(-D_TTY_POSIX_)
ENDIF(UNIX)

IF(WIN32)
  SET (libSrc 
    qextserialbase.cpp 
    qextserialport.cpp
    win_qextserialport.cpp
  )
  ADD_DEFINITIONS(-D_TTY_WIN_)
ENDIF(WIN32)


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDE_DIR} 
)

ADD_LIBRARY(qextserialport STATIC ${libSrc})

IF(WIN32)
  TARGET_LINK_LIBRARIES(qextserialport
    ${QT_LIBRARIES}
  )
ENDIF(WIN32)
