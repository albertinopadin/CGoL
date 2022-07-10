cmake_minimum_required(VERSION 3.17)

project(test-cpp)

add_executable(${PROJECT_NAME} glad.c test.cpp)

target_link_libraries(${PROJECT_NAME} GL dl glfw)