cmake_minimum_required(VERSION 3.16)
project(DHCP LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(UseClangTidy)
include(UseSanitizer)
include(UseCodeCoverage)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

option(BUILD_TESTS "Build tests" ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)

find_package(Boost COMPONENTS json REQUIRED)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SOURCES)

add_subdirectory(sources)

add_executable(DHCP ${SOURCES})
target_include_directories(DHCP
        PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(DHCP
        PRIVATE
    Boost::json
    Threads::Threads
    Configuration
    DhcpPacket
    Statistics
    HttpControllers
)

install(
    TARGETS
        DHCP
    DESTINATION
        /opt/service/
)

add_subdirectory(tests)

include(cmake/CMakeCPack.cmake)
include(CPack)