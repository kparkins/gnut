/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <iostream>
#include <unordered_map>
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
using std::unordered_map;

using namespace glm;
using namespace gnut;


typedef struct model3d {
    glm::mat4 model;
    gfx::pmesh mesh;
}model3d;

static bool left_click;
static vec3 prev_pos;
static int window_height;
static int window_width;

static glm::vec3 eye_position;
static glm::mat4 projection;
static glm::mat4 view;

static int model_index;
static std::shared_ptr<model3d> main_model;

static vector<std::shared_ptr<model3d>> models;
static unordered_map<string, gfx::pshader_program> shader_map;
static gnut::gfx::pshader_program main_shader;

const int NUM_MODELS = 6;

static const string model_files[NUM_MODELS] = {
        "testpatch.off", "armadillo.off", "teapot.off",
        "plane.off", "torus.off", "ico.off"
};

vec3 trackball_coords(double x, double y) {
    vec3 v;
    v.x = (2.f * static_cast<float>(x) - window_width) / window_width;
    v.y = (window_height - 2.f * static_cast<float>(y)) / window_height;
    v.z = 0.f;
    float distance = glm::length(v);
    if(distance > 1.f) {
        distance = 1.f;
    }
    v.z = sqrtf(1.f - (distance * distance));
    return glm::normalize(v);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    int num = NUM_MODELS;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        model_index = gnut::mod<int, int, int>(model_index + 1, num);
    } else if(key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        model_index = gnut::mod<int, int, int>(model_index - 1, num);
    }
}

static void scroll_callback(GLFWwindow* window, double x, double y) {
    main_model->model = glm::scale(main_model->model, glm::vec3(1 + y / 10, 1 + y / 10, 1 + y / 10));
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        left_click = !left_click;
        prev_pos = trackball_coords(x, y);
    }
}

static void cursor_callback(GLFWwindow* window, double x, double y) {
    if(left_click) {
        vec3 position = trackball_coords(x, y);
        float magnitude = glm::length(position - prev_pos);
        if(magnitude > .001f) {
            mat4 rotation;
            float angle = glm::acos(glm::dot(position, prev_pos) / (glm::length(position) * glm::length(prev_pos)));
            vec3 rotation_axis = glm::cross(prev_pos, position);
            main_model->model = glm::rotate(rotation, angle, rotation_axis) * main_model->model;
        }
        prev_pos = position;
    }
}


static void window_size_callback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    projection = glm::perspective(45.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.f);
}


static void errorCallback(int error, const char* errorMessage) {
}

int main(int argc, char* argv[]) {
    left_click = false;
    window_width = 640;
    window_height = 480;

    log::plog console = std::make_shared<log::console>();
    logger->log_level(log::level::trace);
    logger->add(console);

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

    eye_position = glm::vec3(0,0,4);
    view = glm::lookAt(eye_position, vec3(0,0,0), vec3(0,1,0));
    projection = glm::perspective(45.f, static_cast<float>(window_width) / static_cast<float>(window_height), 1.f, 1000.f);

    main_shader = make_shared<gnut::gfx::shader_program>();
    shader_map.insert(make_pair("diffuse", main_shader));

    gfx::shader vertex(GL_VERTEX_SHADER, "res/shaders/basic_lighting.vert");
    gfx::shader frag(GL_FRAGMENT_SHADER, "res/shaders/basic_lighting.frag");

    main_shader->attach(vertex);
    main_shader->attach(frag);
    main_shader->link_program();
    main_shader->uniform("view", view);
    main_shader->uniform("projection", projection);
    main_shader->uniform("ambient_intensity", .2f);
    main_shader->uniform("light_color", glm::vec3(.8,.8,.8));
    main_shader->uniform("light_position", glm::vec3(0,0,400));
    main_shader->uniform("fragment_color", glm::vec3(.4f, 0.2f, .4f));

    gfx::shader debug_vertex(GL_VERTEX_SHADER, "res/shaders/debug.vert");
    gfx::shader debug_frag(GL_FRAGMENT_SHADER, "res/shaders/debug.frag");
    gfx::pshader_program debug = make_shared<gfx::shader_program>();
    shader_map.insert(make_pair("debug", debug));
    debug->attach(debug_vertex);
    debug->attach(debug_frag);
    debug->link_program();
    debug->uniform("view", view);
    debug->uniform("projection", projection);
    debug->uniform("ambient_intensity", .2f);
    debug->uniform("light_color", glm::vec3(.8,.8,.8));
    debug->uniform("light_position", glm::vec3(0,0, 400));

    std::string file;
    for(int i = 0; i < NUM_MODELS; ++i) {
        std::shared_ptr<model3d> m = std::make_shared<model3d>();
        file = "res/models/";
        file += std::string(model_files[i]);
        m->mesh = gfx::mesh_loader::load(file);
        float scale = glm::length(eye_position) / m->mesh->max_vertice() / 2.f;
        m->model = glm::scale(m->model, glm::vec3(scale, scale, scale));
        m->mesh->generate_buffer();
        models.push_back(m);
    }

    model_index = 0;
    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        main_model = models[model_index];
        main_shader->uniform("model", main_model->model);
        main_shader->uniform("projection", projection);
        main_shader->enable();
        main_model->mesh->draw();
        main_shader->disable();

        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



