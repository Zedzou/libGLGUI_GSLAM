#pragma once
#include <memory>
#include <map>
#include <mutex>
#include "graphpredictor/MemoryBlock.h"

namespace GSLAM
{
    class Edge
    {
        inline static std::string None() {return "none";}
        inline static std::string Same() {return "same part";}

        Edge() = default;
        Edge(Edge& edge);

        // 进出节点
        int nodeFrom{}, nodeTo{};

        // Shared
        std::map <std::string, float> labelProp; // 标签
        std::map <std::string, float> mClsWeight; // 权重
        std::map <std::string, std::shared_ptr<MemoryBlock>> mFeatures;

        mutable std::mutex mMutex, mMutLabel; // 互斥锁
        std::string GetLabel() const;
        void UpdatePrediction(const std::map<std::string, float>& prop, bool fusion);
        std::string label = None();
    };
    typedef std::shared_ptr<Edge> EdgePtr;
    static inline EdgePtr MakeEdgePtr(){return std::make_shared<Edge>();}
}