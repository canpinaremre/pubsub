cmake_minimum_required(VERSION 3.0.0)
project(pubsub VERSION 0.3.0)

set(CMAKE_CXX_FLAGS -pthread)

include_directories(src)
include_directories(topics)

add_subdirectory(src)
add_subdirectory(topics)





add_executable(pubsub src/test.cpp)

target_link_libraries(${PROJECT_NAME} src)
target_link_libraries(${PROJECT_NAME} topics)
