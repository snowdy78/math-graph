
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

# use it after including CTest
enable_testing()
include(CTest)
include(Catch)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

function(include_program_headers TARGET)
    target_include_directories(
        ${TARGET}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
endfunction()

function(target_link_catch2 TARGET)
    target_link_libraries(${TARGET} PRIVATE Catch2::Catch2WithMain)
endfunction()


function(add_catch2_test TARGET)

    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/test)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/test)

    include_program_headers(${TARGET})
    target_link_catch2(${TARGET})

    add_test(NAME project_tests COMMAND ${TARGET})
    catch_discover_tests(${TARGET})
endfunction()

add_executable(main_test ${PROJECT_SOURCE_DIR}/test/main.cpp)
add_executable(expression ${PROJECT_SOURCE_DIR}/test/expression.cpp)
add_catch2_test(main_test)
