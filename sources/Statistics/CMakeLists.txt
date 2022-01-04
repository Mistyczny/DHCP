aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} STATISTICS_SOURCES)

add_library(Statistics STATIC ${STATISTICS_SOURCES})
target_include_directories(Statistics
        PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(Statistics
        PUBLIC
    DhcpPacket
)