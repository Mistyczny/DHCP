enable_testing()

add_custom_target(RunAllTests
    COMMAND
        "${CMAKE_COMMAND}" --build . --target all
    COMMAND
        "${CMAKE_COMMAND}" --build . --target test
    COMMENT
        "Running tests"
    WORKING_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
)