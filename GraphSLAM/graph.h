#ifndef GRAPH_H
#define GRAPH_H

#include "../inseg_lib/include/surfel.h"
#include "../inseg_lib/include/map_segments.h"
#include "node.h"
#include "edge.h"
#include "mutex"
#include "config.h"
#include "Tools/thread_pool.hpp"

#include <unordered_set>
#include <unordered_map>
#include <Eigen/Dense>
#include <iostream>

namespace GSLAM
{
    class Graph : public inseg_lib::SegmentsInterface
    {
        public:
            Graph(const ConfigGSLAM *configGslam, bool useThread);
            ~Graph() {}

            int Add(const SurfelPtr &surfel) override;
            void Merge(const int seg_idx_to, const int seg_idx_from) override;
            void Clear() override;
            void Update(const SurfelPtr surfel, const Eigen::Vector3f &pos, const Eigen::Vector3f &normal) override;
            void CheckConnectivity(float margin);

            int UpdateLabel(const SurfelPtr surfel, const int label) override;
            int AddEdge(int from, int to, const std::shared_ptr<Edge> &edge);
            int AddEdge(const std::shared_ptr<Edge> &edge);

            void UpdateSelectedNodes(const std::unordered_set<int> &filtered_selected_nodes, const size_t time, const bool force);
            void Wait();

            // thread
            std::map< int, NodePtr > nodes;
            std::map< std::pair<int, int>, EdgePtr > edges;
            std::set< int > nodes_to_update;
            std::mutex mMutNode; // 处理节点的互斥锁
            std::mutex mMutEdge; // 处理边的互斥锁
            std::mutex mMutThis;

        private:
            bool mbThread; // 是否采用线程
            std::unique_ptr<tools::TaskThreadPool> mPools; // 线程池
            const ConfigGSLAM *mConfigGSLAM; // 模型配置
            void RemoveSurfelFromNode(const SurfelPtr &surfel);
            void RemoveNode(int idx);
            void RemoveEdge(const std::pair<int, int> &pair);   
    };
}


#endif