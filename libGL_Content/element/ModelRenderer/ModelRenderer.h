#pragma once

#include "RendererInterfaces.h"
#include "glModel.h"
#include <memory>
#include <string>
#include <opencv2/imgproc.hpp>
#include <unordered_map>


namespace GLRenderer
{
    class ModelRenderer : public MeshRendererInterface
    {
        public:
        
            // 构造函数
            ModelRenderer(int width, 
                          int height, 
                          const std::string &dataPath, 
                          const std::string &sequence, 
                          bool toReference):MeshRendererInterface(width, 
                                                                  height, 
                                                                  dataPath, 
                                                                  sequence)
            {
                mb_toRef = toReference;
                Init();
            }

            // 渲染
            void DrawScene(GLRenderer::Model *model, GLRenderer::Shader *shader)
            {
                glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                shader->use();
                model->Draw();
            }

            // 渲染
            void Render(const Eigen::Matrix4f &projection, 
                        const Eigen::Matrix4f &view, 
                        float near, 
                        float far) override
            {
                glEnable(GL_DEPTH_TEST);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glViewport(0,0,m_width,m_height);
                mShaderLabel->use();
                mShaderLabel->set("view", view);
                mShaderLabel->set("projection", projection);
                mShaderLabel->set("model", glm::mat4(1.f));

                mShaderRGB->use();
                mShaderRGB->set("view", view);
                mShaderRGB->set("projection", projection);
                mShaderRGB->set("model", glm::mat4(1.f));

                // Render Label & Instances
                DrawScene(mModelLabel.get(), mShaderLabel.get());
                RenderLabel(m_label);
                cv::flip(m_label, m_label, 0);
                RenderInstance(m_label, m_instance);

                // 渲染RGB图
                DrawScene( mModelRGB.get(), mShaderRGB.get() );
                RenderRGB( m_rgb );
                cv::flip( m_rgb, m_rgb, 0 );
                cv::cvtColor(m_rgb, m_rgb, cv::COLOR_RGB2BGR);

                // 渲染深度图
                RenderDepth(m_depth, near,far);
                cv::flip(m_depth, m_depth, 0);
                m_depth.convertTo(m_depth, CV_16U);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

        private:
            bool mb_toRef;
            std::unique_ptr<GLRenderer::Shader> mShaderLabel, mShaderRGB;
            std::unique_ptr<GLRenderer::Model> mModelLabel, mModelRGB;

            std::unordered_map<unsigned long, int> m_color2instances;
            std::unirdered_map<int, std::string> m_instances2label;
            cv::Mat m_label, m_instance;

            void Init()
            {
                mModelLabel = std::make_unique<glUtil::Model>( m_folder+"/"+m_scanId+"/labels.instances.annotated.v2.ply");
                mModelRGB = std::make_unique<glUtil::Model>( m_folder+"/"+m_scanId+"/mesh.refined.v2.obj");
                if (mb_toRef)
                {
                    Eigen::Matrix<float, 4, 4> gt_transformation;
                    std::string path_poseTransform = m_folder + "/Scan3R_1/poseTransform.txt";
                    LoaderPoseTransform(gt_transformation, path_poseTransform);
                    mModelRGB->transform(gt_transformation);
                    mModelLabel->transform(gt_transformation);
                }

                // 导入分割结果
                if(!LoadObjects(m_folder+"/Scan3R_1/objects.json")) throw std::runtime_error("unable to load object data!");

                // 导入着色器
                mShaderLabel = std::make_unique<glUtil::Shader> (vs, fs);
                mShaderRGB = std::make_unique<glUtil::Shader>(vs_texture, fs_texture);
                mModelLabel->setShader(mShaderLabel.get());
                mModelRGB->setShader(mShaderRGB.get());
            }

            bool LoadObjects(const std::string& obj_file)
            {
                // 读取json文件
                std::ifstream is(obj_file);
                std::string dataset( (std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>() );
                std::string err;
                const auto json = json11::Json::parse(dataset, err);
                if (err != "") {
                    std::cout << "didn't find objects.json in " << m_folder << std::endl;
                    return false;
                }

                // 读取objects.json的分割内容
                for (auto &scan_json:json["scans"].array_items()){
                    const std::string &scan_id = scan_json["scan"].string_value(); // 自己采集的数据集默认第一个为当前帧的分割内容
                    const auto &objects = scan_json["objects"].array_items();
                    std::cout << "objects[0]: " << objects.size() << std::endl;

                    for(const auto &obj:objects){
                        const int id = std::stoi(obj["id"].string_value()); // id
                        const std::string hex_str = obj["ply_color"].string_value(); // 颜色
                        unsigned long color_hex;
                        std::istringstream iss(hex_str.substr(1));
                        iss >> std::hex >> color_hex; // 十六进制
                        // 存入字典
                        m_color2instances[color_hex] = id;
                        m_instances2label[id] = obj["label"].string_value();
                    }
                }
                return !m_color2instances.empty();
            }
 
            void RenderLabel(cv::Mat &img)
            {
                img = cv::Mat(m_height, m_width, CV_8UC3);
                glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, img.data);
            }

            // 渲染实例分割图
            void RenderInstance(const cv::Mat &labelImg, cv::Mat &instanceImg) 
            {
                instanceImg = cv::Mat(m_height, m_width, CV_8UC3);
                auto RGB2Hex = [](int r, int g, int b) -> unsigned long
                {
                    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
                };

                for (int i = 0; i < m_width; i++) {
                    for (int j = 0; j < m_height; j++) {
                        const cv::Vec3b& vec = labelImg.at<cv::Vec3b>(j, i);
                        const unsigned long color_hex = RGB2Hex(vec(2), vec(1), vec(0));
                        if (m_color2instances.find(color_hex) != m_color2instances.end()) 
                        {
                            const unsigned short Id = m_color2instances[color_hex]; // instance ID
                            instanceImg.at<unsigned short>(j, i) = Id;
                        } else instanceImg.at<unsigned short>(j, i) = 0;
                    }
                }
            }

            // 渲染深度图
            void RenderDepth(cv::Mat &img, float near, float far) 
            {
                img = cv::Mat::zeros(m_height, m_width, CV_32F);
                glReadPixels(0, 0,m_width,m_height, GL_DEPTH_COMPONENT, GL_FLOAT, img.data);
                for (size_t i = 0; i < (size_t) img.rows * img.cols; ++i) 
                {
                    if (img.at<float>(i) == 1 || img.at<float>(i) == 0) img.at<float>(i) = -1;
                    else {
                        const float zn = (img.at<float>(i) * 2 - 1);
                        const float ze = 2.0f * near * far / (far + near - zn * (far - near));
                        img.at<float>(i) = ze * 1000;;
                    }
                }
            }
    
            // 渲染RGB图
            void RenderRGB(cv::Mat &img) 
            {
                img = cv::Mat(m_height, m_width, CV_8UC3);
                glReadPixels(0, 0, m_width,m_height, GL_RGB, GL_UNSIGNED_BYTE,img.data);
            }

            // 字符串分割
            static const std::vector<std::string> split(const std::string s, const std::string delim) 
            {
                std::vector<std::string> list;
                auto start = 0U;
                auto end = s.find(delim);
                while (true) 
                {
                    list.push_back(s.substr(start, end - start));
                    if (end == std::string::npos)
                        break;
                    start = end + delim.length();
                    end = s.find(delim, start);
                }
                return list;
            }

            // 导入PoseTransform
            static void LoaderPoseTransform(Eigen::Matrix<float, 4, 4> &m_poseTransform, const std::string &path_poseTransform)
            {
                std::string line{""};
                std::ifstream file(path_poseTransform);
                std::getline(file, line);
                std::vector<std::string> float_list = split(line, ",");

                for(int i=0; i<float_list.size(); i++){
                    m_poseTransform(i) = std::stof(float_list[i]);
                }
            }

            // 顶点着色器
            std::string vs =
                    "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "layout (location = 1) in vec3 aColor;\n"
                    "out vec3 color;\n"
                    "uniform mat4 model;\n"
                    "uniform mat4 view;\n"
                    "uniform mat4 projection;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                    "    color = aColor;\n"
                    "}";

            // 片段着色器
            std::string fs =
                    "#version 330 core\n"
                    "out vec4 FragColor;\n"
                    "\n"
                    "in vec3 color;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    FragColor = vec4(color,1);\n"
                    "}";

            // 纹理顶点着色器
            std::string vs_texture =
                    "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "layout (location = 1) in vec3 aColor;\n"
                    "layout (location = 2) in vec3 aNormal;\n"
                    "layout (location = 3) in vec2 aTexCoords;\n"
                    "layout (location = 4) in vec3 aTangent;\n"
                    "layout (location = 5) in vec3 aBiTangent;\n"
                    "\n"
                    "out vec2 TexCoords;\n"
                    "\n"
                    "uniform mat4 model;\n"
                    "uniform mat4 view;\n"
                    "uniform mat4 projection;\n"
                    "\n"
                    "void main() \n"
                    "{\n"
                    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                    "    TexCoords = aTexCoords;\n"
                    "}";

            // 纹理片段着色器
            std::string fs_texture =
                    "#version 330 core\n"
                    "\n"
                    "in vec2 TexCoords;\n"
                    "out vec4 color;\n"
                    "\n"
                    "uniform sampler2D texture_diffuse;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    color = vec4(texture(texture_diffuse, TexCoords));\n"
                    "}";
    }
}