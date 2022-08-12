#include <iostream>
#include <fstream>
#include <sstream>

#include "graphics/Renderer.h"
#include "graphics/VertexBufferLayout.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Grid.h"


void setWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Have to create VAO if running Core
    
    #ifdef __APPLE__
    std::cout << "Running in an Apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

void printOpenGLShaderVersion(GLFWwindow* window)
{
    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "OpenGL shader version: " << major << "." << minor << std::endl;
}

void printOpenGLVersion()
{
    std::cout << "OpenGL version: " << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
}

void initGlew()
{
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
}

int main()
{
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    setWindowHints();

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(960, 540, "CGoL", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    printOpenGLShaderVersion(window);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);  // Make refresh rate same as monitor

    initGlew();

    printOpenGLVersion();

    GLCall(glEnable(GL_BLEND));                                             // Enable blending
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));   // Setting blending function

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    unsigned int generation = 0;
    int gridSize = 100;
    Grid grid(gridSize, gridSize);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();

        // Render Grid of cells:
        generation = grid.Update();
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    // delete Grid and Cells ???
    
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    GLCall(glfwTerminate());
    return 0;
}