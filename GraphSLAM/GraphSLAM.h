#ifndef GRAPHSLAM_H
#define GRAPHSLAM_H

#include "graphpredictor/GraphPredictor.h"
#include "../inseg_lib/include/lib.h"
#include <Tools/Logging.h>
#include <Tools/LogUtil.h>
#include "CameraParameters.h"
#include "config.h"
#include "graph.h"
#include <memory>
#include "tinyply.h"

#include <future>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

namespace GSLAM
{
    class GraphSLAM
    {
        public:
            explicit GraphSLAM(ConfigGSLAM* config, const CameraParameters& camParamD);
            ~GraphSLAM();

            // Frameprocessing
            bool LoadPredictModel();
            bool Initialize(const CameraParameters &camParamD);
            void ProcessFrame(int idx, const cv::Mat &colorImage, const cv::Mat &depthImage, const Eigen::Matrix4f *pose);
            bool &UseThread(){return mConfig->use_thread;}

            // Access
            inseg_lib::InSegLib *GetInSeg() { return inseg_.get(); }
            Graph *GetGraph() { return mGraph.get(); }
            ConfigGSLAM *GetConfig(){return mConfig;}

            GraphPredictor *GetGraphPred() { return mpGraphPredictor.get(); }
            json11::Json GetSceneGraph(bool full);

            enum SAVECOLORMODE 
            { 
                SAVECOLORMODE_RGB, 
                SAVECOLORMODE_SEGMENT, 
                SAVECOLORMODE_INSTANCE, 
                SAVECOLORMODE_SEMANTIC, 
                SAVECOLORMODEL_PANOPTIC
            };

            // IO
            void SaveModel(const std::string &output_folder) const;
            void SaveGraph(const std::string &output_folder, bool fullProb);
            void SaveSurfelsToPLY(int segment_filter, 
                                  const std::string &output_folder, 
                                  const std::string &output_name, 
                                  bool binary);
            void SaveNodesToPLY(int segment_filter, 
                                const std::string &output_folder, 
                                SAVECOLORMODE saveColorMode, 
                                bool binary=false);
            void SaveSurfelsToPLY(const std::string &output_folder, 
                                  const std::string &output_name, 
                                  const std::vector<std::shared_ptr<inseg_lib::Surfel>> &surfels, 
                                  bool binary);
            void RunFullPrediction(); // Graph
            void Start();             // Start backend
            void Stop();              // Stop backend
            void AddSelectedNodeToUpdate(int idx);
            
            // EIGEN_MAKE_ALIGNED_OPERATOR_NEW
            std::set<int> mLastUpdatedSegments{};

        private:

            // Graph
            ConfigGSLAM *mConfig;
            std::shared_ptr<Graph> mGraph;
        
            // Inseg Standard configuration. 实例分割设置
            std::shared_ptr<inseg_lib::InSegLib> inseg_;

            // 位姿和轨迹
            Eigen::Matrix4f pose_;
            std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f>> trajectory_{};

            // system
            size_t mTimeStamp=0;
            bool mbInitMap = false;

            // 图预测指针
            GraphPredictorPtr mpGraphPredictor; // Graph预测器
            std::vector< std::shared_ptr<inseg_lib::Surfel> > GetUpdatedSurfels();
            std::vector< std::shared_ptr<inseg_lib::Surfel> > FilterSegment( int segment_filter, const std::vector<std::shared_ptr<inseg_lib::Surfel>> &surfels);
    };
}
#endif