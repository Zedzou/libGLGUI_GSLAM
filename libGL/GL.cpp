#include "GL.h"
#include <stdexcept>
#include <iostream>
#include <ImGuiExtension/LocalFileSystem.h>
using namespace GLRenderer;

GL_base* GL_base::ptrInstance;
GL_base::GL_base():bDrawDemoUI(true), window_(nullptr)
{
    ptrInstance=this;
    init();
};

// 析构函数
GL_base::~GL_base()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if(window_)delete window_;
    glfwTerminate();
};

// GLFW初始化
void GL_base::init()
{
    // 窗口设置
    glfwSetErrorCallback(GLRenderer::GL_base::error_callback);
    if(!glfwInit()) throw std::runtime_error("Unable to init glfw!");
    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
};

// 渲染窗口初始化
void GL_base::initWindow(const std::string& name, int width, int height, bool visible)
{
    // 创建窗口
    window_ = new GLFWWindowContainer(name, width, height, 0.4, 500.f, visible);
    if(!window_->window){
        std::cout << "Failed to create GLFW window" << std::endl;
        delete window_;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    };

    // 回调函数设置
    glfwMakeContextCurrent(window_->window);
    glfwSetKeyCallback(window_->window, key_callback);
    glfwSetMouseButtonCallback(window_->window, mouse_button_callback);
    glfwSetCursorPosCallback(window_->window, mouse_callback);
    glfwSetScrollCallback(window_->window, scroll_callback);
    glfwSetFramebufferSizeCallback(window_->window, framebuffer_size_callback);

    // 实时动态设置窗口大小
    glfwGetFramebufferSize(window_->window, &window_->runtimeWidth, &window_->runtimeHeight);

    // 获取鼠标
    glfwSetInputMode(window_->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // 初始化OpenGL导入者
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
    // glad 初始化OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        delete window_;
        return nullptr;
    }
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false;
#endif
    if (err) throw std::runtime_error("Failed to initialize OpenGL loader!\n");

    // 设置Dear ImGui上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // 关联ImGui和渲染窗口
    ImGui_ImplGlfw_InitForOpenGL(window_->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // 启用深度测试和混合
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
};

// 渲染循环
void GL_base::run()
{
    while(!glfwWindowShouldClose(window_->window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window_->window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawGL();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window_->window);
    };
};

// OpenGL渲染
void GL_base::drawGL(){};

// UI渲染
void GL_base::drawUI()
{
    if(bDrawDemoUI){
        ImGui::ShowDemoWindow();
    };
};

// 错误回调函数
void GL_base::error_callback_impl(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// 鼠标按键回调函数
inline void GL_base::mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods)
{

}

// 键盘回调函数
void GL_base::key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 窗口大小回调函数
void GL_base::framebuffer_size_callback_impl(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 滚轮回调函数
inline void GL_base::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
{
}

// 鼠标回调函数
inline void GL_base::mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
{
}

void GL_base::ImGuiFileDialog_demo()
{
    // open Dialog Simple
    ImGui::Begin("File system");
    constexpr size_t kFilenameBufSize = 512;
    static char name_buf[kFilenameBufSize];

    ImGui::InputText("Filename##File", name_buf, kFilenameBufSize); ImGui::SameLine();
    ImGui::FileSelectButton("select_file", name_buf,kFilenameBufSize);

    constexpr size_t kFilenameBufSize2 = 512;
    static char name_buf2[kFilenameBufSize2];
    ImGui::InputText("Filename##Folder", name_buf2, kFilenameBufSize2); ImGui::SameLine();
    ImGui::FolderSelectButton("select_folder",name_buf2,kFilenameBufSize2);
    ImGui::End();
}

void GL_base::HelpMarker(const char* desc) 
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}