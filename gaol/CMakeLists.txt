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

# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})
