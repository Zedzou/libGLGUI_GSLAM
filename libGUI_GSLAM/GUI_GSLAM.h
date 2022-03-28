#pragma once

#include <functional>
#include <map>

#include "../libGUI/GUI.h"
#include "utils/shader/glShader.hpp"
#include "utils/camera/glCamera.hpp"
#include "utils/mesh/glMesh.hpp"
#include "utils/glUtils.hpp"

#ifdef COMPILE_WITH_FREETYPE
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H
#endif

namespace  GLGUI
{

    struct Character
    {
        GLuint TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint Advance;
    };

    struct task_element_t
    {
        GLFWWindowContainer* window_;
        int key_;
        const std::function<void()> no_id;
        std::string description;
        explicit task_element_t(GLFWWindowContainer* window, 
                                int key, std::function<void()> f, 
                                std::string description):
                                window_(window), 
                                key_(key), 
                                no_id(std::move(f)), 
                                description(std::move(description)) {};
    };

    // FPS
    class FPSManager
    {
        public:
            explicit FPSManager(double targetFPS=30.0)
            {
                fpsCounter_ = fps_ = targetFPS;
                diff_ = 0;
                fps_time_pre_ = fps_time_ = lasttime_ = -1;
            }

            // Prevent unstable image jumpping.
            void wait()
            {
                if(lasttime_ > 0)
                    while (glfwGetTime() < lasttime_ + maxPeriod_);
                lasttime_ = glfwGetTime();
            }

            void updateFPS()
            {
                stop();
                if (fps_time_pre_ < 0){
                    start();
                    return;
                }
                checkUpdate();
            }

            inline void start()
            {
                fps_time_pre_ = glfwGetTime();
            }

            inline void stop()
            {
                fps_time_ = glfwGetTime();
            }

            inline void checkUpdate(){
                diff_ += fps_time_-fps_time_pre_;
                fpsCounter_++;
                if (diff_ > 1.0) {
                    fps_ = double(fpsCounter_) / diff_;
                    fpsCounter_ = 0;
                    fps_time_pre_ += diff_;
                    diff_=0;
                }
            }

            double& getFPS(){return fps_;}
        
        private:
            double fps_time_pre_, fps_time_, targetFPS_, fps_;
            double maxPeriod_, lasttime_;
            unsigned short fpsCounter_;
            double diff_;
    };

    // GUI_GSLAM
    class GUI_GSLAM:public GUI_base
    {
        public:
            explicit GUI_GSLAM(const std::string &name, 
                               int width, 
                               int height, 
                               bool visible=true);
            virtual ~GUI_GSLAM();

            virtual void drawUI();
            virtual void drawGL();

            // 添加键盘回调函数
            template <typename Task> void registerKeyFunciton(GLFWWindowContainer* window, 
                                                              int key, 
                                                              Task func, 
                                                              const std::string &description = "")
            {
                registeredFunctions_.emplace_back(window,key,static_cast<std::function< void() >>(func), description);
            }

            void showRegisteredKeyFunction();
            void setPlotTracjectory(bool option) {bPlotTrajectory = option;}
            glUtil::Camera* GetCam(){return glCam.get();}
            void RenderText (const std::string &text, float pos_x, float pos_y, float scale, const glm::vec3 &color);
        
        protected:
            std::map<std::string, unsigned int>glBuffers, glVertexArrays, glFrameBuffers, glTextures;
            std::map<std::string, glUtil::Shader*> glShaders;
            std::map<std::string, glUtil::Model_base*> glObjests;
            std::map<GLchar, Character> Characters;
            std::map<int, bool> bKeyProcessFinished;
            FPSManager *fps_;

            bool bShowGrid, bShowFPS;
            bool bPlotTrajectory;

            // 线程池任务
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
            std::vector<task_element_t> registeredFunctions_;
            std::vector<glm::vec3> trajectories_;

            // 渲染文本
            void RenderText(GLuint VAO, 
                            GLuint VBO, 
                            glUtil::Shader* shader, 
                            std::string text, 
                            GLfloat x, 
                            GLfloat y, 
                            GLfloat scale, 
                            glm::vec3 color);
            virtual void processInput(GLFWwindow* window);
            virtual void basicInputRegistration();
            virtual void basicProcess();
            virtual void plot_trajectory(const glm::mat4 *projection);
            virtual void add_trajectory(float x, float y, float z, float interval=0.002);

            // return true is something changed
            bool mouseControl();
            void buildScreen();
            void buildCamera();
            void buildGrid();
            void buildText();
            void buildFreeType();
            void buildTrajectory();

            bool bShowCameraUI;
            void cameraUI();

            std::unique_ptr<glUtil::Camera> glCam;
            glm::vec3 camPose, camUp;
            float yaw, pitch, fov, fovMax, camSpeed;

        private:
            int init();
    };

}; // end of the namespace  GLGUI
