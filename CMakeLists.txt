cmake_minimum_required(VERSION 3.14)


project(MathGraph LANGUAGES CXX)

set(PROGRAM_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)

FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.91.0-1
    GIT_SUBMODULES
    "tools/cmake" # Без него CMake не поймет структуру Boost
    "libs/config" # Конфигурация под ваш компилятор (база)
    "libs/core" # Сам модуль core (addressof, ref и т.д.)
    "libs/assert" # Прямая зависимость для libs/core
    "libs/static_assert" # Прямая зависимость для libs/core
    "libs/throw_exception" # Прямая зависимость для libs/core
    "libs/type_traits" # Прямая зависимость для libs/core
    "libs/preprocessor" # Прямая зависимость для libs/config и core
    "libs/regex"
    "libs/predef"
)
fetchcontent_makeavailable(Boost)

file(GLOB_RECURSE PROGRAM_HEADER_FILES "include/*.hpp" "include/*.h")
file(GLOB_RECURSE PROGRAM_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM PROGRAM_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/main.cpp")
add_executable(${PROGRAM_NAME} "src/main.cpp" ${PROGRAM_SOURCE_FILES})

function(target_link_depending_libraries TARGET)
    target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${TARGET} PRIVATE Boost::regex)
endfunction()
target_link_depending_libraries(${PROGRAM_NAME})

add_subdirectory(test)
