#pragma once

// 控制渲染窗口的相机

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace GLGUI
{
    // 定义相机可能的移动方式
    enum Camera_Movement
    {
        UP,
        DOWN,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // 相机默认值
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED =  2.5f;
    const float TRANSLATIONSENSITIVITY =  0.01f;
    const float ROTATIONSENSITIVITY = 0.2f;
    const float SCROLLSENSITIVITY = 0.2f;

    // 相机控制基础类
    class CameraControlBase
    {
        public:
            bool bShowUI;
            glm::vec3 Position;
            CameraControlBase():bShowUI(false), Position(0, 0, 0), WorldUp(0, 0, 0){}
            virtual void mouse(const glm::vec2& p, int button, bool down)=0;
            virtual void drag(const glm::vec2& p, int button)=0;
            virtual void scroll(const glm::vec2& rel)=0;
            virtual glm::mat4 GetViewMatrix()=0;

            void show(){bShowUI=true;}
            virtual void drawUI()
            {
                if(!bShowUI) return;
                ImGui::Begin("Camera Control", &bShowUI, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::DragFloat3("Position", &Position[0]);
            }
            
            // 设置相机位姿
            virtual void setCamPose(float pitch, 
                                    float yaw, 
                                    float row, 
                                    float x, 
                                    float y, 
                                    float z)
            {
                setPosition(x,y,z);
            }

            // 获取相机姿态
            virtual void getCamPose(float &pitch, 
                                    float &yaw, 
                                    float &row, 
                                    float &x, 
                                    float &y, 
                                    float &z) 
            {
                x = Position.x;
                y = Position.y;
                z = Position.z;
            }
            
            // 获取相机姿态
            virtual void getCamPose(float &x, float &y, float &z) 
            {
                x = Position.x;
                y = Position.y;
                z = Position.z;
            }
            
            // 获取相机姿态
            void setPosition(float x, float y, float z)
            {
                Position.x = x;
                Position.y = y;
                Position.z = z;
            }

        protected:
            glm::vec3 WorldUp;
    };

    // 相机控制类
    class CameraControl:public CameraControlBase
    {
        public:
            //相机属性
            glm::vec3 Front{};
            glm::vec3 Up{};
            glm::vec3 Left{};

            // 欧拉角
            float Pitch; //x
            float Yaw; // y
            float Row{}; // z

            // 相机选择
            float MovementSpeed;
            float RotationSensitivity;
            float TranslationSensitivity;
            float ScrollSensitivity;

            // 控制器
            explicit CameraControl(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                                   float yaw = YAW,
                                   float pitch = PITCH):Pitch(pitch),
                                                        Yaw(yaw),
                                                        MovementSpeed(SPEED),
                                                        RotationSensitivity(ROTATIONSENSITIVITY),
                                                        TranslationSensitivity(TRANSLATIONSENSITIVITY),
                                                        ScrollSensitivity(SCROLLSENSITIVITY)
            {
                Position = position;
                WorlfUp = up;
                left_button_down = middle_button_down = right_button_down = false;
                updateCameraVectors();
            }

            // 获取视角矩阵
            glm::mat4 GetViewMatrix() override
            {
                return glm::lookAt(Position, Position+Front, Up);
            }

            void mouse(const glm::vec2& p, int button, bool down) override
            {
                if (button ==0){
                    left_button_down = down;
                }
                if (button == 2){
                    middle_button_down = down;
                }
                if(button == 1){
                    right_button_down = down;
                }
                drag_last_pos = o;
            }

            // 拖拽
            void drag(const glm::vec2& p, int button) override
            {
                glm::vec2 rel = p - drag_last_pos;
                if(left_button_down){
                    ProcessKeyboard(Camera_Movement::LEFT, rel[0]*TranslationSensitivity);
                    ProcessKeyboard(Camera_Movement::UP, -rel[1]*TranslationSensitivity);
                }

                if (right_button_down) {
                    ProcessMouseMovement(-rel.x, rel.y);
                }

                if (middle_button_down) {
                   Position = glm::angleAxis<float>(float(theta+M_PI_2), glm::vec3(0,0,1)) * glm::vec3(-rel[0],rel[1],0.f)*glm::vec3(distance*1e-3f);
                }
                drag_last_pos = p;
            }

            // 滚轮
            void scroll(const glm::vec2 &rel) override 
            {
                ProcessKeyboard(BACKWARD, 1 * rel.x * ScrollSensitivity);
            }

            // UI
            void drawUI() override 
            {
                if(!bShowUI)return;
                ImGui::Begin("Camera Control", &bShowUI, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::DragFloat3("Position",&Position[0]);
                ImGui::DragFloat3("Front",&Front[0]);
                ImGui::DragFloat3("Left",&Left[0]);
                ImGui::DragFloat3("Up",&Up[0]);
                ImGui::DragFloat("Yaw",&Yaw);
                ImGui::DragFloat("Pitch",&Pitch,0.1f, -89.f,89.f);
                ImGui::DragFloat("Rotation Sensitivity",&RotationSensitivity, 0.01f,0.001f,5.f);
                ImGui::DragFloat("Translation Sensitivity",&TranslationSensitivity, 0.01f,0.001f,5.f);
                ImGui::DragFloat("Scroll Sensitivity",&ScrollSensitivity, 0.01f,0.001f,5.f);
                ImGui::End();
            }

            // 设置相机位姿态
            void setCamPose(float pitch, float yaw, float row, float x, float y, float z) override 
            {
                Pitch = pitch;
                Yaw = yaw;
                Row = row;

                Position.x = x;
                Position.y = y;
                Position.z = z;

                //if (constrainPitch)
                {
                    if (Pitch > 89.0f)
                        Pitch = 89.0f;
                    if (Pitch < -89.0f)
                        Pitch = -89.0f;
                }
                //printf("Row,Yaw,Pitch,x,y,z: %f %f %f %f %f %f\n", Row, Yaw, Pitch, Position.x, Position.y, Position.z);
                updateCameraVectors();
            }

            // 获取相机姿态
            void getCamPose(float &pitch, float &yaw, float &row, float &x, float &y, float &z) override 
            {
                pitch = Pitch;
                yaw = Yaw;
                row = Row;
                x = Position.x;
                y = Position.y;
                z = Position.z;
            }

            // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
            void ProcessKeyboard(Camera_Movement direction, float deltaTime)
            {
                //printf("MovingSpeed: %f\n", MovementSpeed);
                float velocity = MovementSpeed * deltaTime;
                if (direction == UP)
                    Position -= Up * velocity;
                if (direction == DOWN)
                    Position += Up * velocity;
                if (direction == FORWARD)
                    Position -= Front * velocity;
                if (direction == BACKWARD)
                    Position += Front * velocity;
                if (direction == LEFT)
                    Position += Left * velocity;
                if (direction == RIGHT)
                    Position -= Left * velocity;
            }

            // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
            void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
            {
                xoffset *= RotationSensitivity;
                yoffset *= RotationSensitivity;

                Yaw   += xoffset;
                Pitch += yoffset;

                // Make sure that when pitch is out of bounds, screen doesn't get flipped
                if (constrainPitch)
                {
                    if (Pitch > 89.0f)
                        Pitch = 89.0f;
                    if (Pitch < -89.0f)
                        Pitch = -89.0f;
                }

                // Update Front, Right and Up Vectors using the updated Euler angles
                updateCameraVectors();
            }

            void setMovingSpeed(float value)
            {
                MovementSpeed = value;
            }
        
        private:
            bool left_button_down, middlw_button_down, right_button_down;
            glm::vec2 drag_last_pos{};

            /**
             * Calculates the front vector from the Camera's (updated) Euler Angles
             * More details see
             * http://www.songho.ca/opengl/gl_transform.html
             * and
             * https://learnopengl.com/Getting-started/Camera
             */
            void updateCameraVectors()
            {
                glm::vec3 front;
                auto sa = sin(glm::radians(Pitch));
                auto sb = sin(glm::radians(Yaw));
                auto ca = cos(glm::radians(Pitch));
                auto cb = cos(glm::radians(Yaw));

                // ry*rx*rz
                front.x = sb*ca;
                front.y = -sa;
                front.z = cb*ca;
                Front = glm::normalize(front);
                Left = -glm::normalize(glm::cross(Front, WorldUp));
                Up    = glm::normalize(glm::cross(Front, Left));
            }
    };

    class ArcCameraControl:public CameraControlBase
    {
        public:
            explicit ArcCameraControl(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                                      float yaw = YAW,
                                      float pitch = PITCH):theta(0.0f), phi(-60.0f*M_PI/180.0f)
            {
                Position = position;
                WorldUp = up;
                left_button_down = middle_button_down = right_button_down = false;
            };

            ArcCameraContorl(float posX, 
                             float posY, 
                             float posZ, 
                             float upX, 
                             float upY, 
                             float upZ, 
                             float yaw, 
                             float pitch):theta(0.0f), phi(-60.0f * M_PI / 180.0f)
            {
                Position = glm::vec3(posX, posY, posZ);
                WorldUp = glm::vec3(upX, upY, upZ);
                left_button_down = middle_button_down = right_button_down = false;
            };

            void SetDistance(float v) {distance = v;}

            // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
            glm::mat4 GetViewMatrix() override
            {
                glm::vec3 offset = rotation() * glm::vec3(distance, 0.0f, 0.0f);
                glm::vec3 eye = Position + offset;

                glm::mat4 mat = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]), glm::vec3(Position[0], Position[1], Position[2]), glm::vec3(0.0f, 0.0f, 1.0f));
                return mat;
            };

            void getCamPose(float &x, float &y, float &z) override
            {
                glm::vec3 offset = rotation() * glm::vec3(distance, 0.0f, 0.0f);
                glm::vec3 eye = Position + offset;
                x = eye[0];
                y = eye[1];
                z = eye[2];
            };

            void mouse(const glm::vec2& p, int button, bool down) override
            {
                if (button == 0) {
                    left_button_down = down;
                }
                if (button == 2) {
                    middle_button_down = down;
                }
                if (button == 1)
                    right_button_down = down;
                drag_last_pos = p;
            };

            void drag(const glm::vec2& p, int button) override  
            {
                glm::vec2 rel = p - drag_last_pos;
                if (left_button_down) {
                    theta -= rel[0] * 0.01f;
                    phi -= rel[1] * 0.01f;

                    phi = std::min(M_PI_2 - 0.01, std::max(-M_PI_2 + 0.01, phi));
                };

                if (middle_button_down) {
                    Position += glm::angleAxis<float>(float(theta+M_PI_2), glm::vec3(0,0,1)) * glm::vec3(-rel[0],rel[1],0.f)*glm::vec3(distance*1e-3f);
                };

                if(right_button_down) 
                {
                    Position[2] += rel[1]*0.01;
                };

                drag_last_pos = p;
            }

            void scroll(const glm::vec2 &rel) override 
            {
                if (rel[0] > 0) {
                    distance = distance * 0.9f;
                } else if (rel[0] < 0) {
                    distance = distance * 1.1f;
                }
                distance = std::max(0.1, distance);
            }

            glm::quat rotation() const { return glm::angleAxis<float>(theta, glm::vec3(0,0,1)) * glm::angleAxis<float>(phi, WorldUp); }

        private:
            bool left_button_down, middle_button_down, right_button_down;
            double distance{};
            glm::vec2 drag_last_pos{};
            double theta;
            double phi;
    };

};