SET(CMAKE_INSTALL_PREFIX /opt/iqr)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)


SET (libSrc 
  ClsKoalaW.cpp
  ../ClsBaseKRobot.cpp
)

INCLUDE_directories(
  SSCL
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../
  ${QT_INCLUDE_DIR}
)


ADD_SUBDIRECTORY(SSCL)

ADD_LIBRARY(moduleKoalaW SHARED ${libSrc})


SET_TARGET_PROPERTIES(moduleKoalaW PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(moduleKoalaW
  SSCL
)

INSTALL(TARGETS 
  moduleKoalaW
#  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../lib/Modules/ 
#  LIBRARY DESTINATION lib/Modules/ 
  LIBRARY DESTINATION lib/iqr/Modules/ 
  ARCHIVE DESTINATION lib/iqr/Modules/
  RUNTIME DESTINATION bin 
)
