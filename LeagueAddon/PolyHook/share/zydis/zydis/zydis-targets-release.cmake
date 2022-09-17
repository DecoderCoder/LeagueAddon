#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Zydis::Zydis" for configuration "Release"
set_property(TARGET Zydis::Zydis APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Zydis::Zydis PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Zydis.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Zydis.dll"
  )

list(APPEND _cmake_import_check_targets Zydis::Zydis )
list(APPEND _cmake_import_check_files_for_Zydis::Zydis "${_IMPORT_PREFIX}/lib/Zydis.lib" "${_IMPORT_PREFIX}/bin/Zydis.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
