aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} HTTP_CONTROLLERS_SOURCES)

add_library(HttpControllers STATIC ${HTTP_CONTROLLERS_SOURCES})
target_include_directories(HttpControllers
        PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(HttpControllers
        PUBLIC
    Boost::json
    DhcpPacket
    Configuration
    Statistics
)