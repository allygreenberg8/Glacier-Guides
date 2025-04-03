cmake_minimum_required(VERSION 3.10)
project(GlacierGuides)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "resources/*.cpp")

add_executable(glacier_guides ${SOURCES})
