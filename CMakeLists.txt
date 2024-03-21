cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(CGoL VERSION 0.1.0)

find_package(OpenGL REQUIRED)
set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})

message("CMAKE SYSTEM NAME = ${CMAKE_SYSTEM_NAME}")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Check /opt/homebrew/Cellar/glfw/ for correct version on macos
    set(GLFW_VERSION 3.4)
    set(GLEW_VERSION 2.2.0_1)
    set(GLFW_LIBRARIES_GLOB_PATH /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib/*.dylib)
    file(GLOB GLFW_LIBRARIES ${GLFW_LIBRARIES_GLOB_PATH})
    file(GLOB GLEW_LIBRARIES "/opt/homebrew/Cellar/glew/${GLEW_VERSION}/lib/*.dylib")

    set(GLFW_INCLUDE_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/include)
    set(GLFW_LIB_DIR /opt/homebrew/Cellar/glfw/${GLFW_VERSION}/lib)

    set(GLEW_INCLUDE_DIR /opt/homebrew/Cellar/glew/${GLEW_VERSION}/include)
    set(GLEW_LIB_DIR /opt/homebrew/Cellar/glew/${GLEW_VERSION}/lib)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GLFW_LIBRARIES_GLOB_PATH "/usr/lib/aarch64-linux-gnu/libglfw*")
    file(GLOB GLFW_LIBRARIES ${GLFW_LIBRARIES_GLOB_PATH})
    file(GLOB GLEW_LIBRARIES "/usr/lib/aarch64-linux-gnu/libGLEW*")

    set(GLFW_INCLUDE_DIR "/usr/include/GLFW")
    set(GLFW_LIB_DIR "/usr/lib/aarch64-linux-gnu")

    set(GLEW_INCLUDE_DIR "/usr/include/GL")
    set(GLEW_LIB_DIR "/usr/lib/aarch64-linux-gnu")
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GLFW_VERSION 3.4)
    set(GLEW_VERSION 2.2.0_1)
    set(GLFW_LIBRARIES_GLOB_PATH "UNKNOWN")
    file(GLOB GLFW_LIBRARIES ${GLFW_LIBRARIES_GLOB_PATH})
    file(GLOB GLEW_LIBRARIES "UNKNOWN")

    set(GLFW_INCLUDE_DIR "UNKNOWN")
    set(GLFW_LIB_DIR "UNKNOWN")

    set(GLEW_INCLUDE_DIR "UNKNOWN")
    set(GLEW_LIB_DIR "UNKNOWN")
endif ()

set(LIBRARIES ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})

message("LIBRARIES = ${LIBRARIES}")

include_directories(${INCLUDE_DIRS})

set(CGOL_DIR .)
message("CGOL_DIR = ${CGOL_DIR}")
set(CGOL_INCLUDE_DIR ${CGOL_DIR}/dependencies/include)
set(CGOL_LIB_DIR ${CGOL_DIR}/dependencies/library)

file(GLOB PROJECT_HEADER_FILES "src/*.h")
set(HEADER_FILES ${PROJECT_HEADER_FILES} ${CGOL_INCLUDE_DIR}/GLFW/glfw3.h)

file(GLOB PROJECT_SOURCES "src/*.cpp")
file(GLOB PROJECT_GRAPHICS_SOURCES "src/graphics/*.cpp")
file(GLOB VENDOR_SOURCES "src/vendor/stb_image/*.cpp" "src/vendor/glm/*.cpp" "src/vendor/imgui/*.cpp")
set(SOURCES ${PROJECT_SOURCES} ${PROJECT_GRAPHICS_SOURCES} ${VENDOR_SOURCES} ${HEADER_FILES})

set(PROJECT_SRC_DIR ${CGOL_DIR}/src)
set(VENDOR_INCLUDE_DIR ${CGOL_DIR}/src/vendor)

set(RESOURCE_FILES 
    "resources/shaders/Basic.shader"
    "LICENSE"
    "README.md")

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${RESOURCE_FILES})

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

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_FRAMEWORK_IDENTIFIER org.tino.CGoL
        RESOURCE "${RESOURCE_FILES}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/shaders/* $<TARGET_FILE_DIR:${PROJECT_NAME}>)   
