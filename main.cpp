#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;
using std::cerr;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    cout << key << endl;
}

static void errorCallback(int error, const char* errorMessage) {
    cout << error << " -- " << errorMessage << endl;
}

int main(int argc, char* argv[]) {
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        cerr << "Unable to initialize GLFW." << endl;
        exit(EXIT_FAILURE);
    }

    // create a newer opengl context than 2.1 (default on osx)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(640, 480, "vkc", NULL, NULL);

    if(!mainWindow) {
        cerr << "Error creating main window." << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set the opengl context
    glfwMakeContextCurrent(mainWindow);

    // enable GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        cerr << "Error initializing GLEW." << endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // print out the opengl version so we know if the newer context was made
    cout << glGetString(GL_VERSION) << endl;

    // key callback executed for each key event when we call poll events
    glfwSetKeyCallback(mainWindow, keyCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glViewport(0, 0, 640, 480);

    // main loop
    while(!glfwWindowShouldClose(mainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);

        glEnd();
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}

