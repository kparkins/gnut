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
#include <glm/gtx/rotate_vector.hpp>

#include <shader.h>
#include <ground.h>
#include "skybox.h"

#include "logger.h"
#include "console.h"
#include "mesh_loader.h"
#include "depth_map.h"

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
static int use_normalmap;
static int window_height;
static int window_width;
static bool render_dmap;
static GLFWwindow* main_window;

static glm::vec3 eye_position;
static glm::mat4 projection;
static glm::mat4 view;
static glm::vec3 light_position;
static glm::vec3 light_color;
static bool normal_camera;
static bool shadow_camera;
static int use_pcf;

static int model_index;
static std::shared_ptr<model3d> curr_model;
static std::shared_ptr<gnut::gfx::ground> main_ground;

static std::shared_ptr<gfx::skybox> main_skybox;
static vector<std::shared_ptr<model3d>> models;
static gnut::gfx::pshader_program skybox_shader;
static gnut::gfx::pshader_program shadow_shader;
static gnut::gfx::pshader_program normal_shader;
static gnut::gfx::pdepth_map mdepth_map;

const int NUM_MODELS = 3;

static const string model_files[NUM_MODELS] = {
        "cow.off", "armadillo.off",  "turtle.off"
};


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
        render_dmap = !render_dmap;
    } else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        use_normalmap = (use_normalmap == 0) ? 1 : 0;
    } else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        normal_camera = !normal_camera;
    } else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        shadow_camera = !shadow_camera;
    } else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        use_pcf = (use_pcf == 0) ? 1 : 0;
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

static void load_shaders() {
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

    shadow_shader->enable();
    shadow_shader->uniform("ambient_intensity", .4f);
    shadow_shader->uniform("light_color", light_color);
    shadow_shader->uniform("light_position", light_position);
    shadow_shader->disable();

    gfx::shader normal_vertex(GL_VERTEX_SHADER, "res/shaders/shadow_bump.vert");
    gfx::shader normal_frag(GL_FRAGMENT_SHADER, "res/shaders/shadow_bump.frag");
    normal_shader = make_shared<gfx::shader_program>();

    normal_shader->attach(normal_vertex);
    normal_shader->attach(normal_frag);
    normal_shader->link_program();

    normal_shader->enable();
    normal_shader->uniform("ambient_intensity", .4f);
    normal_shader->uniform("light_color", light_color);
    normal_shader->uniform("light_position", light_position);
    normal_shader->disable();
}

static void load_models() {
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
}

static void setup_glcontext() {
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

    main_window = glfwCreateWindow(window_width, window_height, "gnut", NULL, NULL);

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
}

int main(int argc, char* argv[]) {
    left_click = false;
    render_dmap = false;
    window_width = 640;
    window_height = 480;
    model_index = 0;

    log::plog console = std::make_shared<log::console>();
    logger->log_level(log::level::trace);
    logger->add(console);

    setup_glcontext();

    main_skybox = make_shared<gfx::skybox>();
    main_skybox->load(skybox_images);

    main_ground = make_shared<gfx::ground>();
    main_ground->texture("res/images/tile.jpg");
    main_ground->normal_map("res/images/tile_nmap.jpg");

    eye_position = glm::vec3(0,1,5);
    view = glm::lookAt(eye_position, vec3(0,0,0), glm::normalize(vec3(0,1,0)));
    projection = glm::perspective(45.f, static_cast<float>(window_width) / static_cast<float>(window_height), .1f, 1000.f);

    light_color = glm::vec3(1.f, 1.f, 1.f);
    light_position = glm::vec3(-3, 5, 0);
    glm::vec3 light_up(0,1,0);

    float light_rot = glm::acos(glm::length(glm::vec3(-3,0,0)) / glm::length(light_position));
    light_up = glm::rotate(light_up, glm::degrees(light_rot), glm::vec3(0,0,1));

    mdepth_map = make_shared<gfx::depth_map>(2048, 2048);
    mdepth_map->light_view(glm::lookAt(glm::vec3(light_position), glm::vec3(0,0,0), light_up));
    mdepth_map->light_projection(glm::ortho(-5.f, 5.f, -5.f, 5.f, .01f, 20.f));

    load_shaders();
    load_models();

    curr_model = models[NUM_MODELS - 1];

    // main loop
    while(!glfwWindowShouldClose(main_window)) {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        if(normal_camera) {
            view = glm::lookAt(eye_position, vec3(0,0,5), vec3(0,0,-1));
        } else if (shadow_camera) {
            glm::vec3 up(0,1,0);
            glm::vec3 eye(0, .5, .5);
            float rot_angle = glm::acos(glm::length(glm::vec3(0,0,eye.z)) / glm::length(eye));
            up = glm::rotate(up, glm::degrees(rot_angle), glm::vec3(-1,0,0));
            view = glm::lookAt(eye, glm::vec3(0,0,0), glm::normalize(up));
        } else {
            view = glm::lookAt(eye_position, vec3(0,0,0), glm::normalize(vec3(0,1,0)));
        }

        mdepth_map->enable();
        mdepth_map->depth_shader()->uniform("model", main_ground->model_matrix());
        main_ground->draw();

        mdepth_map->depth_shader()->uniform("model", curr_model->model);
        curr_model->mesh->draw();
        mdepth_map->disable();

        if(render_dmap) {
            mdepth_map->render();
        } else {
            skybox_shader->enable();
            skybox_shader->uniform("skybox_sampler", 0);
            skybox_shader->uniform("view", glm::mat4(glm::mat3(view)));
            skybox_shader->uniform("projection", projection);
            main_skybox->draw();
            skybox_shader->disable();

            shadow_shader->enable();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, main_skybox->id());
            shadow_shader->uniform("camera_position", eye_position);
            shadow_shader->uniform("texture_sampler", 1);
            shadow_shader->uniform("environment_sampler", 0);
            shadow_shader->uniform("shadow_sampler", 3);
            shadow_shader->uniform("use_pcf", use_pcf);
            shadow_shader->uniform("view", view);
            shadow_shader->uniform("projection", projection);
            shadow_shader->uniform("light_matrix", mdepth_map->light_matrix());
            shadow_shader->uniform("use_texture", 0);
            shadow_shader->uniform("model", curr_model->model);
            curr_model->mesh->draw();

            shadow_shader->disable();

            normal_shader->enable();
            normal_shader->uniform("use_normal_map", use_normalmap);
            normal_shader->uniform("texture_sampler", 1);
            normal_shader->uniform("shadow_sampler", 3);
            normal_shader->uniform("normal_sampler", 4);
            normal_shader->uniform("use_pcf", use_pcf);
            normal_shader->uniform("view", view);
            normal_shader->uniform("projection", projection);
            normal_shader->uniform("light_matrix", mdepth_map->light_matrix());

            normal_shader->uniform("model", main_ground->model_matrix());
            main_ground->draw();
            normal_shader->disable();

        }

        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();

    return 0;
}



