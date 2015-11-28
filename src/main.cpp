
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "logger.h"
#include "console.h"

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 640, 480);
    gluPerspective(60.0, 640.0 / 480.0, 1.0, 1000.0);

    GLchar error_log[512];
    GLint success;

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertex_shader, NULL);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, 0, error_log);
    }

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_shader, NULL);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, 0, error_log);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

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


    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        LOGE(logger, "test " << " tes2 " << 1 << 2 << " :) ");

        glPopMatrix();
        glfwSwapBuffers(main_window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



