cmake_minimum_required(VERSION 3.17)

project(test-cpp)

# add_dependencies(${PROJECT_NAME} dependencies)
# add_library(glad dependencies/include/glad/glad.h)

add_executable(${PROJECT_NAME} glad.c test.cpp)

target_link_libraries(${PROJECT_NAME} GL dl glfw)