#pragma once

#include "../libGL_UI/GL_UI.h"
#include "element/CameraDrawer.h"
#include "eigen_glm.h"
#include "element/CoordinateAxis.h"

namespace GLRenderer
{
    class GL_Content : public GL_UI
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

        private:
            void drawGL() override;
            void drawUI() override;
            void MainUI();
            void process();
            void mainProcess();

            GLRenderer::CameraDrawer mCameraDrawer;
            GLRenderer::CoordinateAxis mCoordinateAxis;

            int mProcessMode = PROCESS_STOP;
            bool bDrawCam=true; //是否画相机
            bool bDrawCoordinateAxis = true;
            bool bProcess = true;
            bool bNeedUpdate = false;
            bool bRenderSurfel=true;
            
    }; // end of class Renderer

} // end of namespace RD