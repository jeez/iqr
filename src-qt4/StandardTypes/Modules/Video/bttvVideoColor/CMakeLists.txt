SET(CMAKE_INSTALL_PREFIX /opt/iqr)
SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -D_REENTRANT -DQT_NO_DEBUG -DIQRMODULE)

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)



SET (libSrc 
  moduleBttvVideoColor.cpp
  ../Common/v4ldemos/v4l.c
)


INCLUDE_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Common
  ${CMAKE_CURRENT_SOURCE_DIR}/../Common/v4ldemos
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../FE/icons/ModuleIcons/
  ${QT_INCLUDE_DIR}
)


ADD_LIBRARY(moduleBttvVideoColor SHARED ${libSrc})


SET_TARGET_PROPERTIES(moduleBttvVideoColor PROPERTIES PREFIX "")

INSTALL(TARGETS 
  moduleBttvVideoColor
#  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../lib/Modules/ 
#  LIBRARY DESTINATION lib/Modules/ 
  LIBRARY DESTINATION lib/iqr/Modules/ 
  ARCHIVE DESTINATION lib/iqr/Modules/
  RUNTIME DESTINATION bin

)

