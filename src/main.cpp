#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void updateWindow(GLFWwindow* window, double currentTime)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;
    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

    // Initialize GLFW
    if(!glfwInit())
    {
        std::cerr << "Could not initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    const int windowHeight = 600;
    const int windowWidth = 600;
    const std::string windowTitle = "Simple Graphics Engine";

    GLFWwindow* window = glfwCreateWindow(windowHeight, windowWidth, windowTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Could not initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    glfwSwapInterval(1);


    while(!glfwWindowShouldClose(window))
    {
        updateWindow(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Destroy resources
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}