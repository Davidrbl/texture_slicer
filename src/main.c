#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "gl.h"
#include "glfw3.h"
#include "shader.h"

void panic(uint32_t err, char* error)
{
    printf("%s", error);
    exit(err);
}

int main(int32_t argc, char* argv[])
{
    if (!glfwInit()) panic(1, "GLFW init fail!\n");

    uint32_t window_width = 1024;
    uint32_t window_height = 1024;

    GLFWwindow* window = glfwCreateWindow(
        window_width, 
        window_height, 
        "Window title", 
        NULL, 
        NULL
    );

    if (!window) panic(1, "Window create fail!\n");

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) panic(1, "GL load fail\n");

    uint32_t main_program;
    create_program(
        "src/shaders/main.vert",
        NULL,
        NULL,
        NULL,
        "src/shaders/main.frag",
        &main_program
    );

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}