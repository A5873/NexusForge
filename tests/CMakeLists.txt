# Tests root CMakeLists.txt

# Find GoogleTest or Catch2 (disabled for now, enabling later when we have actual tests)
# find_package(GTest REQUIRED)
# include(GoogleTest)

# Create test executable
# add_executable(unit_tests
#     unit/core/engine_test.cpp
#     unit/core/window_test.cpp
# )

# Link test executable with core library and test framework
# target_link_libraries(unit_tests
#     PRIVATE
#     NexusForge::Core
#     GTest::GTest
#     GTest::Main
# )

# Discover and register tests
# gtest_discover_tests(unit_tests)

# Set output directory
# set_target_properties(unit_tests PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
# )

message(STATUS "Testing infrastructure is configured but no tests exist yet.")

# Placeholder for when we have actual tests
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "No tests implemented yet."
    COMMENT "Running tests (placeholder)"
)

