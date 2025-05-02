#!/bin/bash

# NexusForge build script
# This script creates a build directory, generates build files, and compiles the project

# Set default values
BUILD_TYPE="Debug"
BUILD_DIR="build"
BUILD_EXAMPLES=ON
BUILD_TESTS=OFF
BUILD_SHARED=OFF
ENABLE_ASAN=OFF
ENABLE_3D=OFF
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Display help
function show_help {
    echo "NexusForge Build Script"
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -h, --help                 Show this help message"
    echo "  -t, --type <type>          Set build type (Debug, Release, RelWithDebInfo, MinSizeRel)"
    echo "  -d, --dir <directory>      Set build directory (default: build)"
    echo "  -j, --jobs <count>         Set number of parallel jobs (default: auto)"
    echo "  --examples <ON|OFF>        Build examples (default: ON)"
    echo "  --tests <ON|OFF>           Build tests (default: OFF)"
    echo "  --shared <ON|OFF>          Build shared libraries (default: OFF)"
    echo "  --asan <ON|OFF>            Enable address sanitizer (default: OFF)"
    echo "  --3d <ON|OFF>              Enable 3D functionality (default: OFF)"
    echo "  -c, --clean                Clean build directory before building"
    echo "  -g, --gen-only             Only generate build files, don't compile"
    echo "  -r, --run-example          Build and run the hello_world example"
    echo ""
    echo "Example:"
    echo "  $0 --type Release --tests ON -j 8"
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -d|--dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --examples)
            BUILD_EXAMPLES="$2"
            shift 2
            ;;
        --tests)
            BUILD_TESTS="$2"
            shift 2
            ;;
        --shared)
            BUILD_SHARED="$2"
            shift 2
            ;;
        --asan)
            ENABLE_ASAN="$2"
            shift 2
            ;;
        --3d)
            ENABLE_3D="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN_BUILD=1
            shift
            ;;
        -g|--gen-only)
            GEN_ONLY=1
            shift
            ;;
        -r|--run-example)
            RUN_EXAMPLE=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use '$0 --help' for usage information"
            exit 1
            ;;
    esac
done

# Create or clean build directory
if [ -n "$CLEAN_BUILD" ] && [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || { echo "Failed to enter build directory"; exit 1; }

# Configure the project with CMake
echo "Configuring NexusForge with CMake..."
echo "  Build type: $BUILD_TYPE"
echo "  Examples: $BUILD_EXAMPLES"
echo "  Tests: $BUILD_TESTS"
echo "  Shared libraries: $BUILD_SHARED"
echo "  Address sanitizer: $ENABLE_ASAN"
echo "  3D functionality: $ENABLE_3D"

cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DNF_BUILD_EXAMPLES="$BUILD_EXAMPLES" \
    -DNF_BUILD_TESTS="$BUILD_TESTS" \
    -DNF_BUILD_SHARED_LIBS="$BUILD_SHARED" \
    -DNF_ENABLE_ASAN="$ENABLE_ASAN" \
    -DNF_DIMENSION_3D="$ENABLE_3D"

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Exit if only generating
if [ -n "$GEN_ONLY" ]; then
    echo "Build files generated in $BUILD_DIR"
    exit 0
fi

# Build the project
echo "Building NexusForge with $JOBS parallel jobs..."
cmake --build . --config "$BUILD_TYPE" -j "$JOBS"

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build completed successfully!"

# Run the example if requested
if [ -n "$RUN_EXAMPLE" ]; then
    if [ "$BUILD_EXAMPLES" == "ON" ]; then
        echo "Running hello_world example..."
        echo "-------------------------------"
        
        # Determine the path to the example executable based on build type
        EXAMPLE_PATH="bin/examples/hello_world"
        if [ -f "$EXAMPLE_PATH" ]; then
            "./$EXAMPLE_PATH"
        else
            echo "Example executable not found at $EXAMPLE_PATH"
            echo "Build may have placed it in a different location."
            
            # Try to find it
            FOUND_EXAMPLE=$(find . -name "hello_world" -type f -executable)
            if [ -n "$FOUND_EXAMPLE" ]; then
                echo "Found at: $FOUND_EXAMPLE"
                "$FOUND_EXAMPLE"
            else
                echo "Could not find hello
if [ -n "$CLEAN_BUILD" ] && [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || { echo "Failed to enter build directory"; exit 1; }

# Configure the project with CMake
echo "Configuring NexusForge with CMake..."
echo "  Build type: $BUILD_TYPE"
echo "  Examples: $BUILD_EXAMPLES"
echo "  Tests: $BUILD_TESTS"
echo "  Shared libraries: $BUILD_SHARED"
echo "  Address sanitizer: $ENABLE_ASAN"
echo "  3D functionality: $ENABLE_3D"

cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DNF_BUILD_EXAMPLES="$BUILD_EXAMPLES" \
    -DNF_BUILD_TESTS="$BUILD_TESTS" \
    -DNF_BUILD_SHARED_LIBS="$BUILD_SHARED" \
    -DNF_ENABLE_ASAN="$ENABLE_ASAN" \
    -DNF_DIMENSION_3D="$ENABLE_3D"

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Exit if only generating
if [ -n "$GEN_ONLY" ]; then
    echo "Build files generated in $BUILD_DIR"
    exit 0
fi

# Build the project
echo "Building NexusForge with $JOBS parallel jobs..."
cmake --build . --config "$BUILD_TYPE" -j "$JOBS"

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build completed successfully!"

# Run the example if requested
if [ -n "$RUN_EXAMPLE" ]; then
    if [ "$BUILD_EXAMPLES" == "ON" ]; then
        echo "Running hello_world example..."
        echo "-------------------------------"
        
        # Determine the path to the example executable based on build type
        EXAMPLE_PATH="bin/examples/hello_world"
        if [ -f "$EXAMPLE_PATH" ]; then
            "./$EXAMPLE_PATH"
        else
            echo "Example executable not found at $EXAMPLE_PATH"
            echo "Build may have placed it in a different location."
            
            # Try to find it
            FOUND_EXAMPLE=$(find . -name "hello_world" -type f -executable)
            if [ -n "$FOUND_EXAMPLE" ]; then
                echo "Found at: $FOUND_EXAMPLE"
                "$FOUND_EXAMPLE"
            else
                echo "Could not find hello_world executable."
            fi
        fi
    else
        echo "Cannot run example because examples are not being built (--examples OFF)"
    fi
fi

exit 0

