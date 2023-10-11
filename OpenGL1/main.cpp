// This is a example project after finishing the Getting Started chapter of "Learn OpenGl" https://learnopengl.com/Getting-started
// It is recommended to install the "GLSL language integration" for writing Shaders

// Install Glad from https://glad.dav1d.de
// Preinstalled is https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on
// for OpenGl 4.6 Core without Extension
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// From https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Shader.h"

///Possible Predefinied Cameras #CAMERA_MOVEMENT #CAMERA_FLY #CAMERA_FPS #CAMERA_NO_FEATURES
// Default are all features activated
// #define CAMERA_FPS
#include "Camera.h"

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float* value);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int FRAME_RATE = 60;
const double FRAME_TIME = 1.0 / FRAME_RATE;

double lastFrame = 0.0; // Time of last frame
double lastX = 0.0;
double lastY = 0.0;
bool firstMouse = true;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyFirstWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to initalized a Window :(" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to intalized GLAD" << std::endl;
        return 2;
    }

    // resize Viewport with Window
    glfwSetFramebufferSizeCallback(window, resize);

    // Create Shaderprogram
    Shader shader("shader/simple.vert", "shader/textureMix.frag");

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    float vertices3D[] = {
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
    };
    unsigned int VBO_3D, VAO_3D;
    glGenBuffers(1, &VBO_3D);
    glGenVertexArrays(1, &VAO_3D);

    glBindVertexArray(VAO_3D);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_3D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), &vertices3D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // draw wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model(1.0f);
    model *= glm::toMat4(glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    shader.use();
    shader.set("texture1", 0);
    shader.set("texture2", 1);
    unsigned int visible = shader.getSetLocation("visible");
    float visible_value = 0.2f;
    unsigned int localID = shader.getSetLocation("local");
    unsigned int modelID = shader.getSetLocation("model");
    unsigned int viewID = shader.getSetLocation("view");
    unsigned int projectionID = shader.getSetLocation("projection");

    glEnable(GL_DEPTH_TEST);
    double frameTime = glfwGetTime();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // render Loop
    while (!glfwWindowShouldClose(window))
    {
        if (frameTime + FRAME_TIME > glfwGetTime())
        {
            continue;
        }
        frameTime = glfwGetTime();

        processInput(window, &visible_value);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 transform = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.set(visible, visible_value);
        shader.setMat4(localID, transform);
        shader.setMat4(modelID, model);
        shader.setMat4(viewID, view);
        shader.setMat4(projectionID, projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

#if false // Draw Planes
        glBindVertexArray(VAO);

        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        // better use Quaternion, because of Gimbal Lock :(
        transform *= glm::mat4_cast(glm::angleAxis((float)glfwGetTime() * glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f))); // glm::rotate(transRot, (float)glfwGetTime() * glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float time = abs(0.5f * sin(glfwGetTime())) + 0.5f;
        transform = glm::scale(transform, glm::vec3(time, time, time));
        shader.setMat4(localID, transform);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif

        // Draw 10 Cubes
        glBindVertexArray(VAO_3D);
        glm::mat4 rotation = glm::toMat4(glm::angleAxis((float)glfwGetTime() * glm::radians(50.0f), glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f))));
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            transform = glm::mat4(1.0f);
            transform *= (0 == i % 3U ? rotation : glm::mat4(1.0f));
            shader.setMat4(localID, transform);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model *= glm::toMat4(glm::angleAxis(glm::radians(angle), glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f))));
            shader.setMat4(modelID, model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO_3D);
    glDeleteBuffers(1, &VBO_3D);
    shader.remove();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, float* value)
{
    double currentFrame = glfwGetTime();
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        *value += deltaTime * (*value < 1.0f);
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        *value -= deltaTime * (*value > 0.0f);
    }
    int movement = (
        (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) * CameraMovement::FORWARD
        | (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) * CameraMovement::BACKWARD
        | (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) * CameraMovement::LEFT
        | (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) * CameraMovement::RIGHT
        | (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) * CameraMovement::DOWN
        | (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) * CameraMovement::UP
        | (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) * CameraMovement::ROLL_LEFT
        | (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) * CameraMovement::ROLL_RIGHT
        );
    if (movement != 0)
    {
        camera.ProcessKeyboard(static_cast<CameraMovement>(movement), deltaTime);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    double xoffset = xpos - lastX;
    double yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}

void resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}