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
#include <ground.h>
#include "skybox.h"

#include "logger.h"
#include "console.h"
#include "mesh_loader.h"

using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;

using namespace gnut;
using namespace gnut::gfx;
using namespace glm;


typedef struct model3d {
    glm::mat4 model;
    gnut::gfx::pmesh mesh;
}model3d;

static bool left_click;
static vec3 prev_pos;
static int window_height;
static int window_width;

static glm::vec3 eye_position;
static glm::mat4 projection;
static glm::mat4 view;

static int model_index;
static std::shared_ptr<model3d> curr_model;
static std::shared_ptr<gnut::gfx::ground> main_ground;

static std::shared_ptr<gfx::skybox> main_skybox;
static vector<std::shared_ptr<model3d>> models;
static gnut::gfx::pshader_program curr_shader;
static gnut::gfx::pshader_program diffuse_shader;
static gnut::gfx::pshader_program debug_shader;
static gnut::gfx::pshader_program skybox_shader;
static gnut::gfx::pshader_program shadow_shader;

const int NUM_MODELS = 6;

static const string model_files[NUM_MODELS] = {
        "testpatch.off", "armadillo.off", "teapot.off",
        "plane.off", "torus.off", "ico.off"
};


static const string skybox_folder = "res/images/";
static vector<string> skybox_images = {
        "res/images/right.jpg", "res/images/left.jpg", "res/images/top.jpg",
        "res/images/bottom.jpg", "res/images/back.jpg", "res/images/front.jpg"
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
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        curr_model = models[gnut::mod<int, int, int>(++model_index, NUM_MODELS)];
    } else if(key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        curr_model = models[gnut::mod<int, int, int>(--model_index, NUM_MODELS)];
    } else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
        curr_shader = (curr_shader == debug_shader) ? diffuse_shader : debug_shader;
    }
}


static void scroll_callback(GLFWwindow* window, double x, double y) {
    curr_model->model = glm::scale(curr_model->model, glm::vec3(1 + y / 10, 1 + y / 10, 1 + y / 10));
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
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
            mat4 translate;
            float angle = glm::acos(glm::dot(position, prev_pos) / (glm::length(position) * glm::length(prev_pos)));
            vec3 rotation_axis = glm::cross(prev_pos, position);
            curr_model->model = glm::rotate(rotation, angle, rotation_axis) * curr_model->model;
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
    model_index = 0;

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

    main_skybox = make_shared<gfx::skybox>();
     main_skybox->load(skybox_images);

    main_ground = make_shared<gfx::ground>();
    main_ground->texture("res/images/grass_texture242.jpg");

    eye_position = glm::vec3(0,1,5);
    view = glm::lookAt(eye_position, vec3(0,0,0), glm::normalize(vec3(0,1,0)));
    projection = glm::perspective(45.f, static_cast<float>(window_width) / static_cast<float>(window_height), .1f, 1000.f);

    diffuse_shader = make_shared<gnut::gfx::shader_program>();

    gfx::shader vertex(GL_VERTEX_SHADER, "res/shaders/basic_lighting.vert");
    gfx::shader frag(GL_FRAGMENT_SHADER, "res/shaders/basic_lighting.frag");

    diffuse_shader->attach(vertex);
    diffuse_shader->attach(frag);
    diffuse_shader->link_program();
    diffuse_shader->uniform("ambient_intensity", .2f);
    diffuse_shader->uniform("light_color", glm::vec3(.8, .8, .8));
    diffuse_shader->uniform("light_direction", glm::vec4(0, 0, -1, 0));
    diffuse_shader->uniform("fragment_color", glm::vec3(.2f, 0.2f, .2f));

    gfx::shader debug_vertex(GL_VERTEX_SHADER, "res/shaders/debug.vert");
    gfx::shader debug_frag(GL_FRAGMENT_SHADER, "res/shaders/debug.frag");
    debug_shader = make_shared<gfx::shader_program>();

    debug_shader->attach(debug_vertex);
    debug_shader->attach(debug_frag);
    debug_shader->link_program();
    debug_shader->uniform("ambient_intensity", .2f);
    debug_shader->uniform("light_color", glm::vec3(.8,.8,.8));
    debug_shader->uniform("light_direction", glm::vec4(0,0,-1,0));

    gfx::shader skybox_vertex(GL_VERTEX_SHADER, "res/shaders/skybox.vert");
    gfx::shader skybox_frag(GL_FRAGMENT_SHADER, "res/shaders/skybox.frag");
    skybox_shader = make_shared<gfx::shader_program>();

    skybox_shader->attach(skybox_vertex);
    skybox_shader->attach(skybox_frag);
    skybox_shader->link_program();

    gfx::shader shadow_vertex(GL_VERTEX_SHADER, "res/shaders/shadow_map.vert");
    gfx::shader shadow_frag(GL_FRAGMENT_SHADER, "res/shaders/shadow_map.frag");
    shadow_shader = make_shared<gfx::shader_program>();

    shadow_shader->attach(shadow_vertex);
    shadow_shader->attach(shadow_frag);
    shadow_shader->link_program();

    shadow_shader->uniform("texture_sampler", 1);
    shadow_shader->uniform("ambient_intensity", .2f);
    shadow_shader->uniform("light_color", glm::vec3(1,1,1));
    shadow_shader->uniform("light_direction", glm::vec4(1, -1, -1, 0));

    std::string file;
    for(int i = 0; i < NUM_MODELS; ++i) {
        std::shared_ptr<model3d> m = std::make_shared<model3d>();
        file = "res/models/";
        file += std::string(model_files[i]);
        m->mesh = gfx::mesh_loader::load(file);
        float scale = glm::length(eye_position) / m->mesh->max_vertice() / 3.f;
        float miny = m->mesh->min().y;
        m->model = glm::scale(m->model, glm::vec3(scale, scale, scale));
        m->model = glm::translate(m->model, glm::vec3(0,-miny,0));
        m->mesh->generate_buffer();
        models.push_back(m);
    }

    curr_model = models[5];
    curr_shader = diffuse_shader;

    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        skybox_shader->uniform("view", glm::mat4(glm::mat3(view)));
        skybox_shader->uniform("projection", projection);
        skybox_shader->enable();
        main_skybox->draw();
        skybox_shader->disable();

        shadow_shader->uniform("model", main_ground->model_matrix());
        shadow_shader->uniform("view", view);
        shadow_shader->uniform("projection", projection);
        shadow_shader->enable();
        main_ground->draw(shadow_shader);
        shadow_shader->disable();

        curr_shader->uniform("model", curr_model->model);
        curr_shader->uniform("view", view);
        curr_shader->uniform("projection", projection);
        curr_shader->enable();
        curr_model->mesh->draw();
        curr_shader->disable();


        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



