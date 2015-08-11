#
# loop perf
#

# cmake settings
cmake_minimum_required(VERSION 2.6)

# project settings
project("loop_perf" C CXX)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")

# compiler & linker flags
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
endif()

add_executable(loop_perf loop_perf.cpp)
