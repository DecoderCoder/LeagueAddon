#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "asmjit::asmjit" for configuration "Debug"
set_property(TARGET asmjit::asmjit APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(asmjit::asmjit PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/asmjit.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/asmjit.dll"
  )

list(APPEND _cmake_import_check_targets asmjit::asmjit )
list(APPEND _cmake_import_check_files_for_asmjit::asmjit "${_IMPORT_PREFIX}/debug/lib/asmjit.lib" "${_IMPORT_PREFIX}/debug/bin/asmjit.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
