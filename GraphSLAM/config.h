#ifndef GSLAM_CONFIG_H
#define GSLAM_CONFIG_H

#include <string>
#include <fstream>
#include "../inseg_lib/include/inseg_config.h"
#include "../inseg_lib/include/main_config.h"
#include "../inseg_lib/include/map_config.h"
#include "../inseg_lib/include/segmentation_config.h"

namespace GSLAM
{
    struct ConfigGSLAM
    {
        ConfigGSLAM();
        explicit ConfigGSLAM(const std::string &path);
        ~ConfigGSLAM();

        bool use_thread;
        bool use_fusion;
        bool graph_predict; // 是否进行图预测
        int filter_num_node;
        std::string pth_model; // 模型路径

        size_t n_pts;
        float neighbor_margin; // mm
        float update_thres_node_size; // Only update the information of a node if the node has size change to N percent
        int update_thres_time_stamp; // Update a node if its time stamp is older than n
        std::string pth;

        inseg_lib::InSegConfig inseg_config;
        inseg_lib::MainConfig main_config;
        inseg_lib::MapConfig map_config;
        inseg_lib::SegmentationConfig segmentation_config;
    };
}

#endif