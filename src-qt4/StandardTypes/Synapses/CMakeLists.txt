SET(CMAKE_INSTALL_PREFIX /opt/iqr)
SET(CMAKE_VERBOSE_MAKEFILE TRUE) 


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


#this is a nasty hack for minGW...
IF(WIN32)
  TARGET_LINK_LIBRARIES(synapseApicalShunt
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(synapseFixedWeight
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(synapseHebbian
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(synapseTest
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

  TARGET_LINK_LIBRARIES(synapseUniformFixedWeight
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/Item/libIqrItem.dll
  )

ENDIF(WIN32)

IF(UNIX)
  INSTALL(TARGETS 
    synapseApicalShunt  
    synapseFixedWeight  
    synapseHebbian  
    synapseTest  
    synapseUniformFixedWeight
    LIBRARY DESTINATION lib/iqr/Synapses/ 
    ARCHIVE DESTINATION lib/iqr/Synapses/
    RUNTIME DESTINATION bin 
  )
ENDIF(UNIX)

IF(WIN32)
  INSTALL(TARGETS 
    synapseApicalShunt  
    synapseFixedWeight  
    synapseHebbian  
    synapseTest  
    synapseUniformFixedWeight
    LIBRARY DESTINATION Synapses/ COMPONENT types 
    ARCHIVE DESTINATION Synapses/
    RUNTIME DESTINATION Synapses/ COMPONENT types 
  )
ENDIF(WIN32)
