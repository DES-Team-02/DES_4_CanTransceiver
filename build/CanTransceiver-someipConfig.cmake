# Config file for the CanTransceiver package
# Exports the follwing targets:
#   CanTransceiver - CMake target for CanTransceiver SomeIP
# Additionally, the following variables are defined:
#   CanReceiver_VERSION - The CanTransceiver version number

# Compute paths
get_filename_component(CANTRANSCEIVER_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

# Our library dependencies (contains definitions for IMPORTED targets)
include("${CANTRANSCEIVER_CMAKE_DIR}/CanTransceiver-someipTargets.cmake")

# Legacy variable, kept for compatibility
get_target_property(CANTRANSCEIVER_INCLUDE_DIRS CanTransceiver-someip INTERFACE_INCLUDE_DIRECTORIES)

set(CANRECEIVER_VERSION )
set(CANRECEIVER_VERSION_STRING "")
