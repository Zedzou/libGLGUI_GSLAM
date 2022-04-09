#pragma once
#include <memory>
#include "camera_control.hpp"
#include "projection_control.hpp"

#ifdef COMPILE_WITH_EIGEN
#include <Eigen/Core>
#endif

namespace glUtil
{
    class Camera
    {
        public:
            bool bShowUI = false;
            std::unique_ptr<GLGUI::CameraControlBase> camera_control_; // 相机控制
            std::unique_ptr<GLGUI::ProjectionControl> projection_control_; // 投影控制

            // 构造函数
            explicit Camera(int windowWidth, int windowHeight, glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0)
            {
                bShowUI=false;
                camera_control_.reset(new GLGUI::CameraControl(position, up, yaw, pitch));
                projection_control_.reset(new GLGUI::ProjectionControl(windowWidth, windowHeight)); 
            };

            // 设置窗口尺寸
            void setSize(int windowWidth, int windowHeight)
            {
                projection_control_->setSize(windowWidth, windowHeight);
            }
            
            // 鼠标控制
            bool mouse_control()
            {
                bool changed = false;

                // 获取当前鼠标的位置
                ImGuiIO& io = ImGui::GetIO();
                auto mouse_pos = ImGui::GetMousePos();
                glm::vec2 p(mouse_pos.x, mouse_pos.y);

                for (int i = 0; i < 3; i++) 
                {
                    if (ImGui::IsMouseClicked(i)) {
                        changed=true;
                        camera_control_->mouse(p, i, true);
                    }
                    if (ImGui::IsMouseReleased(i)) {
                        changed=true;
                        camera_control_->mouse(p, i, false);
                    }
                    if (ImGui::IsMouseDragging(i)) {
                        changed=true;
                        camera_control_->drag(p, i);
                    }

                }
                camera_control_->scroll(glm::vec2(io.MouseWheel, io.MouseWheelH));
                if(io.MouseWheel != 0 || io.MouseWheelH != 0 )changed=true;
                return changed;
            }
        
            // 获取相机的位姿
            Eigen::Matrix4f getCameraPose() 
            {
                return Eigen::Matrix4f(glm::value_ptr(camera_control_->GetViewMatrix()));
            }
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            // 画相机UI
            void drawUI()
            {
                if(!bShowUI) return;
                ImGui::Begin("GL Camera", &bShowUI);
                if(ImGui::BeginMenu("Camera", bShowUI))
                {
                    ImGui::MenuItem("Projection Control", nullptr, &projection_control_->bShowUI);
                    ImGui::MenuItem("Camera Control", nullptr, &camera_control_->bShowUI);
                    ImGui::EndMenu();
                }
                ImGui::End();
                projection_control_->draw_ui();
                camera_control_->drawUI();
            }

        private:

    };
};