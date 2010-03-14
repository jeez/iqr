SET(CMAKE_INSTALL_PREFIX /opt/iqr)

##first check the required iqr environment variables:SET(CMAKE_INSTALL_PREFIX /opt/iqr)
#IF(NOT EXISTS $ENV{IQR2_CODE_HOME})
#    MESSAGE( FATAL_ERROR "Environment variable \"IQR2_CODE_HOME\" needs to be set")
#ENDIF(NOT EXISTS $ENV{IQR2_CODE_HOME})
#
#IF(NOT EXISTS $ENV{IQR2_LIB_HOME})
#    MESSAGE( FATAL_ERROR "Environment variable \"IQR2_LIB_HOME	\" needs to be set")
#ENDIF(NOT EXISTS $ENV{IQR2_LIB_HOME})

IF(CMAKE_OSX_ARCHITECTURES)
SET(CMAKE_CXX_FLAGS 
"${CMAKE_CXX_FLAGS}  -undefined dynamic_lookup"
)                           
ENDIF(CMAKE_OSX_ARCHITECTURES)


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

INSTALL(TARGETS 
  neuronIntegrateFire
  neuronLinearThreshold
  neuronPyramidApicalShunt
  neuronRandomSpike 
  neuronSigmoid   
#  LIBRARY DESTINATION  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/Neurons/
#  LIBRARY DESTINATION lib/Neurons/ 
  LIBRARY DESTINATION lib/iqr/Neurons/ 
)
