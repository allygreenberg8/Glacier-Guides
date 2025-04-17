cmake_minimum_required(VERSION 3.10)

# Make sure CMAKE uses the vcpkg string
set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_SOURCE_DIR}/resources/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "vcpkg toolchain file"
)

# Declare project
project(GlacierGuides)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find sfml
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Include directories
include_directories("${CMAKE_SOURCE_DIR}")

# Get all your .cpp files at the project root
#file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")

# Build and link the files
#add_executable(Glacier_Guides ${SOURCES})

add_executable(Glacier_Guides
    "${CMAKE_SOURCE_DIR}/resources/main.cpp"
    "${CMAKE_SOURCE_DIR}/resources/Graph.cpp"
    "${CMAKE_SOURCE_DIR}/resources/TerrainParser.cpp"
    "${CMAKE_SOURCE_DIR}/resources/TerrainRenderer.cpp"
    "${CMAKE_SOURCE_DIR}/resources/pugixml.cpp"
)

set_target_properties(Glacier_Guides PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)


target_link_libraries(Glacier_Guides
    PRIVATE sfml-graphics sfml-window sfml-system
)
