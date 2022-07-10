#include <glad/glad.h>
#include <GLFW/glfw3.h>


// From: https://www.youtube.com/watch?v=7I0eNh2XdvA

int main()
{
    glfwInit();
    GLFWwindow* w = glfwCreateWindow(600, 600, "Test Triangle", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    float t[] = {
        -.5f, -.5f, 0.f,
        0.f, .5f, 0.f,
        .5f, -.5f, 0.f
    };

    uint32_t v;  // VAO - Vertex Array Object
    glGenBuffers(1, &v);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_STATIC_DRAW);
    
    // Set up vertex:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);
    glViewport(0, 0, 600, 600);

    while (!glfwWindowShouldClose(w))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(w);
        glfwPollEvents();
    }
}