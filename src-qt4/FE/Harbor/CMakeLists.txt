
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fpermissive")


IF(APPLE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -undefined dynamic_lookup"
)                           
ENDIF(APPLE)


IF(WIN32)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer"
)                           
ENDIF(WIN32)



cmake_minimum_required(VERSION 2.6)

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT) 

IF(UNIX)
ADD_DEFINITIONS(-DLINUX) 
ENDIF(UNIX)

IF(WIN32)
ADD_DEFINITIONS(-D_WINDOWS) 
ENDIF(WIN32)

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

SET(QT_MT_REQUIRED TRUE)
SET(QT_USE_QTUITOOLS TRUE)
SET(QT_USE_QTXML TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})


STRING(COMPARE EQUAL "x${PROJECT_SOURCE_DIR}" "x${CMAKE_CURRENT_SOURCE_DIR}" IN_SOURCE_COMPILE)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Helper 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/Neuron 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Parser 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Types/Neurons 
  ${CMAKE_CURRENT_SOURCE_DIR}/../DiagramManager 
  ${CMAKE_CURRENT_SOURCE_DIR}/../DialogManager/AutoDialog/Widgets 
  ${CMAKE_CURRENT_SOURCE_DIR}/../IQR 
  ${CMAKE_CURRENT_SOURCE_DIR}/../System 
  ${CMAKE_CURRENT_SOURCE_DIR}/../System/Connection/Pattern 
  ${CMAKE_CURRENT_SOURCE_DIR}/../System/Topology 
  ${QT_INCLUDE_DIR} 
)

SET(SOURCES
  ClsFEParamRelais.cpp
  ClsQHarborDialogImpl.cpp
  ClsQHarborImpl.cpp
  ClsQHarborValueDialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../DialogManager/AutoDialog/Widgets/doubleSpinBox.cpp
)
 

SET(UIS
  ClsQHarborDialog.ui
  ClsQHarbor.ui
)


SET(MOC_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/../DialogManager/AutoDialog/Widgets/doubleSpinBox.hpp
  ClsFEParamRelais.h
  ClsQHarborDialogImpl.h
  ClsQHarborImpl.h
  ClsQHarborValueDialog.h
  ui_ClsQHarbor.h 
  ui_ClsQHarborDialog.h
)




QT4_WRAP_UI(UI_HEADERS  ${UIS})
QT4_WRAP_CPP(SOURCES ${MOC_HEADERS} )



ADD_LIBRARY(IqrParamRelais SHARED ${SOURCES})


# this is necessary if we want to be able to do in and out of source compiles. Has to do
# withe the dependency calculation of cmake ...
IF(IN_SOURCE_COMPILE)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Helper ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Helper)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item   ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item)
  SET(libLink IqrItem IqrHelper)
  
ENDIF(IN_SOURCE_COMPILE)

IF(UNIX)
TARGET_LINK_LIBRARIES(IqrParamRelais 
 ${libLink}
 ${QT_LIBRARIES} 
)
ENDIF(UNIX)


IF(WIN32)
  TARGET_LINK_LIBRARIES(IqrParamRelais
  ${QT_LIBRARIES} 
  )
ENDIF(WIN32)

IF(APPLE)
TARGET_LINK_LIBRARIES(IqrParamRelais 
 ${libLink}
 ${QT_LIBRARIES} 
)
ENDIF(APPLE)


SET(CMAKE_INSTALL_PREFIX /usr/lib)

IF(UNIX)
  INSTALL(TARGETS IqrParamRelais 
    LIBRARY DESTINATION lib COMPONENT applications  
    ARCHIVE DESTINATION lib 
    RUNTIME DESTINATION bin 
  )
ENDIF(UNIX)

IF(WIN32)
  INSTALL(TARGETS IqrParamRelais 
    LIBRARY DESTINATION lib/..   
    ARCHIVE DESTINATION lib/..
    RUNTIME DESTINATION lib/.. COMPONENT applications
  )
ENDIF(WIN32)






