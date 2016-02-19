/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

#include "logger.h"
#include "console.h"
#include "mesh_loader.h"
#include "shader_program.h"

using std::cout;
using std::endl;
using std::cerr;
using namespace glm;
using namespace gnut;

static bool left_click;
static vec3 prev_pos;
static int window_height;
static int window_width;
static glm::mat4 projection;
static glm::mat4 view;
static glm::mat4 model;
static gnut::gfx::shader_program main_shader;

vec3 convert_coords(int x, int y) {
    vec3 v;
    v.x = (2.f * x - window_width) / window_width;
    v.y = (window_height - 2.f * y) / window_height;
    v.z = 0.f;
    float distance = glm::length(v);
    if(distance > 1.f) {
        distance = 1.f;
    }
    v.z = sqrtf(1.f - (distance * distance));
    return glm::normalize(v);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

static void scroll_callback(GLFWwindow* window, double x, double y) {
    model = glm::scale(model, glm::vec3(1 + y / 10,1 + y / 10, 1 + y / 10));
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) {
            double x, y;
            left_click = true;
            glfwGetCursorPos(window, &x, &y);
            prev_pos = convert_coords(x, y);
        } else if (action == GLFW_RELEASE) {
            left_click = false;
        }
    }
}

static void cursor_callback(GLFWwindow* window, double x, double y) {
    if(left_click) {
        vec3 position = convert_coords(x, y);
        float magnitude = glm::length(position - prev_pos);
        if(magnitude > .001f) {
            mat4 rotation;
            float angle = glm::acos(glm::dot(position, prev_pos) / (glm::length(position) * glm::length(prev_pos)));
            vec3 rotation_axis = glm::cross(prev_pos, position);
            model = glm::rotate(rotation, angle, rotation_axis) * model;
        }
        prev_pos = position;
    }
}


static void window_size_callback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    projection = glm::perspective(45.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.f);

    GLint p = glGetUniformLocation(main_shader.id(), "projection");
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));
}


static void errorCallback(int error, const char* errorMessage) {
}

int main(int argc, char* argv[]) {
    window_width = 640;
    window_height = 480;

    log::plog console = std::make_shared<log::console>();
    ::logger->log_level(log::level::trace);
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

    GLFWwindow* main_window = glfwCreateWindow(window_width, window_height, "gnut", NULL, NULL);

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
    glfwSetKeyCallback(main_window, key_callback);
    glfwSetScrollCallback(main_window, scroll_callback);
    glfwSetCursorPosCallback(main_window, cursor_callback);
    glfwSetMouseButtonCallback(main_window, mouse_callback);
    glfwSetWindowSizeCallback(main_window, window_size_callback);

    glViewport(0, 0, 640, 480);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    view = glm::lookAt(vec3(0, 0, 222), vec3(0,0,0), vec3(0,1,0));
    projection = glm::perspective(45.f, static_cast<float>(window_width) / static_cast<float>(window_height), 1.f, 1000.f);

    gfx::shader vertex(GL_VERTEX_SHADER, "res/shaders/basic_lighting.vert");
    gfx::shader frag(GL_FRAGMENT_SHADER, "res/shaders/basic_lighting.frag");

    main_shader.attach(vertex);
    main_shader.attach(frag);
    main_shader.link_program();

    main_shader.enable();
    GLint v = glGetUniformLocation(main_shader.id(), "view");
    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));

    GLint p = glGetUniformLocation(main_shader.id(), "projection");
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));

    GLint ambient_intensity = glGetUniformLocation(main_shader.id(), "ambient_intensity");
    glUniform1f(ambient_intensity, .2f);

    GLint light_color = glGetUniformLocation(main_shader.id(), "light_color");
    glUniform3fv(light_color, 1, glm::value_ptr(vec3(.8f, .8f, .8f)));

    GLint light_position = glGetUniformLocation(main_shader.id(), "light_position");
    glUniform3fv(light_position, 1, glm::value_ptr(vec3(0, 0, 400)));

    GLint fragment_color = glGetUniformLocation(main_shader.id(), "fragment_color");
    glUniform3fv(fragment_color, 1, glm::value_ptr(vec3(.4f, 0.2f, .4f)));

    main_shader.disable();

    gfx::pmesh cone = gfx::mesh_loader::load("res/models/armadillo.off");
    cone->generate_buffer();

    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        main_shader.enable();
        GLint m = glGetUniformLocation(main_shader.id(), "model");
        glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(model));
        cone->draw();
        main_shader.disable();

        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



