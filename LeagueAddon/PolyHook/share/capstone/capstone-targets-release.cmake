#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "capstone::capstone" for configuration "Release"
set_property(TARGET capstone::capstone APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(capstone::capstone PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/capstone.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/capstone.dll"
  )

list(APPEND _cmake_import_check_targets capstone::capstone )
list(APPEND _cmake_import_check_files_for_capstone::capstone "${_IMPORT_PREFIX}/lib/capstone.lib" "${_IMPORT_PREFIX}/bin/capstone.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
