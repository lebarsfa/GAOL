# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/src/CMakeLists.txt

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/include/modern/*.hpp")
#set(HEADER_LIST "${gaol_SOURCE_DIR}/include/modern/lib.hpp")
set(SOURCE_LIST 
gaol_common.cpp
gaol_exact.c # No function defintions in the file...
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
s_nextafter.c)
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/gaol/*.h" "${gaol_SOURCE_DIR}/gaol/*.hpp")
file(GLOB OTHER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/gaol/*.ypp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(gaol ${SOURCE_LIST} ${HEADER_LIST} ${OTHER_LIST})

# Inspired from https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
add_library(gaol::gaol ALIAS gaol)

# We need this directory, and users of our library will need it too
#target_include_directories(gaol PUBLIC ../include)
# Inspired from https://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/
target_include_directories(gaol 
	PUBLIC
		"$<INSTALL_INTERFACE:include>"
		"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
		"$<BUILD_INTERFACE:${unistd_msvc_SOURCE_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_link_libraries(gaol PUBLIC mathlib::ultim)

# See https://github.com/friendlyanon/cxx-static-shared-example
include(GenerateExportHeader)
generate_export_header(
    gaol
    BASE_NAME gaol
    EXPORT_FILE_NAME gaol_configuration.h # To check...
)
#file(READ ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h gaol_configuration_content) # To check...

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
include(CheckTypeSize)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckCSourceRuns)

option(GAOL_ENABLE_RELATIONS_CERTAINLY "Relation symbols should stand for \"certainly relations\"" ON)
if(GAOL_ENABLE_RELATIONS_CERTAINLY)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this if relation symbols should stand for \"certainly relations\" */
#define GAOL_CERTAINLY_RELATIONS 1
")
endif()

option(GAOL_ENABLE_DEBUG "Compile gaol with debugging info" OFF)
if(GAOL_ENABLE_DEBUG)
  check_cxx_compiler_flag("-ansi" COMPILER_SUPPORTS_ANSI)
  if(COMPILER_SUPPORTS_ANSI)
    target_compile_options(gaol PUBLIC -ansi)
  endif()
  check_cxx_compiler_flag("-Weffc++" COMPILER_SUPPORTS_WEFFCPP)
  if(COMPILER_SUPPORTS_WEFFCPP)
    target_compile_options(gaol PUBLIC -Weffc++)
  endif()
  check_cxx_compiler_flag("-pedantic" COMPILER_SUPPORTS_PEDANTIC)
  if(COMPILER_SUPPORTS_PEDANTIC)
    target_compile_options(gaol PUBLIC -pedantic)
  endif()
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* define this if you want debugging facilities */
#define GAOL_DEBUGGING 1
")
endif()

option(GAOL_ENABLE_EXCEPTIONS "Raise exceptions to signal errors" ON)
if(GAOL_ENABLE_EXCEPTIONS)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 to allow raising exceptions instead of simply aborting */
#define GAOL_EXCEPTIONS_ENABLED 1
")
endif()

option(GAOL_ENABLE_RELATIONS_POSSIBLY "Relation symbols should stand for \"possibly relations\"" OFF)
if(GAOL_ENABLE_RELATIONS_POSSIBLY)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this if relation symbols should stand for \"possibly relations\" */
#define GAOL_POSSIBLY_RELATIONS 1
")
endif()

option(GAOL_ENABLE_PRESERVE_ROUNDING "Transparently manipulates the FPU control flags" ON)
if(GAOL_ENABLE_PRESERVE_ROUNDING)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* define this to 1 if rounding direction should be preserved */
#define GAOL_PRESERVE_ROUNDING 1
")
endif()

option(GAOL_ENABLE_RELATIONS_SET "Relation symbols should stand for \"set relations\"" OFF)
if(GAOL_ENABLE_RELATIONS_SET)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this if relation symbols should stand for \"set relations\" */
#define GAOL_SET_RELATIONS 1
")
endif()

