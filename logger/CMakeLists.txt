cmake_minimum_required(VERSION 3.22)
project(logger)

set(CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_DIR)
aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_DIR)


add_executable(test ${SRC_DIR} ${TEST_DIR})
