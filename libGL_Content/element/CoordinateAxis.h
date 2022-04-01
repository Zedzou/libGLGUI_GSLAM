#pragma once
#include "../../libGL_UI/utils/shader/glShader.hpp"
#include <memory>

namespace GLRenderer
{
    class CoordinateAxis
    {
        public:
            CoordinateAxis() = default;
            ~CoordinateAxis();
            void Init();
            void Draw(Eigen::Matrix4f model, const Eigen::Matrix4f &projection, const Eigen::Matrix4f &viewMatrix);
            void SetColor(const Eigen::Vector4f &c) { this->color = c;}

        private:
            float mScale = 0.2f;
            bool bInited=false;
            unsigned int VAO, VBO, EBO;
            std::unique_ptr<glUtil::Shader> mShader;
            Eigen::Vector4f color {0,1,1,1};
    };
}