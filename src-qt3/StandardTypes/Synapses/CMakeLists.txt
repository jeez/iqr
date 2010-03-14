SET(CMAKE_INSTALL_PREFIX /opt/iqr)
SET(CMAKE_VERBOSE_MAKEFILE TRUE) 

##first check the required iqr environment variables:
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


SET (ApicalShuntSrc        synapseApicalShunt.cpp)  
SET (FixedWeightSrc  	   synapseFixedWeight.cpp)  
SET (HebbianSrc  	   synapseHebbian.cpp)  
SET (TestSrc  		   synapseTest.cpp)  
SET (UniformFixedWeightSrc synapseUniformFixedWeight.cpp)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../
)


ADD_LIBRARY(synapseApicalShunt        SHARED ${ApicalShuntSrc})       
ADD_LIBRARY(synapseFixedWeight        SHARED ${FixedWeightSrc})       
ADD_LIBRARY(synapseHebbian  	      SHARED ${HebbianSrc})  	     
ADD_LIBRARY(synapseTest  	      SHARED ${TestSrc})  	     
ADD_LIBRARY(synapseUniformFixedWeight SHARED ${UniformFixedWeightSrc})


SET_TARGET_PROPERTIES(synapseApicalShunt PROPERTIES PREFIX "")  
SET_TARGET_PROPERTIES(synapseFixedWeight PROPERTIES PREFIX "")  
SET_TARGET_PROPERTIES(synapseHebbian PROPERTIES PREFIX "")  
SET_TARGET_PROPERTIES(synapseTest PROPERTIES PREFIX "")  
SET_TARGET_PROPERTIES(synapseUniformFixedWeight PROPERTIES PREFIX "")


INSTALL(TARGETS 
  synapseApicalShunt  
  synapseFixedWeight  
  synapseHebbian  
  synapseTest  
  synapseUniformFixedWeight
#  LIBRARY DESTINATION  LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/Synapses/
#  LIBRARY DESTINATION lib/Synapses/ 
  LIBRARY DESTINATION lib/iqr/Synapses/ 
)









