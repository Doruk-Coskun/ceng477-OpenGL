#include "helper.h"

#include <vector>

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;

bool isFull = false;

void initializeWindow(GLFWwindow* window);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initVertices(float vertices[]);

static void errorCallback(int error,
                          const char * description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char * argv[]) {
    
    {
    if (argc != 2) {
        printf("Only one texture image expected!\n");
        exit(-1);
    }
    
    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit()) {
        exit(-1);
    }
    
    win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);
    initializeWindow(win);
    
    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        
        glfwTerminate();
        exit(-1);
    }
    }
    
    float vertices[3 * heightTexture * widthTexture];
    initVertices(vertices);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    
    initShaders();
    glUseProgram(idProgramShader);
    initTexture(argv[1], & widthTexture, & heightTexture);
    
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    
    while (!glfwWindowShouldClose(win)) {
        glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        processInput(win);
        
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(win);
    glfwTerminate();
    
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
    }
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isFull) {
        isFull = true;
        glfwDestroyWindow(window);
        win = glfwCreateWindow(600, 600, "CENG477 - HW3", glfwGetPrimaryMonitor(), NULL);
        initializeWindow(win);
    }
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && isFull) {
        isFull = false;
        glfwDestroyWindow(window);
        win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);
        initializeWindow(win);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void initializeWindow(GLFWwindow* window) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    glfwMakeContextCurrent(window);
}

void initVertices(float vertices[]) {
    for (int i = 0; i <= heightTexture; i++) {
        for (int  j = 0; j <= widthTexture; j++) {
            vertices[i * (widthTexture * 3) + j * 3 + 0] = (float) j;
            vertices[i * (widthTexture * 3) + j * 3 + 1] = 0.0f;
            vertices[i * (widthTexture * 3) + j * 3 + 2] = (float) i;
            std::cout << j << ", " << 0 << ", " << i << std::endl;
        }
    }
}
