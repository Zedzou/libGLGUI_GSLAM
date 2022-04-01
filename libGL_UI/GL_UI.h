#pragma once

#include <functional>
#include <map>

#include "../libGL/GL.h"
#include "utils/shader/glShader.hpp"
#include "utils/camera/glCamera.hpp"
#include "utils/mesh/glMesh.hpp"
#include "utils/glUtils.hpp"

// FreeType
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H

namespace GLRenderer
{
    struct Character
    {
        GLuint TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint Advance;
    };

    // 线程池任务定义
    struct task_element_t
    {
        GLFWWindowContainer* window_;
        int key_;
        const std::function<void()> no_id;
        std::string description;
        explicit task_element_t(GLFWWindowContainer* window, 
                                int key, 
                                std::function<void()> f, 
                                std::string description):window_(window), key_(key),no_id(std::move(f)), description(std::move(description)) {};
    };

    // FPS管理器
    class FPSManager
    {
        public:

            explicit FPSManager(double targetFPS = 30.0){
                fpsCounter_  = fps_ = targetFPS;
                diff_ = 0;
                fps_time_pre_ = fps_time_ = lasttime_ = -1;
                targetFPS_ = targetFPS;
                maxPeriod_ = 1.0 / targetFPS_;
            };

            // Prevent unstable image jumpping.
            void wait(){
                if(lasttime_ > 0)
                    while (glfwGetTime() < lasttime_ + maxPeriod_);
                lasttime_ = glfwGetTime();
            }

            void updateFPS(){
                stop();
                if(fps_time_pre_ < 0) { // init
                    start();
                    return;
                }
                checkUpdate();
            }

            inline void start(){
                fps_time_pre_ = glfwGetTime();
            }
            
            inline void stop(){
                fps_time_ = glfwGetTime();
            };

            inline void checkUpdate(){
                diff_ += fps_time_-fps_time_pre_;
                fpsCounter_++;
                if (diff_ > 1.0) {
                    fps_ = double(fpsCounter_) / diff_;
                    fpsCounter_ = 0;
                    fps_time_pre_ += diff_;
                    diff_=0;
                }
            };

            double& getFPS(){return fps_;}

        private:
            double fps_time_pre_, fps_time_, targetFPS_, fps_;
            double maxPeriod_, lasttime_;
            unsigned short fpsCounter_;
            double diff_;
    };

    // GL_UI
    class GL_UI : public GL_base
    {
        public:
            // 构造析构
            explicit GL_UI (const std::string &name, int width, int height, bool visible=true);
            virtual ~GL_UI ();

            // UI渲染和渲染内容
            virtual void drawUI();
            virtual void drawGL();
            void showRegisteredKeyFunction();
            void basicProcess();
            glUtil::Camera* GetCam(){return glCam.get();}

            // 添加键盘回调函数
            template <typename Task> void registerKeyFunciton(GLFWWindowContainer* window, int key, Task func, const std::string &description = ""){
                registeredFunctions_.emplace_back(window, key, static_cast<std::function< void() >>(func), description);
            }

        protected:
            std::map<GLchar, Character> Characters;
            std::map<int, bool> bKeyProcessFinished;
            std::map<std::string, glUtil::Shader*> glShaders;
            std::map<std::string, glUtil::Model_base*> glObjests;
            std::map<std::string, unsigned int> glBuffers, glVertexArrays, glFrameBuffers, glTextures;
            std::vector<glm::vec3> trajectories_;

            virtual void processInput(GLFWwindow* window);
            virtual void basicInputRegistration();
            void cameraUI();
            bool mouseControl();
            void add_trajectory(float x, float y, float z, float interval);
            void plot_trajectory(const glm::mat4 *projection);
            void RenderText(const std::string &text, float pos_x, float pos_y, float scale, const glm::vec3 &color);
            void RenderText(GLuint VAO, GLuint VBO, glUtil::Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

            // 着色器编译
            void buildText();
            void buildTrajectory();
            void buildFreeType();
            void buildCamera();
            void buildScreen();
            void buildGrid();

            FPSManager *fps_;
            bool bShowGrid, bShowFPS, bPlotTrajectory, bShowCameraUI;

            // 相机位姿
            std::unique_ptr<glUtil::Camera> glCam;
            glm::vec3 camPose, camUp;
            float yaw, pitch, fov, fovMax, camSpeed;

            // 注册键盘按键函数线程池
            struct task_element_t
            {
                GLFWWindowContainer* window_;
                int key_;
                const std::function<void()> no_id;
                std::string description;
                explicit task_element_t(GLFWWindowContainer* window, 
                                        int key, 
                                        std::function<void()> f, 
                                        std::string description):
                                        window_(window),
                                        key_(key),
                                        no_id(std::move(f)),
                                        description(std::move(description)){};
            };
            std::vector<task_element_t> registeredFunctions_; // 注册函数任务池
            
        private:
            int init();
    };

};
