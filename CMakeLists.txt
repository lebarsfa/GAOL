# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/CMakeLists.txt

# Works with 3.14 and tested through 3.25
cmake_minimum_required(VERSION 3.14...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
  gaol
  VERSION 4.2.3
  DESCRIPTION "Not Just Another Interval Library"
  LANGUAGES C CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  option(BUILD_SHARED_LIBS "Build shared libs" OFF)
  if(NOT BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  endif()

endif()

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

find_package(mathlib QUIET)
if(mathlib_FOUND)
  message(STATUS "Found mathlib version ${mathlib_VERSION} in ${mathlib_DIR}")
else()
  message(STATUS "Attempt to install mathlib automatically...")
  FetchContent_Declare(
    mathlib
    URL https://github.com/lebarsfa/mathlib/archive/refs/heads/master.zip
  #  URL ${CMAKE_SOURCE_DIR}/../mathlib-master.zip
  #  FIND_PACKAGE_ARGS
  )
  FetchContent_MakeAvailable(mathlib)
  # Adds mathlib::ultim
endif()

if(MSVC)
  FetchContent_Declare(
    unistd_msvc
#    URL https://github.com/ENSTABretagneRobotics/OSUtils/raw/master/To%20add%20or%20modify%20if%20needed/VS/unistd.h
    URL "${CMAKE_CURRENT_SOURCE_DIR}/3rd/unistd.h"
	DOWNLOAD_NO_EXTRACT true
#	INSTALL_DIR
  )
  FetchContent_MakeAvailable(unistd_msvc)
endif()

# The compiled library code is here
add_subdirectory(gaol)

# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/chapters/install/installing.md
# and https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
install(TARGETS gaol
        EXPORT gaolTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin
#        INCLUDES DESTINATION include
        )

install(FILES 
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_allocator.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_assert.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_common.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_config.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_config_mingw.h # To check...
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_config_msvc.h # To check...
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_apmathlib.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_crlibm.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_m.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_eval_stack.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_exact.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_exceptions.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expression.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expr_eval.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expr_visitor.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_flags.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_fpu.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_fpu_fenv.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_init_cleanup.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval2f.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_intervalf.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_fpu.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_parser.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_sse.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_limits.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_parameters.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_parser.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_port.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_profile.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_version.h # To check...
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_version_mingw.h # To check...
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_version_msvc.h # To check...
${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_c99.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_msvc.h
${PROJECT_BINARY_DIR}/gaol/gaol_double_op.h # To check...
${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h # To check...
    DESTINATION include/gaol
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    gaolConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )
  
# Inspired from https://github.com/fmtlib/fmt/blob/7d4662f7ab39f70107b6e8899bcc4076fe947a39/CMakeLists.txt
export(TARGETS gaol NAMESPACE gaol::
        FILE gaolTargets.cmake)

install(EXPORT gaolTargets
        #FILE gaolTargets.cmake
        NAMESPACE gaol::
        DESTINATION share/cmake/gaol
         )

configure_file(gaolConfig.cmake.in gaolConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gaolConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/gaolConfigVersion.cmake"
        DESTINATION share/cmake/gaol
        )