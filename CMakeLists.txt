# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/CMakeLists.txt

message(WARNING "Please copy gaol/gaol_double_op_apmathlib.h to gaol/gaol_double_op.h, and gaol/gaol_config_mingw.h to gaol/gaol_configuration.h and customize it...")

# Works with 3.14 and tested through 3.25
cmake_minimum_required(VERSION 3.14...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
  GAOL
  VERSION 4.2.3
#  DESCRIPTION "An example project with CMake"
  LANGUAGES C CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
#  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
#  include(CTest)

  # Docs only available if this is the main app
#  find_package(Doxygen)
#  if(Doxygen_FOUND)
#    add_subdirectory(docs)
#  else()
#    message(STATUS "Doxygen not found, not building docs")
#  endif()
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

# Accumulator library
# This is header only, so could be replaced with git submodules or FetchContent
#find_package(Boost REQUIRED)
# Adds Boost::boost

FetchContent_Declare(
  mathlib
  URL https://github.com/lebarsfa/mathlib/archive/refs/heads/master.zip
#  URL ${CMAKE_SOURCE_DIR}/../mathlib-master.zip
#  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(mathlib)
# Adds mathlib::ultim

# The compiled library code is here
add_subdirectory(gaol)

# The executable code is here
#add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
#if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#   AND BUILD_TESTING)
#  add_subdirectory(tests)
#endif()

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
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_configuration.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_apmathlib.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_crlibm.h
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
${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_version.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_c99.h
${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_msvc.h
#_double_op.h
#_configuration.h
    DESTINATION include/gaol
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    gaolConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )
	
install(EXPORT gaolTargets
        FILE gaolTargets.cmake
        NAMESPACE gaol::
        DESTINATION lib/cmake/gaol
         )
		 
configure_file(gaolConfig.cmake.in gaolConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gaolConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/gaolConfigVersion.cmake"
        DESTINATION lib/cmake/gaol
        )