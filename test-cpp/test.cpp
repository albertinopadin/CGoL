#include <glad/glad.h>
#include <GLFW/glfw3.h>


// From: https://www.youtube.com/watch?v=7I0eNh2XdvA

int main()
{
    glfwInit();
    GLFWwindow* w = glfwCreateWindow(600, 600, "this", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}