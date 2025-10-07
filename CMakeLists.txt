cmake_minimum_required(VERSION 3.22)
project(logger)

set(CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/logger_include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_DIR)
aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_DIR)

find_package(GTest REQUIRED)
find_package(fmt REQUIRED) # Optimization


# Find all test cpp files
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/test/*.cpp")

add_library(async_logger STATIC ${SRC_DIR})

enable_testing()

foreach(test_file ${TEST_FILES})
    # Get the filename without path and extension
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name}
        async_logger
        GTest::gtest_main
        fmt::fmt)
    include(GoogleTest)
    gtest_discover_tests(${test_name})
endforeach()
