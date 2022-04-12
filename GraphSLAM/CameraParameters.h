#ifndef _H_PSLAM_CAMERA_PARAMETERS
#define _H_PSLAM_CAMERA_PARAMETERS
namespace GSLAM 
{
    struct CameraParameters 
    {
        float cx, cy;
        float fx, fy;
        unsigned short width, height;

        void Set(unsigned int width, unsigned int height, float fx, float fy, float cx, float cy) 
        {
            this->width = width;
            this->height = height;
            this->fx = fx;
            this->fy = fy;
            this->cx = cx;
            this->cy = cy;
        }
    };
}
#endif