#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaders/program.h"


size_t const sWinWidth = 800;
size_t const sWinHeight = 800;

GLchar const* sVertexShader = R"(#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 vertColor;
uniform vec3 shiftPos;

void main()
{
    gl_Position = vec4(position + shiftPos, 1.0f);
    vertColor = color;//vec3(0.0f, 0.0f, 0.0f);
})";

GLchar const* sFragmentShaderOrange = R"(#version 330 core

in vec3 vertColor;
out vec4 color;
uniform vec4 uniColor;

void main()
{
    color = vec4(vertColor, 1.0f);
}
)";

GLchar const* sFragmentShaderYellow = R"(#version 330 core

in vec3 vertColor;
out vec4 color;

void main()
{
    color = vec4(vertColor, 1.0f);//vec4(5.0f, 1.0f, 0.2f, 1.0f);
}
)";


void glfwErrorReporting(int errCode, char const* msg);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);


int main(void)
{
    if (GL_FALSE == glfwInit())
        return -1;
        // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glfwSetErrorCallback(glfwErrorReporting);

        // Create a window
    GLFWwindow* window = glfwCreateWindow(sWinWidth, sWinHeight, "Renderize", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

        // Initialize Glew
    
    glewExperimental = GL_TRUE;
    GLenum glewCode = glewInit();
    if (GLEW_OK != glewCode)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewCode) << std::endl;
        return -1;
    }
    
    glViewport(0, 0, sWinWidth, sWinHeight);

        // Initialize some GLFW callback
    glfwSetKeyCallback(window, keyCallback);


    program shaderProgOr;
    program shaderProgYe;
    if (!shaderProgOr.create())
    {
        std::cerr << shaderProgOr.lastError() << std::endl;
        return -1;
    }
    if (!shaderProgYe.create())
    {
        std::cerr << shaderProgYe.lastError() << std::endl;
        return -1;
    }

    {
        // Shaders
        shader vertexShader;
        if (!vertexShader.compile(sVertexShader, GL_VERTEX_SHADER))
        {
            std::cerr << vertexShader.lastError() << std::endl;
            return -1;
        }
        shader fragmentShaderOr;
        if (!fragmentShaderOr.compile(sFragmentShaderOrange, GL_FRAGMENT_SHADER))
        {
            std::cerr << fragmentShaderOr.lastError() << std::endl;
            return -1;
        }
        shader fragmentShaderYe;
        if (!fragmentShaderYe.compile(sFragmentShaderYellow, GL_FRAGMENT_SHADER))
        {
            std::cerr << fragmentShaderYe.lastError() << std::endl;
            return -1;
        }

        // Shader program
        shaderProgOr.attach(vertexShader);
        shaderProgOr.attach(fragmentShaderOr);
        if (!shaderProgOr.link())
        {
            std::cerr << shaderProgOr.lastError() << std::endl;
            return -1;
        }
        shaderProgYe.attach(vertexShader);
        shaderProgYe.attach(fragmentShaderOr);
        if (!shaderProgYe.link())
        {
            std::cerr << shaderProgYe.lastError() << std::endl;
            return -1;
        }
    }

    // Geometry to draw
    float const PI2 = 8.0f * atan(1.0f);
    int const numVerts = 20;
    std::vector<GLfloat> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    for (int i = 0; i < numVerts; ++i)
    {
        GLfloat xPos = std::sin(PI2 * i / numVerts);
        GLfloat yPos = std::cos(PI2 * i / numVerts);
        vertices.push_back(xPos);
        vertices.push_back(yPos);
        vertices.push_back(0.0f);
        vertices.push_back(xPos);
        vertices.push_back(yPos);
        vertices.push_back(xPos * yPos);
    }
    vertices.push_back(vertices[6]);
    vertices.push_back(vertices[7]);
    vertices.push_back(vertices[8]);
    vertices.push_back(vertices[6]);
    vertices.push_back(vertices[7]);
    vertices.push_back(vertices[8]);

    std::vector<GLint> indices;
    for (int i = 0; i < numVerts + 2; ++i)
    {
        indices.push_back(i);
    }


    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);
    
        // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

            // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat curTime = static_cast<float>(glfwGetTime());
        auto val = (std::sin(32.0f * curTime + 3.14f * std::sin(curTime * 8.0f)) / 2.0f) + 0.5f;
        GLfloat redColor = val;
        GLfloat greenColor = val;
        GLfloat blueColor = val;
        GLint uniColorLocation = glGetUniformLocation(shaderProgOr, "uniColor");

        GLint shiftPosLocation = glGetUniformLocation(shaderProgOr, "shiftPos");
        glUniform3f(shiftPosLocation, std::sin(curTime), 0.0f, 0.0f);
        
        shaderProgOr.use();
        glUniform4f(uniColorLocation, redColor, greenColor, blueColor, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 2 * (numVerts + 2));
        glBindVertexArray(0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void glfwErrorReporting(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
