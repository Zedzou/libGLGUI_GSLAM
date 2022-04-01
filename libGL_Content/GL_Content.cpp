#include "GL_Content.h"
using namespace GLRenderer;

GL_Content::GL_Content(const std::string& WindowName, int width, int height):GLRenderer::GL_UI("CameraShow", 1280, 700)
{
    mCameraDrawer.Init();
    mCoordinateAxis.Init();
}

GL_Content::~GL_Content()
{

}

// UI设计(virtual override)
void GL_Content::drawUI()
{
    fps_->wait();
    MainUI();
    cameraUI();
    glCam->drawUI();
    mouseControl();
}

// gl渲染(virtual override)
void GL_Content::drawGL()
{
    glClearColor(255.00f, 255.00f, 255.00f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);

    processInput(window_->window); // 窗口输入
    basicProcess(); // 网格、FPS、轨迹
    mainProcess();
}

// mainProcess
void GL_Content::mainProcess()
{
    auto io  = ImGui::GetIO();
    size_t windowWidth = io.DisplaySize.x, windowHeight = io.DisplaySize.y;
    glViewport(0, 0, windowWidth, windowHeight);

    // Draw3D
    const Eigen::Matrix4f eigen_proj = GLM2E<float, 4, 4>(glCam->projection_control_->projection_matrix());
    const Eigen::Matrix4f eigen_vm   = GLM2E<float, 4, 4>(glCam->camera_control_->GetViewMatrix());
    glEnable(GL_DEPTH_TEST);
    
    // 获取pose
    Eigen::Matrix4f pose;
    pose << 0.989695, -0.00490942, 0.143107,  0.0,
            0.141982, -0.0959712, -0.985206,  0.0,
            0.0185709,0.995372,   -0.0942851, 0.0,
            0,0,0,1;

    // 画相机
    if(bDrawCam) {
        mCameraDrawer.SetColor({0, 1, 1, 1});// green
        mCameraDrawer.Draw(pose, eigen_proj, eigen_vm);
    }

    // 画座标轴
    if(bDrawCoordinateAxis ){
        mCoordinateAxis.SetColor({0, 1, 1, 1});
        mCoordinateAxis.Draw(pose, eigen_proj, eigen_vm);
    }
}

