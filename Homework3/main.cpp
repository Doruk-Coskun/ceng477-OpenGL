#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

#include <vector>

#include "helper.h"
#include "bundled-glm/glm.hpp"
#include "bundled-glm/gtc/matrix_transform.hpp"
#include "bundled-glm/gtc/type_ptr.hpp"


#define TARGET_FPS 30

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
glm::mat4x4 viewMatrix;
glm::mat4 projectionMatrix;

int widthTexture, heightTexture;

bool isFull = false;
bool isPressed = false;
float heightFactor = 10.0;
float cameraSpeed = 0.0f;

float yaw = 90.0f; // 0 -> right, -90 -> forward
float pitch = 0.0f;

float lastX = 300;  // width / 2
float lastY = 300;  // height / 2

void initializeWindow(GLFWwindow* window);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setUpCamera();

void drawTest();
void generateHeightMap();

static void errorCallback(int error,
                          const char * description) {
    fprintf(stderr, "Error: %s\n", description);
}

#pragma pack(push, 1)
struct indexV {
    unsigned int first;
    unsigned int second;
    unsigned int third;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct vertex {
    float xPos;
    float yPos;
    float zPos;
    float xTexPos;
    float yTexPos;
};
#pragma pack(pop)

void initVerticesV(std::vector<vertex> &vertices);
void initIndices(std::vector<indexV> &indices);

int main(int argc, char * argv[]) {
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
    
    win = glfwCreateWindow(600, 600, "CENG477 - HW3", nullptr, nullptr);
    
    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        exit(-1);
    }
    
    glEnable(GL_DEPTH_TEST);
    initShaders();
    initTexture(argv[1], & widthTexture, & heightTexture);
    
    std::vector<vertex> verticesV((heightTexture + 1) * (widthTexture + 1));
    std::vector<indexV> indicesV(2 * (heightTexture + 1) * (widthTexture + 1));
    
    initVerticesV(verticesV);
    initIndices(indicesV);
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesV.size() * sizeof(indexV), indicesV.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesV.size() * sizeof(vertex), verticesV.data(), GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(idProgramShader);
    
    glBindVertexArray(VAO);
    
    setUpCamera();
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
    unsigned int MVPLoc = glGetUniformLocation(idProgramShader, "MVP");
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
    
    glUniform1i(glGetUniformLocation(idProgramShader, "widthTexture"), widthTexture);
    glUniform1i(glGetUniformLocation(idProgramShader, "heightTexture"), heightTexture);
    
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        while (glfwGetTime() < lastTime + 1.0/TARGET_FPS) {}
        lastTime += 1.0/TARGET_FPS;
        
        glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        unsigned int MVLoc = glGetUniformLocation(idProgramShader, "MV");
        glUniformMatrix4fv(MVLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        processInput(win);
        
        unsigned int CamPosLoc = glGetUniformLocation(idProgramShader, "cameraPosition");
        glUniform4f(CamPosLoc, cameraPos.x, cameraPos.y, cameraPos.z, 1.0);
        
        glDrawElements(GL_TRIANGLES, 3 * 2 * (heightTexture + 1) * (widthTexture + 1), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(win);
        glfwPollEvents();
        
        // Go forward
        cameraPos += cameraSpeed * cameraFront;
    }
    
    glfwDestroyWindow(win);
    glfwTerminate();
    
    return 0;
}

void processInput(GLFWwindow* window) {
    float sensitivity = 0.05f;
    float yoffset = 20.0f;
    float xoffset = 20.0f;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
    }
    
    //    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isFull) {
    //        isFull = true;
    //        glfwDestroyWindow(window);
    //        win = glfwCreateWindow(600, 600, "CENG477 - HW3", glfwGetPrimaryMonitor(), NULL);
    //        initializeWindow(win);
    //    }
    //
    //    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && isFull) {
    //        isFull = false;
    //        glfwDestroyWindow(window);
    //        win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);
    //        initializeWindow(win);
    //    }
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !isPressed) {
        heightFactor += 0.5f;
        glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
    }
    
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        heightFactor -= 0.5f;
        glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
    }
    
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        cameraSpeed += 0.5f;
        cout << cameraSpeed << endl;
    }
    
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        cameraSpeed -= 0.5f;
        if (cameraSpeed < 0) {
            cameraSpeed = 0;
        }
        cout << cameraSpeed << endl;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        
        pitch += yoffset * sensitivity;
        
        // Restrict the pitch to < 90 degrees
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        
        pitch -= yoffset * sensitivity;
        
        // Restrict the pitch
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        cameraFront = glm::normalize(front);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        
        yaw -= xoffset * sensitivity;;
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
        
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        
        yaw += xoffset * sensitivity;
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
}

void initVerticesV(std::vector<vertex> & vertices) {
    for (int i = 0; i <= heightTexture; i++) {
        for (int  j = 0; j <= widthTexture; j++) {
            vertices[i * widthTexture+ j].xPos = (float) j;
            vertices[i * widthTexture+ j].yPos = 0.0f;
            vertices[i * widthTexture+ j].zPos = (float) i;
            vertices[i * widthTexture+ j].xTexPos = (float) j / widthTexture;
            vertices[i * widthTexture+ j].yTexPos = (float) i / heightTexture;
        }
    }
}

void initIndices(std::vector<indexV> &indices) {
    int counter = 0;
    for (int i = 0; i < heightTexture; i++) {
        for (int j = 0; j < widthTexture - 1 ; j++) {
            indices[counter].first =  i * widthTexture + j;
            indices[counter].second = (i + 1) * widthTexture + j;
            indices[counter].third = i * widthTexture + j + 1;
            
            indices[counter + 1].first =  (i + 1) * widthTexture + j;
            indices[counter + 1].second = (i + 1) * widthTexture + j + 1;
            indices[counter + 1].third = i * widthTexture + j + 1;
            
            counter += 2;
        }
    }
}

void setUpCamera() {
    cameraPos = glm::vec3(widthTexture / 2, widthTexture / 10, - heightTexture / 4);
    
    // cameraFront = glm::vec3(0.0, 0.0, 1.0);
    // TODO: tidy up later
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    
    glm::vec3 up = glm::vec3(0.0f, 1.0, 0.0);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraFront));
    cameraUp = glm::cross(cameraFront, cameraRight);
    
    viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
    unsigned int MVLoc = glGetUniformLocation(idProgramShader, "MV");
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

// TODO: How to use this?
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
