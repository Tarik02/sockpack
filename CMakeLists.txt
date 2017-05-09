cmake_minimum_required(VERSION 3.6)
project(sockpack)

set(CMAKE_CXX_STANDARD 11)

# Include threads
find_package(Threads)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)
include_directories(include)
#add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_executable(sockpack ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sockpack ${CMAKE_THREAD_LIBS_INIT})
