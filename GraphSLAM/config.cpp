#include "config.h"
using namespace GSLAM;

ConfigGSLAM::ConfigGSLAM(const std::string &path)
{
    if(!path.empty()) pth=path;
    std::fstream f(pth,std::ios::in);
    if(!f.is_open())return;
}

ConfigGSLAM::~ConfigGSLAM(){}

ConfigGSLAM::ConfigGSLAM()
{
    use_thread = true;
    use_fusion = true;

    // Use graph predict or not
    graph_predict = true;

    // model path
    pth_model = "/home/sc/research/PersistentSLAM/python/3DSSG/experiments/exp10_2_1/traced/";
    pth_model = "/home/sc/research/PersistentSLAM/python/3DSSG/experiments/exp10_4/traced/";
    pth_model = "/home/sc/research/PersistentSLAM/python/3DSSG/experiments/exp10_5/traced/";

    filter_num_node = 512;
    n_pts = 512;
    neighbor_margin=500; // mm
    update_thres_node_size = 0.2; // Only update the information of a node if the node has size change to N percent
    update_thres_time_stamp = 50; // Update a node if its time stamp is older than n
    pth = "./config_graph.txt";
}