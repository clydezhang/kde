project(FFTWExamples)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Find ITK.
MESSAGE( STATUS "hola"  )
FIND_PACKAGE(FFTW3 REQUIRED)
MESSAGE( STATUS "found: "  ${FFTW3_FOUND})
IF(FFTW3_FOUND)
MESSAGE( STATUS "found it"  )
MESSAGE( STATUS "PATH: "  ${FFTW3_INCLUDE_DIRS})
include_directories(${FFTW3_INCLUDE_DIRS})
set(LIBS ${FFTW3_LIBRARIES})
ENDIF(FFTW3_FOUND)

SET( EXAMPLE_LIST
dct_example dct_example_2 dct_example_3
 )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=c99")

FOREACH( var ${EXAMPLE_LIST} )
  ADD_EXECUTABLE( ${var} ${var}.c )
  TARGET_LINK_LIBRARIES( ${var} ${LIBS})
ENDFOREACH( var )



