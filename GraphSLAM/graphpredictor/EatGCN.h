#ifndef GRAPHSLAM_EATGCN_H
#define GRAPHSLAM_EATGCN_H

#include "OnnxModelBase.h"
#include "ParamLoader.h"
#include "MemoryBlock.h"
#include "UtilDataProcessing.h"
#include <onnxruntime_cxx_api.h>

namespace GSLAM 
{
    class EatGCN : private GSLAM::ONNX::OnnxModelBase 
    {
        public:
            using GSLAM::ParamLoader::mModelParams;
            using GSLAM::ParamLoader::mParams;
            using GSLAM::ParamLoader::mLabels;
            using GSLAM::ParamLoader::mRelationships;

            enum OP_NAME 
            {
                OP_ENC_OBJ, OP_ENC_REL, OP_CLS_OBJ, OP_CLS_REL
            };
            enum GCN_OP_NAME 
            {
                OP_GCN_ATTEN, OP_GCN_PROP
            };

            std::map<OP_NAME, std::string> mOp2Name;
            std::map<GCN_OP_NAME, std::string> mGOp2Name;
            EatGCN(std::string path, Ort::MemoryInfo *memoryInfo, bool verbose=false);
            std::vector<Ort::Value> ComputeGCN(GCN_OP_NAME name, size_t level, const std::vector<float *> &data, const std::vector<std::vector<int64_t>> &dims);
            std::vector<Ort::Value> ComputeGCN(GCN_OP_NAME name, size_t level, const std::vector<Ort::Value> &inputs);
            std::vector<Ort::Value>
            Compute(OP_NAME name, const std::vector<float *> &data, const std::vector<std::vector<int64_t>> &dims);
            std::vector<Ort::Value> Compute(OP_NAME name, std::vector<Ort::Value> &inputs);
            std::tuple<MemoryBlock2D, MemoryBlock2D> Run( const MemoryBlock3D &input_nodes, const MemoryBlock2D &descriptor, const MemoryBlock2D &edge_index);

        protected:
            using GSLAM::ONNX::OnnxModelBase::Run;

        private:
            bool bVerbose=false;
            static MemoryBlock2D ConcatObjFeature(size_t n_node, size_t dim_obj_feature, Ort::Value &obj_feature, const MemoryBlock2D &descriptor);
            void InitModel() override;
    };
}

#endif //GRAPHSLAM_EATGCN_H
