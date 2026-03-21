
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

function(target_set_program_include_directories TARGET)
    target_include_directories(
        ${TARGET}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
endfunction()


file(GLOB_RECURSE TEST_TOOLS_SOURCES ${PROJECT_SOURCE_DIR}/test/tools/*.cpp)
set(TEST_TOOLS test_tools)
add_library(${TEST_TOOLS} STATIC ${TEST_TOOLS_SOURCES})
target_include_directories(${TEST_TOOLS} PUBLIC ${PROJECT_SOURCE_DIR}/test/tools)

function(add_catch2_test TARGET SOURCE_FILE)

    add_executable(${TARGET} ${SOURCE_FILE} ${PROGRAM_SOURCE_FILES})
    target_include_directories(${TARGET} PRIVATE /test)
    target_set_program_include_directories(${TARGET})

    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/test)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/test)

    target_link_libraries(${TARGET} PUBLIC ${TEST_TOOLS})
    target_link_libraries(${TARGET} PRIVATE Catch2::Catch2WithMain)

    add_test(NAME ${TARGET} COMMAND ${TARGET})
    catch_discover_tests(${TARGET})
endfunction()

add_catch2_test(main_test ${PROJECT_SOURCE_DIR}/test/main.cpp)
