SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fpermissive")





ADD_DEFINITIONS(-DQT_THREAD_SUPPORT -DLINUX) 

SET(CMAKE_VERBOSE_MAKEFILE TRUE) 


SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)

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
  ${KDE3_INCLUDE_DIR}
)


SET(libSrc
  ClsFEParamRelais.cpp
  ClsQHarborDialogImpl.cpp
  ClsQHarborImpl.cpp
  ClsQHarborValueDialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../DialogManager/AutoDialog/Widgets/doubleSpinBox.cpp
)




SET(libUi
  ClsQHarborDialog.ui
  ClsQHarbor.ui
)

SET(libMocHdr
  ${CMAKE_CURRENT_SOURCE_DIR}/../DialogManager/AutoDialog/Widgets/doubleSpinBox.hpp
  ClsFEParamRelais.h
  ClsQHarborDialogImpl.h
  ClsQHarborImpl.h
  ClsQHarborValueDialog.h
#  ClsQHarbor.h 
#  ClsQHarborDialog.h
)

IF(CMAKE_OSX_ARCHITECTURES)
SET(CMAKE_CXX_FLAGS 
"${CMAKE_CXX_FLAGS}  -undefined dynamic_lookup"
)                           
ENDIF(CMAKE_OSX_ARCHITECTURES)



QT_WRAP_UI(IqrParamRelais IqrParamRelais_HDRS libSrc  ${libUi})
QT_WRAP_CPP(IqrParamRelais libSrc ${libMocHdr} )


ADD_LIBRARY(IqrParamRelais SHARED ${libSrc})

# this is necessary if we want to be able to do in and out of source compiles. Has to do
# withe the dependency calculation of cmake ...
IF(IN_SOURCE_COMPILE)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Helper ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Helper)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item   ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item)
  SET(libLink IqrItem IqrHelper)
  
ENDIF(IN_SOURCE_COMPILE)

TARGET_LINK_LIBRARIES(IqrParamRelais 
 ${libLink}
 ${QT_LIBRARIES} 
)

SET(CMAKE_INSTALL_PREFIX /usr/lib)

INSTALL(TARGETS IqrParamRelais 
#  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/
#  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION lib
)

#MESSAGE( STATUS "HARBOR:CMAKE_INSTALL_PREFIX:       " ${CMAKE_INSTALL_PREFIX} )
#MESSAGE( STATUS "HARBOR:CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )







