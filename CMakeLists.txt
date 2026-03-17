cmake_minimum_required(VERSION 3.14)


project(MathGraph LANGUAGES CXX)

set(PROGRAM_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE PROGRAM_HEADER_FILES "include/*.hpp" "include/*.h")
file(GLOB_RECURSE PROGRAM_SOURCE_FILES "src/*.cpp")

add_executable(${PROGRAM_NAME} ${PROGRAM_SOURCE_FILES})

target_include_directories(${PROGRAM_NAME} PRIVATE include)

