#pragma once

#include "imgui.h"

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING)
#define GLFW_INCLUDE_NONE
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>
#include <utility>


namespace GLGUI
{
    // GLFW渲染窗口定义
    struct GLFWWindowContainer
    {
        GLFWwindow* window;
        std::string name_;
        int width, height;
        int runtimeWidth{}, runtimeHeight{};
        GLFWWindowContainer(std::string name, int width, int height, float nearPlane=0.4, float farPlane=500.f, bool visible=true)
        :name_(std::move(name)), width(width), height(height)
        {
            if (!visible){
                glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            }
            window = glfwCreateWindow(width, height, name_.c_str(), nullptr, nullptr);
        }

        ~GLFWWindowContainer(){
            glfwDestroyWindow(window);
        };
    };

    // 渲染内容基础类
    class GUI_base
    {
        public:
            static GUI_base& getInstance(){return *ptrInstance;} // 静态函数返回渲染基础类
            GUI_base();
            virtual ~GUI_base();

            // 初始化窗口
            void initWindow(const std::string &name, int width, int height, bool visible=true);
            // 返回GLFW窗口
            GLFWWindowContainer* GetWindow(){return window_;}

            // 虚函数: 运行，UI显示，GL渲染
            virtual void run();
            virtual void drawUI();
            virtual void drawGL();

        protected:
            bool bDrawDemoUI;
            static GUI_base* ptrInstance;
            GLFWWindowContainer* window_;

            // 键盘回调函数
            virtual void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
                getInstance().key_callback_impl(window, key, scancode, action, mods);
            };

            // 鼠标按键回调函数
            virtual void mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
                getInstance().mouse_button_callback(window, button, action, mods);
            };

            // 鼠标位置回调函数
            virtual void mouse_callback_impl(GLFWwindow* window, double xpos, double ypos);
            static void mouse_callback(GLFWwindow* window, double xpos, double ypos){
                getInstance().mouse_callback_impl(window, xpos, ypos);
            };

            // 鼠标滚轮回调函数
            virtual void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset);
            static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
                getInstance().scroll_callback_impl(window, xoffset, yoffset);
            };

            // 渲染画面大小回调函数
            virtual void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height);
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
                getInstance().framebuffer_size_callback_impl(window, width, height);
            };

            // 错误或调函数
            virtual void error_callback_impl(int error, const char* description);
            static void error_callback(int error, const char* description){
                getInstance().error_callback_impl(error, description);
            };

            // 对话框demo
            void ImGuiFileDialog_demo();
            std::vector<std::string> ImGuiFileSelect(const std::string& name="Browse", int num=1);

            void HelpMarker(const char* desc);

        private:
            std::string glsl_version;
            void init();
    };
};