#pragma once

#include "../libGL_ContentBase/GL_ContentBase.h"
#include "../GraphSLAM/GraphSLAM.h"
#include "element/CameraDrawer.h"
#include "element/CoordinateAxis.h"
#include "element/SurfelDrawer.h"
#include "dataLoader/dataLoader.h"
#include "opencv2/core.hpp"
#include "label_NYU40.h"
#include "eigen_glm.h"
#include "../inseg_lib/include/map.h" // inseg_lib

namespace GLRenderer
{
    class GL_Content : public GL_ContentBase
    {
        public:
            GL_Content(const std::string& WindowName, int width, int height);
            ~GL_Content();
            
            enum PROCESS
            {
                PROCESS_STOP, 
                PROCESS_ONCE, 
                PROCESS_CONTINUE
            };

            enum COLORTYPE // 颜色类型
            {
                COLOR_LABEL,
                COLOR_PHONG,
                COLOR_NORMAL,
                COLOR_COLOR,
                COLOR_UPDATED,
                COLOR_SEMANTIC,
                COLOR_INSTANCE,
                COLOR_PANOPTIC,
            };

        private:
            void drawGL() override;
            void drawUI() override;
            void MainUI();
            void process();
            void mainProcess();
            void GetSurfelColor(Eigen::Vector3f& surfel_color, const inseg_lib::Surfel* surfel);

            // 测试相机位姿
            dataLoader* DataLoader;
            std::vector<std::string> fileName;
            std::map<std::string, Eigen::Matrix4f> PoseMap;

            // 各组件模块
            GLRenderer::CameraDrawer mCameraDrawer;
            GLRenderer::CoordinateAxis mCoordinateAxis;
            GLRenderer::SurfelDrawer mSurfelDrawer;
            GSLAM::GraphSLAM *mpGraphSLAM; // GraphSLAM

            int mProcessMode = PROCESS_STOP;
            bool bDrawCam=true; //是否画相机
            bool bDrawCoordinateAxis = true;
            bool bProcess = true;
            bool bNeedUpdate = false;
            bool bRenderSurfel=true;
            int mColorType = COLOR_COLOR;
            bool bFaceCulling=true;
            
    }; // end of class Renderer

} // end of namespace RD