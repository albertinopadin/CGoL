#include <glad/glad.h>
#include <GLFW/glfw3.h>


// From: youtube.com/watch?v=

int main()
{
    glfwInit();
    GLFWwindow* w = glfwCreateWindow(600, 600, "this", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}