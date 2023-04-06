cmake_minimum_required(VERSION 3.0.0)
project(MIPS-simulator VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

aux_source_directory(. SRC_LIST)
add_executable(MIPS-simulator ${SRC_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
include(CPack)