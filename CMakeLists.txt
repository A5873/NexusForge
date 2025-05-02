cmake_minimum_required(VERSION 3.15)

# Project definition
project(
    NexusForge
    VERSION 0.1.0
    DESCRIPTION "A scalable game development framework that grows from 2D to 3D"
    HOMEPAGE_URL "https://github.com/nexusforge/nexusforge"
    LANGUAGES CXX
)

# C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(NF_BUILD_SHARED_LIBS "Build NexusForge as shared libraries" OFF)
option(NF_BUILD_EXAMPLES "Build example applications" ON)
option(NF_BUILD_TESTS "Build test applications" OFF)
option(NF_ENABLE_ASAN "Enable Address Sanitizer for debug builds" OFF)
option(NF_DIMENSION_3D "Enable 3D functionality (experimental)" OFF)

# Global compiler settings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(NF_ENABLE_ASAN AND CMAKE_BUILD_TYPE MATCHES "Debug")
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Determine library type
if(NF_BUILD_SHARED_LIBS)
    set(NF_LIB_TYPE SHARED)
    add_compile_definitions(NF_SHARED_LIBRARY)
else()
    set(NF_LIB_TYPE STATIC)
endif()

# Set include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add subdirectories for different components
add_subdirectory(src)

# Examples
if(NF_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Testing
if(NF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install configuration
include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets
install(EXPORT NexusForgeTargets
    FILE NexusForgeTargets.cmake
    NAMESPACE NexusForge::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NexusForge
)

# Create and install package configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/NexusForgeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/NexusForgeConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NexusForge
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/NexusForgeConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NexusForgeConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NexusForgeConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NexusForge
)

# Display configuration summary
message(STATUS "NexusForge ${PROJECT_VERSION} Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build shared libraries: ${NF_BUILD_SHARED_LIBS}")
message(STATUS "  Build examples: ${NF_BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${NF_BUILD_TESTS}")
message(STATUS "  Enable Address Sanitizer: ${NF_ENABLE_ASAN}")
message(STATUS "  Enable 3D functionality: ${NF_DIMENSION_3D}")

