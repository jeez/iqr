SET(CMAKE_INSTALL_PREFIX /opt/iqr)

IF(APPLE)
SET(CMAKE_CXX_FLAGS 
"${CMAKE_CXX_FLAGS}  -undefined dynamic_lookup"
)                           
ENDIF(APPLE)


SET (IntegrateFireSrc neuronIntegrateFire.cpp)
SET (LinearThresholdSrc neuronLinearThreshold.cpp)
SET (PyramidApicalShuntSrc neuronPyramidApicalShunt.cpp)
SET (RandomSpikeSrc neuronRandomSpike.cpp)
SET (SigmoidSrc neuronSigmoid.cpp)


INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
)


ADD_LIBRARY(neuronIntegrateFire SHARED ${IntegrateFireSrc})
ADD_LIBRARY(neuronLinearThreshold SHARED ${LinearThresholdSrc})
ADD_LIBRARY(neuronPyramidApicalShunt SHARED ${PyramidApicalShuntSrc})
ADD_LIBRARY(neuronRandomSpike SHARED ${RandomSpikeSrc})
ADD_LIBRARY(neuronSigmoid SHARED ${SigmoidSrc})


SET_TARGET_PROPERTIES(neuronIntegrateFire PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(neuronLinearThreshold PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(neuronPyramidApicalShunt PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(neuronRandomSpike PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(neuronSigmoid PROPERTIES PREFIX "")

#this is a nasty hack for minGW...
IF(WIN32)
  TARGET_LINK_LIBRARIES(neuronIntegrateFire
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(neuronLinearThreshold
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(neuronPyramidApicalShunt
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(neuronRandomSpike
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(neuronSigmoid
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

ENDIF(WIN32)

IF(UNIX)
  INSTALL(TARGETS 
    neuronIntegrateFire
    neuronLinearThreshold
    neuronPyramidApicalShunt
    neuronRandomSpike 
    neuronSigmoid   
    LIBRARY DESTINATION lib/iqr/Neurons/ 
    ARCHIVE DESTINATION lib/iqr/Neurons/
    RUNTIME DESTINATION bin 
  )
ENDIF(UNIX)

IF(WIN32)
  INSTALL(TARGETS 
    neuronIntegrateFire
    neuronLinearThreshold
    neuronPyramidApicalShunt
    neuronRandomSpike 
    neuronSigmoid   
    LIBRARY DESTINATION Neurons/ COMPONENT types 
    ARCHIVE DESTINATION Neurons/ 
    RUNTIME DESTINATION Neurons/ COMPONENT types 
  )
ENDIF(WIN32)
