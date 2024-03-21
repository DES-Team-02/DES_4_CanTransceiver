#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CanTransceiver-someip" for configuration ""
set_property(TARGET CanTransceiver-someip APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(CanTransceiver-someip PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libCanTransceiver-someip.so.0.1"
  IMPORTED_SONAME_NOCONFIG "libCanTransceiver-someip.so.0.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS CanTransceiver-someip )
list(APPEND _IMPORT_CHECK_FILES_FOR_CanTransceiver-someip "${_IMPORT_PREFIX}/lib/libCanTransceiver-someip.so.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
