# NexusForge Core Library CMakeLists.txt

# Source files
set(NEXUSFORGE_CORE_SOURCES
    engine.cpp
    window.cpp
)

# Header files (for IDEs and installation)
set(NEXUSFORGE_CORE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/engine.hpp
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/window.hpp
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/ecs/component.hpp
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/ecs/entity.hpp
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/ecs/system.hpp
    ${CMAKE_SOURCE_DIR}/include/nexusforge/core/ecs/world.hpp
)

# ECS implementation headers
set(NEXUSFORGE_ECS_COMPONENT_HEADERS
    ecs/components/transform_component.hpp
    ecs/components/velocity_component.hpp
)

set(NEXUSFORGE_ECS_SYSTEM_HEADERS
    ecs/systems/movement_system.hpp
)

# Create the core library
add_library(nexusforge_core ${NEXUSFORGE_CORE_SOURCES} ${NEXUSFORGE_CORE_HEADERS})

# Create an alias for use within the project
add_library(NexusForge::Core ALIAS nexusforge_core)

# Add include directories
target_include_directories(nexusforge_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Set compile features and options
target_compile_features(nexusforge_core PUBLIC cxx_std_17)
target_compile_options(nexusforge_core
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
)

# Add compile definitions
target_compile_definitions(nexusforge_core
    PUBLIC
        $<$<CONFIG:Debug>:NF_DEBUG>
    PRIVATE
        NF_CORE_BUILDING
)

# Set library properties
set_target_properties(nexusforge_core PROPERTIES
    VERSION 0.1.0
    SOVERSION 0
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    EXPORT_NAME Core
)

# Installation rules
install(TARGETS nexusforge_core
    EXPORT NexusForgeTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES ${NEXUSFORGE_CORE_HEADERS}
    DESTINATION include/nexusforge/core
)

# Install ECS component headers
foreach(HEADER ${NEXUSFORGE_ECS_COMPONENT_HEADERS})
    get_filename_component(HEADER_PATH ${HEADER} DIRECTORY)
    install(FILES ${HEADER}
        DESTINATION include/nexusforge/core/${HEADER_PATH}
    )
endforeach()

# Install ECS system headers
foreach(HEADER ${NEXUSFORGE_ECS_SYSTEM_HEADERS})
    get_filename_component(HEADER_PATH ${HEADER} DIRECTORY)
    install(FILES ${HEADER}
        DESTINATION include/nexusforge/core/${HEADER_PATH}
    )
endforeach()

# Export the targets
install(EXPORT NexusForgeTargets
    FILE NexusForgeTargets.cmake
    NAMESPACE NexusForge::
    DESTINATION lib/cmake/NexusForge
)

