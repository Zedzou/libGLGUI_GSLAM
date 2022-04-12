#ifndef SURFELDRAWER_H
#define SURFELDRAWER_H

#include <string>
#include <memory>
#include <set>
#include "../../libGL_ContentBase/utils/shader/glShader.hpp"
#include "./ModelRenderer/define.h"
#include "../../inseg_lib/include/surfel.h" // inseg_lib

namespace GLRenderer
{
    class SurfelDrawer
    {
        public:
            SurfelDrawer()=default;
            ~SurfelDrawer();

            void Init(std::function<void(Eigen::Vector3f&, const inseg_lib::Surfel*)> getcolor);
            void Update(const std::vector<std::shared_ptr<inseg_lib::Surfel>>& surfels);
            void Draw(const Eigen::Matrix4f& projection, const Eigen::Matrix4f& viewMatrix);
            float mDiffuseStrength=1.f;

        private:
            bool bInited=false;
            std::function< void(Eigen::Vector3f&, const inseg_lib::Surfel*) > pGetColor = nullptr;
            std::unique_ptr<GLUtils::Shader> mShader;
            void UpdateBuffer(size_t newSize, bool force=false);
            unsigned int VAO{}, VBO{};
            size_t mPointSize=0, mBufferSize = 1e3;
    };
}


#endif