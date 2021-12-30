option(MEASURE_CODE_COVERAGE "If ON code coverage can be measured" OFF)

if ($CACHE{MEASURE_CODE_COVERAGE})
    if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
        message("-- Adding measuring code coverage for ${CMAKE_CXX_COMPILER_ID} compiler")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
        set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES Clang OR CMAKE_CXX_COMPILER_ID MATCHES AppleClang)
        message("-- Adding measuring code coverage for ${CMAKE_CXX_COMPILER_ID} compiler")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping")
    endif ()
else ()
    message("-- Skipping code coverage")
endif ()