cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(CGoL VERSION 0.1.0)

find_package(OpenGL REQUIRED)
set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})

# Check /opt/homebrew/Cellar/glfw/ for correct version
set(GLFW_VERSION 3.3.8)
set(GLEW_VERSION 2.2.0_1)
set(GLFW_LIBRARIES_GLOB_PATH /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib/*.dylib)

file(GLOB GLFW_LIBRARIES ${GLFW_LIBRARIES_GLOB_PATH})
file(GLOB GLEW_LIBRARIES "/opt/homebrew/Cellar/glew/2.2.0_1/lib/*.dylib")
set(LIBRARIES ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})

message("LIBRARIES = ${LIBRARIES}")

include_directories(${INCLUDE_DIRS})

set(CGOL_DIR .)
message("CGOL_DIR = ${CGOL_DIR}")
set(CGOL_INCLUDE_DIR ${CGOL_DIR}/dependencies/include)
set(CGOL_LIB_DIR ${CGOL_DIR}/dependencies/library)

set(GLFW_INCLUDE_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/include)
set(GLFW_LIB_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib)

set(GLEW_INCLUDE_DIR /opt/homebrew/Cellar/glew/${GLEW_VERSION}/include)
set(GLEW_LIB_DIR /opt/homebrew/Cellar/glew/${GLEW_VERSION}/lib)

file(GLOB PROJECT_HEADER_FILES "src/*.h")
set(HEADER_FILES ${PROJECT_HEADER_FILES} ${CGOL_INCLUDE_DIR}/GLFW/glfw3.h)

file(GLOB PROJECT_SOURCES "src/*.cpp")
file(GLOB PROJECT_GRAPHICS_SOURCES "src/graphics/*.cpp")
file(GLOB VENDOR_SOURCES "src/vendor/stb_image/*.cpp" "src/vendor/glm/*.cpp" "src/vendor/imgui/*.cpp")
set(SOURCES ${PROJECT_SOURCES} ${PROJECT_GRAPHICS_SOURCES} ${VENDOR_SOURCES} ${HEADER_FILES})

set(PROJECT_SRC_DIR ${CGOL_DIR}/src)
set(VENDOR_INCLUDE_DIR ${CGOL_DIR}/src/vendor)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${CGOL_DIR} 
    ${PROJECT_SRC_DIR}
    ${PROJECT_TESTS_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLFW_LIB_DIR}
    ${GLEW_INCLUDE_DIR} 
    ${GLEW_LIB_DIR}
    ${VENDOR_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
