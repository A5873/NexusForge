# Hello World example
add_executable(hello_world hello_world.cpp)

target_link_libraries(hello_world PRIVATE NexusForge::Core)

# Set output directory
set_target_properties(hello_world PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

# Install example
install(TARGETS hello_world
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
)

