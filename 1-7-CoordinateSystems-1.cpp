#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "stb_image.h"
using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void CreateTexture(unsigned int *texture, const char *filename, int rgbmode, bool flip = false)
{
    // 图像文件的位置、宽度、高度、颜色通道的个数
    int width, height, nrChannels;
    unsigned char *data;

    // 生成纹理
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (flip)
    {
        stbi_set_flip_vertically_on_load(true);
    }
    data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, rgbmode, width, height, 0, rgbmode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture:" << filename << endl;
    }
    // 释放图像的内存
    stbi_image_free(data);
}

int main()
{
    // 调用glfwInit函数来初始化GLFW
    glfwInit();
    // 我们将主版本号(Major)和次版本号(Minor)都设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 明确告诉GLFW我们使用的是核心模式(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 不允许用户调整窗口的大小
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数；
    // 第三个参数表示这个窗口的名称（标题）
    // 最后两个参数我们暂时忽略，先设置为空指针就行
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);

    // must have
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Faile to initialize GLAD" << endl;
        return -1;
    }

    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 在渲染前指定OpenGL该如何解释顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 生成纹理
    unsigned int texture1, texture2;
    CreateTexture(&texture1, "Textures/container.jpg", GL_RGB);
    CreateTexture(&texture2, "Textures/awesomeface.png", GL_RGBA, true);

    Shader shader("Shaders/1-7-CoordinateSystems-1.vs", "Shaders/1-7-CoordinateSystems-1.fs");
    shader.use();
    shader.setInt("ourTexture1", 0);
    shader.setInt("ourTexture2", 1);

    // 让GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        // glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲）
        glfwSwapBuffers(window);
        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1, 0, 0));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0, 0, -3));

        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        shader.setMat4("transform", view);

        // 刚创建的程序对象作为它的参数，以激活这个程序对象：
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}