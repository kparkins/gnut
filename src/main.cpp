
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "logger.h"
#include "console.h"
#include "mesh_loader.h"
#include "shader_program.h"

using std::cout;
using std::endl;
using std::cerr;
using namespace gnut;


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (double) width / ((double) height), 1.f, 1000.f);
}


static void errorCallback(int error, const char* errorMessage) {
}

GLfloat triangle[] = {0.f, .5f, 0.f, -.5f, -.5f, 0.f, .5f, -.5f, 0.f};
const GLchar* vertex_shader = "#version 330 core\n"
                            "layout (location = 0) in vec3 vp;"
                            "void main() {"
                            "    gl_Position = vec4(vp,1.0);"
                            "}";
const GLchar* frag_shader = "#version 330 core\n"
                          "out vec4 frag_color;"
                          "void main() {"
                          "   frag_color = vec4(.5, 0, .5, 1.0);"
                          "}";

int main(int argc, char* argv[]) {
    log::plog console = std::make_shared<log::console>();
    ::logger->log_level(log::level::warning);
    ::logger->add(console);

    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // create a newer opengl context than 2.1 (default on osx)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* main_window = glfwCreateWindow(640, 480, "gnut", NULL, NULL);

    if(!main_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // set the opengl context
    glfwMakeContextCurrent(main_window);

    // enable GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        glfwDestroyWindow(main_window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // key callback executed for each key event when we call poll events
    glfwSetKeyCallback(main_window, keyCallback);
    glfwSetWindowSizeCallback(main_window, window_size_callback);

    glViewport(0, 0, 640, 480);

    gfx::shader_program shader_program;
    shader_program.attach(GL_VERTEX_SHADER, vertex_shader);
    shader_program.attach(GL_FRAGMENT_SHADER, frag_shader);
    shader_program.link_program();

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, &triangle[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    gfx::pmesh cone = gfx::mesh_loader::load("res/models/cone.off");

    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.enable();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        shader_program.disable();

        glfwSwapBuffers(main_window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



