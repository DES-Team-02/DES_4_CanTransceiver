### SET UP ###
cmake_minimum_required(VERSION 3.15)
project(CanTransceiver 
        VERSION 0.1
        DESCRIPTION "DES_PROJECT_4_Team_02"
        HOMEPAGE_URL  "https://github.com/DES-Team-02"
        LANGUAGES CXX
)

### PACKAGES ###
find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)
find_package(vsomeip3 REQUIRED)

### VARIABLES ###
set(APP_PACKAGE_NAME commonapi)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)

### Compiler Flags ###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Install Directories ###
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for binaries")
set(INSTALL_INCLUDE_DIR include/CanTransceiver CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_CMAKE_DIR lib/cmake/CanTransceiver-someip)
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

### Executable Sources ###
set(Can_Transceiver_SRCS
    ./src/main.cpp
    ./src/CanReceiver.cpp
    ./src/CanTransceiverStubImpl.cpp
    ./src/MovingAverageFilter.cpp
    ./src/RpmProcess.cpp
    ./src/SonarProcess.cpp
)
### add more src files later

### Library Sources ###
FILE(GLOB PRJ_SOMEIP_LIB_SRCS ./src-gen/someip/v${MAJOR_VERSION}/${APP_PACKAGE_NAME}/*.cpp)
### INCLUDE & LINK DIRECTORIES ###
include_directories(
    ./include
    ./src-gen/core
    ./src-gen/someip
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
)

link_directories(
    ${COMMONAPI_LIBDIR}
    ${COMMONAPI_SOMEIP_CMAKE_DIR}/build
)

### BUILD TARGETS ###

## Executable ##
add_executable(CanTransceiver
    ${Can_Transceiver_SRCS}
)

target_link_libraries(CanTransceiver 
    CommonAPI
    CommonAPI-SomeIP 
    vsomeip3
  )

## Build New Libaries for CanTransceiver##
add_library(CanTransceiver-someip
    SHARED ${PRJ_SOMEIP_LIB_SRCS}
)
target_link_libraries(CanTransceiver-someip
    CommonAPI
    CommonAPI-SomeIP
)

## Set the Properties for CanTransceiver Library ##
set_target_properties(CanTransceiver-someip PROPERTIES 
    VERSION ${MAJOR_VERSION}.${MINOR_VERSION} 
    SOVERSION ${MAJOR_VERSION}.${MINOR_VERSION}
)
target_include_directories(CanTransceiver-someip PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src-gen/core>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src-gen/someip>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)
set_target_properties (CanTransceiver-someip PROPERTIES 
    INTERFACE_LINK_LIBRARY ""
)


### Installation for new CanTransceiver Library ###

## Headers ##
file(GLOB_RECURSE HEADER_FILES "src-gen/core/v${MAJOR_VERSION}/${APP_PACKAGE_NAME}/*.hpp")
foreach(HEADER ${HEADER_FILES})
    get_filename_component(HEADER_ABS_PATH ${HEADER} ABSOLUTE)
    file(RELATIVE_PATH HEADER_REL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src-gen/core ${HEADER_ABS_PATH})
    get_filename_component(HEADER_DIR ${HEADER_REL_PATH} DIRECTORY)
    install(FILES ${HEADER_ABS_PATH} DESTINATION ${INSTALL_INCLUDE_DIR}/${HEADER_DIR})

endforeach()


## Targets ##
## Executable file ##
install(TARGETS CanTransceiver
        EXPORT CanTransceiverTargets
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)
## Libraries ##
install(TARGETS CanTransceiver-someip
        EXPORT CanTransceiver-someipTargets
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
)


### Export ###
## Targets ##
# Add all targets to the build-tree export set #

export(TARGETS CanTransceiver-someip
    FILE "${PROJECT_BINARY_DIR}/CanTransceiver-someipTargets.cmake"
)
#(Export the package for use from the build-tree(this registers the build-tree with a global CMake-registry)
export(PACKAGE CanTransceiver-someip)
## Configuration files ##
#(Create the CanTransceiver-someipConfig.cmake and CanTransceiver-someipConfigVersion files ...)
file(RELATIVE_PATH REL_INCLUDE_DIR 
    "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}"
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CanTransceiver-someipConfig.cmake.in
  "${PROJECT_BINARY_DIR}/CanTransceiver-someipConfig.cmake" @ONLY
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CanTransceiver-someipConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/CanTransceiver-someipConfigVersion.cmake" @ONLY
)
install(FILES
  "${PROJECT_BINARY_DIR}/CanTransceiver-someipConfig.cmake"
  "${PROJECT_BINARY_DIR}/CanTransceiver-someipConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
)
install(EXPORT CanTransceiver-someipTargets
  DESTINATION "${INSTALL_CMAKE_DIR}"
)
### FOR CHECK PATHS ###

message(STATUS "COMMONAPI_INCLUDE_DIRS : ${COMMONAPI_INCLUDE_DIRS}")
message(STATUS "COMMONAPI_SOMEIP_INCLUDE_DIRS : ${COMMONAPI_SOMEIP_INCLUDE_DIRS}")
message(STATUS "COMMONAPI_LIBDIR : ${COMMONAPI_LIBDIR}")
message(STATUS "COMMONAPI_SOMEIP_CMAKE_DIR : ${COMMONAPI_SOMEIP_CMAKE_DIR}")
