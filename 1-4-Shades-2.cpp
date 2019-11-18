#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *vShaderSource = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
const char *fShaderSource = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "uniform vec4 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = ourColor;\n"
                            "}\n\0";

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", nullptr, nullptr);
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

    // 编译顶点着色器
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource, nullptr);
    glCompileShader(vShader);

    // 检测编译是否成功
    int success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, sizeof(infoLog), nullptr, infoLog);
        cout << "complie vertex shader error:" << infoLog << endl;
    }

    // 编译片段着色器
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource, nullptr);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, sizeof(infoLog), nullptr, infoLog);
        cout << "complie fragment shader error:" << infoLog << endl;
    }

    // 着色器程序-多个着色器合并之后并最终链接完成的版本
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);
    // 检查是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        cout << "link shader program error:" << infoLog << endl;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 在渲染前指定OpenGL该如何解释顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // 让GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        // glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲）
        glfwSwapBuffers(window);
        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 刚创建的程序对象作为它的参数，以激活这个程序对象：
        glUseProgram(shaderProgram);

        float time = glfwGetTime();
        float greenValue = sin(time) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0, greenValue, 0, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}