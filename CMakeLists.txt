cmake_minimum_required(VERSION 3.14)

project(MathGraph LANGUAGES CXX)

set(PROGRAM_NAME ${PROJECT_NAME})

file(GLOB_RECURSE
    PROJECT_HEADER_FILES
    ./include/*.hpp
    ./include/*.h
)

file(GLOB_RECURSE
    PROJECT_SOURCE_FILES
    ./src/*.cpp
)

add_executable(${PROGRAM_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories(${PROGRAM_NAME} PRIVATE include)
target_compile_features(${PROGRAM_NAME} PRIVATE cxx_std_20)
