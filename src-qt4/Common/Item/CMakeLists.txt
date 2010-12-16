SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_DEBUG  -DQT_THREAD_SUPPORT")

IF(WIN32)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--exclude-symbols,pthreadVC2_NULL_THUNK_DATA ")

ENDIF(WIN32)

SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../FE/icons/ModuleIcons
  ${CMAKE_CURRENT_SOURCE_DIR}/../Helper
  ${QT_INCLUDE_DIR}
)


SET (libSrc
 boolParameter.cpp
 doubleParameter.cpp
 intParameter.cpp
 item.cpp
 module.cpp
 neuron.cpp
 optionsParameter.cpp
 parameter.cpp
 stateVariable.cpp
 stateVariableHolder.cpp
 stringParameter.cpp
 colorParameter.cpp
 pathParameter.cpp
 synapse.cpp
 threadModule.cpp
 xRef.cpp
 xRefHolder.cpp
)


ADD_LIBRARY(IqrItem SHARED ${libSrc})

IF(WIN32)
  TARGET_LINK_LIBRARIES(IqrItem
  pthreadGCE2
  )
ENDIF(WIN32)


SET(CMAKE_INSTALL_PREFIX /usr/lib)


IF(UNIX)
  install(TARGETS IqrItem
    LIBRARY DESTINATION lib COMPONENT applications  
    ARCHIVE DESTINATION lib COMPONENT applications  
    RUNTIME DESTINATION bin COMPONENT applications  
  )
ENDIF(UNIX)

IF(WIN32)
  install(TARGETS IqrItem
    LIBRARY DESTINATION lib/.. 
    ARCHIVE DESTINATION lib/.. 
    RUNTIME DESTINATION bin/.. COMPONENT applications  
  )
ENDIF(WIN32)


