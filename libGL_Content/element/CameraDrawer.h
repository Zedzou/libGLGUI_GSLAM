// 相机类，用于在窗口中画相机

#ifndef GRAPHSLAM_CAMERADRAWER_H
#define GRAPHSLAM_CAMERADRAWER_H

#include "../../libGL_ContentBase/utils/shader/glShader.hpp"
#include <memory>

namespace GLRenderer
{
    class CameraDrawer
    {
        public:
            CameraDrawer()=default;
            ~CameraDrawer();
            void Init();
            void Draw(Eigen::Matrix4f model, const Eigen::Matrix4f& projection, const Eigen::Matrix4f& viewMatrix);
            void SetColor(const Eigen::Vector4f &c) { this->color = c;}
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        private:
            unsigned int VAO, VBO, EBO;
            std::unique_ptr<GLUtils::Shader> mShader;
            bool bInited=false;
            Eigen::Vector4f color {0,1,1,1};
    };
}

#endif //GRAPHSLAM_CAMERADRAWER_H
