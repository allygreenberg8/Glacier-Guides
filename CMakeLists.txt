cmake_minimum_required(VERSION 3.10)

# Declare project
project(GlacierGuides)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOMEBREW_PATH "/opt/homebrew")
link_directories("${HOMEBREW_PATH}/lib")

# Find SFML installed by Homebrew
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Include directories
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("/opt/homebrew/include") # Add Homebrew include path

add_executable(Glacier_Guides
    "${CMAKE_SOURCE_DIR}/resources/main.cpp"
    "${CMAKE_SOURCE_DIR}/resources/Graph.cpp"
    "${CMAKE_SOURCE_DIR}/resources/TerrainParser.cpp"
    "${CMAKE_SOURCE_DIR}/resources/TerrainRenderer.cpp"
    "${CMAKE_SOURCE_DIR}/resources/pugixml.cpp"
)

target_link_libraries(Glacier_Guides
    PRIVATE sfml-graphics sfml-window sfml-system
)