option(USE_GAOL_USING_APMATHLIB "IBM APMathlib library" ON)
if(USE_GAOL_USING_APMATHLIB)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if using the IBM APMathlib library */
#define GAOL_USING_APMATHLIB 1
")
endif()

option(GAOL_ENABLE_ASM "Allow ASM support" ON)
if(GAOL_ENABLE_ASM)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* define this to 1 if you want to allow ASM support */
#define GAOL_USING_ASM 1
")
endif()

option(USE_GAOL_USING_CRLIBM "CRLibm Mathematical library" OFF)
if(USE_GAOL_USING_CRLIBM)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if using the CRLibm Mathematical library */
#define GAOL_USING_CRLIBM 1
")
endif()

option(GAOL_ENABLE_VERBOSE_MODE "Use verbose mode" ON)
if(GAOL_ENABLE_VERBOSE_MODE)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 to enable verbose mode */
#define GAOL_VERBOSE_MODE 1
")
endif()

# Check for sys/resource.h and getrusage function
check_include_files("sys/resource.h" HAVE_SYS_RESOURCE_H)
if(HAVE_SYS_RESOURCE_H)
    check_c_source_runs("
        #include <sys/resource.h>
        int main() {
            struct rusage usage;
            getrusage(RUSAGE_SELF, &usage);
            return 0;
        }" HAVE_GETRUSAGE_IN_HEADER)
endif()

# If sys/resource.h is not found, check for time.h and clock function
if(NOT HAVE_SYS_RESOURCE_H)
    check_include_files("time.h" HAVE_TIME_H)
    if(HAVE_TIME_H)
        check_c_source_runs("
            #include <time.h>
            int main() {
                clock_t time;
                time = clock();
                return 0;
            }" HAVE_CLOCK_IN_HEADER)
    endif()
endif()

if(HAVE_GETRUSAGE_IN_HEADER)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if function getrusage() is declared in sys/resource.h */
#define GETRUSAGE_IN_HEADER 1
")
else()
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if function getrusage() is declared in sys/resource.h */
#define GETRUSAGE_IN_HEADER 0
")
endif()

if(HAVE_CLOCK_IN_HEADER)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if function clock() is declared in time.h */
#define CLOCK_IN_HEADER 1
")
else()
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if function clock() is declared in time.h */
#define CLOCK_IN_HEADER 0
")
endif()

check_include_files("cassert" HAVE_CASSERT_H)
if(HAVE_CASSERT_H)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
#/* Define to 1 if you have the <cassert> header file. */
#define HAVE_CASSERT 1
")
endif()

check_function_exists(finite HAVE_FINITE)
if(HAVE_FINITE)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
#/* Define to 1 if you have the `finite' function. */
#define HAVE_FINITE 1
")
endif()

check_function_exists(getrusage HAVE_GETRUSAGE)
if(HAVE_GETRUSAGE)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
#/* Define to 1 if you have the `getrusage' function. */
#define HAVE_GETRUSAGE 1
")
endif()

option(GAOL_ENABLE_CPP11 "Use C++ 11" ON)
if(GAOL_ENABLE_CPP11)
  target_compile_features(gaol PUBLIC cxx_std_11)
endif()

option(GAOL_ENABLE_OPTIMIZE "Compile gaol with full optimization" ON)
if(GAOL_ENABLE_OPTIMIZE)
  check_cxx_compiler_flag("-funroll-loops" COMPILER_SUPPORTS_FUNROLL_LOOPS)
  if(COMPILER_SUPPORTS_FUNROLL_LOOPS)
    target_compile_options(gaol PUBLIC -funroll-loops)
  endif()
  check_cxx_compiler_flag("-fomit-frame-pointer" COMPILER_SUPPORTS_FOMIT_FRAME_POINTER)
  if(COMPILER_SUPPORTS_FOMIT_FRAME_POINTER)
    target_compile_options(gaol PUBLIC -fomit-frame-pointer)
  endif()
  check_cxx_compiler_flag("-fexpensive-optimizations" COMPILER_SUPPORTS_FEXPENSIVE_OPTIMIZATIONS)
  if(COMPILER_SUPPORTS_FEXPENSIVE_OPTIMIZATIONS)
    target_compile_options(gaol PUBLIC -fexpensive-optimizations)
  endif()
  check_cxx_compiler_flag("-fvisibility-inlines-hidden" COMPILER_SUPPORTS_FVISIBILITY_INLINES_HIDDEN)
  check_cxx_compiler_flag("-fvisibility=hidden" COMPILER_SUPPORTS_FVISIBILITY_HIDDEN)
  if((COMPILER_SUPPORTS_FVISIBILITY_INLINES_HIDDEN) AND (COMPILER_SUPPORTS_FVISIBILITY_HIDDEN))
    target_compile_options(gaol PUBLIC -fvisibility-inlines-hidden -fvisibility=hidden)
    file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
#/* Define this to 1 if the compiler supports the \"-fvisibility\" option */
#define HAVE_VISIBILITY_OPTIONS 1
")
  endif()
endif()

check_type_size("int" SIZEOF_INT)
if(SIZEOF_INT)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT ${SIZEOF_INT}
")
endif()

check_type_size("long" SIZEOF_LONG)
if(SIZEOF_LONG)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG ${SIZEOF_LONG}
")
endif()

check_type_size("long long int" SIZEOF_LONG_LONG_INT)
if(SIZEOF_LONG_LONG_INT)
  file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* The size of `long long int', as computed by sizeof. */
#define SIZEOF_LONG_LONG_INT ${SIZEOF_LONG_LONG_INT}
")
endif()

if(WIN32)
  option(GAOL_ENABLE_SIMD "Use SIMD instructions to implement arithmetic operations" OFF)
else()
  option(GAOL_ENABLE_SIMD "Use SIMD instructions to implement arithmetic operations" ON)
endif()
if(GAOL_ENABLE_SIMD)
  check_cxx_compiler_flag("/arch:SSE2" COMPILER_SUPPORTS_ARCH_SSE2)
  if(COMPILER_SUPPORTS_ARCH_SSE2)
    target_compile_options(gaol PUBLIC /arch:SSE2)
    file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if SSE2 instructions are to be used */
#define USING_SSE2_INSTRUCTIONS 1
")
  endif()
  check_cxx_compiler_flag("-xP" COMPILER_SUPPORTS_XP)
  check_cxx_compiler_flag("-mtune=pentium4" COMPILER_SUPPORTS_MTUNE_PENTIUM4)
  if((COMPILER_SUPPORTS_XP) AND (COMPILER_SUPPORTS_MTUNE_PENTIUM4))
    target_compile_options(gaol PUBLIC -xP -mtune=pentium4)
    file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if SSE2 instructions are to be used */
#define USING_SSE2_INSTRUCTIONS 1
")
  endif()
  check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_MSSE2)
  if(COMPILER_SUPPORTS_MSSE2)
    target_compile_options(gaol PUBLIC -msse2)
    file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if SSE2 instructions are to be used */
#define USING_SSE2_INSTRUCTIONS 1
")
  endif()
  check_cxx_compiler_flag("-msse3" COMPILER_SUPPORTS_MSSE3)
  if(COMPILER_SUPPORTS_MSSE3)
    target_compile_options(gaol PUBLIC -msse3)
    file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "
/* Define this to 1 if SSE3 instructions are to be used */
#define USING_SSE3_INSTRUCTIONS 1
")
  endif()
endif()

file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "

")

#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/gaol_config_mingw.h gaol_config_mingw_content)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/gaol_version_mingw.h gaol_version_mingw_content)
file(APPEND ${PROJECT_BINARY_DIR}/gaol/gaol_configuration.h "${gaol_version_mingw_content}") # To check...
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gaol_double_op_apmathlib.h ${PROJECT_BINARY_DIR}/gaol/gaol_double_op.h COPYONLY) # To check...

# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
source_group(
  TREE "${PROJECT_SOURCE_DIR}/gaol"
  PREFIX "Other Files"
  FILES ${OTHER_LIST})
