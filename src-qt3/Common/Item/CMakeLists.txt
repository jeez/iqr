SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DLINUX -DNO_DEBUG  -DQT_THREAD_SUPPORT")


SET(QT_MT_REQUIRED TRUE)
FIND_PACKAGE(Qt3 REQUIRED)


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
 synapse.cpp
 threadModule.cpp
 xRef.cpp
 xRefHolder.cpp
)

ADD_LIBRARY(IqrItem SHARED ${libSrc})

SET(CMAKE_INSTALL_PREFIX /usr/lib)

install(TARGETS IqrItem
#  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/
  LIBRARY DESTINATION lib
#  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
)

