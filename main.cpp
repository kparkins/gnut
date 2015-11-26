
#include <iostream>
#define GLEW_STATIC
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

GLfloat triangle[] = {0.f, .5f, 0.f, -.5f, -.5f, 0.f, .5f, -.5f, 0.f};
const GLchar* vertex_shader = "#version 330 core\n"
                            "layout (location = 0) in vec3 vp;"
                            "void main() {"
                            "    gl_Position = vec4(vp,1.0);"
                            "}";
const GLchar* frag_shader = "#version 330 core\n"
                          "out vec4 frag_color;"
                          "void main() {"
                          "   frag_color = vec4(.5, .5, 0, 1.0);"
                          "}";

int main(int argc, char* argv[]) {
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        cerr << "Unable to initialize GLFW." << endl;
        exit(EXIT_FAILURE);
    }

    // create a newer opengl context than 2.1 (default on osx)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(640, 480, "gfx", NULL, NULL);

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

    glViewport(0, 0, 640, 480);

    GLchar error_log[512];
    GLint success;

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertex_shader, NULL);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, 0, error_log);
        cout << error_log << endl;
    }

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_shader, NULL);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, 0, error_log);
        cout << error_log << endl;
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
    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindow);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}


