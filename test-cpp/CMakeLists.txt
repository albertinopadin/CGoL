cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(test-cpp)

# add_dependencies(${PROJECT_NAME} dependencies)
# add_library(glad dependencies/include/glad/glad.h)

find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)

# set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
# set(LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})

# Check /opt/homebrew/Cellar/glfw/ for correct version
set(GLFW_VERSION 3.3.8)
set(GLFW_LIBRARIES_GLOB_PATH /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib/*.dylib)

# file(GLOB GLFW_LIBRARIES "/opt/homebrew/Cellar/glfw/3.3.8/lib/*.dylib")
file(GLOB GLFW_LIBRARIES ${GLFW_LIBRARIES_GLOB_PATH})
file(GLOB GLEW_LIBRARIES "/opt/homebrew/Cellar/glew/2.2.0_1/lib/*.dylib")
set(LIBRARIES ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})

message("LIBRARIES = ${LIBRARIES}")

include_directories(${INCLUDE_DIRS})

set(TEST_CPP_DIR .)
message("TEST_CPP_DIR = ${TEST_CPP_DIR}")
set(TEST_CPP_INCLUDE_DIR ${TEST_CPP_DIR}/dependencies/include)
set(TEST_CPP_LIB_DIR ${TEST_CPP_DIR}/dependencies/library)

set(GLFW_INCLUDE_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/include)
set(GLFW_LIB_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib)

set(GLEW_INCLUDE_DIR /opt/homebrew/Cellar/glew/2.2.0_1/include)
set(GLEW_LIB_DIR /opt/homebrew/Cellar/glew/2.2.0_1/lib)

# set(GLM_INCLUDE_DIR /opt/homebrew/Cellar/glm/0.9.9.8/include)
# set(GLM_LIB_DIR /opt/homebrew/Cellar/glm/0.9.9.8/lib)

# set(HEADER_FILES ${TEST_CPP_INCLUDE_DIR}/glad/glad.h ${TEST_CPP_INCLUDE_DIR}/GLFW/glfw3.h)
file(GLOB PROJECT_HEADER_FILES "src/*.h")
set(HEADER_FILES ${PROJECT_HEADER_FILES} ${TEST_CPP_INCLUDE_DIR}/GLFW/glfw3.h)

# add_library("test-cpp-lib" ${HEADER_FILES})

# target_include_directories(${PROJECT_NAME} PRIVATE ${TEST_CPP_DIR})

# add_subdirectory(dependencies)

# file(GLOB GLFW_LIBRARIES "/opt/homebrew/Cellar/glfw/3.3.7/lib/*.dylib")
# message("GLFW_LIBRARIES = ${GLFW_LIBRARIES}")

# file(GLOB GLEW_LIBRARIES "/opt/homebrew/Cellar/glew/2.2.0_1/lib/*.dylib")
# message("GLEW_LIBRARIES = ${GLEW_LIBRARIES}")

# file(GLOB GLM_LIBRARIES "/opt/homebrew/Cellar/glm/0.9.9.8/lib/*.dylib")
# message("GLM_LIBRARIES = ${GLM_LIBRARIES}")

# set(SOURCES glad.c cherno.cpp ${HEADER_FILES} ${TEST_CPP_LIB_DIR}/libglfw.3.3.dylib ${GLEW_LIB_DIR}/libGLEW.2.2.0.dylib)
file(GLOB PROJECT_SOURCES "src/*.cpp")
file(GLOB PROJECT_TEST_SOURCES "src/tests/*.cpp")
file(GLOB VENDOR_SOURCES "src/vendor/stb_image/*.cpp" "src/vendor/glm/*.cpp" "src/vendor/imgui/*.cpp")
set(SOURCES ${PROJECT_SOURCES} ${PROJECT_TEST_SOURCES} ${VENDOR_SOURCES} ${HEADER_FILES})

set(PROJECT_SRC_DIR ${TEST_CPP_DIR}/src)
set(PROJECT_TESTS_DIR ${TEST_CPP_DIR}/src/tests)
set(VENDOR_INCLUDE_DIR ${TEST_CPP_DIR}/src/vendor)

# add_executable(${PROJECT_NAME} glad.c test.cpp)
# add_executable(${PROJECT_NAME} glad.c test1.cpp ${HEADER_FILES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${TEST_CPP_DIR} 
    ${PROJECT_SRC_DIR}
    ${PROJECT_TESTS_DIR}
    # ${TEST_CPP_INCLUDE_DIR} 
    # ${TEST_CPP_LIB_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLFW_LIB_DIR}
    ${GLEW_INCLUDE_DIR} 
    ${GLEW_LIB_DIR}
    # ${GLM_INCLUDE_DIR} 
    # ${GLM_LIB_DIR}
    ${VENDOR_INCLUDE_DIR}
)

# add_library(${PROJECT_NAME} ${TEST_CPP_LIB_DIR}/libglfw.3.3.dylib)

# target_link_libraries(${PROJECT_NAME} GL dl glfw)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${GLM_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PRIVATE glm)
# target_link_libraries(${PROJECT_NAME} GLEW)
# target_link_libraries(${PROJECT_NAME} find_package(GLEW REQUIRED) glfw)