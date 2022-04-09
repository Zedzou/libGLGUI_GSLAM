#pragma once
#include "../../libGL_ContentBase/utils/shader/glShader.hpp"
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

        private:
            bool bInited=false;
            unsigned int VAO, VBO, EBO;
            std::unique_ptr<GLUtils::Shader> mShader;
    };
}