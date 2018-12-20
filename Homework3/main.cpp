#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helper.hpp"

#define TARGET_FPS 30

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;

bool isFull = false;
bool isPressed = false;
float heightFactor = 10.0;

void initializeWindow(GLFWwindow* window);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initVertices(float vertices[]);

void drawTest();

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
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        glewExperimental = GL_TRUE;
        
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
    
    glViewport(0, 0, 600, 600);
    
    initShaders();
    glUseProgram(idProgramShader);
    initTexture(argv[1], & widthTexture, & heightTexture);
    
    float vertices[3 * (heightTexture + 1) * (widthTexture + 1)];
    initVertices(vertices);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    
    //    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        while (glfwGetTime() < lastTime + 1.0/TARGET_FPS) {}
        lastTime += 1.0/TARGET_FPS;
        
        glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        processInput(win);
        
        drawTest();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //        glDrawArrays(GL_TRIANGLES, 0, heightTexture * widthTexture * 2);
        
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
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !isPressed) {
        heightFactor += 0.5f;
        //        isPressed = true;
        glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
        cout << heightFactor << std::endl;
    }
    
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        heightFactor -= 0.5f;
        //        isPressed = true;
        glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
        cout << heightFactor << std::endl;
    }
    
    //    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE && isPressed) {
    //        isPressed = false;
    //    }
    //
    //    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && isPressed) {
    //        isPressed = false;
    //    }
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
        }
    }
}

void drawTest() {
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}
