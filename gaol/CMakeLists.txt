# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/src/CMakeLists.txt

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/include/modern/*.hpp")
#set(HEADER_LIST "${gaol_SOURCE_DIR}/include/modern/lib.hpp")
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/gaol/*.h" "${gaol_SOURCE_DIR}/gaol/*.hpp")
file(GLOB OTHER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/gaol/*.ypp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(gaol 
gaol_common.cpp
gaol_exact.c
gaol_exceptions.cpp
gaol_expression.cpp
gaol_init_cleanup.cpp
gaol_interval.cpp
gaol_intervalf.cpp
gaol_interval_lexer.cpp
gaol_interval_parser.cpp
gaol_parser.cpp
gaol_port.cpp
gaol_profile.cpp
s_nextafter.c
${HEADER_LIST}
${OTHER_LIST})

# We need this directory, and users of our library will need it too
#target_include_directories(gaol PUBLIC ../include)
target_include_directories(gaol 
	PUBLIC
        $<INSTALL_INTERFACE:include>
	PRIVATE
		.. ${unistd_msvc_SOURCE_DIR}
)

# This depends on (header only) boost
#target_link_libraries(gaol PRIVATE Boost::boost)

target_link_libraries(gaol PUBLIC mathlib::ultim)

# All users of this library will need at least C++11
#target_compile_features(gaol PUBLIC cxx_std_11)

# See https://github.com/friendlyanon/cxx-static-shared-example
include(GenerateExportHeader)
generate_export_header(
    gaol
    BASE_NAME gaol
    EXPORT_FILE_NAME gaol_configuration.h # Not used a.t.m.
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(gaol PUBLIC gaol_STATIC_DEFINE)
else()
  target_compile_definitions(gaol PUBLIC gaol_SHARED_DEFINE) # For compatibility...
endif()

if(MSVC)
  add_definitions(-D_COMPILING__GAOL_PUBLIC__)
  add_definitions(-D_MATHLIB_DLL_=extern\ \"C\")
endif()



# TO CHECK DEFAULTS DEPENDING ON THE COMPILER, ALSO THE .CMAKE...

include(CheckCXXCompilerFlag)

#option(USE_ENABLE_SIMD "Use SIMD instructions to implement arithmetic operations" ON)
option(USE_ENABLE_SIMD "Use SIMD instructions to implement arithmetic operations" OFF)
if(USE_ENABLE_SIMD)
  check_cxx_compiler_flag("--enable-simd" COMPILER_SUPPORTS_ENABLE_SIMD)
  if(COMPILER_SUPPORTS_ENABLE_SIMD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --enable-simd")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --enable-simd")
  endif()
endif()

#option(USE_SSE2 "Use SSE2 instructions" ON)
option(USE_SSE2 "Use SSE2 instructions" OFF)
if(USE_SSE2)
  add_definitions(-DUSING_SSE2_INSTRUCTIONS)
  check_cxx_compiler_flag("/arch:SSE2" COMPILER_SUPPORTS_ARCH_SSE2)
  if(COMPILER_SUPPORTS_ARCH_SSE2)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
  endif()
  check_cxx_compiler_flag("-xP" COMPILER_SUPPORTS_XP)
  check_cxx_compiler_flag("-mtune=pentium4" COMPILER_SUPPORTS_MTUNE_PENTIUM4)
  if((COMPILER_SUPPORTS_XP) AND (COMPILER_SUPPORTS_MTUNE_PENTIUM4))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -xP -mtune=pentium4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xP -mtune=pentium4")
  endif()
  check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_MSSE2)
  if(COMPILER_SUPPORTS_MSSE2)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
  endif()
endif()

#option(USE_SSE3 "Use SSE3 instructions" ON)
option(USE_SSE3 "Use SSE3 instructions" OFF)
if(USE_SSE3)
  add_definitions(-DUSING_SSE3_INSTRUCTIONS)
  check_cxx_compiler_flag("-msse3" COMPILER_SUPPORTS_MSSE3)
  if(COMPILER_SUPPORTS_MSSE3)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
  endif()
endif()

#option(USE_GAOL_VERBOSE_MODE "Use verbose mode" ON)
option(USE_GAOL_VERBOSE_MODE "Use verbose mode" OFF)
if(USE_GAOL_VERBOSE_MODE)
  add_definitions(-DGAOL_VERBOSE_MODE)
endif()

option(USE_GAOL_DEBUGGING "Use debugging facilities" OFF)
if(USE_GAOL_DEBUGGING)
  add_definitions(-DGAOL_DEBUGGING)
endif()

#option(USE_GAOL_PRESERVE_ROUNDING "Transparently manipulates the FPU control flags" ON)
option(USE_GAOL_PRESERVE_ROUNDING "Transparently manipulates the FPU control flags" OFF)
if(USE_GAOL_PRESERVE_ROUNDING)
  add_definitions(-DGAOL_PRESERVE_ROUNDING)
endif()

#option(USE_GAOL_EXCEPTIONS_ENABLED "Raise exceptions to signal errors" ON)
option(USE_GAOL_EXCEPTIONS_ENABLED "Raise exceptions to signal errors" OFF)
if(USE_GAOL_EXCEPTIONS_ENABLED)
  add_definitions(-DGAOL_EXCEPTIONS_ENABLED)
endif()



# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
