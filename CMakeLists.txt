cmake_minimum_required(VERSION 2.8.3)
project(vicon_stream)

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

# Check the architecture
set(HOST_ARCH linux32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(HOST_ARCH linux64)
endif()

# Check arch and select correct libs
if (HOST_ARCH STREQUAL linux64)
    set(VICON_SDK_PATH lib/Linux_DataStreamSDK1_5/Linux64-boost-1.53.0)
elseif (HOST_ARCH STREQUAL linux32)
    set(VICON_SDK_PATH lib/Linux_DataStreamSDK1_5/Linux32-boost-1.53.0)
else()
    message(FATAL_ERROR "Configuration error: Unable to detect if 64 or 32-bit.
    CMAKE_SIZEOF_VOID_P is not 4 or 8: CMAKE_SIZEOF_VOID_P=" ${CMAKE_SIZEOF_VOID_P})
endif()

file(GLOB_RECURSE VICON_SDK ${VICON_SDK_PATH}/libViconDataStreamSDK_CPP.*)
file(GLOB_RECURSE VICON_DEBUG ${VICON_SDK_PATH}/libDebug*)
file(GLOB_RECURSE VICON_BTHREAD ${VICON_SDK_PATH}/libboost_thread*)
file(GLOB_RECURSE VICON_BUT ${VICON_SDK_PATH}/libboost_unit_test*)
file(GLOB_RECURSE VICON_BSYS ${VICON_SDK_PATH}/libboost_system*)
file(GLOB_RECURSE VICON_BLOC ${VICON_SDK_PATH}/libboost_locale*)


# Library linking and source
add_library(vicon_stream SHARED viconstream.cpp)
#add_executable(vicon_stream viconstream.cpp)
target_link_libraries(vicon_stream ${VICON_SDK} ${VICON_DEBUG} ${VICON_BUT}
    ${VICON_BSYS} ${VICON_BLOC})

# Messages

message(STATUS " ")
message(STATUS "Paths and variables:")
message(STATUS HOST_ARCH=${HOST_ARCH})
message(STATUS VICON_SDK_PATH=${VICON_SDK_PATH})
message(STATUS VICON_SDK=${VICON_SDK})
message(STATUS VICON_DEBUG=${VICON_DEBUG})
message(STATUS VICON_BTHREAD=${VICON_BTHREAD})
message(STATUS VICON_BUT=${VICON_BUT})
message(STATUS VICON_BSYS=${VICON_BSYS})
message(STATUS VICON_BLOC=${VICON_BLOC})
message(STATUS " ")
#message(STATUS " ")
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS " ")
