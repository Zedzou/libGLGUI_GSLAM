#include "dataLoader.h"
using namespace GLRenderer;

dataLoader::dataLoader()
{
    readPose();
    readRGBpath();
    readDepthpath();
}


dataLoader::~dataLoader()
{

}

// 读取RGB路径
void dataLoader::readRGBpath()
{
    for(int i=0; i<frameNum+1; i++)
    {
        std::stringstream num;
        std::string pathName;
        num << std::setfill('0') << std::setw(6) << i;
        pathName = "/home/zed/Project/GraphSLAM/test_dataset/Test_2/sequence/" + prefix + num.str() + rgb_suffix;
        rgb_path.push_back(pathName);
    }
}

// 读取RGB路径
void dataLoader::readDepthpath()
{
    for(int i=0; i<frameNum+1; i++)
    {
        std::stringstream num;
        std::string pathName;
        num << std::setfill('0') << std::setw(6) << i;
        pathName = "/home/zed/Project/GraphSLAM/test_dataset/Test_2/sequence/" + prefix + num.str() + depth_suffix;
        depth_path.push_back(pathName);
    }
}

// 读取位姿
void dataLoader::readPose()
{

    for(int i=0; i<frameNum+1; i++)
    {
        // 路径
        std::stringstream num;
        std::string pathName;
        num << std::setfill('0') << std::setw(6) << i;
        pathName = "/home/zed/Project/GraphSLAM/test_dataset/Test_2/sequence/" + prefix + num.str() + pose_suffix;

        // 读取位姿
        Eigen::Matrix4f pose;
        pose.setIdentity();
        std::ifstream file(pathName);
        assert(file.is_open());
        if (file.is_open()) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    file >> pose(i, j);
            // pose.block<3, 1>(0, 3) *= 1000.0f;
            file.close();
        }
        poseMap[pathName] = pose;
    }

}