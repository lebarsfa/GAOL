if(SIMD_COND)
    set(SIMD_FILES interval2f)
else()
    set(SIMD_FILES)
endif()

if(CPPUNIT_COND)
    set(CPPUNIT_LIB -lcppunit)
    set(CHECK_PROGRAMS arithmetic assignment constants constructor float_functions fpu generic_functions input_output
        interval_functions misc non_arithmetic relations reverse_mappings trigonometric intervalf essai ${SIMD_FILES})
else()
    set(CPPUNIT_LIB)
endif()

add_executable(performances performance_test_set.cpp performances.cpp)

foreach(prog ${CHECK_PROGRAMS})
    add_executable(${prog} ${prog}.cpp)
endforeach()

if(IS_CYGWIN)
    target_link_libraries(performances -lgaol -lm ${CPPUNIT_LIB} ${MATHLIB})
else()
    target_link_libraries(performances -lm -lgaol ${CPPUNIT_LIB} -ldl ${MATHLIB})
endif()

set_target_properties(performances PROPERTIES LINK_FLAGS "-L../gaol/.libs")

set(EXTRA_DIST gaolcc.in tests.h)

enable_testing()
foreach(prog ${CHECK_PROGRAMS})
    add_test(NAME ${prog} COMMAND ${prog})
endforeach()

set(PROBABILITIES .05 .1 .1 .75)

add_custom_target(perf ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Running performance tests..."
    COMMAND ${CMAKE_COMMAND} -E echo "Compiler: ${CMAKE_CXX_COMPILER}"
    COMMAND performances add 10 ${PROBABILITIES}
    COMMAND performances sub 10 ${PROBABILITIES}
    COMMAND performances mul 10 ${PROBABILITIES}
    COMMAND performances div 10 ${PROBABILITIES}
    COMMAND performances inv 10 ${PROBABILITIES}
    COMMAND performances pow 10 ${PROBABILITIES}
    COMMAND performances cos 10 ${PROBABILITIES}
    COMMAND performances sin 10 ${PROBABILITIES}
    COMMAND performances tan 10 ${PROBABILITIES}
    COMMAND performances acos 10 ${PROBABILITIES}
    COMMAND performances asin 10 ${PROBABILITIES}
    COMMAND performances atan 10 ${PROBABILITIES}
    COMMAND performances cosh 10 ${PROBABILITIES}
    COMMAND performances sinh 10 ${PROBABILITIES}
    COMMAND performances tanh 10 ${PROBABILITIES}
    COMMAND performances acosh 10 ${PROBABILITIES}
    COMMAND performances asinh 10 ${PROBABILITIES}
    COMMAND performances atanh 10 ${PROBABILITIES}
    COMMAND performances exp 10 ${PROBABILITIES}
    COMMAND performances log 10 ${PROBABILITIES}
)
