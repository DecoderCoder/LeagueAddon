#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PolyHook_2::PolyHook_2" for configuration "Debug"
set_property(TARGET PolyHook_2::PolyHook_2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PolyHook_2::PolyHook_2 PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/PolyHook_2.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/PolyHook_2.dll"
  )

list(APPEND _cmake_import_check_targets PolyHook_2::PolyHook_2 )
list(APPEND _cmake_import_check_files_for_PolyHook_2::PolyHook_2 "${_IMPORT_PREFIX}/debug/lib/PolyHook_2.lib" "${_IMPORT_PREFIX}/debug/bin/PolyHook_2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
