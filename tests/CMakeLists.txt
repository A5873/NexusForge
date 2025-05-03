# Tests CMakeLists.txt for NexusForge

# Require modern CMake
cmake_minimum_required(VERSION 3.15)

# Add a custom target for all tests
add_custom_target(build_tests)

# Set compiler flags for tests
set(TEST_COMPILER_FLAGS
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
    -pedantic
)

# Common include directories for all tests
set(TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Function to add a test
function(add_nexusforge_test TEST_NAME TEST_SOURCES)
    # Create the test executable
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    
    # Add test executable to the build_tests target
    add_dependencies(build_tests ${TEST_NAME})
    
    # Set include directories
    target_include_directories(${TEST_NAME} PRIVATE ${TEST_INCLUDE_DIRS})
    
    # Set compiler flags
    target_compile_options(${TEST_NAME} PRIVATE ${TEST_COMPILER_FLAGS})
    
    # Link with required libraries (e.g., nexusforge_core)
    # target_link_libraries(${TEST_NAME} PRIVATE nexusforge_core)
    
    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # Set test properties
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30  # 30 second timeout
        ENVIRONMENT "NEXUSFORGE_TEST=1"
    )
endfunction()

# Add subdirectories for test categories
add_subdirectory(ecs)

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

