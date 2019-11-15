#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// #include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

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
    // 第三个参数表示这个窗口的名称（标题），这里我们使用"LearnOpenGL"，当然你也可以使用你喜欢的名称；
    // 最后两个参数我们暂时忽略，先设置为空指针就行
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);

    // GLEW是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLEW。
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 视口(Viewport)，通过调用glViewport函数来设置窗口的维度(Dimension)
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, width, height);

    // 让GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数
        glfwPollEvents();
        // glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲）
        glfwSwapBuffers(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // glfwTerminate函数来释放GLFW分配的内存
    glfwTerminate();
    return 0;
}