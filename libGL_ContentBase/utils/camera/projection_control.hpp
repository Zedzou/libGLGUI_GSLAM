#pragma once
#include <glm/glm.hpp>

namespace GLGUI
{
    class ProjectionControl
    {
        public:
            ProjectionControl(unsigned int windowWidth, unsigned int windowHeight);
            ~ProjectionControl();

            void setSize(unsigned int windowWidth, unsigned int windowHeight)
            {
                width_ = windowWidth;
                height_ = windowHeight;
            }

            glm::mat4 projection_matrix() const;
            glm::mat4 projection_matrix(float fx, float fy, float cx, float cy, int w, int h, float n, float f);
            void draw_ui();
            void show();

            float fovy;
            bool bShowUI;
            float near;
            float far;

        private:
            unsigned int width_, height_;
            int projection_mode;
            float width;
    };
};