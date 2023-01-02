# From https://gitlab.com/CLIUtils/modern-cmake/-/blob/b47192867e86e5f00f70ec860638a28473edc8bf/examples/extended-project/src/CMakeLists.txt

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/include/modern/*.hpp")
#set(HEADER_LIST "${gaol_SOURCE_DIR}/include/modern/lib.hpp")
file(GLOB OTHER_SOURCES_AND_HEADERS_LIST CONFIGURE_DEPENDS "${gaol_SOURCE_DIR}/*.h" "${gaol_SOURCE_DIR}/*.hpp" "${gaol_SOURCE_DIR}/*.ypp")

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
${OTHER_SOURCES_AND_HEADERS_LIST})


# We need this directory, and users of our library will need it too
#target_include_directories(gaol PUBLIC ../include)
target_include_directories(gaol 
#	PUBLIC
#        $<INSTALL_INTERFACE:.>
	PRIVATE
		..)

# This depends on (header only) boost
#target_link_libraries(gaol PRIVATE Boost::boost)

# All users of this library will need at least C++11
#target_compile_features(gaol PUBLIC cxx_std_11)

include(CheckCCompilerFlag)
check_c_compiler_flag ("-frounding-math" COMPILER_SUPPORTS_FROUNDING_MATH)
if (COMPILER_SUPPORTS_FROUNDING_MATH)
  target_compile_options(gaol PUBLIC "-frounding-math")
endif ()
check_c_compiler_flag ("-ffloat-store" COMPILER_SUPPORTS_FFLOAT_STORE)
if (COMPILER_SUPPORTS_FFLOAT_STORE)
  target_compile_options(gaol PUBLIC "-ffloat-store")
endif ()
check_c_compiler_flag ("-fno-fast-math" COMPILER_SUPPORTS_FNO_FAST_MATH)
if (COMPILER_SUPPORTS_FNO_FAST_MATH)
  target_compile_options(gaol PUBLIC "-fno-fast-math")
endif ()
check_c_compiler_flag ("-ffp-contract=off" COMPILER_SUPPORTS_FFP_CONTRACT_OFF)  
if (COMPILER_SUPPORTS_FFP_CONTRACT_OFF)
  target_compile_options(gaol PUBLIC "-ffp-contract=off")
endif ()
check_c_compiler_flag ("-ffp-mode=full" COMPILER_SUPPORTS_FFP_MODE_FULL)  
if (COMPILER_SUPPORTS_FFP_MODE_FULL)
  target_compile_options(gaol PUBLIC "-ffp-mode=full")
endif ()
check_c_compiler_flag ("-fp-model=strict" COMPILER_SUPPORTS_FP_MODEL_STRICT)  
if (COMPILER_SUPPORTS_FP_MODEL_STRICT)
  target_compile_options(gaol PUBLIC "-fp-model=strict")
endif ()
check_c_compiler_flag ("-fp:strict" COMPILER_SUPPORTS_FP_STRICT)  
if (COMPILER_SUPPORTS_FP_STRICT)
  target_compile_options(gaol PUBLIC "-fp:strict")
endif ()
check_c_compiler_flag ("-mpc64" COMPILER_SUPPORTS_MPC64)  
if (COMPILER_SUPPORTS_MPC64)
  target_compile_options(gaol PUBLIC "-mpc64")
endif ()

# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
