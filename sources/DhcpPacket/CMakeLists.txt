add_library(DhcpPacket INTERFACE)
target_include_directories(DhcpPacket
        INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)