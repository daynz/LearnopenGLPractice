#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

int main()
{
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.98f, 0.79f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}