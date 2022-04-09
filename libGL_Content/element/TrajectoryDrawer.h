
#ifndef GL_CONTENT_TRAJECTORYDRAWER_H
#define GL_CONTENT_TRAJECTORYDRAWER_H

#include "../../libGL_ContentBase/utils/shader/glShader.hpp"
#include <memory>

namespace GLRenderer
{
    class TrajectoryDrawer
    {
        public:
            TrajectoryDrawer();
            ~TrajectoryDrawer();
            void Init();

        private:
            std::unique_ptr<GLUtils::Shader> mShader;
    };
}


#endif