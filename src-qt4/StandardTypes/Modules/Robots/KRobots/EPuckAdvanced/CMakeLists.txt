SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-Wall -Iserialstream -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

#SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(libSrc
    serialstream/SerialStream.cc
    serialstream/SerialStreamBuf.cc
    moduleEpuck.cc
    epuck.cc
)
INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  /usr/include/iqr
  ${QT_INCLUDE_DIR}
)
ADD_LIBRARY(moduleEPuckAdvanced SHARED ${libSrc})

SET(calSrc
    serialstream/SerialStream.cc
    serialstream/SerialStreamBuf.cc
    epuck.cc
)
ADD_EXECUTABLE(calibrate calibrate.cc ${calSrc})

SET_TARGET_PROPERTIES(moduleEPuckAdvanced PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(moduleEPuckAdvanced m)

INSTALL(TARGETS 
  moduleEPuckAdvanced
  LIBRARY DESTINATION lib/iqr/Modules/
  ARCHIVE DESTINATION lib/iqr/Modules/
  RUNTIME DESTINATION bin 
)

cmake_minimum_required(VERSION 2.4)
