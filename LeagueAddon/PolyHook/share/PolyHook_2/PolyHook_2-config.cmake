
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was PolyHook_2-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(POLYHOOK_BUILD_SHARED_LIBS )
set(POLYHOOK_BUILD_DLL )
set(POLYHOOK_BUILD_STATIC_RUNTIME OFF)

set(POLYHOOK_DISASM_CAPSTONE ON)
set(POLYHOOK_DISASM_CAPSTONE_FULL OFF)
set(POLYHOOK_DISASM_ZYDIS ON)

set(POLYHOOK_FEATURE_EXCEPTION ON)
set(POLYHOOK_FEATURE_DETOURS ON)
set(POLYHOOK_FEATURE_INLINENTD ON)
set(POLYHOOK_FEATURE_PE ON)
set(POLYHOOK_FEATURE_VIRTUALS ON)

get_filename_component(POLYHOOK_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include("${POLYHOOK_CMAKE_DIR}/PolyHook_2-targets.cmake")
