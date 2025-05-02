# Core library
add_library(nexusforge_core ${NF_LIB_TYPE}
    core/engine.cpp
    core/window.cpp
)

target_include_directories(nexusforge_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set properties
set_target_properties(nexusforge_core PROPERTIES
    OUTPUT_NAME nf_core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME Core
)

# If building with 3D support, add the preprocessor definition
if(NF_DIMENSION_3D)
    target_compile_definitions(nexusforge_core PUBLIC NF_DIMENSION_3D)
endif()

# Install targets
install(TARGETS nexusforge_core
    EXPORT NexusForgeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create an alias target for use within the build tree
add_library(NexusForge::Core ALIAS nexusforge_core)

# Future subsystems will be added here as the engine grows
# add_subdirectory(graphics)
# add_subdirectory(physics)
# add_subdirectory(audio)
# add_subdirectory(input)
# add_subdirectory(utils)