// UI设计
void GL_Content::MainUI()
{
    if(!ImGui::Begin("Control Panel",nullptr, ImGuiWindowFlags_MenuBar)){
        ImGui::End();
        return;
    }

    /// Control Buttons
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
    if(ImGui::Button("Stop")) mProcessMode=PROCESS_STOP;
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
    if(ImGui::Button("Step")) mProcessMode=PROCESS_ONCE;
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(2);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
    if(ImGui::Button("Continue")) mProcessMode=PROCESS_CONTINUE;
    ImGui::PopStyleColor(1);
    ImGui::PopID();

    ImGui::Checkbox("Process", &bProcess);

//     /// Render options
//     ImGui::Separator();
//     bNeedUpdate |= ImGui::Checkbox("Surfel", &bRenderSurfel);
//     ImGui::SameLine();
//     bNeedUpdate |= ImGui::Checkbox("Node", &mGraphDrawer.mbDrawNode);
//     ImGui::SameLine();
//     bNeedUpdate |= ImGui::Checkbox("Edge", &mGraphDrawer.mbDrawEdge);
//     bNeedUpdate |= ImGui::Checkbox("Label", &mGraphDrawer.mbDrawLabel);

//     bNeedUpdate |= ImGui::Checkbox("NodeLabel", &bRenderNodeLabel);
//     ImGui::SameLine();
//     bNeedUpdate |= ImGui::Checkbox("EdgeLabel", &bRenderEdgeLabel);

//     ImGui::Checkbox("face culling",&bFaceCulling);
//     ImGui::Checkbox("Draw grid", &bShowGrid);
//     ImGui::Checkbox("Draw Traj.", &bDrawTraj);
//     ImGui::Checkbox("Draw Cam", &bDrawCam);

//     ImGui::Checkbox("FPS", &bShowFPS);
//     ImGui::Checkbox("Use thread", &mpGraphSLAM->UseThread());

//     if(ImGui::Button("Run Prediciton"))
//     {
//         mpGraphSLAM->RunFullPrediction();
//         bNeedUpdate = true;
//     }

//     // Select target node
//     {
//         static int selected_node = mSelectedNodeIdx;
//         auto nodes = mpGraphSLAM->GetGraph()->nodes;
//         std::vector<int> node_ids;
//         for(const auto &n : nodes) {
//             if(n.second->surfels.size() < (size_t)mNodeFilterSize) continue;
//             node_ids.push_back(n.first);
//         }
//         if(node_ids.empty()) node_ids.push_back(0);

//         int ori = selected_node;
//         ImGui::Text("Selected node");
//         ImGui::SameLine();
//         if (ImGui::Button(std::to_string(node_ids[selected_node]).c_str()))
//             ImGui::OpenPopup("node_selection_popup");
//         if (ImGui::BeginPopup("node_selection_popup")) {
//             for(size_t i=0;i<node_ids.size();++i) {
//                 if (ImGui::Selectable(std::to_string(node_ids[i]).c_str())) {
//                     selected_node = i;
//                 }
//             }
//             ImGui::EndPopup();
//         }
//         if (ori != selected_node) {
//             bNeedUpdate = true;
//             mSelectedNodeIdx = node_ids[selected_node];
//         }
//     }

//     static float text_size = mGraphDrawer.mTextDrawer.text_size;
//     ImGui::PushItemWidth(0.3f * ImGui::GetWindowWidth());
//     if(ImGui::DragFloat("TextSize", &text_size, 0.001f, 0.01f, 5.0f, "%.3f", ImGuiDragDropFlags_None)){
//         mGraphDrawer.mTextDrawer.text_size = text_size;
//     }
//     static float node_radius = mGraphDrawer.mNodeDrawer.mfPointSize;

//     ImGui::PushItemWidth(0.3f * ImGui::GetWindowWidth());
//     if(ImGui::DragFloat("diffuse strength", &mSurfelDrawer.mDiffuseStrength, 0.01f, 0.0f, 1.0f, "%.3f", ImGuiDragDropFlags_None)){
//     }

//     ImGui::PushItemWidth(0.3f * ImGui::GetWindowWidth());
//     if(ImGui::DragFloat("NodeRadius", &node_radius, 0.001f, 0.0f, 10.0f, "%.3f", ImGuiDragDropFlags_None)){
//         mGraphDrawer.mNodeDrawer.SetPointSize(node_radius);
//     }

//     ImGui::PushItemWidth(0.3f * ImGui::GetWindowWidth());
//     if(ImGui::DragInt("node size filter",&mNodeFilterSize,1.0,0,1000)) {
//         bNeedUpdate = true;
//     }
//     /// Show all labels and relationships
//     if (ImGui::CollapsingHeader("Graph Config"))
//         UI_Graph_Config();

//     // 图预测
//     static bool bNodeInfo = false;
//     if(mpGraphSLAM) {
//         if(mpGraphSLAM->GetGraphPred()){
//             if (ImGui::CollapsingHeader("Label names"))
//                 UI_Class_Relationships();
//             if(ImGui::Button("Show Graph Info")){
//                 bNodeInfo = !bNodeInfo;
//             }
//             if(bNodeInfo){
//                 ImGui::Begin("Node Properties", &bNodeInfo);
//                 UI_Graph_Info();
//                 ImGui::End();
//             }
//         }
//     }
//     ImGui::Separator();

//     /// Render Controls
//     {
//         /// Color Rendering Menu
//         /// color render type
//         {
//             static int selected_color_type = mColorType;
//             const char *color_names[] = {"LABEL", "PHONG", "NORMAL", "COLOR", "UPDATED", "SEMANTIC", "INSTANCE", "PANOPTIC"};

//             ImGui::Text("Color Render Type");
//             ImGui::SameLine();
//             if (ImGui::Button(color_names[selected_color_type]))
//                 ImGui::OpenPopup("surfel_rendering_popup");
//             ImGui::SameLine();
//             if (ImGui::BeginPopup("surfel_rendering_popup")) {
//                 for (int i = 0; i < IM_ARRAYSIZE(color_names); i++)
//                     if (ImGui::Selectable(color_names[i]))
//                         selected_color_type = i;
//                 ImGui::EndPopup();
//             }
//             if (mColorType != selected_color_type) {
//                 bNeedUpdate = true;
//                 mColorType = selected_color_type;
//             }
//         }
//         /// Label render type
//         {
//             static int selected_label_type = mLabelType;
//             const char *label_names[] = {"SEGMENT", "INST", "NAME"};

//             ImGui::NewLine();
//             ImGui::Text("Label Render Type");
//             ImGui::SameLine();
//             if (ImGui::Button(label_names[selected_label_type]))
//                 ImGui::OpenPopup("label_rendering_popup");
//             if (ImGui::BeginPopup("label_rendering_popup")) {
//                 for (int i = 0; i < IM_ARRAYSIZE(label_names); i++)
//                     if (ImGui::Selectable(label_names[i]))
//                         selected_label_type = i;
//                 ImGui::EndPopup();
//             }
//             if (mLabelType != selected_label_type) {
//                 bNeedUpdate = true;
//                 mLabelType = selected_label_type;
//             }
//         }
//         /// edge type
//         {
//             static int selected_edge_type = mEdgeType;
//             const char *edge_names[] = {"RELATIONSHIP", "NEIGHBOR"};
//             ImGui::Text("Edge Render Type");
//             ImGui::SameLine();
//             if (ImGui::Button(edge_names[selected_edge_type]))
//                 ImGui::OpenPopup("edge_rendering_popup");
//             if (ImGui::BeginPopup("edge_rendering_popup")) {
//                 for (int i = 0; i < IM_ARRAYSIZE(edge_names); i++)
//                     if (ImGui::Selectable(edge_names[i]))
//                         selected_edge_type = i;
//                 ImGui::EndPopup();
//             }
//             if (mEdgeType != selected_edge_type) {
//                 bNeedUpdate = true;
//                 mEdgeType = selected_edge_type;
//             }
//         }
//         /// graph type
//         {
//             static int selected_edge_type = mDrawGraphType;
//             const char *edge_names[] = {"SEGMENTS", "INSTANCE"};
//             ImGui::Text("Graph Render Type");
//             ImGui::SameLine();
//             if (ImGui::Button(edge_names[selected_edge_type]))
//                 ImGui::OpenPopup("graph_rendering_popup");
//             if (ImGui::BeginPopup("graph_rendering_popup")) {
//                 for (int i = 0; i < IM_ARRAYSIZE(edge_names); i++)
//                     if (ImGui::Selectable(edge_names[i]))
//                         selected_edge_type = i;
//                 ImGui::EndPopup();
//             }
//             if (mDrawGraphType != selected_edge_type) {
//                 bNeedUpdate = true;
//                 mDrawGraphType = selected_edge_type;
//             }
//         }
//     }
//     ImGui::Separator();

//     for(auto &pair:mEdgeUISwitch) {
//         if(pair.second) {
//             ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
//             std::string name = std::to_string(pair.first) + ": " + mpGraphSLAM->GetGraph()->nodes.at(pair.first)->GetLabel();
//             ImGui::Begin(name.c_str(), &pair.second);
//             UI_Graph_EdgeInfo(pair.first);
//             ImGui::End();
//         }
//     }

//     // Save
//     const std::string pth_out = "./"; //TODO: make this an input
//     if(ImGui::Checkbox("RecordImg",&bRecordImg)) ;
//     if(ImGui::Button("Save image")) RecordImg();
//     if(ImGui::Button("Save Map")) {
//         mpGraphSLAM->SaveModel(pth_out);
//         printf("map saved at %s\n",pth_out.c_str());
//     }
//     static bool save_binary_ply = true;
//     if(ImGui::Button("Output surfels to ply")) {
//         mpGraphSLAM->SaveSurfelsToPLY(mNodeFilterSize,pth_out, "inseg.ply",save_binary_ply);
//         printf("ply saved at %s\n",pth_out.c_str());
//     }
//     ImGui::SameLine();
//     ImGui::Checkbox("binary",&save_binary_ply);

//     static int node_color_saving_mode = mpGraphSLAM->SAVECOLORMODE_INSTANCE;
//     const char* items[] = { "RGB", "Segment", "Instance", "Semantic", "Panoptic"};
//     if(ImGui::Combo("Node color mode", &node_color_saving_mode , items, IM_ARRAYSIZE(items))){

//     };
//     if(ImGui::Button("Save Nodes to ply")) {
//         mpGraphSLAM->SaveNodesToPLY(mNodeFilterSize,pth_out, static_cast<PSLAM::GraphSLAM::SAVECOLORMODE>(node_color_saving_mode),save_binary_ply);
//         printf("ply saved at %s\n",pth_out.c_str());
//     }
//     static bool save_full_prop = true;
//     if(ImGui::Button("Save Graph")) {
//         auto scan_id = tools::PathTool::getFileName(tools::PathTool::find_parent_folder(mpDataLoader->GetDataBase()->folder, 1));
//         auto predictions = mpGraphSLAM->GetSceneGraph(save_full_prop);
//         json11::Json::object json;
//         json[scan_id] = predictions;
//         ORUtils::JsonUtils::Dump(json, pth_out + "/predictions.json");

// //        mpGraphSLAM->SaveGraph(pth_out,save_full_prop);
// //        printf("graph saved at %s\n",pth_out.c_str());
//     } ImGui::SameLine();
//     ImGui::Checkbox("Full",&save_full_prop);

    ImGui::End();
}





