cmake_minimum_required(VERSION 2.6)

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

IF(UNIX)
  ADD_DEFINITIONS(-D_TTY_POSIX_)
ENDIF(UNIX)

IF(WIN32)
  ADD_DEFINITIONS(-D_TTY_WIN_)
ENDIF(WIN32)


SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})


SET (libSrc 
  moduleEPuck.cpp
  ../ClsBaseKRobot.cpp
)

INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qtEPuckDisp
  ${CMAKE_CURRENT_SOURCE_DIR}/qextserialport
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../
  ${QT_INCLUDE_DIR}
)


ADD_SUBDIRECTORY(qtEPuckDisp)
ADD_SUBDIRECTORY(qextserialport)

ADD_LIBRARY(moduleEPuck SHARED ${libSrc})


SET_TARGET_PROPERTIES(moduleEPuck PROPERTIES PREFIX "")

IF(UNIX)
  TARGET_LINK_LIBRARIES(moduleEPuck
    ${QT_LIBRARIES}
    EPuckDisp
    qextserialport
  )
ENDIF(UNIX)

IF(WIN32)
  TARGET_LINK_LIBRARIES(moduleEPuck
    ${QT_LIBRARIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../Common/Item/libIqrItem.dll
    EPuckDisp
    qextserialport
  )
ENDIF(WIN32)



IF(UNIX)
  INSTALL(TARGETS 
    moduleEPuck
    LIBRARY DESTINATION lib/iqr/Modules/
    ARCHIVE DESTINATION lib/iqr/Modules/
    RUNTIME DESTINATION bin 
  )
ENDIF(UNIX)

IF(WIN32)
  INSTALL(TARGETS
    moduleEPuck
    LIBRARY DESTINATION Modules/ COMPONENT types 
    ARCHIVE DESTINATION Modules/ 
    RUNTIME DESTINATION Modules/ COMPONENT types 
  )
ENDIF(WIN32)
