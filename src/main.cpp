#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

size_t const sWinWidth = 320;
size_t const sWinHeight = 240;

GLchar const* sVertexShader = R"(#version 330 core

layout(location = 0) in vec3 position;
out vec3 vertColor;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
    vertColor = vec3(0.0f, 0.0f, 0.0f);
})";

GLchar const* sFragmentShaderOrange = R"(#version 330 core

in vec3 vertColor;
out vec4 color;
uniform vec4 uniColor;

void main()
{
    color = uniColor;
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

        // Shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &sVertexShader, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        std::cerr << "Error compiling vertex shader: " << infoLog << std::endl;
        return -1;
    }
    GLuint fragmentShaderOr;
    fragmentShaderOr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOr, 1, &sFragmentShaderOrange, nullptr);
    glCompileShader(fragmentShaderOr);
    glGetShaderiv(fragmentShaderOr, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderOr, 1024, nullptr, infoLog);
        std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
        return -1;
    }
    GLuint fragmentShaderYe;
    fragmentShaderYe = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYe, 1, &sFragmentShaderYellow, nullptr);
    glCompileShader(fragmentShaderYe);
    glGetShaderiv(fragmentShaderYe, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderYe, 1024, nullptr, infoLog);
        std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
        return -1;
    }

        // Shader program
    GLuint shaderProgOr;
    shaderProgOr = glCreateProgram();
    glAttachShader(shaderProgOr, vertexShader);
    glAttachShader(shaderProgOr, fragmentShaderOr);
    glLinkProgram(shaderProgOr);
    glGetProgramiv(shaderProgOr, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgOr, 1024, nullptr, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        return -1;
    }
    GLuint shaderProgYe;
    shaderProgYe = glCreateProgram();
    glAttachShader(shaderProgYe, vertexShader);
    glAttachShader(shaderProgYe, fragmentShaderYe);
    glLinkProgram(shaderProgYe);
    glGetProgramiv(shaderProgYe, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgYe, 1024, nullptr, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOr);
    glDeleteShader(fragmentShaderYe);

    // Geometry to draw
    float const PI2 = 8.0f * atan(1.0f);
    int const numVerts = 10;
    std::vector<GLfloat> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    for (int i = 0; i < numVerts; ++i)
    {
        GLfloat xPos = std::sin(PI2 * i / numVerts);
        GLfloat yPos = std::cos(PI2 * i / numVerts);
        vertices.push_back(xPos);
        vertices.push_back(yPos);
        vertices.push_back(0.0f);
    }
    vertices.push_back(vertices[3]);
    vertices.push_back(vertices[4]);
    vertices.push_back(vertices[5]);

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
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);
    
        // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

            // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat curTime = glfwGetTime();
        auto val = (std::sin(32.0f * curTime + 3.14f * std::sin(curTime * 8.0f)) / 2.0f) + 0.5f;
        GLfloat redColor = val;
        GLfloat greenColor = val;
        GLfloat blueColor = val;
        GLint uniColorLocation = glGetUniformLocation(shaderProgOr, "uniColor");
        
        glUseProgram(shaderProgOr);
        glUniform4f(uniColorLocation, redColor, greenColor, blueColor, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts + 2);
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
